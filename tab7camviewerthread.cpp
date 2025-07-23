#include "tab7camviewerthread.h"
#include "ui_tab7camviewerthread.h"

Tab7CamViewerThread::Tab7CamViewerThread(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tab7CamViewerThread)
{
    ui->setupUi(this);
    camViewFlag = false;
    cnt = 0;
}

void Tab7CamViewerThread::run()
{
    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cout << "카메라가 연결되지 않았습니다." << endl;
        exit(1);
    }
    while (camViewFlag) {
        capture.read(frame);

        put_string(frame, "Count: ", Point(10, 40), cnt);
        fname = "cam_" + to_string(cnt++);
        fname += ".jpg";
        //        imshow("카메라 영상보기", frame);
        QImage qImage(frame.data,
                      frame.cols,
                      frame.rows,
                      QImage::Format_BGR888); //QT는 RGB, opencv는 BGR (BGR각각 8비트씩)
        pCamView->setPixmap(QPixmap::fromImage(qImage));
        // int key = waitKey(33);
        // if(key == 's') //115
        //     imwrite(fname,frame);
        // else if(key == 'b')  //98
        // {
        //     cout << "key : " << key << endl;
        //     break;
        // }
    }
    capture.release();
    pCamView->setPixmap(QPixmap("initDisplay.png"));
}
// 문자열 출력 함수 - 그림자 효과
void Tab7CamViewerThread::put_string(Mat &frame, string text, Point pt, int value)
{
    text += to_string(value);
    Point shade = pt + Point(2, 2);
    int font = FONT_HERSHEY_SIMPLEX;
    putText(frame, text, shade, font, 0.7, Scalar(0, 0, 0), 2);   // 그림자 효과
    putText(frame, text, pt, font, 0.7, Scalar(120, 200, 90), 2); // 작성 문자
}

void Tab7CamViewerThread::snapShot()
{
    imwrite(fname, frame);
}

Tab7CamViewerThread::~Tab7CamViewerThread()
{
    delete ui;
}

void Tab7CamViewerThread::on_pPBcamStart_clicked(bool checked)
{
    if (checked) {
        pTab7->camViewFlag = true;
        if (!->isRunning()) {
            pWebCamThread->start(); //run함수 실행
            ui->pPBcamStart->setText("CamStop");
            ui->pPBsnapShot->setEnabled(true);
        }

    } else {
        pWebCamThread->camViewFlag = false;
        ui->pPBcamStart->setText("CamStart");
        ui->pPBsnapShot->setEnabled(false);
    }
}

void Tab7CamViewerThread::on_pPBsnapShot_clicked()
{
    pWebCamThread->snapShot();
}
