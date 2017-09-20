#include "form.h"
#include "ui_form.h"
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <QFileDialog>
#include <QDateTime>
#include <QSizePolicy>
#include <QTimer>
#include <QDebug>

void Form::setArgs(const QString& src, const QString& dest, const QString& mode)
{
    ui->le_src->setText(src);
    ui->le_dest->setText(dest);
    m_automatic_mode = ( mode == "AUTO");
    qDebug() << QString::number(m_automatic_mode);
}

// Return false to exit program for automatic mode
// Return true to opened program for manual mode
bool Form::launch_automatic_mode()
{
    if ( !m_automatic_mode )
    {
        qDebug() << "Automatic Mode Disabled";
        return true;
    }

    qDebug() << "Automatic Mode Enabled";

    if ( ! on_pushButton_clicked() )
    {
        qDebug() << "Failed !";
        return false;
    }

    return true;
}

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    ui->gb_1->setLayout(ui->boxlayout_1);
    ui->gb_2->setLayout(ui->boxlayout_2);
    ui->gb_3->setLayout(ui->boxlayout_3);

    QVBoxLayout *main_box = new QVBoxLayout(this);
    main_box->addWidget(ui->gb_1);
    main_box->addWidget(ui->gb_2);
    main_box->addWidget(ui->gb_3);
    setLayout(main_box);

    qRegisterMetaType< MyMap >("MyMap");
    m_current_scan_size = 0;
    m_scan_size = 0;
    m_automatic_mode = 0;

    //this->setLayout(ui->verticalLayout);
    //ui->progressBar->setMaximum(100);
    //ui->textEdit->setReadOnly(true);
    //ui->textEdit->setUndoRedoEnabled(false);

    QTimer * timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refresh()));
    timer->start(1000);


    //ui->textEdit->setWordWrapMode(QTextOption::NoWrap);

    //ui->l_res->setSizePolicy(QSizePolicy::Ignored);

#if defined(Q_OS_WIN)
    /// WINDOWS
    ui->le_src->setText("C:/");
    ui->le_dest->setText("E:/");
#else
    /// LINUX ///
    ui->le_src->setText("/home/");
    ui->le_dest->setText("/tmp/");
#endif

}

Form::~Form()
{
    delete ui;
}

void Form::refresh()
{
    qApp->processEvents();
}

void Form::reset_ui()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pb_scan->setValue(0);
    metrics = new QFontMetrics(ui->l_res->font());

    /// Scanning
    m_src_map_dir.clear();
    m_src_map_files.clear();
    m_dest_map_dir.clear();
    m_dest_map_files.clear();

    /// Del Files
    ui->tabs_entries->setTabText(TAB_DELETE_FILES, "Delete Files");
    ui->te_del_files->setText("");
    m_del_files.clear();

    /// Del Dir
    ui->tabs_entries->setTabText(TAB_DELETE_DIR, "Delete Dir");
    ui->te_del_dir->setText("");
    m_del_dir.clear();

    /// New Dir
    ui->tabs_entries->setTabText(TAB_NEW_DIR, "New Dir");
    ui->te_new_dir->setText("");
    m_new_dir.clear();

    /// New Files
    ui->tabs_entries->setTabText(TAB_NEW_FILES, "New Files");
    ui->te_new_files->setText("");
    m_new_files.clear();

    /// Upd Files
    ui->tabs_entries->setTabText(TAB_UPD_FILES, "Upd Files");
    ui->te_upd_files->setText("");
    m_upd_files.clear();

    /// Errors
    ui->tabs_entries->setTabText(TAB_ERRORS, "Errors");
    ui->te_errors->setText("");
    m_errors.clear();

    /// Size
    m_size = 0;

    ui->progressBar->setValue(0);
}

