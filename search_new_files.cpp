#include "search_new_files.h"
#include <QDateTime>

Search_New_Files::Search_New_Files(QMap<QString, QFileInfo> src_map, QMap<QString, QFileInfo> dest_map, QObject *parent) :
    QThread(parent),
    m_src_map_files(src_map),
    m_dest_map_files(dest_map)
{

}

void Search_New_Files::run()
{
    run_new_files();
    emit complete(m_new_files, m_upd_files, m_errors, m_size);
}

void Search_New_Files::run_new_files()
{
    m_size = 0;
    QMap<QString, QFileInfo>::const_iterator src_entries;
    for (src_entries = m_src_map_files.constBegin(); src_entries != m_src_map_files.constEnd(); ++src_entries)
    {
        QFileInfo elt = src_entries.value();

        if ( ! elt.exists() && !elt.isSymLink() /*! elt.symLinkTarget().isEmpty()*/ )
        {
            m_errors.append("ERROR NEW FILE : " + elt.filePath());
        }
        else
        {

            QMap<QString, QFileInfo>::const_iterator dest_entries = m_dest_map_files.constFind(src_entries.key());

            if ( dest_entries == m_dest_map_files.constEnd() )
            {
                QString s_new_files = src_entries.key();
                m_new_files.append(s_new_files);
                m_size += elt.size();
            }
            else
            {
                // check size and modification date
                QFileInfo elt2 = dest_entries.value();
                if ( elt.size() != elt2.size() || elt.lastModified() != elt2.lastModified() )
                {
                    m_upd_files.append(src_entries.key());
                    m_size += elt.size();
                }
            }
        }

        emit updateSingleProgression();
    }
}
