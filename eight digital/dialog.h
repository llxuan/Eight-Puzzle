#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QPainter>
#include<QMouseEvent>
#include<QMessageBox>
#include<QFileDialog>
#include <QtWidgets/QApplication>
#include<QDesktopWidget>
#include   <vector>
#include<ctime>
#include<Python.h>
#include<QElapsedTimer>
#include<QKeyEvent>
#include<QSound>
#include<QFuture>
#include<QFutureWatcher>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void setbackg();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void initialpix(QString name);
    void randpix();
    void movepix(int i,int j);
    int findblank();
    void swap(int &a,int &b);
    bool finish();
    void autofinish();

private slots:
    void on_openpic_clicked();

    void on_showpic_clicked();

    void on_autofinish_clicked();

    void on_hint_clicked();

private:
    Ui::Dialog *ui;
    QPixmap pix;//图片
    QPixmap backg;//小背景
    QPixmap Backg;//多张小背景合成大背景
    bool source_picture_show;
    QList<QPixmap>pixmaps;//保存分割了的图片
    int n;//n*n分割
    int N;//空白块
    int **a;//记录i行j列的内容
    int subw;//分割每个图片的宽
    int subh;//分割每个图片的高
    int wsize;//打开的图片的大小
    int hsize;
    bool open_pix;//判断是否打开了图片
    int count;//记录次数
    QList<int>autostep;//自动模式的步骤
    bool auto_startstop;
   // QFuture *future;
    //QFutureWatcher* FutureWatcher;

};

#endif // DIALOG_H
