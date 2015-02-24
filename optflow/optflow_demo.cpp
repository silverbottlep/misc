#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/gpu/gpu.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

string f1 = "./src1.bmp";
string f2 = "./src2.bmp";
string f1_out = "./output_optx.jpg";
string f2_out = "./output_opty.jpg";


int main(int argc, char** argv )
{
		string nframes;
		cv::Mat image[2];
		cv::gpu::GpuMat gpu_src[2];
		cv::gpu::GpuMat gpu_flow_x;
		cv::gpu::GpuMat gpu_flow_y;
		cv::Mat flow_x;
		cv::Mat flow_y;
		cv::Mat flow_x_8u;
		cv::Mat flow_y_8u;
		cv::Scalar avg_x;
		cv::Scalar avg_y;
		
		image[0] = cv::imread(f1,CV_LOAD_IMAGE_GRAYSCALE);
		image[0].convertTo(image[0], CV_32FC1, 1.0/255.0);
		image[1] = cv::imread(f2,CV_LOAD_IMAGE_GRAYSCALE);
		image[1].convertTo(image[1], CV_32FC1, 1.0/255.0);

		gpu_src[0].upload(image[0]);
		gpu_src[1].upload(image[1]);

		cv::gpu::BroxOpticalFlow OpticalFlowGPU = 
			cv::gpu::BroxOpticalFlow(0.197f, 50.0f, 0.8f, 10, 77, 10);
		OpticalFlowGPU(gpu_src[0],gpu_src[1],gpu_flow_x,gpu_flow_y);

		gpu_flow_x.download(flow_x);
		gpu_flow_y.download(flow_y);

		avg_x = cv::mean(flow_x);
		avg_y = cv::mean(flow_y);
		flow_x = flow_x - avg_x;
		flow_y = flow_y - avg_y;

		double min=1000000.0, max=-1000000.0;
		double loc_x_min, loc_x_max;
		double loc_y_min, loc_y_max;
		cv::minMaxIdx(flow_x, &loc_x_min, &loc_x_max);
		cv::minMaxIdx(flow_y, &loc_y_min, &loc_y_max);
		if (min > loc_x_min)
			min = loc_x_min;
		if (min > loc_y_min)
			min = loc_y_min;
		if (max < loc_x_max)
			max = loc_x_max;
		if (max < loc_y_max)
			max = loc_y_max;
		double scale = 255/(max-min);
		
		flow_x.convertTo(flow_x_8u, CV_8UC1, scale, -min*scale);
		flow_y.convertTo(flow_y_8u, CV_8UC1, scale, -min*scale);

		imwrite(f1_out, flow_x_8u);
		imwrite(f2_out, flow_y_8u);

    return 0;
}
