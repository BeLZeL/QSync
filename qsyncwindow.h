#ifndef QSYNCWINDOW_H
#define QSYNCWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextLine>

class QSyncWindow : public QWidget
{
    Q_OBJECT
public:
    explicit QSyncWindow(QWidget *parent = 0);

signals:

public slots:

private:
    QLabel *l_dir_src;
    QLabel *l_dir_dest;
    QLineEdit *le_dir_src;
    QLineEdit *le_dir_dest;
    QPushButton *pb_scan;
    QTextLine *tl_results;
};

#endif // QSYNCWINDOW_H