bool Form::on_pushButton_clicked()
{
    reset_ui();

    //--------------------------------------------------

    src_directory = ui->le_src->text();
    src_directory.replace('\\', '/');
    if ( src_directory.lastIndexOf('/') != src_directory.size() - 1 )
        src_directory += '/';
    ui->le_src->setText(src_directory);

    if ( ! QFileInfo(src_directory).isDir() )
    {
        QString error_log = "Directory doesn't exist : " + ui->le_src->text();
        if ( m_automatic_mode )
        {
            qDebug() << error_log;
            return false;
        }
        else
            QMessageBox::information(this, "BAD", error_log);
    }

    dest_directory = ui->le_dest->text();
    dest_directory.replace('\\', '/');
    if ( dest_directory.lastIndexOf('/') != dest_directory.size() - 1 )
        dest_directory += '/';
    ui->le_dest->setText(dest_directory);

    if ( ! QFileInfo(dest_directory).isDir() )
    {
        QString error_log = "Directory doesn't exist : " + ui->le_dest->text();
        if ( m_automatic_mode )
        {
            qDebug() << error_log;
            return false;
        }
        else
            QMessageBox::information(this, "BAD", error_log);
    }

    count_threads = 2;

    threadA = new Scan_entries(src_directory, m_src_map_dir, m_src_map_files);
    connect(threadA, SIGNAL(updatePath(const QString&)), this, SLOT(updatePathSrc(const QString&)));
    connect(threadA, SIGNAL(complete_scan(const QMap<QString,QFileInfo>&, const QMap<QString,QFileInfo>&)), this, SLOT(complete_scan_src(const QMap<QString,QFileInfo>&, const QMap<QString,QFileInfo>&)),Qt::QueuedConnection);
    threadA->start();

    threadB = new Scan_entries(dest_directory, m_dest_map_dir, m_dest_map_files);
    connect(threadB, SIGNAL(updatePath(const QString&)), this, SLOT(updatePathDest(const QString&)));
    connect(threadB, SIGNAL(complete_scan(const QMap<QString,QFileInfo>&, const QMap<QString,QFileInfo>&)), this, SLOT(complete_scan_dest(const QMap<QString,QFileInfo>&, const QMap<QString,QFileInfo>&)),Qt::QueuedConnection);
    threadB->start();

    ///threadA->wait();
    ///threadB->wait();

    /******************
    ui->l_res->setText("Scan : " + src_directory);
    qApp->processEvents();
    scan(src_directory, "", src_map_dir, src_map_files);

    ui->l_res->setText("Scan : " + dest_directory);
    qApp->processEvents();
    scan(dest_directory, "", dest_map_dir, dest_map_files);

    ui->l_res->setText("Scan : " + QString::number(m_scan_size));
    qApp->processEvents();
    m_scan_size = src_map_dir.size() + src_map_files.size() + dest_map_dir.size() + dest_map_files.size();
    m_current_scan_size = 0;

    //-------------------------------------------------------------------------

    ///ui->l_res->setText("Check if dest not exists in src (files)" + QString::number(current_scan_size) + " " + QString::number(scan_size) );
    ///qApp->processEvents();
    count_threads = 4;

    thread0 = new Search_Del_Files(src_map_files, dest_map_files);
    connect(thread0, SIGNAL(updateSingleProgression()), this, SLOT(updateSingleProgression()));
    connect(thread0, SIGNAL(complete(const QStringList&, const QStringList&)), this, SLOT(Search_Del_Files_Complete(const QStringList&, const QStringList&)));
    thread0->start();

    thread1 = new Search_Del_Dir(src_map_dir, dest_map_dir);
    connect(thread1, SIGNAL(updateSingleProgression()), this, SLOT(updateSingleProgression()));
    connect(thread1, SIGNAL(complete(const QStringList&, const QStringList&)), this, SLOT(Search_Del_Dir_Complete(const QStringList&, const QStringList&)));
    thread1->start();

    thread2 = new Search_New_Dir(src_map_dir, dest_map_dir);
    connect(thread2, SIGNAL(updateSingleProgression()), this, SLOT(updateSingleProgression()));
    connect(thread2, SIGNAL(complete(const QStringList&, const QStringList&)), this, SLOT(Search_New_Dir_Complete(const QStringList&, const QStringList&)));
    thread2->start();

    thread3 = new Search_New_Files(src_map_files, dest_map_files);
    connect(thread3, SIGNAL(updateSingleProgression()), this, SLOT(updateSingleProgression()));
    connect(thread3, SIGNAL(complete(const QStringList&, const QStringList&, const QStringList&, qint64)), this, SLOT(Search_New_Files_Complete(const QStringList&, const QStringList&, const QStringList&, qint64)));
    thread3->start();

    //ui->progressBar->setValue(100);
    ui->l_res->setText("Scan finished");
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ************/

    return true;
}

