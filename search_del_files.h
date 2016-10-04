#ifndef SEARCH_DEL_FILES_H
#define SEARCH_DEL_FILES_H

#include <QThread>
#include <QMap>
#include <QStringList>
#include <QFileInfo>

class Search_Del_Files : public QThread
{
    Q_OBJECT

public:
    explicit Search_Del_Files(QMap<QString, QFileInfo> src_map, QMap<QString, QFileInfo> dest_map, QObject *parent = 0);

public slots:
    void run();

signals:
    void updateSingleProgression();
    void complete(const QStringList& m_del_files, const QStringList& m_errors);

private :
    /// input
    QMap<QString, QFileInfo> m_src_map_files;
    QMap<QString, QFileInfo> m_dest_map_files;

    /// method
    void run_del_files();

    /// output
    QStringList m_del_files;
    QStringList m_errors;
};

#endif // SEARCH_DEL_FILES_H
