#ifndef ELEMENT_H
#define ELEMENT_H

#include <QString>

//enum ORIGIN { ORIGIN_NONE=0, ORIGIN_SRC, ORIGIN_DEST, ORIGIN_MAX };
enum STATUS { STATUS_NONE=0, STATUS_DIR, STATUS_FILE, STATUS_MAX };

class Element
{
public:
    Element();
    Element(const QString& sub_dir, const QString& filename, enum STATUS status);
    const QString get_sub_dir();
    const QString get_filename();
    const QString get_status_html();

private:
    //enum ORIGIN origin;
    QString m_sub_dir;
    QString m_filename;
    qint64 size;
    enum STATUS m_status;
};

#endif // ELEMENT_H
