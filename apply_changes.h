#ifndef APPLY_CHANGES_H
#define APPLY_CHANGES_H

#include <QThread>
#include <QString>
#include <QStringList>

class Apply_changes : public QThread
{
    Q_OBJECT
public:
    explicit Apply_changes(QString src_directory,
                           QString dest_directory,
                           QStringList new_dir,
                           QStringList new_files,
                           QStringList del_dir,
                           QStringList del_files,
                           qint64 size,
                           QObject *parent = 0);
    bool pause;

signals:
    void complete();
    void updateAction(const QString&);
    void updateError(const QString&);
    void updateProgression(int);

public slots:
    void run();

private :
    /// Input
    QString m_src_directory;
    QString m_dest_directory;
    QStringList m_new_dir;
    QStringList m_new_files;
    QStringList m_del_dir;
    QStringList m_del_files;
    qint64 m_size;

    /// method
    void apply_changes();

    /// output
    QStringList m_errors;
};

#endif // APPLY_CHANGES_H
