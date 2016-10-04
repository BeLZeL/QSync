#ifndef SEARCH_NEW_FILES_H
#define SEARCH_NEW_FILES_H

#include <QThread>
#include <QMap>
#include <QStringList>
#include <QFileInfo>

class Search_New_Files : public QThread
{
    Q_OBJECT

public:
    explicit Search_New_Files(QMap<QString, QFileInfo> src_map, QMap<QString, QFileInfo> dest_map, QObject *parent = 0);

public slots:
    void run();

signals:
    void updateSingleProgression();
    void complete(const QStringList& m_new_files, const QStringList& m_upd_files, const QStringList& m_errors, qint64 size);

private :
    /// input
    QMap<QString, QFileInfo> m_src_map_files;
    QMap<QString, QFileInfo> m_dest_map_files;

    /// method
    void run_new_files();

    /// output
    qint64 m_size;
    QStringList m_new_files;
    QStringList m_upd_files;
    QStringList m_errors;
};

#endif // SEARCH_NEW_FILES_H
