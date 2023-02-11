#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

void* filter(void*);

//KERNEL MATRICES//
constexpr double gb = 0.0625;
cv::Mat gauss = (cv::Mat1d(3, 3) << 0.25, 2, 0.25, 2, 4, 2, 0.25, 2, 0.25) * gb;
cv::Mat boxblur = (cv::Mat1d(3, 3) << (1.0 / 9.0), (1.0 / 9.0), (1.0 / 9.0), (1.0 / 9.0), (1.0 / 9.0), (1.0 / 9.0), (1.0 / 9.0), (1.0 / 9.0), (1.0 / 9.0));
cv::Mat edge_detect = (cv::Mat1d(3, 3) << 0.25, 0, -0.25, 0, 0, 0, -0.25, 0, 0.25);
cv::Mat sharpen = (cv::Mat1d(3, 3) << 0, -0.5, 0, -0.5, 3, -0.5, 0, -0.5, 0);

typedef struct thread_stru
{
	int tid = 0;
	cv::Mat* img = nullptr;
	const cv::Mat* kernel = nullptr;
}thread_stru;