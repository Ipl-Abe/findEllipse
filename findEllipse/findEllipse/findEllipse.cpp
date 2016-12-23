#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include<iostream>
#include<vector>

using namespace cv;
using namespace std;
Mat undist(Mat src);

int para1 = 150;
int para2 = 200;

int voteArray[181] = { 0 };
int finalArray[181] = { 0 };
int at = 0;
int at1 = 0;
int at2 = 0;


Mat element = Mat::ones(3, 3, CV_8UC1); //@comment 追加　3×3の行列で要素はすべて1　dilate処理に必要な行列
int main(){
	Mat src = imread("../ellipse.png", 1);
	Mat origin;
	Mat gaus, lapla;
	Mat canny;
	Mat gray;

	vector<Vec3f> circles;
	vector<Vec4i> lines;

	if (!src.data) return -1;


	resize(src, src, Size(), 0.05, 0.05);
	src.copyTo(origin);

	GaussianBlur(src, gaus, Size(9, 9), 2, 2);
	imshow("Gaussian", gaus);

	Canny(gaus, canny, 50, 200, 3);

	//dilate(canny, canny, element, Point(-1, -1), 1); //@comment 膨張処理

	imshow("canny", canny);

	cvtColor(gaus, gaus, CV_BGR2GRAY);
	Laplacian(gaus, lapla, gaus.depth(), 3, 9);

	imshow("Lapla", lapla);

	convertScaleAbs(lapla, lapla, 1, 0);

	//cvtColor(lapla,gray,CV_BGR2GRAY);

	float tan, tan1, tan2;

	//imshow("gray",gray);
	cout << canny.cols << endl;
	cout << canny.rows << endl;
	for (int j = 0; j < canny.rows; j++){
		for (int i = 0; i < canny.cols; i++){
			cout << " " << canny.at<unsigned char>(j, i);
		}
		cout << endl;
	}




	//@comment count number
	for (int j = 0; j < canny.rows; j++){
		for (int i = 0; i < canny.cols; i++){
			//cout <<" "<<canny.at<unsigned char>(j,i);
			if ((int)canny.at<unsigned char>(j, i) >= 255){
				cout << "i " << i << " j " << j << " value " << (int)canny.at<unsigned char>(j, i) << endl;

				for (int k = j - 4; k < j + 5; k++){
					for (int l = i - 4; l < i + 5; l++){
						if (k < 0 || l < 0) continue;
						if ((int)canny.at<unsigned char>(k, l) >= 255){
							if (l == i && k == j) continue;
							/////////////////////////////////////////////////////
							else if (l == i){
								at = 90;

								tan1 = (float)(-0.5) / (k - j);
								tan2 = (float)(+0.5) / (k - j);

								if (k - j < 0){
									at1 = atanf(tan1) * 180 / CV_PI + 90;
									at2 = atanf(tan2) * 180 / CV_PI + 90;
								}
								else{
									at1 = atanf(tan1) * 180 / CV_PI + 90;
									at2 = atanf(tan2) * 180 / CV_PI + 90;
								}
							}
							else if (k == j){
								tan = 0;
								tan1 = (float)(-0.5) / (l - i);

								tan2 = (float)(0.5) / (l - i);

								if (l - i > 0){
									at = 0;
									tan1 = (float)(-0.5) / (l - i);
									tan2 = (float)(0.5) / (l - i);
									at1 = atanf(tan1) * 180 / CV_PI;
									at2 = atanf(tan2) * 180 / CV_PI;
								}
								else {
									at = 180;
									tan1 = -(float)(-0.5) / (l - i);
									tan2 = -(float)(0.5) / (l - i);
									at1 = atanf(tan1) * 180 / CV_PI + 180;
									at2 = atanf(tan2) * 180 / CV_PI + 180;

								}

							}
							///////////////////////////////////////////////////
							else if (fabs(-(float)((k - j) / (l - i))) > 1){
								if (k == j){
									tan = 0;
								}

								else{
									tan = -(float)(l - i) / (k - j);


									if (k - j < 0){
										tan1 = -(float)(l - i + 0.5) / (k - j);
										tan2 = -(float)(l - i - 0.5) / (k - j);
										at = atanf(tan) * 180 / CV_PI + 90;
										at1 = atanf(tan1) * 180 / CV_PI + 90;
										at2 = atanf(tan2) * 180 / CV_PI + 90;
									}
									else{
										tan1 = -(float)(l - i - 0.5) / (k - j);
										tan2 = -(float)(l - i + 0.5) / (k - j);
										at = atanf(tan) * 180 / CV_PI + 180;
										at1 = atanf(tan1) * 180 / CV_PI + 180;
										at2 = atanf(tan2) * 180 / CV_PI + 180;
									}
								}
								if (tan == 0){
									at = 90;
								}
							}
							////////////////////////////////////////////////////
							else{

								tan = -(float)(k - j) / (l - i);

								if (l - i > 0){
									if (tan <= 0){
										tan1 = -(float)(k - j + 0.5) / (l - i);
										tan2 = -(float)(k - j - 0.5) / (l - i);
										at = atanf(tan) * 180 / CV_PI + 180;
										at1 = atanf(tan1) * 180 / CV_PI + 180;
										at2 = atanf(tan2) * 180 / CV_PI + 180;
									}
									else{
										tan1 = -(float)(k - j + 0.5) / (l - i);
										tan2 = -(float)(k - j - 0.5) / (l - i);
										at = atanf(tan) * 180 / CV_PI;
										at1 = atanf(tan1) * 180 / CV_PI;
										at2 = atanf(tan2) * 180 / CV_PI;

									}
								}
								else{
									if (tan < 0){
										tan1 = -(float)(k - j + 0.5) / (l - i);
										tan2 = -(float)(k - j - 0.5) / (l - i);
										at = atanf(tan) * 180 / CV_PI + 180;
										at1 = atanf(tan1) * 180 / CV_PI + 180;
										at2 = atanf(tan2) * 180 / CV_PI + 180;
									}
									else{
										tan1 = -(float)(k - j - 0.5) / (l - i);
										tan2 = -(float)(k - j + 0.5) / (l - i);
										at = atanf(tan) * 180 / CV_PI;
										at1 = atanf(tan1) * 180 / CV_PI;
										at2 = atanf(tan2) * 180 / CV_PI;

									}

								}

							}
							if (at1 <= at && at <= at2){
								//for (int e = (int)at1; e < (int)at2; e++){
								voteArray[at]++;
								//}
							}
							cout << "l: " << l << " k: " << k << " tan: " << tan << endl;
							cout << "at: " << at << " at1 " << at1 << " at2 " << at2 << endl << endl;
						}
					}
				}
				cout << endl;

				for (int i = 0; i < 181; i++){
					cout << " θ= " << i << " : num= " << voteArray[i] << " ";
				}
				cout << endl;
				cout << endl;
			}
		}
		cout << endl;
	}



	//imshow("origin", origin);

	for (int i = 0; i < 181; i++){
		cout << "θ" << i << " num " << voteArray[i] << endl;
		if (voteArray[i] > 10){
			finalArray[i] = voteArray[i];
		}
	}
	cout << endl;
	for (int i = 0; i < 181; i++){
		cout << "θ" << i << " num " << finalArray[i] << endl;

	}

	waitKey(0);
	return 0;

}



//@comment カメラキャリブレーション用関数(gopro用)
Mat undist(Mat src_img)
{
	Mat dst_img;

	//@comment カメラマトリックス(gopro)
	Mat cameraMatrix = (Mat_<double>(3, 3) << 469.96, 0, 400, 0, 467.68, 300, 0, 0, 1);
	//@comment 歪み行列(gopro)
	Mat distcoeffs = (Mat_<double>(1, 5) << -0.18957, 0.037319, 0, 0, -0.00337);

	undistort(src_img, dst_img, cameraMatrix, distcoeffs);
	return dst_img;
}

