#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include<iostream>
#include<vector>

using namespace cv;
using namespace std;
#define XMAX 100
#define YMAX 100
#define RMAX 100
#define THMAX 180
#define AMAX 40
#define BMAX 40
int voteArray[181] = { 0 };
int finalArray[181] = { 0 };
int at = 0;
int at1 = 0;
int at2 = 0;

int Circle[XMAX][YMAX][RMAX];

int Ellipse[XMAX][YMAX][AMAX][BMAX];
int Ellipse2[THMAX][AMAX][BMAX];
Mat element = Mat::ones(3, 3, CV_8UC1); //@comment 追加　3×3の行列で要素はすべて1　dilate処理に必要な行列

void colorExtraction(cv::Mat* src, cv::Mat* dst,
	int code,
	int ch1Lower, int ch1Upper, //@comment H(色相)　最小、最大
	int ch2Lower, int ch2Upper, //@comment S(彩度)　最小、最大
	int ch3Lower, int ch3Upper  //@comment V(明度)　最小、最大
	);

//vector<vector<vector<int> > > vc(a, vector<vector<int> >(b, vector<int>(c))); // vc[a][b][c]と同じ
class El{
private:
	int x, y, th, a, b;
	int count;
public:
	El(int x, int y, int th, int a, int b);
	int get_x();
	int get_y();
	int get_th();
	int get_a();
	int get_b();
	int get_count();
	int set_count(int num);

	vector<vector<int>> sss;
};


El::El(int x, int y, int th, int a, int b){
	this->x = x;
	this->y = y;
	this->th = th;
	this->a = a;
	this->b = b;

}

int El::get_x(){
	return x;
}
int El::get_y(){
	return y;
}
int El::get_th(){
	return th;
}
int El::get_a(){
	return a;
}
int El::get_b(){
	return b;
}
int El::get_count(){
	return count;
}
int El::set_count(int num){
	count = num;
	return 1;
}

int a = 30;
int b = 50;
int theta = 10;
int X = 0;
int Y = 0;
int rex = 0;
int rey = 0;


vector<Point2i> center;
vector<int> th;
vector<Point2i> ax;
vector<Vec3i> ell;
//Mat element = Mat::ones(3, 3, CV_8UC1); //@comment 追加　3×3の行列で要素はすべて1　dilate処理に必要な行列
int main(){
	vector<El> v;
	vector<vector<vector<vector<vector<int> > > > >
		elll(1, vector<vector<vector<vector<int> > > >
		(1, vector<vector<vector<int> > >
		(1, vector<vector<int> >
		(1, vector<int>(1)))));

	Mat src = imread("../ellipse.png", 1);
	Mat origin, origin2;
	Mat gaus, lapla;
	Mat canny;
	Mat gray;

	vector<Vec3f> circles;
	vector<Vec4i> lines;

	if (!src.data) return -1;


	resize(src, src, Size(), 0.5, 0.5);

	colorExtraction(&src, &src, CV_BGR2HSV, 0, 180, 100, 255, 100, 255);

	src.copyTo(origin);
	src.copyTo(origin2);
	GaussianBlur(src, gaus, Size(7, 7), 2, 2);
	//cv::imshow("Gaussian", gaus);

	Canny(gaus, canny, 40, 220, 3);
	dilate(canny, canny, element, Point(-1, -1), 8);
	//erode(canny, canny, element, Point(-1, -1), 7);
	resize(canny, canny, Size(), 0.05, 0.05);
	//dilate(canny, canny, element, Point(-1, -1), 1); //@comment 膨張処理

	cv::imshow("canny", canny);
	waitKey(10);
	cvtColor(gaus, gaus, CV_BGR2GRAY);


	float tan, tan1, tan2;

	std::cout << canny.cols << endl;
	std::cout << canny.rows << endl;
	for (int j = 0; j < canny.rows; j++){
		for (int i = 0; i < canny.cols; i++){
			std::cout << " " << canny.at<unsigned char>(j, i);

		}
		std::cout << endl;
	}

	vector<Point2i> ce;

	//(xcosθ+ysinθ)^2 / a^2 + (-xsinθ + ycosθ)^2 /b ^2 = 1
	int arr[1][1][1][1][1][1];
	int flag = 0;

	int num = 0;
	int maxnum = 50;
	std::cout << "start" << endl;


	std::cout << canny.cols << endl;
	std::cout << canny.rows << endl;

	cv::Mat image = Mat::zeros(canny.cols, canny.rows, CV_8UC3);
	cv::Mat image2;
	canny.copyTo(image2);
	//cv::resize(image2,image2,Size(),2,2);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int y = 0; y < canny.rows; y += 1){
		std::cout << "test " << y << endl;
		for (int x = 0; x < canny.cols; x += 1){
			for (int l = 0; l < THMAX; l += 1){
				for (int m = 1; m < 60; m += 1){
					for (int n = 1; n < 30; n += 1){
						////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						El el = El(x, y, l, m, n);
						for (int o = 0; o < 360; o += 1){
							X = (int)(m)*cos(o*CV_PI / 180) + x;
							Y = (int)(n)*sin(o*CV_PI / 180) + y;
							rex = (int)X*cos(l*CV_PI / 180) - Y*sin(l*CV_PI / 180);
							rey = (int)X*sin(l*CV_PI / 180) + Y*cos(l*CV_PI / 180);
							/*
							if (rex >= 0 && rex < canny.cols  && rey >= 0 && rey < canny.rows){
							image2.at<cv::Vec3b>(rey, rex)[0] = 255;
							image2.at<cv::Vec3b>(rey, rex)[1] = 255;
							image2.at<cv::Vec3b>(rey, rex)[2] = 255;
							//cv::resize(image2,image2,Size(),2,2);
							cv::imshow("image2", image2);
							cv::waitKey(100);
							}*/
							if (rex >= 0 && rex < canny.cols  && rey >= 0 && rey < canny.rows && (int)canny.at<unsigned char>(rey, rex) >= 255){
								num++;
								//std::cout << "X : " << rex << "Y : " << rey << endl;
								//image.at<cv::Vec3b>(rey, rex)[0] = 255;
								//image.at<cv::Vec3b>(rey, rex)[1] = 255;
								//image.at<cv::Vec3b>(rey, rex)[2] = 255;
								//cv::imshow("image", image);
								//cv::waitKey(100);
							}
						}

						if (num > maxnum){
							//std::cout << num << endl;
							el.set_count(num);
							v.push_back(el);
							/*
							std::cout << "x " << el.get_x() << " y " << el.get_y()
								<< " th " << el.get_th() << " a " << el.get_a()
								<< " b " << el.get_b() << " count " << el.get_count() << endl;
							*/
							cv::ellipse(image2, Point(el.get_x(), el.get_y()), Size(el.get_a(), el.get_b()), el.get_th(), 0, 360, Scalar(255, 255, 255), 1, 2);
							cv::circle(image2,Point(el.get_x(),el.get_y()),1,Scalar(255,255,255),-1,CV_AA);
							imshow("origin", image2);
							//cvWaitKey(10);
						}
						num = 0;
						////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					}
				}
			}
		}
	}


	std::cout << "v size : " << v.size() << endl;
	vector<El>::iterator it;
	for (it = v.begin(); it != v.end(); it++){
		std::cout << "x " << it->get_x() << " y " << it->get_y()
			<< " th " << it->get_th() << " a " << it->get_a()
			<< " b " << it->get_b() << " count " << it->get_count() << endl;
		cv::ellipse(image2, Point(it->get_x(), it->get_y()), Size(it->get_a(), it->get_b()), it->get_th(), 0, 360, Scalar(255, 255, 255), 1, 2);
		imshow("origin", image2);
	}
	//resize(origin,origin,Size(),5,5);






	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




	

	cv::waitKey(0);
	return 0;
}



