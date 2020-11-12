#include<math.h>
#include<stdio.h>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/dnn/dnn.hpp>

#include<iostream>
#include<fstream>
using namespace std;
using namespace cv;
using namespace cv::dnn;

//矩阵切片操作[:,i]
Mat matrix_op_split(Mat matrix_pre,int i) {
	Mat  matrix_out(1,2, CV_64FC1);
	for (int j = 0; j < matrix_pre.rows; j++) {
		matrix_out.at<double>(0,j) = matrix_pre.at<double>(j, i);		
	}
	return matrix_out;
}

struct dd_result {
	int success = 0;
	Mat v_end;
}dd;

struct calmov_result {
	int success = 0;
	double v_end[2];
	double agent_pos[2];
}calmov;

struct dd_result DD(Mat p0, double p1[2], Mat v0, Mat pGShip) {
	dd_result dd;

	Mat  v = Mat::zeros(2,10001, CV_64FC1);
	for (int i = 0; i < 2; i++) { v.at<double>(i, 0) = v0.at<double>(0,i); }
	//cout << "智能体初始速度" << v << endl;

	Mat  p = Mat::zeros(2,10001, CV_64FC1);
	for (int i = 0; i < 2; i++) { p.at<double>(i, 0) = p0.at<double>(0, i); }
	//cout << "智能体初始位置" << p << endl;

	int rGShip = 70;
	int Pcnt = 0;
	int split_idx = 0;
	int success = 0;

	while (p.at<double>(0, split_idx) - p1[0] < 0 && split_idx < 10000 &&
		v.at<double>(0, split_idx)>0) {
		

		//double test1 = norm(matrix_op_split(p, split_idx) - matrix_op_split(pGShip, 0));
		//
		//cout << "test1 = " << test1 << endl;
		//double test2 = norm(matrix_op_split(p, split_idx) - matrix_op_split(pGShip, 1));
		//cout << "test2 = " << test2 << endl;
		if (norm(matrix_op_split(p, split_idx) - matrix_op_split(pGShip, 0)) < rGShip ||
			norm(matrix_op_split(p, split_idx) - matrix_op_split(pGShip, 1)) < rGShip) {
			Pcnt = Pcnt + 1;
		}
		//cout << "split_idx: " << split_idx << endl;
		split_idx = split_idx + 1;

		for (int i = 0; i < 2; i++) { p.at<double>(i, split_idx) = p.at<double>(i, split_idx-1) + 
			v.at<double>(i, split_idx - 1); }

		//double atann = (p1[1] - p.at<double>(1, split_idx - 1)) / (p1[0] - p.at<double>(0, split_idx - 1));

		//cout << "atann: " << atann << endl;

		double alpha = atan((p1[1] - p.at<double>(1, split_idx - 1)) / (p1[0] - p.at<double>(0, split_idx - 1)));

		
		//cout << "alpha: " << alpha << endl;
		Mat vt = Mat::zeros(2, 1, CV_64FC1);
		vt.at<double>(0, 0) = cos(alpha) * v.at<double>(0, split_idx - 1) + sin(alpha) * v.at<double>(1, split_idx - 1);
		vt.at<double>(1, 0) = -sin(alpha) * v.at<double>(0, split_idx - 1) + cos(alpha) * v.at<double>(1, split_idx - 1);

		Mat p1_matrix(1, 2, CV_64FC1, p1);
		double D = norm(matrix_op_split(p, split_idx-1) - p1_matrix);

		//cout << "D: " << D << endl;
		double a2 = -vt.at<double>(1, 0) / D * 3 * norm(matrix_op_split(v, split_idx - 1));
		double a1 = 3 * exp(-6)*norm(matrix_op_split(v, split_idx - 1))* norm(matrix_op_split(v, split_idx - 1)) - 0.5*abs(a2);

		Mat a0 = Mat::zeros(2, 1, CV_64FC1);
		a0.at<double>(0, 0) = -a1;
		a0.at<double>(1, 0) = a2;

		Mat a = Mat::zeros(2, 1, CV_64FC1);
		a.at<double>(0, 0) = cos(alpha) * a0.at<double>(0, 0) - sin(alpha) * a0.at<double>(1, 0);
		a.at<double>(1, 0) = sin(alpha) * a0.at<double>(0, 0) + cos(alpha) * a0.at<double>(1, 0);

		v.at<double>(0, split_idx) = v.at<double>(0, split_idx - 1) + a.at<double>(0, 0);
		v.at<double>(1, split_idx) = v.at<double>(1, split_idx - 1) + a.at<double>(1, 0);
	}
	//cout << "Pcnt； " << Pcnt << endl;
	//cout << "split_idx； " << split_idx << endl;
	bool p_block = (Pcnt > 0);
	double v_end1 = norm(matrix_op_split(v, split_idx));
	double v_end2 = norm(matrix_op_split(v, split_idx - 1));
	double v_end_[2] = { v_end1, v_end2 };

