#include "Ass3.h"
#define num_threads 3
using namespace std;
using namespace cv;
int main(void)
{
	cv::Mat img1 = cv::imread("E:\Courses\3rd CESS\Parallel & Cluster Comp\Assignments\Ass3\images.jfif", cv::IMREAD_COLOR);

	pthread_t threads[num_threads];
	thread_stru thread_data[num_threads];
	for (int tid = 0; tid < num_threads; tid++)
	{
		thread_data[tid].tid = tid;
		thread_data[tid].img = &img1;
		thread_data[tid].kernel = &gauss;
		int rc = pthread_create(&threads[tid], NULL, filter, (void*)&thread_data);
		if (rc)
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	imshow("img", img1);
	int k = cv::waitKey(0);
}

void* filter(void* thread_data)
{
	struct thread_stru* my_data = (struct thread_stru*)thread_data;

	std::cout << "WORKING ON THREAD #" << my_data->tid << std::endl;
	for (int i = 0; i < my_data->img->rows - 2; i++)
	{
		for (int j = 0; j < my_data->img->cols - 2; j++)
		{
			double sum = 0;

			sum += my_data->img->at<cv::Vec3b>(i, j)[my_data->tid] * my_data->kernel->at<double>(0, 0);
			sum += my_data->img->at<cv::Vec3b>(i + 1, j)[my_data->tid] * my_data->kernel->at<double>(1, 0);
			sum += my_data->img->at<cv::Vec3b>(i + 2, j)[my_data->tid] * my_data->kernel->at<double>(2, 0);

			sum += my_data->img->at<cv::Vec3b>(i, j + 1)[my_data->tid] * my_data->kernel->at<double>(0, 1);
			sum += my_data->img->at<cv::Vec3b>(i + 1, j + 1)[my_data->tid] * my_data->kernel->at<double>(1, 1);
			sum += my_data->img->at<cv::Vec3b>(i + 2, j + 1)[my_data->tid] * my_data->kernel->at<double>(2, 1);

			sum += my_data->img->at<cv::Vec3b>(i, j + 2)[my_data->tid] * my_data->kernel->at<double>(0, 2);
			sum += my_data->img->at<cv::Vec3b>(i + 1, j + 2)[my_data->tid] * my_data->kernel->at<double>(1, 2);
			sum += my_data->img->at<cv::Vec3b>(i + 2, j + 2)[my_data->tid] * my_data->kernel->at<double>(2, 2);

			if (sum < 0)
				sum = 0;
			if (sum > 255)
				sum = 255;
			my_data->img->at<cv::Vec3b>(i + 1, j + 1)[my_data->tid] = (uchar)sum;
		}
	}
	std::cout << "FINISHED THREAD #" << my_data->tid << std::endl;
	pthread_exit(NULL);
	return 0;
}