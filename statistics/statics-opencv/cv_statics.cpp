#include "cv_statics.hpp"

///The function that make a copy from a matrix to another one
/** http://bit.ly/hxMugm
 * \param source The source cv::Matrix
 * \param dest The Destination  cv::Matrix, where the values are stored.
 */
void cvCopyMat (CvMat* source, CvMat* dest){
	int i,j;
	for (i=0; i<source->rows; i++)
		for (j=0; j<source->cols;j++)
			dest->data.fl[i*source->cols+j]=source->data.fl[i*source->cols+j];
	
}

void cvMatDump(cv::Mat input_mat){
	int col_count=0;
	cv::MatConstIterator_<double> iter = input_mat.begin<double>();
	while(iter != input_mat.end<double>()){
		col_count++;
		printf("%f ", *iter++);
		if (col_count == input_mat.cols) {
			printf("\n");
			col_count=0;
		}
	}
}


/** 平均値
 * \param[in] input_mat 独立変数の行列
 * \return mean 行列内の値の平均値
 */
double cvMatMean(cv::Mat input_mat){
	int size=0;
	double total=0.0;
	
	cv::MatConstIterator_<double> iter = input_mat.begin<double>();
	while(iter != input_mat.end<double>()){
		size++;
		total += *iter++;
	}
	return total/size;	
}


/** 偏差と分散と標準偏差の計算
 * \param[in] vector データ列A
 * \param[out] dispersion 分散
 * \param[out] standard_deviation 標準偏差
 * \return deviation 平均からの偏差
 */
cv::Mat cvCalcStandardDeviation(cv::Mat vector,double* dispersion,double* standard_deviation){
	
	//平均からの偏差の計算
	double mean = cvMatMean(vector);//平均
	cv::Mat deviation = vector-mean;//平均からの偏差
	
	//分散(偏差の二乗の平均)の計算
	double temp=0.0;
	for (int d_num=0; d_num<deviation.rows; d_num++){
		temp += deviation.at<double>(d_num)*deviation.at<double>(d_num);
	}
	temp = temp/deviation.rows;
	*dispersion = temp;
	
	//標準偏差（sqrt(分散)  ）
	*standard_deviation = sqrt(temp);
	
	dprintf("deviation %dx%d\n",deviation.rows,deviation.cols);
	dcvMatDump(deviation);
	
	dprintf("dispersion = %f , standard_deviation = %f \n",*dispersion,*standard_deviation);
	
	return deviation;
}

/** 相関係数
 * \param[in] vector_A データ列A X行1列
 * \param[in] vector_B データ列B X行1列
 * \return CorrelationCoefficient 相関係数
 */
double cvCalcCorrelationCoefficient(cv::Mat vector_A,cv::Mat vector_B){
	
	//偏差と標準偏差を計算しとく
	double dispersion_A,standard_deviation_A;
	cv::Mat deviation_A = cvCalcStandardDeviation(vector_A,&dispersion_A,&standard_deviation_A);
	double dispersion_B,standard_deviation_B;
	cv::Mat deviation_B = cvCalcStandardDeviation(vector_B,&dispersion_B,&standard_deviation_B);
	
	//相関（偏差積の平均/(標準偏差A*標準偏差B)）を計算
	double correlation = 0.0;
	double deviation_mal_average=0.0;
	for (int d_num=0; d_num<deviation_A.rows; d_num++){
		deviation_mal_average += deviation_A.at<double>(d_num) * deviation_B.at<double>(d_num);
	}
	deviation_mal_average = deviation_mal_average/deviation_A.rows;
	correlation = deviation_mal_average/(standard_deviation_A*standard_deviation_B);
	
	dprintf("correlation = %f\n",correlation);
	
	return correlation;
}

/** t値を計算する  ---  作成中
 * t分布を作るためのアレじゃなく、t分布と比較するためのあれなので注意。
 * \param[in] vector_A データ列A X行1列
 * \param[in] vector_B データ列B X行1列
 * \param[out] freedom 自由度
 * \return t t値
 */
double cvCalcT(cv::Mat vector_A,cv::Mat vector_B,int* freedom){
	//自由度の計算
	*freedom  = (vector_A.rows-1)+(vector_B.rows-1);
	dprintf("freedom = %d\n",*freedom);
	
	double average_A = cvMatMean(vector_A);//標本平均
	double average_B = cvMatMean(vector_B);
	
	
	//平均からの偏差の平方和 = 分散*標本数
	double dispersion_A,standard_deviation_A;
	cv::Mat deviation_A = cvCalcStandardDeviation(vector_A,&dispersion_A,&standard_deviation_A);
	double ave_disp_sum_A = dispersion_A*vector_A.rows;
	
	double dispersion_B,standard_deviation_B;
	cv::Mat deviation_B = cvCalcStandardDeviation(vector_B,&dispersion_B,&standard_deviation_B);
	double ave_disp_sum_B = dispersion_B*vector_B.rows;
	
	dprintf("average dispersion square sum (A,B) = (%f,%f)\n",ave_disp_sum_A,ave_disp_sum_B);
	
	
	//推定母分散
	double emulate_mothor_disp = (ave_disp_sum_A+ave_disp_sum_B)/((vector_A.rows-1)+(vector_B.rows-1));
	dprintf("emulate_mothor_disp = %f\n",emulate_mothor_disp);

	
	//差の標準誤差
	double standard_error = sqrt(emulate_mothor_disp*((1.0/vector_A.rows)+(1.0/vector_B.rows)));
	dprintf("standard_error = %f\n",standard_error);
	
	
	double t = (average_A-average_B)/standard_error;
	dprintf("t = %f\n",t);
	
	return t;
}

