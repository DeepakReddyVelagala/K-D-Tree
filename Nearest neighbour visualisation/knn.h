#ifndef KNN_H
#define KNN_H

#include <QDialog>
#include <QPainter>
namespace Ui {
class knn;
}

class knn : public QDialog
{
    Q_OBJECT

public:
    explicit knn(QWidget *parent = nullptr);
    ~knn();

private:
    Ui::knn *ui;

private slots:
    void paintEvent(QPaintEvent *final);

};

#endif // KNN_H
