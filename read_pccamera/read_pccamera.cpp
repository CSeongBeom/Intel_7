#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

// 문자열 출력 함수 - 그림자 효과 
void put_string(Mat &frame, string text, Point pt, int value)
{
	text += to_string(value);
	Point shade = pt + Point(2, 2);
	int font = FONT_HERSHEY_SIMPLEX;
	putText(frame, text, shade, font, 0.7, Scalar(0, 0, 0), 2);		// 그림자 효과 	
	putText(frame, text, pt, font, 0.7, Scalar(120, 200, 90), 2);// 작성 문자
}

int main()
{
	TickMeter tm;
	int cnt=0;
	string fname;
	VideoCapture  capture(0);
	if (!capture.isOpened())
	{
		cout << "카메라가 연결되지 않았습니다." << endl;
		exit(1);
	}
	capture.set(CAP_PROP_FRAME_WIDTH,640);
	capture.set(CAP_PROP_FRAME_HEIGHT,480);
//	capture.set(CAP_PROP_FPS,30);
	cout << "너비 " << capture.get(CAP_PROP_FRAME_WIDTH) << endl;
	cout << "높이 " << capture.get(CAP_PROP_FRAME_HEIGHT) << endl;
	cout << "노출 " << capture.get(CAP_PROP_EXPOSURE) << endl;
	cout << "밝기 " << capture.get(CAP_PROP_BRIGHTNESS) << endl;
	for (;;) {
//		tm.start();	
		Mat frame;
		capture.read(frame);

		put_string(frame, "Count: ", Point(10, 40), cnt);
		fname = "cam_" + to_string(cnt++);
		fname += ".jpg";
		imshow("카메라 영상보기", frame);
		int key = waitKey(33);
		if(key == 's') //115
			imwrite(fname,frame);
		else if(key == 'b')  //98
		{
			cout << "key : " << key << endl;
			break;
		}
//		tm.stop();
// 		cout << tm.getTimeMilli() << endl;
//		cout << tm.getTimeMicro() << endl;
	}
	capture.release();
	return 0;
}
