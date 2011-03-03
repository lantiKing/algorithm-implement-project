/*
 *  Chromosome.h
 *  CA brittle star
 *
 *  Created by sunil lal on 6/24/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *染色体の設定
 */

#include <stdlib.h>
#include <iostream>

#ifndef _CHROMOSOME_H
	#define _CHROMOSOME_H

class Chromosome {
	public:
	Chromosome();
	Chromosome(int,int);
	~Chromosome();
	
	int getLength();
	void setGene(int, int);
	int getGene(int);
	void setFitness(double);
	double getFitness();
	
	void mutate(double);
	//void crossover(Chromosome, Chromosome &, Chromosome &);
	//void crossover(Chromosome , Chromosome*, Chromosome*);
	void crossover(Chromosome*);
	
	void getSolution(int*);
	void print();
		
  private:
	
	int *gene;
	int length;
	int gene_type_num;
	double fitness;
	
};

#endif
