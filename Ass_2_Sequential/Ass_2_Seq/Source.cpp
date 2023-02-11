#pragma warning(disable:4996)
#include<iostream>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<pthread.h>


using namespace cv;
using namespace std;

/*void f_test()
{
	string img_path1 = "images.jfif";
	Mat img1 = imread(img_path1, IMREAD_COLOR);
	if (img1.empty())
	{
		cout << "Could not read the image:" << endl;
	}

	rotate(img1, img1, ROTATE_90_COUNTERCLOCKWISE);
	resize(img1, img1, Size(500, 500));

	for (int i = 0; i < img1.rows; i++) {
		for (int j = 0; j < img1.cols; j++) {
			Vec3b bgr = img1.at<Vec3b>(i, j);
			unsigned char grayScale = (bgr[2] + bgr[1] + bgr[0]) / 3;
			img1.at<Vec3b>(i, j) = { grayScale, grayScale, grayScale };
		}
	}
	imshow("img", img1);
	int k = waitKey(0);
}*/
//KERNEL MATRICES//

double gb = 0.0625;
Mat gauss = (Mat_<double>(3, 3) << 0.25, 2, 0.25, 2, 4, 2, 0.25, 2, 0.25)*gb;
Mat boxblur=(Mat_<double>(3, 3) << (1.0 / 9.0), (1.0 / 9.0), (1.0 / 9.0), (1.0 / 9.0), (1.0 / 9.0), (1.0 / 9.0), (1.0 / 9.0), (1.0 / 9.0), (1.0 / 9.0));
Mat edge_detect = (Mat_<double>(3, 3) << 0.25, 0, -0.25, 0, 0, 0, -0.25, 0, 0.25);
Mat sharpen= (Mat_<double>(3, 3) << 0,-0.5,0,-0.5,3,-0.5, 0, -0.5, 0);

int filter(Mat kernel)
{
	string img_path1 = "images.jfif";
	Mat img1 = imread(img_path1, IMREAD_COLOR);
	if (img1.empty())
	{
		cout << "Could not read the image:" << endl;
		return 1;
	}
	for (int i = 0; i < img1.rows - 2; i++)
	{
		for (int j = 0; j < img1.cols - 2; j++)
		{
			double sum[3] = { 0,0,0 };
			for (int x = 2; x > -1; x--) {

				sum[x] += img1.at<Vec3b>(i, j)[x]  *		 kernel.at<double>(0, 0);
				sum[x] += img1.at<Vec3b>(i + 1, j)[x]  *	 kernel.at<double>(1, 0);
				sum[x] += img1.at<Vec3b>(i + 2, j)[x]  *	 kernel.at<double>(2, 0);

				sum[x] += img1.at<Vec3b>(i, j + 1)[x]  *	 kernel.at<double>(0, 1);
				sum[x] += img1.at<Vec3b>(i + 1, j + 1)[x] *  kernel.at<double>(1, 1);
				sum[x] += img1.at<Vec3b>(i + 2, j + 1)[x]  * kernel.at<double>(2, 1);

				sum[x] += img1.at<Vec3b>(i, j + 2)[x]  *	 kernel.at<double>(0, 2);
				sum[x] += img1.at<Vec3b>(i + 1, j + 2)[x] *  kernel.at<double>(1, 2);
				sum[x] += img1.at<Vec3b>(i + 2, j + 2)[x]  * kernel.at<double>(2, 2);

			}
			for (int x = 0; x < 3; x++)
			{
				if (sum[x] < 0)
					sum[x] = 0;
				if (sum[x] > 255)
					sum[x] = 255;
			}
			img1.at<Vec3b>(i + 1, j + 1)[2] = sum[2];
			img1.at<Vec3b>(i + 1, j + 1)[1] = sum[1];
			img1.at<Vec3b>(i + 1, j + 1)[0] = sum[0];
		}
	}
	imshow("img", img1);
	int k = waitKey(0);
	return 0;
}

