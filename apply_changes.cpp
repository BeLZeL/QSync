#include "apply_changes.h"

#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QFileInfo>

#if !defined(Q_OS_WIN)
#include <sys/types.h>
#include <utime.h>
#include <time.h>
#endif

Apply_changes::Apply_changes(QString src_directory,
                             QString dest_directory,
                             QStringList new_dir,
                             QStringList new_files,
                             QStringList del_dir,
                             QStringList del_files,
                             qint64 size,
                             QObject *parent) :
    QThread(parent),
    m_src_directory(src_directory),
    m_dest_directory(dest_directory),
    m_new_dir(new_dir),
    m_new_files(new_files),
    m_del_dir(del_dir),
    m_del_files(del_files),
    m_size(size)
{
    pause = false;
}

void Apply_changes::run()
{
    apply_changes();
    emit complete();
}

void Apply_changes::apply_changes()
{
    for(auto &data : m_del_files)
    {
        QString filepath = m_dest_directory + data;
        emit updateAction("DELETE FILE : " + filepath);
        if ( QFile(filepath).remove() == false )
            emit updateError("ERROR DELETE FILE : " + filepath);
    }

    for(auto &data : m_del_dir)
    {
        QString dirpath = m_dest_directory + data;
        emit updateAction("DELETE DIRECTORY : " + dirpath);
        if ( QDir(m_dest_directory).rmdir(data) == false )
            emit updateError("ERROR DELETE DIRECTORY : " + dirpath);
    }

    for(auto &data : m_new_dir)
    {
        QString dirpath = m_dest_directory + data;
        emit updateAction("CREATE DIRECTORY : " + dirpath);
        if ( QDir(m_dest_directory).mkdir(data) == false )
            emit updateError("ERROR CREATE DIRECTORY : " + dirpath);
    }

    qint64 current_size = 0;
    int pb_value = 0;
    for(auto &data : m_new_files)
    {
        while ( pause )
            QThread::sleep(1);

        emit updateAction("COPY : " + data);

        if ( QFile(m_src_directory + data).copy(m_dest_directory + data) == false )
            emit updateError("ERROR COPY : " + data);

        current_size += QFileInfo(m_dest_directory + data).size();
        pb_value = int(current_size / (double)m_size * 100.0 );
        emit updateProgression(pb_value);
    }

#if !defined(Q_OS_WIN)
/*** LINUX ONLY ??? Set last modification properly ***/
    current_size = 0;
    pb_value = 0;
    for(auto &data : m_new_files)
    {
        while ( pause )
            QThread::sleep(1);

        emit updateAction("SET LAST DATETIME : " + data);

        struct utimbuf timebuffer;
        QString dest = m_dest_directory + data;
        timebuffer.modtime = QFileInfo(m_src_directory + data).lastModified().toTime_t();

        QByteArray latinArray = dest.toLatin1();
        const char* filename = latinArray.constData();

        if ( utime(filename, &timebuffer) < 0 )
            emit updateError("ERROR SET LAST DATETIME : " + QString(tr(filename)) + QString::number(strlen(filename)) + "|" + dest + QString::number(dest.size()) + "|" + QString::number(errno) + "|" + QString::number(timebuffer.modtime));

        current_size += QFileInfo(m_dest_directory + data).size();
        pb_value = int(current_size / (double)m_size * 100.0 );
        emit updateProgression(pb_value);
    }
#endif
}
