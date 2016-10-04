#include "search_new_dir.h"

Search_New_Dir::Search_New_Dir(QMap<QString, QFileInfo> src_map, QMap<QString, QFileInfo> dest_map, QObject *parent) :
    QThread(parent),
    m_src_map_dir(src_map),
    m_dest_map_dir(dest_map)
{

}

void Search_New_Dir::run()
{
    run_new_dir();
    emit complete(m_new_dir, m_errors);
}

void Search_New_Dir::run_new_dir()
{
    QMap<QString, QFileInfo>::const_iterator src_entries;
    for (src_entries = m_src_map_dir.constBegin(); src_entries != m_src_map_dir.constEnd(); ++src_entries)
    {
        QFileInfo elt = src_entries.value();

        if ( ! elt.exists() )
        {
            m_errors.append("ERROR NEW DIR : " + elt.filePath());
        }

        else if ( ! m_dest_map_dir.contains(src_entries.key()) )
        {
            m_new_dir.append(src_entries.key());
        }

        emit updateSingleProgression();
    }
}