/*
int box_blur(Mat kernel)
{
	string img_path1 = "images.jfif";
	Mat img1 = imread(img_path1, IMREAD_COLOR);
	if (img1.empty())
	{
		cout << "Could not read the image:" << endl;
		return 1;
	}
	for (int i = 0; i < img1.rows - 2; i++)
	{
		for (int j = 0; j < img1.cols - 2; j++)
		{
			double sum[3] = { 0,0,0 };
			for (int x = 2; x > -1; x--) {

				sum[x] += img1.at<Vec3b>(i, j)[x] * kernel.at<double>(0, 0);
				sum[x] += img1.at<Vec3b>(i + 1, j)[x] * kernel.at<double>(1, 0);
				sum[x] += img1.at<Vec3b>(i + 2, j)[x] * kernel.at<double>(2, 0);

				sum[x] += img1.at<Vec3b>(i, j + 1)[x] * kernel.at<double>(0, 1);
				sum[x] += img1.at<Vec3b>(i + 1, j + 1)[x] * kernel.at<double>(1, 1);
				sum[x] += img1.at<Vec3b>(i + 2, j + 1)[x] * kernel.at<double>(2, 1);

				sum[x] += img1.at<Vec3b>(i, j + 2)[x] * kernel.at<double>(0, 2);
				sum[x] += img1.at<Vec3b>(i + 1, j + 2)[x] * kernel.at<double>(1, 2);
				sum[x] += img1.at<Vec3b>(i + 2, j + 2)[x] * kernel.at<double>(2, 2);

			}
			for (int x = 0; x < 3; x++)
			{
				if (sum[x] < 0)
					sum[x] = 0;
				if (sum[x] > 255)
					sum[x] = 255;
			}
			img1.at<Vec3b>(i + 1, j + 1)[2] = sum[2];
			img1.at<Vec3b>(i + 1, j + 1)[1] = sum[1];
			img1.at<Vec3b>(i + 1, j + 1)[0] = sum[0];
		}
	}
	imshow("img", img1);
	int k = waitKey(0);
	return 0;
}
int edgedetect_filter(Mat kernel)
{
	string img_path1 = "images.jfif";
	Mat img1 = imread(img_path1, IMREAD_COLOR);
	if (img1.empty())
	{
		cout << "Could not read the image:" << endl;
		return 1;
	}
	for (int i = 0; i < img1.rows - 2; i++)
	{
		for (int j = 0; j < img1.cols - 2; j++)
		{
			double sum[3] = { 0,0,0 };
			for (int x = 2; x > -1; x--) {

				sum[x] += img1.at<Vec3b>(i, j)[x] * kernel.at<double>(0, 0);
				sum[x] += img1.at<Vec3b>(i + 1, j)[x] * kernel.at<double>(1, 0);
				sum[x] += img1.at<Vec3b>(i + 2, j)[x] * kernel.at<double>(2, 0);

				sum[x] += img1.at<Vec3b>(i, j + 1)[x] * kernel.at<double>(0, 1);
				sum[x] += img1.at<Vec3b>(i + 1, j + 1)[x] * kernel.at<double>(1, 1);
				sum[x] += img1.at<Vec3b>(i + 2, j + 1)[x] * kernel.at<double>(2, 1);

				sum[x] += img1.at<Vec3b>(i, j + 2)[x] * kernel.at<double>(0, 2);
				sum[x] += img1.at<Vec3b>(i + 1, j + 2)[x] * kernel.at<double>(1, 2);
				sum[x] += img1.at<Vec3b>(i + 2, j + 2)[x] * kernel.at<double>(2, 2);

			}
			for (int x = 0; x < 3; x++)
			{
				if (sum[x] < 0)
					sum[x] = 0;
				if (sum[x] > 255)
					sum[x] = 255;
			}
			img1.at<Vec3b>(i + 1, j + 1)[2] = sum[2];
			img1.at<Vec3b>(i + 1, j + 1)[1] = sum[1];
			img1.at<Vec3b>(i + 1, j + 1)[0] = sum[0];
		}
	}
	imshow("img", img1);
	int k = waitKey(0);
	return 0;
}
int sharpen_filter(Mat kernel)
{
	string img_path1 = "images.jfif";
	Mat img1 = imread(img_path1, IMREAD_COLOR);
	if (img1.empty())
	{
		cout << "Could not read the image:" << endl;
		return 1;
	}
	for (int i = 0; i < img1.rows - 2; i++)
	{
		for (int j = 0; j < img1.cols - 2; j++)
		{
			double sum[3] = { 0,0,0 };
			for (int x = 2; x > -1; x--) {

				sum[x] += img1.at<Vec3b>(i, j)[x] * kernel.at<double>(0, 0);
				sum[x] += img1.at<Vec3b>(i + 1, j)[x] * kernel.at<double>(1, 0);
				sum[x] += img1.at<Vec3b>(i + 2, j)[x] * kernel.at<double>(2, 0);

				sum[x] += img1.at<Vec3b>(i, j + 1)[x] * kernel.at<double>(0, 1);
				sum[x] += img1.at<Vec3b>(i + 1, j + 1)[x] * kernel.at<double>(1, 1);
				sum[x] += img1.at<Vec3b>(i + 2, j + 1)[x] * kernel.at<double>(2, 1);

				sum[x] += img1.at<Vec3b>(i, j + 2)[x] * kernel.at<double>(0, 2);
				sum[x] += img1.at<Vec3b>(i + 1, j + 2)[x] * kernel.at<double>(1, 2);
				sum[x] += img1.at<Vec3b>(i + 2, j + 2)[x] * kernel.at<double>(2, 2);

			}
			for (int x = 0; x < 3; x++)
			{
				if (sum[x] < 0)
					sum[x] = 0;
				if (sum[x] > 255)
					sum[x] = 255;
			}
			img1.at<Vec3b>(i + 1, j + 1)[2] = sum[2];
			img1.at<Vec3b>(i + 1, j + 1)[1] = sum[1];
			img1.at<Vec3b>(i + 1, j + 1)[0] = sum[0];
		}
	}
	imshow("img", img1);
	int k = waitKey(0);
	return 0;
}
*/
int main()
{
	//filter(gauss);
	//filter(boxblur);
	//filter(edge_detect);
	filter(sharpen);
	return 0;
}