	//cout << "v_end1； " << v_end1 << endl;
	//cout << "v_end2； " << v_end2 << endl;

	Mat v_end_matrix(1, 2, CV_64FC1, v_end_);
	Point Number;
	cv::minMaxLoc(v_end_matrix, NULL, NULL, NULL, &Number);
	int num = Number.x;
	Mat v_end = matrix_op_split(v, split_idx - num);
	Mat p1_matrix_(1, 2, CV_64FC1, p1);
	double d_end1 = norm(matrix_op_split(p, split_idx)- p1_matrix_);
	double d_end2 = norm(matrix_op_split(p, split_idx - 1) - p1_matrix_);
	double d_end = min({ d_end1, d_end2 });
	//cout << "p_block； " << p_block << endl;
	//cout << "num; " << num << endl;
	//cout << "d_end; " << d_end << endl;
	if (p_block == false && d_end < 10)
	{
		success = 1;
	}
	else {
		success = 0;
	}
	dd.success = success;
	dd.v_end = v_end;		 
	return dd;
}
struct calmov_result cal_move(double agent_pos[2], double v[2], int action, double ships[2][2], int s) {
	calmov_result calmov;
	Mat agent_v(1, 2, CV_64FC1, v);
	Mat ship(2, 2, CV_64FC1, ships);
	action = action - 10;
	double pre_pos[2];
	for (int i = 0; i < 2; i++) { pre_pos[i] = agent_pos[i]; }

	if (s == 1) {
		agent_pos[0] = 250;
		agent_pos[1] = agent_pos[1] + 15 * action;
	}
	else if (s == 2) {
		agent_pos[0] = 450; 
		agent_pos[1] = agent_pos[1] + 15 * action;
	}
	else {
		agent_pos[0] = 540; 
		agent_pos[1] = 440;
	}
	//for (int i = 0; i < 2; i++) { cout << "智能体下一个位置"<<agent_pos[i] << endl; }
	//Mat agent_pos_matrix(1, 2, CV_64FC1, agent_pos);
	Mat pre_pos_matrix(1, 2, CV_64FC1, pre_pos);
	//cout << "智能体当前位置" << pre_pos_matrix << endl;
	//cout << "智能体当前速度" << agent_v << endl;
	//cout << "障碍物位置" << ship << endl;
	dd = DD(pre_pos_matrix, agent_pos, agent_v, ship);


	calmov.success = dd.success;
	calmov.v_end[0] = dd.v_end.at<double>(0,0);
	calmov.v_end[1] = dd.v_end.at<double>(0,1);
	calmov.agent_pos[0] = agent_pos[0];
	calmov.agent_pos[1] = agent_pos[1];
	return calmov;
}


//void array_test(double* a) {
//	a[0] = 100;
//	a[1] = a[1] + 20;
//}
//
//
//struct result {
//	bool success = true;
//	double v  = 4.00000;
//
//}re;
//struct result pointer(){
//    	
//	return re;
//
//}

int Load_mlp(char* modelTxt, char* modelBin, double* coordinates)
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
	net.setInput(input, "data");
	Mat output = net.forward();
	Point classNumber;
	//double classProb;
	minMaxLoc(output, NULL, NULL, NULL, &classNumber);
	int classIdx = 0;
	classIdx = classNumber.x;
	//cout << classIdx << endl;
	return classIdx;

	//gettimeofday(&end, NULL);
	//time_use = 1000000 * (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);
	//printf("mlp网络运行时间（us): %ld \n", time_use);

}



