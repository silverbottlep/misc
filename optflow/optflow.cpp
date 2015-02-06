#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <opencv2/gpu/gpu.hpp>
#include <iostream>

using namespace cv;

int main(int argc, char** argv )
{
    if ( argc != 3 )
    {
        printf("usage: optflow <source1 image> <source2 image>\n");
        return -1;
    }

		cv::gpu::GpuMat gpu_src1;
		cv::gpu::GpuMat gpu_src2;
		cv::gpu::GpuMat gpu_flow_x;
		cv::gpu::GpuMat gpu_flow_y;
		cv::Mat src1_gray;
		cv::Mat src2_gray;

		cv::Mat src1 = cv::imread("src1.bmp",CV_LOAD_IMAGE_COLOR);
		cv::Mat src2 = cv::imread("src2.bmp",CV_LOAD_IMAGE_COLOR);

		cvtColor(src1, src1_gray, CV_BGR2GRAY);
		cvtColor(src2, src2_gray, CV_BGR2GRAY);

		gpu_src1.upload(src1_gray);
		gpu_src2.upload(src2_gray);

		cv::gpu::BroxOpticalFlow OpticalFlowGPU = cv::gpu::BroxOpticalFlow(0.197f, 0.8f, 50.0f, 10, 77, 10);
		OpticalFlowGPU(gpu_src1,gpu_src2,gpu_flow_x,gpu_flow_y);

		cv::Mat flow_x;
		cv::Mat flow_y;
		gpu_flow_x.download(flow_x);
		gpu_flow_y.download(flow_y);

    imshow("Display Image", src1_gray);
    waitKey(0);
    imshow("Display Image", src2_gray);
    waitKey(0);
    imshow("Display Image", flow_x);
    waitKey(0);
    imshow("Display Image", flow_y);
    waitKey(0);

    return 0;
}
