#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QMap>
#include <QFileInfo>
#include <QFontMetrics>
#include "search_del_files.h"
#include "search_del_dir.h"
#include "search_new_dir.h"
#include "search_new_files.h"
#include "apply_changes.h"
#include "scan_entries.h"
//#include <QMetaType>
//typedef QMap<QString, QFileInfo> MyArray;
//qRegisterMetaType<MyArray>("MyArray");

//#include <QMetaType>
//Q_DECLARE_METATYPE(QMap<QString>)
//Q_DECLARE_METATYPE(QMap<QFileInfo>)
//typedef QMap<QString, QFileInfo> myMap;
//qRegisterMetaType<myMap>("myMap&");

//Q_DECLARE_METATYPE(namespace::QMap<QString, QFileInfo>)

namespace Ui {
class Form;
}

enum TAB_NAME
{
    TAB_DELETE_FILES=0,
    TAB_DELETE_DIR,
    TAB_NEW_DIR,
    TAB_NEW_FILES,
    TAB_UPD_FILES,
    TAB_ERRORS
};

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    void setArgs(const QString& src, const QString& dest, const QString& mode);
    bool launch_automatic_mode();
    ~Form();

private slots:
    bool on_pushButton_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_pushButton_2_clicked();
    void refresh();

    /// Handle threads
    void Search_Del_Files_Complete(const QStringList& del_files, const QStringList& errors);
    void Search_Del_Dir_Complete(const QStringList& del_dir, const QStringList& errors);
    void Search_New_Dir_Complete(const QStringList& new_dir, const QStringList& errors);
    void Search_New_Files_Complete(const QStringList& new_files, const QStringList& upd_files, const QStringList& errors, qint64 size);
    void updateSingleProgression();
    void updatePathSrc(const QString&);
    void updatePathDest(const QString&);
    void complete_scan_src(const QMap<QString,QFileInfo>&, const QMap<QString,QFileInfo>&);
    void complete_scan_dest(const QMap<QString,QFileInfo>&, const QMap<QString,QFileInfo>&);
    void complete_scan();

    void updateAction(const QString&);
    void updateError(const QString&);
    void updateProgression(int);
    void Apply_changes_Complete();

    void on_pb_pause_clicked();

private:
    /// Interface
    Ui::Form *ui;
    QFontMetrics *metrics;
    void reset_ui();
    bool m_automatic_mode;

    /// SCAN
    /// Input
    QMap<QString, QFileInfo> m_src_map_dir;
    QMap<QString, QFileInfo> m_src_map_files;
    QMap<QString, QFileInfo> m_dest_map_dir;
    QMap<QString, QFileInfo> m_dest_map_files;

    /// Functions
    void scan(const QString& dir, const QString &sub_dir, QMap<QString, QFileInfo>& map_dir, QMap<QString, QFileInfo>& map_files);
    void Search_Complete();
    int m_scan_size;
    int m_current_scan_size;


    /// APPLY
    /// Input
    QString src_directory;  // répertoire source (modifié par rapport au label -> /)
    QString dest_directory; // répertoire destination (modifié par rapport au label -> /)

    /// Computing
    qint64 m_size;
    QStringList m_new_dir;
    QStringList m_new_files;
    QStringList m_del_dir;
    QStringList m_del_files;
    QStringList m_upd_files;
    QStringList m_errors;

    /// THREADS
    int count_threads;

    /// Threads for scanning
    Scan_entries *threadA;
    Scan_entries *threadB;

    /// Threads for searching
    Search_Del_Files *thread0;
    Search_Del_Dir *thread1;
    Search_New_Dir *thread2;
    Search_New_Files *thread3;
    Apply_changes *thread4;
};

/*
Le scan remplit une map QString / QFileInfo, pour src et dest
A partir de cette map, on va vérifier si :
* chaque clef de src est bien dans dest -> sinon create (compute size)
* -> si oui, check size et modification date -> sinon update
* chaque clef de dest est bien dans src -> sinon delete
*
* Faire ca pour les répertoires puis les fichiers
*/

#endif // FORM_H
