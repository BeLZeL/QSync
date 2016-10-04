#include "search_del_dir.h"

Search_Del_Dir::Search_Del_Dir(QMap<QString, QFileInfo> src_map, QMap<QString, QFileInfo> dest_map, QObject *parent) :
    QThread(parent),
    m_src_map_dir(src_map),
    m_dest_map_dir(dest_map)
{

}

void Search_Del_Dir::run()
{
    run_del_dir();
    emit complete(m_del_dir, m_errors);
}

void Search_Del_Dir::run_del_dir()
{
    QMap<QString, QFileInfo>::const_iterator dest_entries;
    for (dest_entries = m_dest_map_dir.constBegin(); dest_entries != m_dest_map_dir.constEnd(); ++dest_entries)
    {
        QFileInfo elt = dest_entries.value();

        if ( ! elt.exists() )
        {
            m_errors.append("ERROR DELETE DIR : " + elt.filePath());
        }

        else if ( ! m_src_map_dir.contains(dest_entries.key()) )
        {
            m_del_dir.prepend(dest_entries.key());
        }

        emit updateSingleProgression();
    }
}