/** "対応のある"t値を計算する
 * t分布を作るためのアレじゃなく、t分布と比較するためのあれなので注意。
 * ひとつの母集団から抜き出したふたつの標本（つまり対応のある標本）のときはこちららしい。
 http://kogolab.jp/elearn/hamburger/chap5/sec3.html

 * \param[in] vector_A データ列A X行1列
 * \param[in] vector_B データ列B X行1列
 * \return ct 対応のあるt値
 
 t＝差の平均／差の標準誤差
 
 */
double cvCalcCorrespondT(cv::Mat vector_A,cv::Mat vector_B,int* freedom){
	cv::Mat error = vector_A-vector_B;
	
	//自由度の計算
	*freedom  = error.rows-1;
	dprintf("freedom = %d\n",*freedom);
	
	double error_mean = cvMatMean(error);//差の平均
	
	//差の標準誤差
	double dispersion_E,standard_deviation_E;
	cv::Mat deviation_E = cvCalcStandardDeviation(error,&dispersion_E,&standard_deviation_E);
	double standard_error = sqrt(dispersion_E/(error.rows-1));
	
	double ct = error_mean/standard_error;
	
	dprintf("t (correspond) = %f\n",ct);
	
	return ct;
}

/** カイ二乗値
 * カイ二乗分布を作るためのアレじゃなく、カイ二乗分布と比較するためのあれなので注意。
 http://kogolab.jp/elearn/hamburger/chap3/sec2.html
 
 * \param[in] vector_observe 観測値のデータ列 X行1列
 * \param[in] vector_expectation 期待値のデータ列 X行1列
 * \return chi カイ二乗値
 
 カイ２乗値＝（（（観測度数－期待度数）の２乗）÷期待度数）の総和
 
 */
double cvCalcChiSquare(cv::Mat vector_observe,cv::Mat vector_expectation){
	cv::Mat error = vector_observe-vector_expectation;
	
	double chi = 0.0;
	for (int d_num=0; d_num<vector_observe.rows; d_num++){
		chi += error.at<double>(d_num)*error.at<double>(d_num)/vector_expectation.at<double>(d_num);
	}
	
	dprintf("chi = %f\n",chi);
	
	return chi;
}

/** 重回帰分析
 偏相関係数と定数項を返すよ。
 http://www-akaz.ist.osaka-u.ac.jp/~pak/ise-ex2/regression/regCheck_c.txt
 http://kogolab.jp/elearn/icecream/chap6/sec3.html
 http://www.eco.osakafu-u.ac.jp/~kano/images/file/note_econometrics/suppl_ch11.pdf
 * \param[in] x_mat 独立変数の行列
 * \param[in] y_mat 従属変数のベクトル
 * \param[out] constant_value 定数項
 * \return a_vector 偏回帰係数(partial regression coefficient)のベクトル
 
 y_vector = x_mat*a_vector
 なので、
 a_vector=( (x_mat'x_mat)^{-1}x_mat')y_vector
 
 */
