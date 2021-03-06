#ifndef SEARCH_DEL_DIR_H
#define SEARCH_DEL_DIR_H

#include <QThread>
#include <QMap>
#include <QStringList>
#include <QFileInfo>

class Search_Del_Dir : public QThread
{
    Q_OBJECT

public:
    explicit Search_Del_Dir(QMap<QString, QFileInfo> src_map, QMap<QString, QFileInfo> dest_map, QObject *parent = 0);

public slots:
    void run();

signals:
    void updateSingleProgression();
    void complete(const QStringList& m_del_dir, const QStringList& m_errors);

private :
    /// input
    QMap<QString, QFileInfo> m_src_map_dir;
    QMap<QString, QFileInfo> m_dest_map_dir;

    /// method
    void run_del_dir();

    /// output
    QStringList m_del_dir;
    QStringList m_errors;
};

#endif // SEARCH_DEL_DIR_H
