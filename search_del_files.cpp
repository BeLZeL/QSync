#include "search_del_files.h"

Search_Del_Files::Search_Del_Files(QMap<QString, QFileInfo> src_map, QMap<QString, QFileInfo> dest_map, QObject *parent) :
    QThread(parent),
    m_src_map_files(src_map),
    m_dest_map_files(dest_map)
{

}

void Search_Del_Files::run()
{
    run_del_files();
    emit complete(m_del_files, m_errors);
}

void Search_Del_Files::run_del_files()
{
    QMap<QString, QFileInfo>::const_iterator dest_entries;
    for (dest_entries = m_dest_map_files.constBegin(); dest_entries != m_dest_map_files.constEnd(); ++dest_entries)
    {
        QFileInfo elt = dest_entries.value();

        if ( ! elt.exists() && !elt.isSymLink() /*! elt.symLinkTarget().isEmpty()*/ )
        {
            m_errors.append("ERROR DELETE FILE : " + elt.filePath());
        }

        else if ( ! m_src_map_files.contains(dest_entries.key()) )
        {
            m_del_files.append(dest_entries.key());
        }

        emit updateSingleProgression();
    }
}
