#include<iostream> 
#include<opencv2/core.hpp> 
//#include<opencv/cv.h> 
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/highgui.hpp> 
#include<opencv2/imgproc.hpp> 
#include <opencv2/plot.hpp>
#include <windows.h>
#include <thread>
#include <chrono>
#include <vector>
#include <ctime>
using namespace cv;
using namespace std;


vector<int> containValues;
Point pt1(1, 1);
Point pt2(1, 1);
int cnt = 0;
bool m_bMouseClick1 = false;
bool m_bMouseClick2 = false;

void mouse_callback(int  event, int  x, int  y, int  flag, void* param)
{
	if (event == EVENT_LBUTTONDOWN && cnt == 0)
	{
		pt1.x = 2*x;
		pt1.y = 2*y;
		cnt++;
		cout << "pt1 :" << pt1.x << " And " << pt1.y << endl;
		m_bMouseClick1 = true;
	}
	else if (event == EVENT_LBUTTONDOWN && cnt == 1)
	{
		pt2.x = 2*x;
		pt2.y = 2*y;
		cnt++;
		cout << "pt2:" << pt2.x << " AND "<< pt2.y << endl;
		m_bMouseClick2 = true;
	}


}


int main() {

	Mat xData, yData, display, scaled;
	double xgap = 0;
	Ptr<plot::Plot2d> plot;




	//Mat image = imread("Resources/(1,18)HDR.bmp", 0);
	Mat image = imread("Resources/Clahe(Gamma)_116.bmp", 0);
	if (!image.data) { cout << "Error: cannot load the image" << endl; return 0; }
	

	

	resize(image, scaled, Size(), 0.5, 0.5);
	

	imshow("image", scaled);
	namedWindow("image", 1);

	

	setMouseCallback("image", mouse_callback);
	

	while (true)
	{
		


		//cout << "LOOP " << m_bMouseClick1 << " and " << m_bMouseClick2 << endl;
		if (m_bMouseClick1 == true && m_bMouseClick2 == true)
		{
			if (pt1.x < pt2.x) {
				break;
			}
			else {
				cout << "Try again, First pt = left, and Second pt = right" << endl;
				cnt = 0;
				m_bMouseClick1 == false;
				m_bMouseClick2 == false;
			}
		}

		waitKey(1);
		Sleep(10);
	}
	clock_t start;

	start = clock();


	//xgap = (double)(pt2.x - pt1.x);

	xData.create(1, 2352, CV_64F);//1 Row, 100 columns, Double
	yData.create(1, 2352, CV_64F);

	
	

	int slope = (int)((pt2.y - pt1.y) / (pt2.x - pt1.x));

	

	for (int i = 0; i < 2352; i++) {
		xData.at<double>(i) = i;
		
	}

	int tempPt2Y = pt1.y;

	for (int i = pt1.x; i < pt2.x; i++) {
		yData.at<double>(i) = ((int)image.at<uchar>(Point(i, tempPt2Y))) * -1;

		containValues.push_back(yData.at<double>(i));

		tempPt2Y += slope;

		//cout << "Value :" << yData.at<double>(i) << endl;

	}

	vector<int>::iterator itor = containValues.begin();

	/*for (; itor != containValues.end(); itor++) {
		cout << "Vector Values :" << *itor << endl;
	}

	cout << "Image Width:" << (double)image.size().width << endl;*/






	plot = plot::Plot2d::create(xData, yData);
	plot->setPlotSize(800, 800);
	plot->setMaxX((int)pt2.x-1);
	plot->setMinX((int)pt1.x+1);
	plot->setMaxY(0);
	plot->setMinY(-300);
	plot->render(display);


	putText(display, "255", Point(0, 115), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255), 1);
	line(display, Point(0, 118), Point(2352, 118), Scalar(0, 0, 255), 2);

	cout << "Time of operation: " << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;

	imshow("Plot", display);



	Point p1(pt1.x, pt1.y), p2(pt2.x, pt2.y);

	int thickness = 6;


	line(image,p1 , p2, Scalar(0, 255, 255),
		thickness, LINE_8);




	resize(image, image, Size(), 0.3, 0.3);

	imshow("img", image);

	waitKey();

}
