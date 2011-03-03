/*
 *  GAEngine.cpp
 *  CA brittle star
 *
 *  Created by sunil lal on 6/24/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "GAEngine.h"

GAEngine::GAEngine(int num,int type_num,int pop_num,double crate,double mrate){
  std::cout << "initialize gene engine" <<std::endl;
	gene_num = num;
	gene_type_num = type_num;
	population_num = pop_num;
	
	//http://ppwww.phys.sci.kobe-u.ac.jp/~akusumoto/program/detail.php?d=c/05-pointer/pointer_array
	population = new Chromosome*[population_num];
	for( int n = 0; n < population_num; n++ )
		population[n] = new Chromosome(gene_num,gene_type_num);
	
	cross_rate = crate;
	mutation_rate = mrate;
}

/*ルーレット選択
*/
void GAEngine::selection(){

	double sum=0, r, border;
	int count,max=0;
	Chromosome *newPopl[population_num];//ポインタを入れとく配列だけ欲しいので、初期化はイラネ。
	
	//carry forward the best sol in a given generation
	//最適な遺伝子を探す
	for(int i=0; i<population_num; i++){
		if(population[i]->getFitness() > population[max]->getFitness())
			max = i;
	}
	newPopl[0] = population[max];
	
	//using roulette wheel selection method pick individuals for performing crossover operation
	//ルーレット選択で使う準備
	for(int i=0; i<population_num; i++)
		sum += population[i]->getFitness();
	
	for(int i=1; i<population_num; i++){

		r = sum * ((rand()%10001)/10000.0);
		count=0;
		border = population[0]->getFitness();

		while(border<r){
			count++;
			border += population[count]->getFitness();
		}

		newPopl[i] = population[count];
	}

	for(int i=0; i<population_num; i++)
		population[i] = newPopl[i];
}

/*交叉（二点交叉）
*/
void GAEngine::crossover(){

	double r;
	
	for(int i=1; i<population_num-1; i+=2){ //starts from 1 so as to preserve the best solution
		r = (rand()%10001)/10000.0;

		if(r <= cross_rate){
			population[i]->crossover(population[i+1]);
		}
	}
}

//突然変異
void GAEngine::mutation(){

	double r;

	for(int i=1; i<population_num; i++){  //starts from 1 so as to preserve the best solution
		r = (rand()%10001)/10000.0;

		if(r <= mutation_rate)
			population[i]->mutate(mutation_rate);
	}
}

//結果をまとめる
void GAEngine::collateResults(){

	double sum=0;
	int max=0;

	for(int i=0; i<population_num; i++){
		sum+=population[i]->getFitness();
		
		if(population[i]->getFitness() > population[max]->getFitness())
			max = i;
	}

	avgFitness = sum/population_num;

	bestChromosome = population[max];
}

/**適応度の設定
*@param[in] pop_id 世代番号
*@param[in] fit 適応度
*/
void GAEngine::setFitness(int pop_id, double fit){
	population[pop_id]->setFitness(fit);
}

/**最適解の取得*/
Chromosome* GAEngine::getBestChromosome(){
	return bestChromosome;
}

/**平均適応度の取得
*/
double GAEngine::getAvgFitness(){
	return avgFitness;
}

/**最適な回答を返す
*
*@param[in] solution 回答を保存する配列のポインタ
*/
void GAEngine::getBestSolution(int* solution){
	bestChromosome->getSolution(solution);
}

/**回答の出力
*
*@param[in] solutuin 回答を保存する配列のポインタ
*@param[in] pop_id 世代番号
*/
void GAEngine::getSolutions(int* solution,int pop_id){
	population[pop_id]->getSolution(solution);
}

/*デストラクタ（お片づけ関数）
*
*/
GAEngine::~GAEngine(){
	free(population);
}