void Form::updatePathSrc(const QString& absolute_path)
{
    static QFontMetrics src_metrics(ui->l_src->font());
    QString elidedText = src_metrics.elidedText("Scan : " + absolute_path, Qt::ElideMiddle, ui->l_src->width());
    ui->l_src->setText(elidedText);
}

void Form::updatePathDest(const QString& absolute_path)
{
    static QFontMetrics dest_metrics(ui->l_dest->font());
    QString elidedText = dest_metrics.elidedText("Scan : " + absolute_path, Qt::ElideMiddle, ui->l_dest->width());
    ui->l_dest->setText(elidedText);
}

void Form::scan(const QString& dir, const QString &sub_dir, QMap<QString, QFileInfo>& map_dir, QMap<QString, QFileInfo>& map_files)
{
    //QMessageBox::information(this, "OK", dir);
    QString absolute_path = dir + sub_dir;
    QFileInfoList entries = QDir(absolute_path).entryInfoList(QDir::AllEntries | QDir::System | QDir::NoDotAndDotDot);

    QString text("Scan : " + absolute_path);
    QString elidedText = metrics->elidedText(text, Qt::ElideMiddle, ui->l_res->width());
    ui->l_res->setText(elidedText);
    /*
    qApp->processEvents();*/

/*
    if ( QFileInfo("G:/Divers/z_Stéphanie/Documents and Settings/Stéphanie BRIET/Application Data/Microsoft/Internet Explorer/Quick Launch/QuickTime Player.lnk").exists() )
        QMessageBox::information(this, "OK", "Errors found");
    else
        QMessageBox::information(this, "BAD", "Errors found");

    if ( QFileInfo("G:/Divers/z_Stéphanie/Documents and Settings/Stéphanie BRIET/Application Data/Microsoft/Internet Explorer/Quick Launch/QuickTime Player.lnk").isSymLink() )
        QMessageBox::information(this, "OK", QFileInfo("G:/Divers/z_Stéphanie/Documents and Settings/Stéphanie BRIET/Application Data/Microsoft/Internet Explorer/Quick Launch/QuickTime Player.lnk").symLinkTarget());
    else
        QMessageBox::information(this, "BAD", "Errors found");
*/

    //ui->textEdit->append("-->" + QString::number(entries.size()));

    QFileInfoList::const_iterator fi_entry;
    for (fi_entry = entries.constBegin(); fi_entry != entries.constEnd(); ++fi_entry)
    {/*
        if ( fi_entry->isSymLink())
            ui->textEdit->append((*fi_entry).symLinkTarget() + " " + QString::number(entries.size()));
        else
            ui->textEdit->append((*fi_entry).fileName() + " " + QString::number(entries.size()));
*/
        //if ( (*fi_entry).fileName() == "." || (*fi_entry).fileName() == ".." )
//            continue;

        //QFileInfo fi_entry = QFileInfo(absolute_path + (*entry));
        QString relative_path = sub_dir + (*fi_entry).fileName();



        if ( fi_entry->isDir() )
        {
            if ( ! fi_entry->isSymLink() )
            {
                map_dir.insert(relative_path + "/", *fi_entry);
                scan(dir, relative_path + "/", map_dir, map_files);
            }
            else
                map_files.insert(relative_path, *fi_entry);
        }
        else
        {
            map_files.insert(relative_path, *fi_entry);
        }
    }
}

void Form::complete_scan_src(const QMap<QString, QFileInfo>& map_dir, const QMap<QString, QFileInfo>& map_files)
{
    m_src_map_dir = map_dir;
    m_src_map_files = map_files;
    updatePathSrc("finished");
    complete_scan();
}

void Form::complete_scan_dest(const QMap<QString,QFileInfo>& map_dir, const QMap<QString, QFileInfo>& map_files)
{
    m_dest_map_dir = map_dir;
    m_dest_map_files = map_files;
    updatePathDest("finished");
    complete_scan();
}

