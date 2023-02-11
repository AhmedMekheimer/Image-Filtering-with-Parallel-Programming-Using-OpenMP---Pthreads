#include "Source.h"

const std::string KT_ToString(KernelType k) {
	switch (k)
	{
	case KernelType::BLURR:
		return "Blurr_";
	case KernelType::SHARPEN:
		return "Sharpen_";
	case KernelType::EMBOSS:
		return "Emboss_";
	case KernelType::G_BLURR:
		return "GaussianBlurr_";
	default:
		return "new_";
	}
}
const cv::Mat useKernel(KernelType x, KernelSize y) {
	switch (x)
	{
	case KernelType::BLURR:
		switch (y)
		{
		case KernelSize::THREE:
			return (
				cv::Mat1f(static_cast<int>(y), static_cast<int>(y)) <<
				1 / 9.f, 1 / 9.f, 1 / 9.f,
				1 / 9.f, 1 / 9.f, 1 / 9.f,
				1 / 9.f, 1 / 9.f, 1 / 9.f
				);
		}
	case KernelType::SHARPEN:
		switch (y)
		{
		case KernelSize::THREE:
			return (
				cv::Mat1f(static_cast<int>(y), static_cast<int>(y)) <<
				0.0f, -0.5f, 0.0f,
				-0.5f, 3.0f, -0.5f,
				0.0f, -0.5f, 0.0f
				);
		}
	case KernelType::EMBOSS:
		switch (y)
		{
		case KernelSize::THREE:
			return (
				cv::Mat1f(static_cast<int>(y), static_cast<int>(y)) <<
				-2.f, -1.f, 0.f,
				-1.f, 1.f, 1.f,
				0.f, 1.f, 2.f
				);
		}
	case KernelType::G_BLURR:
		switch (y)
		{
		case KernelSize::THREE:
			return (
				cv::Mat1f(static_cast<int>(y), static_cast<int>(y)) <<
				1 / 16.f, 2 / 16.f, 1 / 16.f,
				2 / 16.f, 4 / 16.f, 2 / 16.f,
				1 / 16.f, 2 / 16.f, 1 / 16.f
				);
		}
	}
}

const cv::Mat applyKernel(const cv::Mat& src, const cv::Mat& kernel) {
	cv::Mat dst = src.clone();
	const int OFFSET = kernel.rows / 2;
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
