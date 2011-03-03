/*
 *  Chromosome.cpp
 *  CA brittle star
 *
 *  Created by sunil lal on 6/24/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "Chromosome.h"

/**空の染色体を生成、ポインタだけ欲しい時に使ってね
*/
Chromosome::Chromosome(){
}

Chromosome::Chromosome(int gene_num,int type_num){
	length = gene_num;
	gene_type_num = type_num;
	
	gene = (int*)malloc(sizeof(int)*(length));
	if(gene == NULL) {
      std::cout << "[Chromosome:Error] Can not allocate memory" << std::endl;
      exit(-1);
   }
	for(int i=0; i<length; i++)
		gene[i]=rand()%gene_type_num;
}

/**染色体のサイズ（遺伝子数）を取得
*
*@return 染色体のサイズ
*/
int Chromosome::getLength(){
	return length;
}

/**遺伝子の値を更新
*
*@param[in] id 遺伝子の番号
*@param[in] value 遺伝子
*/
void Chromosome::setGene(int id, int value){
	gene[id] = value;
}

/**遺伝子の値を取得
*
*@param[in] id 遺伝子の番号
*@return id番目の遺伝子
*/
int Chromosome::getGene(int id){
	return gene[id];
}

/**適応度を設定
*
*@param[in] fitness_value 適応度
*/
void Chromosome::setFitness(double fitness_value){
	fitness = fitness_value;
}

/**適応度を取得
*
*@return 適応度
*/
double Chromosome::getFitness(){
	return fitness;
}

/**突然変異
*
*@param[in] mutate_rate 突然変異確立
*/
void Chromosome::mutate(double mutate_rate){
	float r;
	
	for(int i=0; i<length; i++){
	
		r = (rand()%10001)/10000.0;
		
		if(r <= mutate_rate) gene[i]=rand()%gene_type_num;
	}
}

/**結果の出力
*
*@param[in] other 交差する相手
*/
void Chromosome::crossover(Chromosome* other){

	int crosspos;
	crosspos = (rand()%(length-2)) + 1;

	int temp_self,temp_other;
	
	for(int point = 0; point<crosspos; point++){
		temp_self = gene[point];
		temp_other = other->getGene(point);
		gene[point] = temp_other;
		other->setGene(point,temp_self);
	}
}

/**回答を取得
*
*@param[in] solution 回答を格納する配列、長さはCLENないとダメ
*/
void Chromosome::getSolution(int* solution){ 
	for(int i=0; i<length; i++)
		solution[i] = gene[i];
}

/**結果の出力
*
*@return 結果を標準出力に出力
*/
void Chromosome::print(){
#ifdef DETAIL
	for(int i=0;i<length; i++)	std::cout<<gene[i],",";
#endif
	std::cout<<" "<<fitness<<std::endl;
}

/*デストラクタ（お片づけ関数）
*
*/
Chromosome::~Chromosome(){
	free(gene);
}
