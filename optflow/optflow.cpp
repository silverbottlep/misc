#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <opencv2/gpu/gpu.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

#define MAX_FRAMES 2000

string dir = "/home/eunbyung/Works/data/ucf101_jpeg/";
string outdir = "/home/eunbyung/Works/data/ucf101_opt_jpeg/";

int main(int argc, char** argv )
{
    if ( argc != 3 )
    {
        printf("usage: optflow <filelist> <nframes list>\n");
        return -1;
    }

	std::ifstream filelist;
	std::ifstream nframeslist;

	filelist.open(argv[1],std::ifstream::in);
	nframeslist.open(argv[2],std::ifstream::in);
	int count = 0;
	for( std::string fname; getline(filelist, fname); )
	{
		string nframes;
		getline(nframeslist, nframes);
		cv::Mat image[MAX_FRAMES];
		cv::gpu::GpuMat gpu_src[MAX_FRAMES];
		cv::gpu::GpuMat gpu_flow_x[MAX_FRAMES];
		cv::gpu::GpuMat gpu_flow_y[MAX_FRAMES];
		cv::Mat flow_x[MAX_FRAMES];
		cv::Mat flow_y[MAX_FRAMES];
		cv::Mat flow_x_8u[MAX_FRAMES];
		cv::Mat flow_y_8u[MAX_FRAMES];
		cv::Scalar avg_x;
		cv::Scalar avg_y;
		

		// load image for one avi file, which is usually around 100 frames
		int N = atoi(nframes.c_str());
		for (int i=0; i<N; i++) {
			char str[6]={0};
			sprintf(str, "%04u", i+1);	
			string f1 = dir+fname+"_f"+str+".jpg";
			//cout << f1 << endl;

			image[i] = cv::imread(f1,CV_LOAD_IMAGE_GRAYSCALE);
			image[i].convertTo(image[i], CV_32FC1, 1.0/255.0);
		}

		// compute optical flow(mean substraction)
		for (int i=0; i<N-1; i++) {
			gpu_src[i].upload(image[i]);
			gpu_src[i+1].upload(image[i+1]);

			cv::gpu::BroxOpticalFlow OpticalFlowGPU = 
				cv::gpu::BroxOpticalFlow(0.197f, 50.0f, 0.8f, 10, 77, 10);
			OpticalFlowGPU(gpu_src[i],gpu_src[i+1],gpu_flow_x[i],gpu_flow_y[i]);
			gpu_flow_x[i].download(flow_x[i]);
			gpu_flow_y[i].download(flow_y[i]);
			
			avg_x = cv::mean(flow_x[i]);
			avg_y = cv::mean(flow_y[i]);
			flow_x[i] = flow_x[i] - avg_x;
			flow_y[i] = flow_y[i] - avg_y;
		}

		// compute min and max for later scaling
		double min=1000000, max=-1000000;
		for (int i=0; i<N-1; i++) {
			double loc_x_min, loc_x_max;
			double loc_y_min, loc_y_max;

			cv::minMaxIdx(flow_x[i], &loc_x_min, &loc_x_max);
			cv::minMaxIdx(flow_y[i], &loc_y_min, &loc_y_max);

			if (min > loc_x_min)
				min = loc_x_min;
			if (min > loc_y_min)
				min = loc_y_min;
			if (max < loc_x_max)
				max = loc_x_max;
			if (max < loc_y_max)
				max = loc_y_max;
		}
		double scale = 255/(max-min);

		string tar_file;
		string command;
		// scaling into [0 255]
		for (int i=0; i<N-1; i++) {
			char str[6]={0};
			sprintf(str, "%04u", i+1);	
			string f1 = outdir+fname+"_f"+str+"_optx.jpg";
			string f2 = outdir+fname+"_f"+str+"_opty.jpg";
			
			flow_x[i].convertTo(flow_x_8u[i], CV_8UC1, scale, -min*scale);
			flow_y[i].convertTo(flow_y_8u[i], CV_8UC1, scale, -min*scale);

			imwrite(f1, flow_x_8u[i]);
			imwrite(f2, flow_y_8u[i]);

			tar_file = tar_file + " " + f1 + " " + f2;
		}

		command = "tar cf " + outdir + fname + ".tar" + tar_file;
		cout << command << endl;	
		cout << "count: " << ++count << " " << fname << endl;
	}

  return 0;
}
