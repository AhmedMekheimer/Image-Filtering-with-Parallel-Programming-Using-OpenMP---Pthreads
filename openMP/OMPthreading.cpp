#include "OMPthreading.h"

const cv::Mat OMP_applyKernel(const cv::Mat& src, const cv::Mat& kernel) {
	cv::Mat dst = src.clone();
	const int OFFSET = kernel.rows / 2;
	
	int THREADS_NUM = omp_get_max_threads();
	int chunk = src.rows / THREADS_NUM;
	omp_set_num_threads(THREADS_NUM);
#pragma OMP PARALLEL for SCHEDULE(DYNAMIC, chunk)
	for (int y = OFFSET; y < src.rows - OFFSET; y++)
	{
		for (int x = OFFSET; x < src.cols - OFFSET; x++)
		{
			cv::Vec3f acc = { 0 ,0, 0 };
			for (int kernel_y = 0; kernel_y < kernel.rows; kernel_y++)
			{
				for (int kernel_x = 0; kernel_x < kernel.cols; kernel_x++)
				{
					const cv::Vec3b& temp_pix = src.at<cv::Vec3b>(y + (kernel_y - OFFSET), x + (kernel_x - OFFSET));
					const float& conv = kernel.at<float>(kernel_y, kernel_x);
					acc[0] += temp_pix[0] * conv; //R
					acc[1] += temp_pix[1] * conv; //G
					acc[2] += temp_pix[2] * conv; //B
				}
			}
			dst.at<cv::Vec3b>(y, x)[0] = (uint8_t)std::min(std::max(acc[0], 0.f), 255.f); //R
			dst.at<cv::Vec3b>(y, x)[1] = (uint8_t)std::min(std::max(acc[1], 0.f), 255.f); //G
			dst.at<cv::Vec3b>(y, x)[2] = (uint8_t)std::min(std::max(acc[2], 0.f), 255.f); //B
		}
	}
	return dst;
}