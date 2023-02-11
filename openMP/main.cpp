#include "Source.h"
#include "Threading.h"
#include "OMPthreading.h"

int main(int argc, char* argv[]) {
	const cv::Mat src = cv::imread(argv[1], cv::IMREAD_COLOR);
	const cv::Mat kernel = useKernel(KernelType::BLURR, KernelSize::THREE);
	cv::Mat dst;

//	dst = pthread_applyKernel(src, kernel);
	dst = OMP_applyKernel(src, kernel);
//	dst = applyKernel(src, kernel);

	cv::imwrite(KT_ToString(KernelType::BLURR) + std::string(argv[1]), dst);
}