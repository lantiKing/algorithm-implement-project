#include <OpenCV/cv.h>
#include <OpenCV/highgui.h>

#include <cv_statics.hpp>


//アイスクリーム屋さんで学ぶ楽しい統計学──相関から因子分析まで──
//http://kogolab.jp/elearn/icecream/index.html
int main(int argc, char * argv[]){

	printf("\n==== t検定 ==== \n");
	double wakuwaku_point[] = {70,75,70,85,90,70,80,75};
	double mogmog_point[]   = {85,80,95,70,80,75,80,90};
	
	cv::Mat wakuwaku_point_vector(8, 1, CV_64FC1, wakuwaku_point);
	cv::Mat mogmog_point_vector(8, 1, CV_64FC1, mogmog_point);
	
	printf("ワクワクバーガーの点数 size = %dx%d\n",wakuwaku_point_vector.rows,wakuwaku_point_vector.cols);
	cvMatDump(wakuwaku_point_vector);
	printf("モグモグバーガーの点数 size = %dx%d\n",mogmog_point_vector.rows,mogmog_point_vector.cols);
	cvMatDump(mogmog_point_vector);
	
	int tfree=0;
	double t = cvCalcT(wakuwaku_point_vector,mogmog_point_vector,&tfree);
	printf("t値 = %f , 自由度 = %d\n",t,tfree);
	
	
	printf("\n==== 対応のあるt検定 ==== \n");
	printf("#同じ母集団に属する標本を使うとき（同じ人が両方に行ってつけた点数を使う場合） \n");
	double wakuwaku[] = {90,75,75,75,80,65,75,80};
	double mogmog[]   = {95,80,80,80,75,75,80,85};
	
	cv::Mat wakuwaku_vector(8, 1, CV_64FC1, wakuwaku);
	cv::Mat mogmog_vector(8, 1, CV_64FC1, mogmog);
	
	printf("ワクワクバーガーの点数 size = %dx%d\n",wakuwaku_vector.rows,wakuwaku_vector.cols);
	cvMatDump(wakuwaku_vector);
	printf("モグモグバーガーの点数 size = %dx%d\n",mogmog_vector.rows,mogmog_vector.cols);
	cvMatDump(mogmog_vector);
	
	int ctfree=0;
	double collespond_t = cvCalcCorrespondT(wakuwaku_vector,mogmog_vector,&ctfree);
	printf("対応のある t値 = %f  自由度 = %d\n",collespond_t,ctfree);
	
	printf("\n==== 重回帰分析 ====\n");
	//Chap 5- 重回帰分析
	//少し違う値になります。
	//気温（最高気温,最低気温）
	//http://kogolab.jp/elearn/icecream/chap5/sec1.html
	double temperature[][2] = 
	{ {33.0,22.0},{33.0,26.0},{34.0,27.0},{34.0,28.0},{35.0,28.0}//1-5
	 ,{35.0,27.0},{34.0,28.0},{32.0,25.0},{28.0,24.0},{35.0,24.0}//6-10
	 ,{33.0,26.0},{28.0,25.0},{32.0,23.0},{25.0,22.0},{28.0,21.0}//11-15
	 ,{30.0,23.0},{29.0,23.0},{32.0,25.0},{34.0,26.0},{35.0,27.0}//16-20
	};
	double customer[] = 
	{382.0,324.0,338.0,317.0,341.0,360.0,339.0,329.0,218.0,402.0
	,342.0,205.0,368.0,196.0,304.0,294.0,275.0,336.0,384.0,368.0};
	
	cv::Mat temperature_mat(20, 2, CV_64FC1, temperature);
	cv::Mat customer_vector(20, 1, CV_64FC1, customer);
	
	printf("最高気温,最低気温 size = %dx%d\n",temperature_mat.rows,temperature_mat.cols);
	cvMatDump(temperature_mat);
	
	printf("顧客数 size = %dx%d\n",customer_vector.rows,customer_vector.cols);
	cvMatDump(customer_vector);
	
	double constant = 0.0;
	cv::Mat prc_vector = cvMultipleRegressionAnalysis(temperature_mat,customer_vector,&constant);
	printf("定数項 = %f\n",constant);
	printf("偏回帰係数 size = %dx%d\n",prc_vector.rows,prc_vector.cols);
	cvMatDump(prc_vector);
	
	cv::Mat emulate_customer = constant + temperature_mat*prc_vector;
	printf("重回帰分析を用いた予測客数 size = %dx%d\n",emulate_customer.rows,emulate_customer.cols);
	cvMatDump(emulate_customer);
	
	
	double correlation = cvCalcCorrelationCoefficient(customer_vector,emulate_customer);
	printf("相関係数 = %f \n",correlation);
	
	/*
	 //Chap 1-4
	 //最高温度と客数の例
	 double data_customer[] = {326.0,364.0,283.0,369.0,417.0,436.0,438.0,296.0,263.0,389.0};
	 double data_hot[]      = { 29.0, 29.0, 30.0, 32.0, 33.0, 32.0, 31.0, 26.0, 28.0, 31.0};
	 
	 cv::Mat customer2_vector(10, 1, CV_64FC1, data_customer);
	 cv::Mat hot_vector(10, 1, CV_64FC1, data_hot);
	 
	 double dispersion,standard_deviation;
	 cv::Mat deviation = cvCalcStandardDeviation(customer2_vector,&dispersion,&standard_deviation);
	 
	 printf("偏差 %dx%d\n",deviation.rows,deviation.cols);
	 cvMatDump(deviation);
	 printf("分散 = %f , 標準偏差 = %f \n",dispersion,standard_deviation);
	 
	 double correlation2 = cvCalcCorrelationCoefficient(hot_vector,customer2_vector);
	 printf("相関係数 = %f \n",correlation2);
	 */
	
	return 0;
}
