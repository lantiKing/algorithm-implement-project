#include <OpenCV/cv.h>


void cvCopyMat (CvMat* source, CvMat* dest);
void cvMatDump(cv::Mat input_mat);

double cvMatMean(cv::Mat input_mat);

double cvCalcT(cv::Mat vector_A,cv::Mat vector_B,int* freedom);//t値
double cvCalcCorrespondT(cv::Mat vector_A,cv::Mat vector_B,int* freedom);//対応のあるt値
double cvCalcChiSquare(cv::Mat vector_observe,cv::Mat vector_expectation);//カイ二乗値

cv::Mat cvCalcStandardDeviation(cv::Mat vector,double* dispersion,double* standard_deviation);//標準偏差
double cvCalcCorrelationCoefficient(cv::Mat vector_A,cv::Mat vector_B);//相関係数

cv::Mat cvMultipleCorrelationAnalysis(cv::Mat x_mat,cv::Mat y_vector,cv::Mat a_vector,double* r_value);
cv::Mat cvMultipleRegressionAnalysis(cv::Mat x_mat,cv::Mat y_vector,double* constant_value);


//デバッグ用にいろいろ準備
#ifdef DEBUG
#define dprintf(s,...) printf("%s:%d: " s,__func__,__LINE__,__VA_ARGS__)
#define dprintf(s,...) printf("%s:%d: " s,__func__,__LINE__,__VA_ARGS__)
#define dcvMatDump(mat) cvMatDump(mat)
#else
#define dprintf(...) ((void)0)
#define dcvMatDump(...) ((void)0)
#endif