void colorExtraction(cv::Mat* src, cv::Mat* dst,
	int code,
	int ch1Lower, int ch1Upper, //@comment H(色相)　最小、最大
	int ch2Lower, int ch2Upper, //@comment S(彩度)　最小、最大
	int ch3Lower, int ch3Upper  //@comment V(明度)　最小、最大
	)
{
	cv::Mat colorImage;
	int lower[3];
	int upper[3];

	cv::Mat lut = cv::Mat(256, 1, CV_8UC3);

	cv::cvtColor(*src, colorImage, code);

	lower[0] = ch1Lower;
	lower[1] = ch2Lower;
	lower[2] = ch3Lower;

	upper[0] = ch1Upper;
	upper[1] = ch2Upper;
	upper[2] = ch3Upper;

	for (int i = 0; i < 256; i++) {
		for (int k = 0; k < 3; k++) {
			if (lower[k] <= upper[k]) {
				if ((lower[k] <= i) && (i <= upper[k])) {
					lut.data[i*lut.step + k] = 255;
				}
				else {
					lut.data[i*lut.step + k] = 0;
				}
			}
			else {
				if ((i <= upper[k]) || (lower[k] <= i)) {
					lut.data[i*lut.step + k] = 255;
				}
				else {
					lut.data[i*lut.step + k] = 0;
				}
			}
		}
	}
	//@comment LUTを使用して二値化
	cv::LUT(colorImage, lut, colorImage);

	//@comment Channel毎に分解
	std::vector<cv::Mat> planes;
	cv::split(colorImage, planes);
	//imshow("test", planes[2]);
	//@comment マスクを作成
	cv::Mat maskImage;
	cv::bitwise_and(planes[0], planes[1], maskImage);
	//imshow("test2", maskImage);
	cv::bitwise_and(maskImage, planes[2], maskImage);
	//imshow("test3", maskImage);
	dilate(maskImage, maskImage, element, Point(-1, -1), 25);
	erode(maskImage, maskImage, element, Point(-1, -1), 25);
	//@comemnt 出力
	cv::Mat maskedImage;
	src->copyTo(maskedImage, ~maskImage);
	*dst = maskedImage;

}
