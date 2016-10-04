#include "mythread.h"
#include <QStringList>
#include <QDateTime>

MyThread::MyThread(int mode, QObject *parent) :
    QThread(parent)
{
    m_mode = mode;
}

void MyThread::setArgs(QMap<QString, QFileInfo> src_map_files, QMap<QString, QFileInfo> dest_map_files)
{
    m_src_map_files = src_map_files;
    m_dest_map_files = dest_map_files;
}

void MyThread::run()
{
    if ( m_mode == 0 )
    {
        QStringList result = run_del_files(m_src_map_files, m_dest_map_files);
        emit complete(result);
    }
    else if ( m_mode == 1 )
    {
        QStringList result = run_del_dir(m_src_map_files, m_dest_map_files);
        emit complete(result);
    }
    else if ( m_mode == 2 )
    {
        QStringList result = run_new_dir(m_src_map_files, m_dest_map_files);
        emit complete(result);
    }
    else if ( m_mode == 3 )
    {
        QStringList result = run_new_files(m_src_map_files, m_dest_map_files);
        emit complete(result);
    }
}


QStringList MyThread::run_del_files(const QMap<QString, QFileInfo>& src_map_files, const QMap<QString, QFileInfo>& dest_map_files)
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
        //QMessageBox::information(this, "BAD", "Errors found");
        return sl_errors;
    }
    else
        return sl_del_files;
}

QStringList MyThread::run_del_dir(const QMap<QString, QFileInfo>& src_map_dir, const QMap<QString, QFileInfo>& dest_map_dir)
{
    QStringList sl_del_dir;
    QStringList sl_errors;

    QMap<QString, QFileInfo>::const_iterator dest_entries;
    for (dest_entries = dest_map_dir.constBegin(); dest_entries != dest_map_dir.constEnd(); ++dest_entries)
    {
        QFileInfo elt = dest_entries.value();

        if ( ! elt.exists() )
        {
            sl_errors.append(elt.filePath() + " <font color=red><b>ERROR B</b><br>");
        }

        if ( ! src_map_dir.contains(dest_entries.key()) )
        {
            sl_del_dir.append(dest_entries.key());
        }

        ///++current_scan_size;
    }

    ///ui->textEdit->append(results);
    ///ui->progressBar->setValue(int(current_scan_size / (double)scan_size * 100.0));
    ///qApp->processEvents();

    if ( ! sl_errors.isEmpty() )
    {
        //QMessageBox::information(this, "BAD", "Errors found");
        return sl_errors;
    }
    else
        return sl_del_dir;
}

QStringList MyThread::run_new_dir(const QMap<QString, QFileInfo>& src_map_dir, const QMap<QString, QFileInfo>& dest_map_dir)
{
    QStringList sl_new_dir;
    QStringList sl_errors;

    QMap<QString, QFileInfo>::const_iterator src_entries;
    for (src_entries = src_map_dir.constBegin(); src_entries != src_map_dir.constEnd(); ++src_entries)
    {
        QFileInfo elt = src_entries.value();

        if ( ! elt.exists() )
        {
            sl_errors.append(elt.filePath() + " <font color=red><b>ERROR C</b><br>");
        }

        if ( ! dest_map_dir.contains(src_entries.key()) )
        {
            sl_new_dir.append(src_entries.key());
        }

        ///++current_scan_size;
    }

    ///ui->textEdit->append(results);
    ///ui->progressBar->setValue(int(current_scan_size / (double)scan_size * 100.0));
    ///qApp->processEvents();

    if ( ! sl_errors.isEmpty() )
    {
        //QMessageBox::information(this, "BAD", "Errors found");
        return sl_errors;
    }
    else
        return sl_new_dir;
}

QStringList MyThread::run_new_files(const QMap<QString, QFileInfo>& src_map_files, const QMap<QString, QFileInfo>& dest_map_files)
{
    QStringList sl_new_files;
    QStringList sl_del_files;
    QStringList sl_errors;

    QMap<QString, QFileInfo>::const_iterator src_entries;
    for (src_entries = src_map_files.constBegin(); src_entries != src_map_files.constEnd(); ++src_entries)
    {
        QFileInfo elt = src_entries.value();

        if ( ! elt.exists() && elt.symLinkTarget().isEmpty() )
        {
            sl_errors.append(elt.filePath() + " <font color=red><b>ERROR D</b>[" + elt.symLinkTarget() +"]<font><br>");
            //ret = false;
        }

        QMap<QString, QFileInfo>::const_iterator dest_entries = dest_map_files.constFind(src_entries.key());

        if ( dest_entries == dest_map_files.constEnd() )
        {
            QString s_new_files = src_entries.key();
            sl_new_files.append(s_new_files);
            ///size += elt.size();
            ///results += "<font color=green><b>COPY FILE</b></font> : " + s_new_files + "<BR>";
        }
        else
        {
            // check size and modification date
            QFileInfo elt2 = dest_entries.value();
            if ( elt.size() != elt2.size() || elt.lastModified() != elt2.lastModified() )
            {
                QString s_new_files = src_entries.key();
                sl_new_files.append(s_new_files);
                sl_del_files.append(dest_entries.key());
                ///size += elt.size();
                ///results += "<font color=orange><b>UPD FILE</b></font> : " + s_new_files + "<BR>";
            }
        }

        ///++current_scan_size;
    }

    ///ui->textEdit->append(results);
    ///ui->progressBar->setValue(int(current_scan_size / (double)scan_size * 100.0));
    ///qApp->processEvents();

    if ( ! sl_errors.isEmpty() )
    {
        //QMessageBox::information(this, "BAD", "Errors found");
        return sl_errors;
    }
    else
        return sl_new_files;
}
