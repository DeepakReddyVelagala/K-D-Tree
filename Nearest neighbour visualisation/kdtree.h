#ifndef KDTREE_H
#define KDTREE_H

#include <QDialog>
#include <knn.h>
#include <QPainter>

namespace Ui {
class kdtree;
}

class kdtree : public QDialog
{
    Q_OBJECT

public:
    explicit kdtree(QWidget *parent = nullptr);
    ~kdtree();

private slots:
    void on_pushButton_clicked();
    void paintEvent(QPaintEvent *point);

private:
    Ui::kdtree *ui;
    knn *neigh;
};

#endif // KDTREE_H
