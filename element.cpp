#include "element.h"

Element::Element()
{
    //origin = ORIGIN_NONE;
    QString m_sub_dir();
    QString m_filename();
    m_status = STATUS_NONE;
}

Element::Element(const QString& sub_dir, const QString& filename, enum STATUS status)
{
    //origin = ORIGIN_NONE;
    m_sub_dir = sub_dir;
    m_filename = filename;
    m_status = status;
}

const QString Element::get_sub_dir()
{
    return m_sub_dir;
}


const QString Element::get_filename()
{
    return m_filename;
}

const QString Element::get_status_html()
{
    switch ( m_status )
    {
    case STATUS_DIR: return "(<font color=green>répertoire</font>)";
    case STATUS_FILE: return "(<font color=blue>fichier</font>)";
    case STATUS_NONE:
    case STATUS_MAX:
        return "";
    }
}
