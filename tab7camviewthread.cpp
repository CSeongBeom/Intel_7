#include "tab7camviewthread.h"
#include "ui_tab7camviewthread.h"
Tab7CamViewThread::Tab7CamViewThread(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tab7CamViewThread)
{
    ui->setupUi(this);
    setWindowTitle("CamViewer");
    ui->pPBsnapShot->setEnabled(false);
    ui->pCBrgb->setEnabled(false);
    pWebCamThread = new WebCamThread(this);
    pWebCamThread->pCamView = ui->plabelCamView;
}

Tab7CamViewThread::~Tab7CamViewThread()
{
    delete ui;
}

void Tab7CamViewThread::on_pPBcamStart_clicked(bool checked)
{
    if(checked)
    {
        pWebCamThread->camViewFlag = true;
        if(!pWebCamThread->isRunning())
        {
            pWebCamThread->start(); //run함수 실행
            ui->pPBcamStart->setText("CamStop");
            ui->pPBsnapShot->setEnabled(true); //실행중일때만 캡처 가능하게끔
        }

    }
    else
    {
        pWebCamThread->camViewFlag = false;
        ui->pPBcamStart->setText("CamStart");
        ui->pPBsnapShot->setEnabled(false);
    }
    ui->pCBrgb->setEnabled(checked);
}

void Tab7CamViewThread::on_pPBsnapShot_clicked()
{
    pWebCamThread->snapShot();
}

void Tab7CamViewThread::on_pCBrgb_clicked(bool checked)
{
    if(checked)
        pWebCamThread->rgbTimerStart();
    else
        pWebCamThread->rgbTimerStop();
}
WebCamThread *Tab7CamViewThread::getpWebCamThread()
{
    return pWebCamThread;
}
