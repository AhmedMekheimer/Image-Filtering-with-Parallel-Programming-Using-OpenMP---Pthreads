#pragma warning(disable:4996)
#include<iostream>
#include <stdlib.h>
#include<fstream>
#include<sstream>
#include <opencv2/opencv.hpp>

//Mat image = imread("monument.jpg", IMREAD_GRAYSCALE);
using namespace cv;
using namespace std;

void blue_filter()
{
	ifstream image; 
	ofstream newimage;
	image.open("monument.ppm");
	newimage.open("blue_monument.ppm");

	//copy over header info
	string type = "", width = "", height = "", RGB = "";
	image >> type;
	image >> width;
	image >> height;
	image >> RGB;

	newimage << type << endl;
	newimage << width << "  " << height << endl;
	newimage << RGB << endl;


	string red = "", green = "", blue = "";
	int r = 0, g = 0, b = 0;
	while (!image.eof())
	{
		image >> red;
		image >> green;
		image >> blue;

		stringstream redstream(red);
		stringstream greenstream(green);
		stringstream bluestream(blue);

		redstream >> r;
		greenstream >> g;
		bluestream >> b;

		if (b + 50 >= 255) {
			b = 255;
		}
		else {
			b += 50;
		}

		newimage << r << " " << g << " " << b << endl;
	}
	image.close();
	newimage.close();
}
/*void maximize_filter()
{
	ifstream image;
	ofstream newimage;
	image.open("monument.ppm");
	newimage.open("max_monument.ppm");

	//copy over header info
	string type = "", width = "", height = "", RGB = "";
	image >> type;
	image >> width;
	image >> height;
	image >> RGB;
	
	int w = 0, h = 0;

	stringstream wstream(width);
	stringstream hstream(height);

	wstream >> w;
	hstream >> h;

	w = w / 5;
	h = h / 5; 

	newimage << type << endl;
	newimage <<w << "  " << h << endl;
	newimage << RGB << endl;

	string red = "", green = "", blue = "";
	int r = 0, g = 0, b = 0;
	while (!image.eof())
	{
		image >> red;
		image >> green;
		image >> blue;

		stringstream redstream(red);
		stringstream greenstream(green);
		stringstream bluestream(blue);

		redstream >> r;
		greenstream >> g;
		bluestream >> b;

		newimage << r << " " << g << " " << b << endl;
	}

	image.close();
	newimage.close();
}*/

void MirrorImage()
{
	Mat ay = image.clone();
	for (int row = 0; row < ay.rows; row++)
	{
		for (int col = 0; col < ; col++)
		{
			int swapRow = WIDTH - 1 - row; // Mirror pixel

			unsigned char temp = ay.at<uchar>(row, col);
			PixelGrid[row][col] = PixelGrid[swapRow][col];
			PixelGrid[swapRow][col] = temp;

		}
	}
}

int main()
{
	cout << "Choose filter" << endl;
	cout << "1 for blue filter" << endl;
	cout << "2 for maximize filter" << endl;
	int m;
	//cin >> m;
	//if (m == 1)
	//	blue_filter();

	Mat image = imread("monument.jpg",IMREAD_GRAYSCALE);

	// Error Handling
	//if (image.empty()) {
		//cout << "Image File "
			//<< "Not Found" << endl;



	Mat Kernel = (Mat_<double>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j<image.cols; j++)
		{
			
			for (int ky = 0; ky < Kernel.rows; ky++)
			{
				for (int kx = 0; kx < Kernel.cols; kx++)
				{

				}
			}
		}
	}

	
	return 0;
}