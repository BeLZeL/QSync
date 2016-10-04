#ifndef SCAN_ENTRIES_H
#define SCAN_ENTRIES_H

#include <QThread>
#include <QMap>
#include <QString>
#include <QFileInfo>

//typedef QMap<QString, QFileInfo> MyArray;
typedef QMap<QString,QFileInfo> MyMap;

//#include <QMetaType>
//Q_DECLARE_METATYPE(QMap<QString>)
//Q_DECLARE_METATYPE(QMap<QFileInfo>)


class Scan_entries : public QThread
{
    Q_OBJECT

public:
    explicit Scan_entries(const QString& dir, const QMap<QString, QFileInfo>& map_dir, const QMap<QString, QFileInfo>& map_files, QObject *parent = 0);

public slots:
    void run();

signals:
    void updatePath(const QString& path);
    void complete_scan(const QMap<QString,QFileInfo>& map_dir, const QMap<QString,QFileInfo>& map_files);

private :
    /// input
    QString m_dir;

    /// method
    void scan(const QString& dir, const QString& sub_dir, QMap<QString, QFileInfo>& map_dir, QMap<QString, QFileInfo>& map_files);

    /// output
    QMap<QString, QFileInfo> m_map_dir;
    QMap<QString, QFileInfo> m_map_files;
};

#endif // SCAN_ENTRIES_H
