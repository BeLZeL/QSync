#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QMap>
#include <QStringList>
#include <QFileInfo>

class MyThread : public QThread
{
    Q_OBJECT

public:
    explicit MyThread(int mode, QObject *parent = 0);
    void setArgs(QMap<QString, QFileInfo> src_map_files, QMap<QString, QFileInfo> dest_map_files);

public slots:
    void run();

signals:
    void complete(const QStringList& s);

private:
    QStringList run_del_files(const QMap<QString, QFileInfo>& src_map_files, const QMap<QString, QFileInfo>& dest_map_files);
    QStringList run_del_dir(const QMap<QString, QFileInfo>& src_map_dir, const QMap<QString, QFileInfo>& dest_map_dir);
    QStringList run_new_dir(const QMap<QString, QFileInfo>& src_map_dir, const QMap<QString, QFileInfo>& dest_map_dir);
    QStringList run_new_files(const QMap<QString, QFileInfo>& src_map_files, const QMap<QString, QFileInfo>& dest_map_files);

    int m_mode;
    QMap<QString, QFileInfo> m_src_map_files;
    QMap<QString, QFileInfo> m_dest_map_files;
};

#endif // MYTHREAD_H