void Form::complete_scan()
{
    --count_threads;
    if ( count_threads == 0 )
    {
        m_scan_size = m_src_map_dir.size() + m_src_map_files.size() + m_dest_map_dir.size() + m_dest_map_files.size();
        m_current_scan_size = 0;

        count_threads = 4;

        thread0 = new Search_Del_Files(m_src_map_files, m_dest_map_files);
        connect(thread0, SIGNAL(updateSingleProgression()), this, SLOT(updateSingleProgression()));
        connect(thread0, SIGNAL(complete(const QStringList&, const QStringList&)), this, SLOT(Search_Del_Files_Complete(const QStringList&, const QStringList&)));
        thread0->start();

        thread1 = new Search_Del_Dir(m_src_map_dir, m_dest_map_dir);
        connect(thread1, SIGNAL(updateSingleProgression()), this, SLOT(updateSingleProgression()));
        connect(thread1, SIGNAL(complete(const QStringList&, const QStringList&)), this, SLOT(Search_Del_Dir_Complete(const QStringList&, const QStringList&)));
        thread1->start();

        thread2 = new Search_New_Dir(m_src_map_dir, m_dest_map_dir);
        connect(thread2, SIGNAL(updateSingleProgression()), this, SLOT(updateSingleProgression()));
        connect(thread2, SIGNAL(complete(const QStringList&, const QStringList&)), this, SLOT(Search_New_Dir_Complete(const QStringList&, const QStringList&)));
        thread2->start();

        thread3 = new Search_New_Files(m_src_map_files, m_dest_map_files);
        connect(thread3, SIGNAL(updateSingleProgression()), this, SLOT(updateSingleProgression()));
        connect(thread3, SIGNAL(complete(const QStringList&, const QStringList&, const QStringList&, qint64)), this, SLOT(Search_New_Files_Complete(const QStringList&, const QStringList&, const QStringList&, qint64)));
        thread3->start();
    }
}

void Form::Search_Del_Files_Complete(const QStringList& del_files, const QStringList& errors)
{
    m_del_files += del_files;
    m_errors    += errors;
    Search_Complete();
}

void Form::Search_Del_Dir_Complete(const QStringList& del_dir, const QStringList& errors)
{
    m_del_dir += del_dir;
    m_errors  += errors;
    Search_Complete();
}

void Form::Search_New_Dir_Complete(const QStringList& new_dir, const QStringList& errors)
{
    m_new_dir += new_dir;
    m_errors  += errors;
    Search_Complete();
}

void Form::Search_New_Files_Complete(const QStringList& new_files, const QStringList& upd_files, const QStringList& errors, qint64 size)
{
    m_new_files += new_files;
    m_upd_files += upd_files;
    m_errors    += errors;
    m_size      += size;
    Search_Complete();
}

