#ifndef TAB7CAMVIEWERTHREAD_H
#define TAB7CAMVIEWERTHREAD_H

#include <QLabel>
#include <QThread>
#include <QWidget>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
namespace Ui {
class Tab7CamViewerThread;
}

class Tab7CamViewerThread : public QThread
{
    Q_OBJECT
    void run();
    int cnt;

    string fname;
    Mat frame;

    void put_string(Mat &frame, string text, Point pt, int value);

public:
    explicit Tab7CamViewerThread(QWidget *parent = nullptr);
    ~Tab7CamViewerThread();

private slots:
    void on_pPBcamStart_clicked(bool checked);

    void on_pPBsnapShot_clicked();

private:
    Ui::Tab7CamViewerThread *ui;
    bool camViewFlag;
    QLabel *pCamView;
    void snapShot();
};

class Tab7CamViewerWidget : public QWidget
{};

#endif // TAB7CAMVIEWERTHREAD_H