cv::Mat cvMultipleRegressionAnalysis(cv::Mat x_mat,cv::Mat y_vector,double* constant_value){
	//int data_num = x_mat.rows;//データ数
	int variable_num = x_mat.cols;//独立変数の数
	
	dprintf("x_mat %dx%d \n",x_mat.rows,x_mat.cols);
	dprintf("y_vector %dx%d\n",y_vector.rows,y_vector.cols);
	
	cv::Mat a_vector = cv::Mat::zeros(variable_num,1, CV_64FC1);//偏回帰係数
	cv::Mat x_mean_vector = cv::Mat::zeros(1,variable_num, CV_64FC1);//xの平均値格納用
	double constant_num=0.0;//定数項
	
	// ** 最小二乗法(OLS)を用いた回帰係数の推定 ** //
	cv::Mat x_matT = x_mat.t();// x の転置行列 x' の計算
	dprintf("x_matT %dx%d\n",x_matT.rows,x_matT.cols); dcvMatDump(x_matT);
	
	cv::Mat x_matTx_mat = x_matT*x_mat;//x'x の計算
	dprintf("x_matTx_mat %dx%d\n",x_matTx_mat.rows,x_matTx_mat.cols); dcvMatDump(x_matTx_mat);
	
	cv::Mat x_matTx_mat_inv = x_matTx_mat.inv();// (x'x) の逆行列 (x'x)^-1 の計算
	dprintf("x_matTx_mat_inv %dx%d\n",x_matTx_mat_inv.rows,x_matTx_mat_inv.cols); dcvMatDump(x_matTx_mat_inv);
	
	cv::Mat matrix = x_matTx_mat_inv*x_matT;// (x'x)^-1 x' の計算
	dprintf("matrix %dx%d\n",matrix.rows,matrix.cols);dcvMatDump(matrix);
	
	a_vector = matrix*y_vector;
	dprintf("a_vector %dx%d\n",a_vector.rows,a_vector.cols);
	dcvMatDump(a_vector);

	// ** 定数項の計算 ** //
	for (int v_num=0; v_num<variable_num; v_num++){
		
		//ここは定数項の計算用
		//xの変数ごとのデータの平均値を計算しとく
		x_mean_vector.at<double>(v_num) = cvMatMean(x_mat.col(v_num));
		constant_num += a_vector.at<double>(v_num)*x_mean_vector.at<double>(v_num);
	}
	constant_num = cvMatMean(y_vector) - constant_num;
	*constant_value = constant_num;
		
	dprintf("constant_num = %f\n",constant_num);

	
	return a_vector;
}

/** 重相関分析
 * \param[in] x_mat 独立変数の行列
 * \param[in] y_vector 従属変数のベクトル
 * \param[in] a_vector 偏回帰係数のベクトル
 * \param[out] t_distribution_vector t値
 * \return R 重相関係数
 */
cv::Mat cvMultipleCorrelationAnalysis(cv::Mat x_mat,cv::Mat y_vector,cv::Mat a_vector,double* r_value){
	
	dprintf("x_mat %dx%d \n",x_mat.rows,x_mat.cols);
	dprintf("y_vector %dx%d\n",y_vector.rows,y_vector.cols);
	
	int data_num = x_mat.rows;//データ数
	int variable_num = x_mat.cols;//独立変数の数
	
	cv::Mat t_vector = cv::Mat::zeros(variable_num,1, CV_64FC1);//t分布
	
	
	//推計値の計算
	cv::Mat estimate_y_vector = x_mat*a_vector;
	dprintf("estimate_y_vector %dx%d\n",estimate_y_vector.rows,estimate_y_vector.cols);
	dcvMatDump(estimate_y_vector);
	
	//重相関係数の計算
	//http://ja.wikipedia.org/wiki/相関係数
	double s=0.0,sp=0.0,sps=0.0;
	double y_mean = cvMatMean(y_vector);
	dprintf("y_mean %f\n",y_mean);
	
	for (int d_num=0; d_num<data_num; d_num++){
		double ey = estimate_y_vector.at<double>(d_num);
		double y = y_vector.at<double>(d_num);
		
		sp += (ey-y_mean)*(ey-y_mean);
		s += (y-y_mean)*(y-y_mean);
		sps = (ey-y_mean)*(y-y_mean);
	}
	double r_value_temp = sps/(sqrt(sp)*sqrt(s));
	*r_value = r_value_temp;
	dprintf("r_value %f\n",*r_value);
	
	
	//t検定の計算
	printf("R値？？ %f\n",(r_value_temp*sqrt(data_num-2))/(1-r_value_temp*r_value_temp));
	
	cv::Mat error = estimate_y_vector - y_vector;
	dprintf("error %dx%d\n",error.rows,error.cols);
	dcvMatDump(error);
	
	double ee = 0.0;
	for (int d_num=0; d_num<data_num; d_num++){
		double er = error.at<double>(d_num);
		ee += er*er;
	}
	ee = ee/(data_num-variable_num);
	
	cv::Mat xtx = (x_mat.t())*x_mat;
	dprintf("xtx %dx%d\n",xtx.rows,xtx.cols);
	dcvMatDump(xtx);
	
	cv::Mat xtx_diag = xtx.diag();
	dprintf("xtx_diag %dx%d\n",xtx_diag.rows,xtx_diag.cols);
	//dcvMatDump(xtx_diag);//なんかしらんができないorz
	
	for (int v_num=0; v_num<variable_num; v_num++){
		dprintf("%f\n",xtx_diag.at<double>(v_num));
		t_vector.at<double>(v_num) = a_vector.at<double>(v_num) / sqrt(ee * xtx_diag.at<double>(v_num));
	}
	
	dprintf("t_vector %dx%d\n",t_vector.rows,t_vector.cols);
	dcvMatDump(t_vector);
	
	//t_distribution_vector = t_vector.clone();
	
	return t_vector;
	
	//return y_mean;
}