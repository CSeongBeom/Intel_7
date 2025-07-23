#ifndef WEBCAMTHREAD_H
#define WEBCAMTHREAD_H

#include <QThread>
#include <opencv2/opencv.hpp>
#include <QLabel>
#include <QTimer>

using namespace cv;
using namespace std;
class WebCamThread : public QThread
{
    Q_OBJECT
    void run();
    int cnt;
    string fname;
    QString strColor, strColorPre;
    Mat frame, frameQt; //opencv 라이브러리, namespace cv 필요
    QImage qImage;
    QTimer *pQTimer;
    bool rgbClassifyFlag;
    void put_string(Mat &frame, string text, Point pt, int value = -1);

public:

    explicit WebCamThread(QObject *parent = nullptr);
    bool camViewFlag;
    QLabel *pCamView;
    void snapShot();
    void rgbTimerStart();
    void rgbTimerStop();
private slots:
    void rgbClassifySlot();
signals:
    void socketSendDataSig(QString);

};

#endif // WEBCAMTHREAD_H
