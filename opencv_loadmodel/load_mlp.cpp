#include"mlp.h"
#include<iostream>
//#ifdef _cplusplus
//extern "C" {
//#endif
using namespace std;
int Load_mlp_c(char* modelTxt, char* modelBin, double* coordinates);
int Load_mlp(char* modelTxt, char* modelBin, double* coordinates);
int main()
{	
	char* modelTxt = "E:/deve/project/OpenCV_load_caffe/ppo.prototxt";
	char* modelBin = "E:/deve/project/OpenCV_load_caffe/ppo.caffemodel";
	
	double coordinates[8] = { 9.00000000e+01, 1.28000000e+02, -1.68869409e-01,
		1.78865033e-01,1.80000000e+02, -5.50000000e+01,
		1.55000000e+02, -5.00000000e+00 };




	 int classIdx = 0 ;

	 classIdx = Load_mlp_c(modelTxt, modelBin,coordinates);


	cout<< classIdx << endl;


	double  percent = (double)27848/ (double)52500;
	cout << "百分比" << percent << endl;

	//memset(coordinates, 0, sizeof(coordinates));

	//cout << coordinates[0] << endl;

	//memset(&classIdx, 0, sizeof(int));

	//cout << classIdx << endl;

	system("pause");
	
}

int Load_mlp(char* modelTxt, char* modelBin,double* coordinates)
{

	//long time_use = 0;
	//struct timeval start;
	//struct timeval end;
	//gettimeofday(&start, NULL);
	

	Net net = readNetFromCaffe(modelTxt, modelBin);

	if (net.empty())
	{
		std::cerr << "Can't load network by using the following files: " << std::endl;
		std::cerr << "prototxt:   " << modelTxt << std::endl;
		std::cerr << "caffemodel: " << modelBin << std::endl;
		exit(-1);
	}

	Mat input(1, 8, CV_64FC1, coordinates);
	//cout << input << endl;
	//cout << input.size << endl;
	net.setInput(input, "data");

	Mat output = net.forward();

	//cout << output << endl;

	//cout << output.size << endl;

	Point classNumber;

	double classProb;

	minMaxLoc(output, NULL, &classProb, NULL, &classNumber);

	int classIdx = 0;
	classIdx = classNumber.x;

	//cout << classIdx << endl;

	return classIdx;

	//gettimeofday(&end, NULL);
	//time_use = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
	//printf("mlp网络运行时间（us): %ld \n", time_use);

}
//
//#ifdef _cplusplus
//}
//#endif




int Load_mlp_c(char* modelTxt, char* modelBin, double* coordinates)
{
	return Load_mlp(modelTxt, modelBin, coordinates);


}