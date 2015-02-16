#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <opencv2/gpu/gpu.hpp>
#include <iostream>

#include "mex.h"

using namespace cv;

mxArray* CvMat_to_new_mxArr (const CvMat* mat)
{
	const int TYPE = cvGetElemType (mat);

	// 2-d image
	if (CV_64FC1 == TYPE) {
		return helper_2dcvmat_to_mat<CV_64FC1> (mat);
	}
	else if (CV_32FC1 == TYPE) {
		return helper_2dcvmat_to_mat<CV_32FC1> (mat);
	}
	else if (CV_32SC1 == TYPE) {
		return helper_2dcvmat_to_mat<CV_32SC1> (mat);
	}
	else if (CV_16SC1 == TYPE) {
		return helper_2dcvmat_to_mat<CV_16SC1> (mat);
	}
	else if (CV_16UC1 == TYPE) {
		return helper_2dcvmat_to_mat<CV_16UC1> (mat);
	}
	else if (CV_8UC1 == TYPE) {
		return helper_2dcvmat_to_mat<CV_8UC1> (mat);
	}
	else if (CV_8SC1 == TYPE) {
		return helper_2dcvmat_to_mat<CV_8SC1> (mat);
	}
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	char output_namex[256];
	char output_namey[256];
	double min,max,scale;

	cv::gpu::GpuMat gpu_src1;
	cv::gpu::GpuMat gpu_src2;
	cv::gpu::GpuMat gpu_flow_x;
	cv::gpu::GpuMat gpu_flow_y;
	cv::Mat flow_x;
	cv::Mat flow_y;
	cv::Mat flow_x_8u;
	cv::Mat flow_y_8u;

	//cv::Mat src1 = cv::imread(prhs[0],CV_LOAD_IMAGE_GRAYSCALE);
	//cv::Mat src2 = cv::imread(prhs[1],CV_LOAD_IMAGE_GRAYSCALE);

	src1.convertTo(src1, CV_32FC1);
	src2.convertTo(src2, CV_32FC1);
	gpu_src1.upload(src1);
	gpu_src2.upload(src2);

	cv::gpu::BroxOpticalFlow OpticalFlowGPU = 
			cv::gpu::BroxOpticalFlow(0.197f, 50.0f, 0.8f, 10, 77, 10);
	OpticalFlowGPU(gpu_src1,gpu_src2,gpu_flow_x,gpu_flow_y);

	gpu_flow_x.download(flow_x);
	gpu_flow_y.download(flow_y);

	plhs[0] = cvMat_to_new_mxArr(flow_x);
	plhs[1] = cvMat_to_new_mxArr(flow_y);

//	cv::minMaxIdx(flow_x, &min, &max);
//	scale = 255/(max-min);
//	flow_x.convertTo(flow_x_8u, CV_8UC1, scale, -min*scale);
//
//	cv::minMaxIdx(flow_y, &min, &max);
//	scale = 255/(max-min);
//	flow_y.convertTo(flow_y_8u, CV_8UC1, scale, -min*scale);

	//strcpy(output_namex,argv[3]);
	//strcpy(output_namey,argv[3]);
	//imwrite(strcat(output_namex,"_optx.jpg"), flow_x_8u);
	//imwrite(strcat(output_namey,"_opty.jpg"), flow_y_8u);
}
