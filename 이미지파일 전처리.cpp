/*#include <opencv2/opencv.hpp>
#include <iostream>


using namespace cv;
using namespace std;


int main()
{
	Mat img_color;

	img_color = imread("original.jpg", IMREAD_COLOR);
	if (img_color.empty())
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat img_gray;

	cvtColor(img_color, img_gray, COLOR_BGR2GRAY);
	imwrite("gray image.jpg", img_gray);


	namedWindow("BGR", WINDOW_AUTOSIZE);
	namedWindow("Grayscale", WINDOW_AUTOSIZE);

	imshow("BGR", img_color);
	imshow("Grayscale", img_gray);


	while (waitKey(0) != 27);

	return 0;
}
*/

// ��ķ ���� ����
/*

#include <opencv2/opencv.hpp>
#include <iostream>


using namespace cv;
using namespace std;


int main()
{
	Mat img_color;

	//���� ĸ�� �ʱ�ȭ
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cerr << "���� - ī�޶� �� �� �����ϴ�.\n";
		return -1;
	}

	// ������ ������ �����ϱ� ���� �غ�  
	Size size = Size((int)cap.get(CAP_PROP_FRAME_WIDTH),
		(int)cap.get(CAP_PROP_FRAME_HEIGHT));

	VideoWriter writer;
	double fps = 30.0;
	writer.open("output.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, size, true);
	if (!writer.isOpened())
	{
		cout << "�������� �����ϱ� ���� �ʱ�ȭ �۾� �� ���� �߻�" << endl;
		return 1;
	}


	while (1)
	{
		cap.read(img_color);
		if (img_color.empty()) {
			cerr << "�� ������ ĸ�ĵǾ����ϴ�.\n";
			break;
		}

		//������ ���Ͽ� �� �������� ������.  
		writer.write(img_color);

		imshow("Color", img_color);

		if (waitKey(25) >= 0)
			break;


	}


	return 0;
}




*/


#include "opencv2/opencv.hpp" 
#include <iostream>  
#include <string> 


using namespace cv;
using namespace std;



void setLabel(Mat& image, vector<Point> contour)
{
	int fontface = FONT_HERSHEY_SIMPLEX; //x
	double scale = 0.5; //x
	int thickness = 1; //x
	int baseline = 0; //x
	String str; //x

	Size text = getTextSize(str, fontface, scale, thickness, &baseline);
	Rect r = boundingRect(contour);

	Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	rectangle(image, pt + Point(0, baseline), pt + Point(text.width, -text.height), CV_RGB(200, 200, 200), FILLED);
	
}



int main(int, char**)
{
	Mat img_input, img_result, img_gray;



	//--- INITIALIZE VIDEOCAPTURE
	VideoCapture cap;
	// open the default camera using default API
	// cap.open(0);
	// OR advance usage: select any API backend
	int deviceID = 0;             // 0 = open default camera
	int apiID = cv::CAP_ANY;      // 0 = autodetect default API
								  // open selected camera using selected API
	cap.open(deviceID + apiID);
	// check if we succeeded
	if (!cap.isOpened()) {
		cerr << "ERROR! Unable to open camera\n";
		return -1;
	}




	for (;;)
	{

		cap.read(img_input);
		// check if we succeeded
		if (img_input.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
			break;
		}


		Mat img_temp;
		//�׷��̽����� �̹����� ��ȯ  
		cvtColor(img_input, img_temp, COLOR_BGR2GRAY);
		GaussianBlur(img_temp, img_temp, Size(5, 5), 0, 0);


		//����ȭ �̹����� ��ȯ
		adaptiveThreshold(img_temp, img_gray,
			255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV,
			201, 7);                                  // <-----------------------   parameter 1,  2


													  //contour�� ã�´�.
		vector<vector<Point> > contours;
		findContours(img_gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

		//contour�� �ٻ�ȭ�Ѵ�.
		vector<Point2f> approx;
		img_result = img_input.clone();

		for (size_t i = 0; i < contours.size(); i++)
		{
			approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

			if (fabs(contourArea(Mat(approx))) > 1000 && fabs(contourArea(Mat(approx)))< 10000)  // <-----------------------   parameter 3
			{

				int size = approx.size();
				const char *name[] = { "none", "none", "none", "none", "quadrangle", "none", "none", "none", "none", "none", "none" };

				switch (size) {

				case 3: case 4: case 5:
				case 6: case 10:
					if (isContourConvex(Mat(approx))) { // convex ���� �˻� 


														//Contour�� �ٻ�ȭ�� ������ �׸���.
						if (size  == 4) {
							line(img_result, approx[0], approx[approx.size() - 1], Scalar(0, 255, 0), 3);

							for (int k = 0; k < size - 1; k++)
								line(img_result, approx[k], approx[k + 1], Scalar(0, 255, 0), 3);

							for (int k = 0; k < size; k++)
								circle(img_result, approx[k], 3, Scalar(0, 0, 255));
						}

						//����� ������ ���� ���� ����Ѵ�.

						setLabel(img_result, contours[i]);
					}

					break;



				deafult:

					break;
				}


			}



		}


		// show window and wait for a key with timeout long enough to show images
		imshow("result", img_result);
		
		if (waitKey(5) == 27) //ESC Ű ���� �� ���� ���
			break;

	}

	return 0;
}
/*

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main() {
	Mat img;                // ������ ���� ����� ���� ��� ����
	VideoCapture cap(0);    // ķ���κ��� ������ �޾ƿ´� (Ȥ�� 1��)

	int count = 0;
	char savefile[10];        // �̹��� ���� �̸��� 200�� �̳��� �����ϱ� ���� char ���� ����

	while (1) {
		cap >> img;
		imshow("image", img);      // ���� ���    

		resize(img, img, Size(100, 100), 0, 0, INTER_CUBIC);    // �������� ������ ���� ũ�⸦ downsizing�ؼ� �����Ѵ�

		sprintf(savefile, "image%d.jpg", count++);
		imwrite(savefile, img);        // img�� ���Ϸ� �����Ѵ�.

		if (waitKey(100) == 27)  break; // esc�� ������ �����Ѵ�
	}
	return 0;
}
*/