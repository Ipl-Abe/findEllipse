#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include<iostream>
#include<vector>
#include <math.h>
#include <stdio.h>



using namespace cv;
using namespace std;


int eight_neighbor(Point2i ce, int x, int y);

int main()
{
	vector<Point2i> E;
	Mat src(30,30,CV_8UC3);
	Mat canny;
	Mat gray;
	Mat gaus;
	ellipse(src, Point(15, 15),Size(10,5),0,0,360,Scalar(255,0,0),-1 );
	
	cvtColor(src,gray,CV_BGR2GRAY);

	GaussianBlur(gray, gaus, Size(7, 7), 2, 2);

	Canny(gaus,canny,50,200);
	for (int j = 0; j < canny.rows; j++){
		const short* p = canny.ptr<short>(j);
		for (int i = 0; i < canny.cols; i++){
			cout << " "<< p[i];
		}
		cout << endl;
	}

	cout << endl;

	for (int j = 0; j < src.rows; j++){
		for (int i = 0; i < src.cols;i++){
			cout << " "<< (int)canny.at<unsigned char>(j,i)/10;
		}
		cout << endl;
	}
	for (int j = 0; j < src.rows; j++){
		for (int i = 0; i < src.cols; i++){
			if ((int)canny.at<unsigned char>(j, i) > 0){
				Point2i ce(i,j);
				E.push_back(ce);
			}
		}
	}
	cout << E.size() << endl;
	vector<Point2i>::iterator it;
	for (it = E.begin(); it != E.end(); it++){
		cout << "i : " << it->x << " j : " << it->y << endl;
	}
	vector<vector<Point2i>> edges;

	cout << "loop" << endl;
	for (it = E.begin(); it != E.end(); it++){
		vector<Point2i>E2;
		for (int j = it->y - 6; j < it->y + 7; j++){
			for (int i = it->x; i < it->x + 7; i++){
			
				if ((j != it->y && i != it->x) &&
					(j > 0 && j < canny.rows) &&
					(i > 0 && i < canny.cols)
					){

					if ((int)canny.at<unsigned char>(j, i) > 0){
						Point2i ce;
						ce.x = it->x;
						ce.y = it->y;
						if (!eight_neighbor(ce, i, j)){
							E2.push_back(ce);
							cout << "center x:" << ce.x << "y: " << ce.y << endl;
							cout << " i : " << i << " j : " << j << endl;
						}
					}
				}
			}
		}
		cout << endl;
		edges.push_back(E2);
		E2.clear();
	
	}

	vector<vector<Point2i>>::iterator ss;
	vector<Point2i>::iterator sss;
	for (ss = edges.begin(); ss != edges.end();ss++){
		for (sss = ss->begin(); sss != ss->end(); sss++){
			//cout << ss->size() << endl;
			//cout << "i : " << sss->x << " j : " << sss->y << endl;
		}
		//cout << endl;

	}

	imshow("show", src);
	imshow("gray",gray);
	imshow("canny",canny);
	waitKey(0);

	return 0;
}

int eight_neighbor(Point2i ce,int x, int y){
	if ( ce.x == x && ce.y == y - 1 ||
	     ce.x == x && ce.y == y + 1 ||
		 ce.x == x - 1 && ce.y == y - 1 ||
		 ce.x == x - 1 && ce.y == y + 1 ||
		 ce.x == x + 1 && ce.y == y - 1 ||
		 ce.x == x + 1 && ce.y == y + 1 ||
		 ce.x == x + 1 && ce.y == y     ||
		 ce.x == x - 1 && ce.y == y     ||
		 ce.x == x && ce.y == y
		){
		return 1;
	}
	return 0;
}