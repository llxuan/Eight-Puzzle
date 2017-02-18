#include "dialog.h"
#include "ui_dialog.h"
#include<QDebug>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{   srand(time(NULL));
    ui->setupUi(this);
    source_picture_show=false;
    ui->spinBox->setMinimum(2);
    ui->spinBox->setMaximum(5);
    ui->spinBox->setValue(3);
    n=ui->spinBox->value();


    N=n*n;
    count=0;
    /*a=new int* [n+2];
    for(int i=0; i <n+2; i++)
        a[i]=new int[n+2];*/
    subh=127;
    subw=127;//背景图片127*127
    backg.load(":/images/background2.png");
    Backg=backg;
    setbackg();
    ui->verticalWidget->setGeometry(n*subw+10,10,150,230);

    ui->count->setEnabled(false);
    ui->count->setText("0");
    open_pix=false;
    Py_Initialize();//调用python
    if(!Py_IsInitialized())qDebug()<<"error";
    auto_startstop=false;


}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setbackg(){
    Backg=Backg.scaled( n*subw , n*subh,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPainter pb(&Backg);
    for (int i=0;i<=n*subw/127;i++ )//画n*n个背景
        for (int j=0;j<=n*subh/127;j++)
            pb.drawPixmap( i*127, j*127, backg);
}

void Dialog::paintEvent(QPaintEvent *){
    QPainter painter(this);
    int i,j,p;
    if(source_picture_show)
        painter.drawPixmap(0,0,pix);
    else{
        painter.drawPixmap(0,0,Backg);
        for(int l=0;l<pixmaps.size();l++){
            i=l/n+1;//图片表地址映射
            j=l%n+1;
            p=a[i][j]-1;
            if(p<pixmaps.size()-1)
                painter.drawPixmap((j-1)*subw,(i-1)*subh,pixmaps[p]);
        }

    }
}
void Dialog::mousePressEvent(QMouseEvent *e){
    if (source_picture_show)
        return;
    else if(!open_pix){
        return;
    }
    else
    {
        int x = e->pos().x();
        int y = e->pos().y();
        movepix(y/subh+1 , x/subw+1);
    }
}
void Dialog::movepix(int i,int j){  //行 列

    if (i<1 || j<1 || i>n || j>n)
        return;
    else{
    QSound::play(":/music/move.wav");
    if (a[i-1][j]==N)
    {

        a[i-1][j]=a[i][j];
        a[i][j]=N;
        count++;
    }
    else if (a[i+1][j]==N)
    {
        a[i+1][j]=a[i][j];
        a[i][j]=N;
        count++;
    }
    else if (a[i][j-1]==N)
    {
        a[i][j-1]=a[i][j];
        a[i][j]=N;
        count++;
    }
    else if (a[i][j+1]==N)
    {
        a[i][j+1]=a[i][j];
        a[i][j]=N;
        count++;
    }

            ui->count->setText(QString::number(count,10));
    update();
    if (finish())
    {
        if(auto_startstop){QMessageBox::about(NULL,"完成","你弱爆了！");
        QSound::play(":/music/failure.wav");
        }
        else {QMessageBox::about(NULL,"完成","厉害了我的哥！");
        QSound::play(":/music/finish.wav");
        }

    }
    }
}

void Dialog::randpix(){

    for(int i=0;i<n+2;i++)
        for(int j=0;j<n+2;j++)
        {
            if (i==0||j==0||i==n+1||j==n+1)
                a[i][j]=0;
            else
                a[i][j]=(i-1)*n+j;
        }
    int i,j;
    int p;
  /* if(n==5){
   int sequence[25]={4,22,6,21,3,20,14,17,11,19,5,7,8,9,10,23,12,25,2,16,18,13,24,15,1};
       for(int i=1;i<n+1;i++)
           for(int j=1;j<n+1;j++){
               a[i][j]=sequence[(i-1)*n+j-1];
           }

   }*/

   //else{


   for(int l=1;l<100*N;l++)
    {
        p=findblank();
        i=(p-1)/n+1;
        j=(p-1)%n+1;

        switch (rand()%4)
        {
        case 0:
            if ((i-1)>0)
                swap(a[i][j],a[i-1][j]);
            break;
        case 1:
            if ((i+1)<n+1)
                swap(a[i][j],a[i+1][j]);
            break;
        case 2:
            if ((j-1)>0)
                swap(a[i][j],a[i][j-1]);
            break;
        case 3:
            if ((j+1)<n+1)
                swap(a[i][j],a[i][j+1]);
            break;
        }
    }
     //}
}

void Dialog::initialpix(QString name){
    count=0;

    ui->count->setText("0");
    if(!pix.load(name)){
        return;
    }
    open_pix=true;

    if(n>=4)ui->hint->hide();
    else ui->hint->show();
    a=new int* [n+2];
        for(int i=0; i <n+2; i++)
            a[i]=new int[n+2];
    wsize=pix.width();
    hsize=pix.height();

    if (hsize>700)
    {
        pix=pix.scaled( wsize*700/hsize,  700, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        wsize = pix.width();
        hsize = pix.height();
    }
    else if (hsize<400)
    {
        pix=pix.scaled( wsize*400/hsize,  400, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        wsize = pix.width();
        hsize = pix.height();
    }

    subw = wsize/n;
    subh = hsize/n;
    pix=pix.copy(0,0,n*subw,n*subh);
    pixmaps.clear();
    //将图片分割
    for (int y = 0; y < n; y++)
    {
        for (int x = 0; x < n; x++)
        {
            QPixmap pieceImage = pix.copy(x*subw, y*subh, subw, subh);
            pixmaps.append(pieceImage);
        }
    }
    randpix();
    setbackg();
    //改变主窗口大小
    setGeometry( (QApplication::desktop()->width() - (n*subw+100))/2+300, (QApplication::desktop()->height() - n*subh)/2,
                 n*subw+100,n*subh);

    ui->verticalWidget->setGeometry(n*subw,10,100,230);
    update();
}

void Dialog::on_openpic_clicked()
{       QString tempfile;
        this->setFocus();
        //QFileDialog *fi=new QFileDialog;
        //fi->setFileMode(QFileDialog::AnyFile);
       //fi->setNameFilter(tr("Images (*.png *.bmp *.jpg)"));
        //fi->setFilter("Image Files (*.png *.jpg *.bmp)");
        //tempfile=fi->getOpenFileName(this);
        n=ui->spinBox->value();
        backg.load(":/images/background3.png");
        Backg=backg;
        N=n*n;
        if(n<=3){
        tempfile=":/images/yjb.jpg";}
        else if(n==4) tempfile=":/images/h1.jpg";
        else tempfile=":/images/h2.png";

    initialpix(tempfile);
}
int Dialog::findblank(){
    for (int i=1;i<=n;i++)
        for (int j=1;j<=n;j++)
            if (a[i][j]==N)
                return n*(i-1)+j;
}
void Dialog::swap(int &a, int &b){
    int t;
    t=a;
    a=b;
    b=t;
}

void Dialog::on_showpic_clicked()
{
    if (source_picture_show)
    {
        ui->showpic->setText(tr("显示原图"));
        source_picture_show=false;
        update();
    }
    else
    {
        ui->showpic->setText(tr("回到游戏"));
        source_picture_show=true;
        update();
    }
}
bool Dialog::finish(){
    bool good=true;
    for (int i=1;i<=n;i++)
        for (int j=1;j<=n;j++)
            good=good&&(a[i][j]==n*(i-1)+j);
    return good;
}

void Dialog::on_autofinish_clicked()
{   if(!open_pix)return;
    if(!auto_startstop){ui->autofinish->setText("停止");
    auto_startstop=true;}
    else {
        auto_startstop=false;
        ui->autofinish->setText("自动");
        return;
    }
    PyObject *pModule = NULL;
    PyObject *pFunc = NULL;
    PyObject *pReturn = NULL;
    PyObject *pyParams = PyTuple_New(1);
    PyObject *pyValue  = PyList_New(0);
    PyObject*lis=PyList_New(0);
    autostep.clear();
    int row,column;
    for( row=1;row<=n;row++){
       for(column=1;column<=n;column++){qDebug()<<a[row][column];}}
    for( row=1;row<=n;row++){
       for(column=1;column<=n;column++){
           if(a[row][column]==N)PyList_Append(lis,Py_BuildValue("i",0));
           else PyList_Append(lis,Py_BuildValue("i",a[row][column]));


       }
       PyList_Append(pyValue,lis);
       lis=PyList_New(0);

    }
   qDebug()<<"-----";
     // PyObject* pName = NULL;


    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('/Users/apple/Desktop/MIthird')"); //python源文件所在的路径


    pModule = PyImport_ImportModule("eightDigits");
    if(!pModule)qDebug()<<"module error";
    pFunc = PyObject_GetAttrString(pModule,"solveDigit");

     PyTuple_SetItem(pyParams,0,pyValue);
    pReturn = PyEval_CallObject(pFunc, pyParams);
    int len=PyObject_Size(pReturn);
    qDebug()<<"changdu:"<<len;
    PyObject*list_item;

    for(int i=0;i<len;i++){
        list_item=PyList_GetItem(pReturn,i);
        autostep.append( PyLong_AsLong(list_item));
        qDebug()<<PyLong_AsLong(list_item);
    }

     autofinish();


}
void Dialog::autofinish(){
    QElapsedTimer t;
    int timep=300;
    if(n==5)timep=100;
    else if(n==4)timep=200;
    for(int i=0;i<autostep.length()&&auto_startstop;i++){
        for(int p=1;p<=n;p++){
            for(int q=1;q<=n;q++){
                if(a[p][q]==autostep[i]){movepix(p,q);
                    p=n+1;
                   // qDebug()<<p<<q;
                    break;}
            }
        }
        t.start();
        while(t.elapsed()<timep)
        {
            QCoreApplication::processEvents();
        }
    }
    auto_startstop=false;
    ui->autofinish->setText("自动");
}

void Dialog::on_hint_clicked()
{
    PyObject *pModule = NULL;
    PyObject *pFunc = NULL;
    PyObject *pReturn = NULL;
    PyObject *pyParams = PyTuple_New(1);
    PyObject *pyValue  = PyList_New(0);
    PyObject*lis=PyList_New(0);
    autostep.clear();
    int row,column;
    for( row=1;row<=n;row++){
       for(column=1;column<=n;column++){qDebug()<<a[row][column];}}
    for( row=1;row<=n;row++){
       for(column=1;column<=n;column++){
           if(a[row][column]==N)PyList_Append(lis,Py_BuildValue("i",0));
           else PyList_Append(lis,Py_BuildValue("i",a[row][column]));


       }
       PyList_Append(pyValue,lis);
       lis=PyList_New(0);

    }
   qDebug()<<"-----";
     // PyObject* pName = NULL;


    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('/Users/apple/Desktop/MIthird')"); //python源文件所在的路径


    pModule = PyImport_ImportModule("eightDigits");
    if(!pModule)qDebug()<<"module error";
    pFunc = PyObject_GetAttrString(pModule,"solveDigit");

     PyTuple_SetItem(pyParams,0,pyValue);
    pReturn = PyEval_CallObject(pFunc, pyParams);
    int len=PyObject_Size(pReturn);
    qDebug()<<"changdu:"<<len;
    PyObject*list_item;

        list_item=PyList_GetItem(pReturn,0);
        autostep.append( PyLong_AsLong(list_item));
        qDebug()<<PyLong_AsLong(list_item);


            for(int p=1;p<=n;p++){
                for(int q=1;q<=n;q++){
                    if(a[p][q]==autostep[0]){movepix(p,q);
                        p=n+1;
                       // qDebug()<<p<<q;
                        break;}
                }
            }




}
void Dialog::keyPressEvent(QKeyEvent *e){
    int p,i,j;
    int keyValue = e->key();
    p=findblank();
    i=(p-1)/n+1;
    j=(p-1)%n+1;
       if(keyValue == Qt::Key_Left ){
           if(j+1<=n)movepix(i,j+1);

       }else if(keyValue == Qt::Key_Up){
           if(i+1<=n)movepix(i+1,j);
       }
       else if(keyValue== Qt::Key_Right){
           if(j-1>0)movepix(i,j-1);
       }
       else if(keyValue==Qt::Key_Down){
           if(i-1>0)movepix(i-1,j);
       }
}