void main() {
	/*矩阵测试********************************************/
	//double m1[2][4] = { {1,2,3,4},{5,6,7,8} };
	//Mat matrix1(2,4,CV_64FC1,m1);
	////Mat ma1 = matrix_op_split(matrix1, 0);
	//for (int i = 0; i < matrix1.cols; i++) {

	//	cout << matrix1.at<double>(0, i) ;
	//}



	//double m2[2][2] = { { 9,10 },{ 13,14 } };
	//Mat matrix2(2, 2, CV_64FC1, m2);
	//Mat ma2 = matrix_op_split(matrix2, 0);
	////Mat ma_result = ma1 - ma2;
	////cout << norm(ma_result) << endl;
	//Mat ma3 = matrix_op_split(matrix2, 1);
	//cout << matrix2 << endl;
	//cout << ma2 << endl;
	//cout << ma3 << endl;

	/*矩阵测试********************************************/


	/*数组测试********************************************/
	//double b[] = { 30,40 };
	//array_test(b);
	//for (int i = 0; i < 2; i++) {
	//	cout << b[i] << endl;
	//}
	/*数组测试********************************************/


	/*指针测试********************************************/
	//int a = 1;
	//int* p = &a;
	//cout << p << endl;//p是一个地址
	//cout << *p << endl;//*p是地址里的值

	//int* p = NULL;
	//p = pointer(p);
	//cout << *p << endl;
	/*指针测试********************************************/



	/*运行程序********************************************/

	char* modelTxt = "E:/deve/project/OpenCV_load_caffe/ppo.prototxt";
	char* modelBin = "E:/deve/project/OpenCV_load_caffe/ppo.caffemodel";
    //int count = 0;
	/*经过第一途径点*/

	/*for (double region_x = 50; region_x <= 200; region_x++) {
		for (double region_y = 250; region_y <= 600; region_y++) {*/

	/*const int row = 50000;
	const int col = 6;
	static  double test[row][col];
	ifstream in("./data(2).txt");
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			in >> test[i][j];
		}
	}
	in.close();*/

	//for (int i = 0; i < row; i++) {
		//for (int j = 0; j < col; j++) {


		double object_coor[2] = { 540,440 };
		double agent_pos1[2] = { 198, 399 };
		double ships[2][2] = { { 310,330 },{ 440,515 } };
		//cout << agent_pos1[0] << " " << agent_pos1[1] << " " << ships[0][0] << " " << ships[0][1] << " " << ships[1][0] << " " << ships[1][1] << endl;
		double v1[2] = { 10,0 };
		printf("目标已识别，目标位置[%lf,%lf]\n", object_coor[0], object_coor[1]);
		printf("无人机初始位置 [%lf,%lf]\n", agent_pos1[0], agent_pos1[1]);
		printf("无人机初始速度 [%lf,%lf]\n", v1[0], v1[1]);
		printf("障碍物位置 {[%lf,%lf],[%lf,%lf]}\n", ships[0][0], ships[1][0], ships[0][1], ships[1][1]);

		printf("即将经过第一途径点,开始判断.............\n");
		//cout << "即将经过第一途径点,开始判断：............." << endl;
		printf("多层感知机开始执行：.............\n");
		//cout << "多层感知机开始执行：............." << endl;
		double coordinates1[8] = { object_coor[0] - agent_pos1[0], object_coor[1] - agent_pos1[1],
			v1[0],v1[1],object_coor[0]-ships[0][0],object_coor[1]-ships[1][0],object_coor[0]-ships[0][1],object_coor[1]-ships[1][1] };

		/*printf("多层感知机输入：[%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf]\n", coordinates1[0], coordinates1[1], coordinates1[2],
			coordinates1[3], coordinates1[4], coordinates1[5], coordinates1[6], coordinates1[7]);*/
		int classIdx1 = 0;
		classIdx1 = Load_mlp(modelTxt, modelBin, coordinates1);
		printf("多层感知机输出行动代码：%d\n", classIdx1);

		//Mat agent_v(1,2, CV_64FC1, v);
		int action1 = classIdx1;
		//double ships[2][2] = { {310,320},{400,450} };
		//Mat ship(2, 2, CV_64FC1, ships);
		int s1 = 1;
		calmov = cal_move(agent_pos1, v1, action1, ships, s1);
		printf("无人机能否到达下一个途经点：%d\n", calmov.success);
		//cout << "无人机能否到达下一个途经点： " << calmov.success << endl;
		printf("无人机下一位置 [%lf,%lf]\n", calmov.agent_pos[0], calmov.agent_pos[1]);
		//for (int i = 0; i < 2; i++) { cout<<"智能体当前位置"<< calmov.agent_pos[i] << endl; }
		printf("无人机当前速度 [%lf,%lf]\n", calmov.v_end[0], calmov.v_end[1]);
		//for (int i = 0; i < 2; i++) { cout<<"智能体当前速度" << calmov.v_end[i] << endl; }
		printf("第一途径点判断结束.............\n");
		//cout << "第一途径点判断结束............." << endl;

		/*经过第二途径点*/
		printf("即将经过第二途径点,开始判断.............\n");
		//cout << "即将经过第二途径点,开始判断：............." << endl;
		double agent_pos2[2];
		agent_pos2[0] = calmov.agent_pos[0];
		agent_pos2[1] = calmov.agent_pos[1];
		double v2[2];
		v2[0] = calmov.v_end[0];
		v2[1] = calmov.v_end[1];
		printf("多层感知机开始执行.............\n");
		//cout << "多层感知机开始执行：............." << endl;
		double coordinates2[8] = { object_coor[0] - agent_pos2[0], object_coor[1] - agent_pos2[1],
			v2[0],v2[1],object_coor[0] - ships[0][0],object_coor[1] - ships[1][0],object_coor[0] - ships[0][1],object_coor[1] - ships[1][1] };

		/*printf("多层感知机输入：[%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf]\n", coordinates2[0], coordinates2[1], coordinates2[2],
			coordinates2[3], coordinates2[4], coordinates2[5], coordinates2[6], coordinates2[7]);*/
		int classIdx2 = 0;
		classIdx2 = Load_mlp(modelTxt, modelBin, coordinates2);
		printf("多层感知机输出行动代码：%d\n", classIdx2);

		int action2 = classIdx2;
		int s2 = 2;
		calmov = cal_move(agent_pos2, v2, action2, ships, s2);
		printf("无人机能否到达下一个途经点：%d\n", calmov.success);
		//cout << "无人机能否到达下一个途经点： " << calmov.success << endl;
		printf("无人机下一位置 [%lf,%lf]\n", calmov.agent_pos[0], calmov.agent_pos[1]);
		printf("无人机当前速度 [%lf,%lf]\n", calmov.v_end[0], calmov.v_end[1]);
		/*for (int i = 0; i < 2; i++) { cout << "智能体当前位置" << calmov.agent_pos[i] << endl; }
		for (int i = 0; i < 2; i++) { cout << "智能体当前速度" << calmov.v_end[i] << endl; }*/
		printf("第二途径点判断结束.............\n");
		//cout << "第二途径点判断结束............." << endl;

		/*判断能否到达目标点*/
		printf("即将判断能否到达目标点,开始判断.............\n");
		//cout << "即将判断能否到达目标点,开始判断：............." << endl;
		double agent_pos3[2];
		agent_pos3[0] = calmov.agent_pos[0];
		agent_pos3[1] = calmov.agent_pos[1];
		double v3[2];
		v3[0] = calmov.v_end[0];
		v3[1] = calmov.v_end[1];
		int action3 = -1;
		int s3 = 3;
		calmov = cal_move(agent_pos3, v3, action3, ships, s3);
		printf("无人机能否到达目标点： %d\n", calmov.success);
		//cout << "无人机能否到达目标点： " << calmov.success << endl;
		printf("无人机下一位置 [%lf,%lf]\n", calmov.agent_pos[0], calmov.agent_pos[1]);
		printf("无人机当前速度 [%lf,%lf]\n", calmov.v_end[0], calmov.v_end[1]);
		/*for (int i = 0; i < 2; i++) { cout << "智能体当前位置" << calmov.agent_pos[i] << endl; }
		for (int i = 0; i < 2; i++) { cout << "智能体当前速度" << calmov.v_end[i] << endl; }*/
		printf("目标点判断结束..............\n");
		//cout << "目标点判断结束.............." << endl;


		/*if (calmov.success == 1) {
			count = count + 1;
		}*/
	//}
	//	}
	//}
	/*cout << "成功次数" << count << endl;
	double percent = (double)count / (double)50000;
	cout << "成功率"<<percent << endl;*/

	/*运行程序*******************************************/
	system("pause");
}






