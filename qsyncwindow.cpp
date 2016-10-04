#include "qsyncwindow.h"
#include <QVBoxLayout>

QSyncWindow::QSyncWindow(QWidget *parent) : QWidget(parent)
{
    l_dir_src = new QLabel();
    l_dir_dest = new QLabel();
    le_dir_src = new QLineEdit();
    le_dir_dest = new QLineEdit();
    pb_scan = new QPushButton();
    tl_results = new QTextLine();

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(l_dir_src);
    vbox->addWidget(l_dir_dest);
    vbox->addWidget(le_dir_src);
    vbox->addWidget(le_dir_dest);
    vbox->addWidget(pb_scan);
    //vbox->addWidget(tl_results);

    setLayout(vbox);
}