void Form::Search_Complete()
{
    --count_threads;
    if ( count_threads == 0 )
    {
        /// Del Files
        ui->tabs_entries->setTabText(TAB_DELETE_FILES, "Delete Files (" + QString::number(m_del_files.count()) + ")");
        ui->te_del_files->setPlainText(m_del_files.join("\n"));

        /// Del Dir
        ui->tabs_entries->setTabText(TAB_DELETE_DIR, "Delete Dir (" + QString::number(m_del_dir.count()) + ")");
        ui->te_del_dir->setPlainText(m_del_dir.join("\n"));

        /// New Dir
        ui->tabs_entries->setTabText(TAB_NEW_DIR, "New Dir (" + QString::number(m_new_dir.count()) + ")");
        ui->te_new_dir->setPlainText(m_new_dir.join("\n"));

        /// New Files
        ui->tabs_entries->setTabText(TAB_NEW_FILES, "New Files (" + QString::number(m_new_files.count()) + ")");
        ui->te_new_files->setPlainText(m_new_files.join("\n"));

        /// Upd Files
        ui->tabs_entries->setTabText(TAB_UPD_FILES, "Upd Files (" + QString::number(m_upd_files.count()) + ")");
        ui->te_upd_files->setPlainText(m_upd_files.join("\n"));

        /// Errors
        ui->tabs_entries->setTabText(TAB_ERRORS, "Errors (" + QString::number(m_errors.count()) + ")");
        if ( ! m_errors.isEmpty() )
        {
            //ui->te_errors->append(m_errors.join("\n"));
            for(auto &data : m_errors)
                ui->te_errors->append("<font color=red>" + data + "</font><br>");
            ui->tabs_entries->setCurrentIndex(TAB_ERRORS);
        }

        /// UPD -> DEL + NEW
        m_del_files += m_upd_files;
        m_new_files += m_upd_files;

        /// Print size to copy
        if  ( m_size < 10*1024*1024 )
            ui->l_res->setText("Scan finished : copy " +  QString::number(m_size/1024) + " Ko");
        else if  (  m_size < (qint64)10*1024*1024*1024 )
            ui->l_res->setText("Scan finished : copy " +  QString::number(m_size/1024/1024) + " Mo");
        else
            ui->l_res->setText("Scan finished : copy " +  QString::number(m_size/1024/1024/1024) + "Go");

        /// Enable - Finished
        ui->pushButton->setEnabled(true);
        ui->pb_scan->setValue(100);

        qDebug() << "Scan is finished";

        if ( m_del_files.count() > 0 || m_del_dir.count() > 0 || m_new_dir.count() > 0 || m_new_files.count() > 0 || m_upd_files.count() > 0  )
        {
            ui->pushButton_2->setEnabled(true);
            if ( m_automatic_mode )
                on_pushButton_2_clicked();
        }
        else if ( m_automatic_mode )
        {
            qDebug() << "Nothing to update";
            qApp->exit();
        }
    }
}

/*
void Form::scan(const QString& dir, const QString &sub_dir, QMap<QString, QFileInfo>& map_dir, QMap<QString, QFileInfo>& map_files)
{
    //QMessageBox::information(this, "OK", dir);
    QString absolute_path = dir + sub_dir;
    QStringList entries = QDir(absolute_path).entryList();

    QStringList::const_iterator entry;
    for (entry = entries.constBegin(); entry != entries.constEnd(); ++entry)
    {
                ui->textEdit->append((*entry));

        if ( (*entry) == "." || (*entry) == ".." )
            continue;

        QFileInfo fi_entry = QFileInfo(absolute_path + (*entry));
        QString relative_path = sub_dir + *entry;



        if ( fi_entry.isDir() )
        {
            if ( ! fi_entry.isSymLink() )
            {
                map_dir.insert(relative_path + "/", fi_entry);
                scan(dir, relative_path + "/", map_dir, map_files);
            }
            else
                map_files.insert(relative_path, fi_entry);
        }
        else
        {
            map_files.insert(relative_path, fi_entry);
        }
    }
}
*/
void Form::on_toolButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setDirectory(ui->le_src->text());
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::List);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    QString fileName;
    if(dialog.exec())
    {
        fileName = dialog.selectedFiles().at(0);
        if ( fileName.lastIndexOf('/') != fileName.size() - 1 )
            fileName += '/';
        ui->le_src->setText(fileName);
    }
    else
        dialog.reject();
}

void Form::on_toolButton_2_clicked()
{
    QFileDialog dialog(this);
    dialog.setDirectory(ui->le_dest->text());
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setViewMode(QFileDialog::List);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    QString fileName;
    if(dialog.exec())
    {
        fileName = dialog.selectedFiles().at(0);
        if ( fileName.lastIndexOf('/') != fileName.size() - 1 )
            fileName += '/';
        ui->le_dest->setText(fileName);
    }
    else
        dialog.reject();
}

void Form::on_pushButton_2_clicked()
{
    ui->progressBar->setValue(0);
    metrics = new QFontMetrics(ui->l_res->font());
    ui->pushButton->setEnabled(false);
    ui->pb_pause->setEnabled(true);

////////////
    thread4 = new Apply_changes(src_directory, dest_directory, m_new_dir, m_new_files, m_del_dir, m_del_files, m_size);
    connect(thread4, SIGNAL(updateAction(const QString&)), this, SLOT(updateAction(const QString&)));
    connect(thread4, SIGNAL(updateError(const QString&)), this, SLOT(updateError(const QString&)));
    connect(thread4, SIGNAL(updateProgression(int)), this, SLOT(updateProgression(int)));
    connect(thread4, SIGNAL(complete()), this, SLOT(Apply_changes_Complete()));
    thread4->start();
////////////
}

