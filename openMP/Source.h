#pragma once
#include "opencv2/imgcodecs.hpp"

enum class KernelType {
	BLURR, SHARPEN, EMBOSS, G_BLURR
};
enum class KernelSize {
	THREE = 3
};

const std::string KT_ToString(KernelType k);

const cv::Mat useKernel(KernelType, KernelSize);

const cv::Mat applyKernel(const cv::Mat&, const cv::Mat&);
