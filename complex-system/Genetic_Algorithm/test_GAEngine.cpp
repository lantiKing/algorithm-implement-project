#include "Chromosome.h"
#include "GAEngine.h"
#include <iostream>


#define MAX_GENERATION 10
#define GENE_NUM 3
#define GENE_TYPE_NUM 20
#define POP_SIZE 5
#define CROSS_RATE 0.2
#define MUTATION_RATE 0.1

int main(){

	GAEngine ga(GENE_NUM,GENE_TYPE_NUM,POP_SIZE,CROSS_RATE,MUTATION_RATE);
	int solution[GENE_NUM];
	int chro[GENE_NUM];
	double fitness=0.0;

	std::cout << "start Calculation" << std::endl;
	for(int i=0; i<=MAX_GENERATION; i++){//5世代まで
		std::cout << "generation :" << i << std::endl;

		//適応度の更新
		for(int pop_id=0; pop_id<POP_SIZE; pop_id++){
			fitness=0.0;
			std::cout << "\tpopulation :" << pop_id << std::endl;
			//染色体を取ってくる
			ga.getSolutions(chro,pop_id);
			//適応度を計算
			for(int gene_id=0;gene_id<GENE_NUM;gene_id++){
				std::cout << "\t\tChromosome[" << gene_id << "]=" << chro[gene_id] << std::endl;
				fitness+=chro[gene_id]*0.1;
			}

			std::cout << "\tfitness :" << fitness << std::endl;
			//適応度を設定
			ga.setFitness(pop_id,fitness);
		}

		std::cout << "collate " << std::endl;
		ga.collateResults();//結果を纏める
		std::cout<<"genaration:"<<i<<std::endl;
        std::cout<<"fitness:"<<ga.getBestChromosome()->getFitness()<<std::endl;
        std::cout<<"fitness(average):"<<ga.getAvgFitness()<<std::endl;
        
		ga.getBestSolution(solution);//一番いい結果をファイルに保存

		std::cout << "selection" << std::endl;
		ga.selection();//選択
		std::cout << "crossover" << std::endl;
		ga.crossover();//crossover
		std::cout << "mutation" << std::endl;
		ga.mutation(); //mutation
	}

	return 0;
}