void Form::updateAction(const QString& action)
{
    QString elidedText = metrics->elidedText(action, Qt::ElideMiddle, ui->l_res->width());
    ui->l_res->setText(elidedText);
}

void Form::updateError(const QString& error)
{
    if ( ! error.isEmpty() )
    {
        ui->tabs_entries->setCurrentIndex(TAB_ERRORS);
        ui->te_errors->append("<font color=red><b>" + error + "</b></font><br>");
    }
}

void Form::updateSingleProgression()
{
    ++m_current_scan_size;
    ui->pb_scan->setValue(m_current_scan_size / (double)m_scan_size * 100.0);
}

void Form::updateProgression(int value)
{
    ui->progressBar->setValue(value);
}

void Form::Apply_changes_Complete()
{
    ui->l_res->setText("DONE");
    ui->progressBar->setValue(100);
    ui->pb_pause->setEnabled(false);
    ui->pushButton->setEnabled(true);

    if ( m_automatic_mode )
    {
        qDebug() << "Synchro is finished";
        qApp->exit();
    }
}

/*
extern (C) nothrow version (Windows)
{
    enum EPERM = 1;
    enum ENOENT = 2;
    enum ESRCH = 3;
    enum EINTR = 4;
    enum EIO = 5;
    enum ENXIO = 6;
    enum E2BIG = 7;
    enum ENOEXEC = 8;
    enum EBADF = 9;
    enum ECHILD = 10;
    enum EAGAIN = 11;
    enum ENOMEM = 12;
    enum EACCES = 13;
    enum EFAULT = 14;
    enum EBUSY = 16;
    enum EEXIST = 17;
    enum EXDEV = 18;
    enum ENODEV = 19;
    enum ENOTDIR = 20;
    enum EISDIR = 21;
    enum EINVAL = 22;
    enum ENFILE = 23;
    enum EMFILE = 24;
    enum ENOTTY = 25;
    enum EFBIG = 27;
    enum ENOSPC = 28;
    enum ESPIPE = 29;
    enum EROFS = 30;
    enum EMLINK = 31;
    enum EPIPE = 32;
    enum EDOM = 33;
    enum ERANGE = 34;
    enum EDEADLK = 36;
    enum ENAMETOOLONG = 38;
    enum ENOLCK = 39;
    enum ENOSYS = 40;
    enum ENOTEMPTY = 41;
    enum EILSEQ = 42;
    enum EDEADLOCK = EDEADLK;
}
*/

/*
QStringList Form::func_search_del_files(const QMap<QString, QFileInfo>& src_map_files, const QMap<QString, QFileInfo>& dest_map_files)
{
    QStringList sl_del_files;
    QStringList sl_errors;

    QMap<QString, QFileInfo>::const_iterator dest_entries;
    for (dest_entries = dest_map_files.constBegin(); dest_entries != dest_map_files.constEnd(); ++dest_entries)
    {
        QFileInfo elt = dest_entries.value();

        if ( ! elt.exists() && elt.symLinkTarget().isEmpty() )
        {
            sl_errors.append(elt.filePath() + " <font color=red><b>ERROR A</b></font><br>");
        }

        if ( ! src_map_files.contains(dest_entries.key()) )
        {
            sl_del_files.append(dest_entries.key());
        }
    }

    //for(auto &data : sl_del_files)
//        ui->textEdit->append("<font color=green><b>DELETE FILE</b></font> : " + data);

    //ui->textEdit->append(results);
    //ui->textEdit->append(sl_del_files);
    //ui->progressBar->setValue(int(current_scan_size / (double)scan_size * 100.0));
    ///qApp->processEvents();

    if ( ! sl_errors.isEmpty() )
    {
        QMessageBox::information(this, "BAD", "Errors found");
        return sl_errors;
    }
    else
        return sl_del_files;
}
*/

void Form::on_pb_pause_clicked()
{
    thread4->pause = ! thread4->pause;
    if ( thread4->pause )
        ui->pb_pause->setText("Continue");
    else
        ui->pb_pause->setText("Pause");
}
