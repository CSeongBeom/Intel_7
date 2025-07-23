#include "webcamthread.h"

WebCamThread::WebCamThread(QObject *parent)
    : QThread{parent}
{
    camViewFlag = false;
    cnt = 0;
    strColor = "NONE";
    strColorPre = "NONE";
    rgbClassifyFlag = false;
    pQTimer = new QTimer(this);
    connect(pQTimer,SIGNAL(timeout()), this, SLOT(rgbClassifySlot()));
}

void WebCamThread::run()
{
    VideoCapture capture(0);
    int value = -1;
    if (!capture.isOpened())
    {
        cout << "카메라가 연결되지 않았습니다." << endl;
        exit(1);
    }
    while(camViewFlag) {
        capture.read(frame);

        fname = "cam_" + to_string(cnt++);
        fname += ".jpg";
        cvtColor(frame, frameQt, COLOR_BGR2RGB); //opencv 함수, opencv이미지를 QT용 이미지 포맷으로 변경(BGR->RGB);

//        imshow("카메라 영상보기", frame);
        //opencv의 라이브버리인 imshow를 쓰레드로 실행시키면 중간에 멈춤 ->Qt의 이미지 라이브러리 사용
        int x = frameQt.cols / 2;
        int y = frameQt.rows / 2;
        if(rgbClassifyFlag)
        {
            Scalar meanHsv;
            Mat frameRoi, hsvImage;
            frameRoi =frame(Rect((x-32), (y-32), 64, 64)); //Rect(시작점, x축 폭, y축 폭);
            cvtColor(frameRoi,hsvImage, COLOR_BGR2HSV);// 이미지의 RGB를 HSV값으로 변경
            meanHsv = mean(hsvImage); //사각형의 HSV 평균을 구함

            //qDebug() << "meanHSV H :" << meanHsv[0] << " S : " << meanHsv[1] << " V : " << meanHsv[2];
            if( 170 <= meanHsv[0] || meanHsv[0] < 10)
                strColor = "RED";
            else if( 50 <= meanHsv[0] && meanHsv[0] < 70)
                strColor = "GREEN";
            else if( 110 <= meanHsv[0] && meanHsv[0] < 130)
                strColor = "BLUE";
            else
                strColor = "NONE";

            if(strColor != strColorPre)
            {
                emit socketSendDataSig("[CSB_LIN]COLOR@"+strColor);
                strColorPre = strColor;
            }

            rgbClassifyFlag = false; //한번 실행하고 꺼진 뒤 다음 타임아웃에 실행되도록 설정
            qDebug() << strColor;
        }
        put_string(frameQt, strColor.toStdString(), Point(10, 40));

        line(frameQt, Point((x-32),y), Point((x+32), y), Scalar(255,0,0), 2); //opencv함수, 이미지의 중심에 십자 표시 (Point는 시작과 끝점, Scalar는 색상)
        line(frameQt, Point(x,(y-32)), Point(x, (y+32)), Scalar(255,0,0), 2);
        rectangle(frameQt, Point((x-32), (y-32)), Point((x+32), (y+32)), Scalar(0,255,0), 2); //사각형 표시
        qImage = QImage(frameQt.data, frameQt.cols,  frameQt.rows, QImage::Format_RGB888); //QT는 RGB 포맷, opencv는 BGR (BGR각각 8비트씩)
        pCamView->setPixmap(QPixmap::fromImage(qImage));





    }
    capture.release();
    pCamView->setPixmap(QPixmap(":/Images/Images/initDisplay.png"));
    //Resource에 있는 파일 경로는 우클릭하고 경로 복사 가능
}
// 문자열 출력 함수 - 그림자 효과
void WebCamThread::put_string(Mat &frame, string text, Point pt, int value)
{
    Scalar colorScalar;
    if(value != -1)
        text += to_string(value);
    if(text ==  "RED")
        colorScalar = {255,0,0};
    else if (text == "GREEN")
        colorScalar = {0,255,0};
    else if (text == "BLUE")
        colorScalar = {0,0,255};
    else
        colorScalar = {100,100,100};
    Point shade = pt + Point(2, 2);
    int font = FONT_HERSHEY_SIMPLEX;
    putText(frame, text, shade, font, 0.7, Scalar(0, 0, 0), 2);    // shade 그림자 효과
    putText(frame, text, pt, font, 0.7, colorScalar, 2);  // 작성 문자
}

void WebCamThread::snapShot()
{
    //imwrite(fname,frame); //opencv 함수인 imwrite도 가능, QT 명령어로도 저장해보자
    qImage.save(QString::fromStdString(fname),"JPG",80); // 3번째 인덱스는 압축률(100이면 원본저장)
    //qDebug() << "capture";
}

void WebCamThread::rgbTimerStart()
{
    pQTimer->start(1000); //1초마다
}
void WebCamThread::rgbTimerStop()
{
    if(pQTimer->isActive())
        pQTimer->stop();
}
void WebCamThread::rgbClassifySlot()
{
    rgbClassifyFlag = true;
}
