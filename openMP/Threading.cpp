#include "Threading.h"
#include <pthread.h>

/// <summary>
/// Bad implmentation. doesn't check or care for boundary regions between thread.
/// A better implmentation would be to divide the image intto smaller images, then border each subimage, 
/// and finally, after applying the filter on each subimage, remove the border and piece togeather the new imgae.
/// To easily do this, the use of OpenCV subimgaging and make border functions are usefull, however this project forbids
/// the use of OpenCV's API other than the use of opening the imgae.
/// </summary>

typedef struct global_data {
	const cv::Mat& src;
	cv::Mat& dst;
	const cv::Mat& kernel;
} global_data;

typedef struct thread_data {
	int id;
	const global_data* GB;
} thread_data;

#define THREADS_NUM 4

// private
static void* t_applyKernel(void* t)
{
	thread_data* data = (thread_data*)t;
	const int OFFSET = data->GB->kernel.rows / 2;
	const int chunk = data->GB->src.rows / THREADS_NUM;
	const int start = data->id * chunk + OFFSET;
	const int end = start + chunk - OFFSET;

	for (int y = start; y < end; y++)
	{
		for (int x = OFFSET; x < data->GB->src.cols - OFFSET; x++)
		{
			cv::Vec3f acc = { 0 ,0, 0 };
			for (int kernel_y = 0; kernel_y < data->GB->kernel.rows; kernel_y++)
			{
				for (int kernel_x = 0; kernel_x < data->GB->kernel.cols; kernel_x++)
				{
					const cv::Vec3b& temp_pix = data->GB->src.at<cv::Vec3b>(y + (kernel_y - OFFSET), x + (kernel_x - OFFSET));
					const float& conv = data->GB->kernel.at<float>(kernel_y, kernel_x);
					acc[0] += temp_pix[0] * conv; //R
					acc[1] += temp_pix[1] * conv; //G
					acc[2] += temp_pix[2] * conv; //B
				}
			}
			data->GB->dst.at<cv::Vec3b>(y, x)[0] = (uint8_t)std::min(std::max(acc[0], 0.f), 255.f); //R
			data->GB->dst.at<cv::Vec3b>(y, x)[1] = (uint8_t)std::min(std::max(acc[1], 0.f), 255.f); //G
			data->GB->dst.at<cv::Vec3b>(y, x)[2] = (uint8_t)std::min(std::max(acc[2], 0.f), 255.f); //B
		}
	}
	return 0;
}

const cv::Mat pthread_applyKernel(const cv::Mat& src, const cv::Mat& kernel) {
	cv::Mat dst = src.clone();
	const global_data GB = { src, dst, kernel };
	pthread_t  threads[THREADS_NUM];
	thread_data t_data[THREADS_NUM];

	for (int i = 0; i < THREADS_NUM; i++)
	{
		t_data[i] = { i, &GB };
		pthread_create(&threads[i], NULL, t_applyKernel, (void*)&t_data[i]);
	}
	for (int i = 0; i < THREADS_NUM; i++)
	{
		pthread_join(threads[i], NULL);
	}
	return dst;
}