#include "scan_entries.h"
#include <QDir>
#include <QFileInfoList>

Scan_entries::Scan_entries(const QString& dir, const QMap<QString, QFileInfo>& map_dir, const QMap<QString, QFileInfo>& map_files, QObject *parent) :
    QThread(parent),
    m_dir(dir),
    m_map_dir(map_dir),
    m_map_files(map_files)
{
    qRegisterMetaType< MyMap >("MyMap");
}

void Scan_entries::run()
{
    scan(m_dir, "", m_map_dir, m_map_files);
    emit complete_scan(m_map_dir, m_map_files);
}

void Scan_entries::scan(const QString& dir, const QString &sub_dir, QMap<QString, QFileInfo>& map_dir, QMap<QString, QFileInfo>& map_files)
{
    QString absolute_path = dir + sub_dir;
    QFileInfoList entries = QDir(absolute_path).entryInfoList(QDir::AllEntries | QDir::System | QDir::NoDotAndDotDot | QDir::Hidden);

    emit updatePath(sub_dir);

    QFileInfoList::const_iterator fi_entry;
    for (fi_entry = entries.constBegin(); fi_entry != entries.constEnd(); ++fi_entry)
    {
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
