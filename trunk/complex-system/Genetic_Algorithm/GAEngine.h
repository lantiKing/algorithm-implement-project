/*
 *  GAEngine.h
 *  CA brittle star
 *
 *  Created by sunil lal on 6/24/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */
 
#include "Chromosome.h"

class GAEngine {

	public:
		GAEngine(int,int,int,double,double);
		~GAEngine();
		void selection();
		void crossover();
		void mutation();	
		void collateResults();
		
		double getAvgFitness();
		Chromosome* getBestChromosome();
		void print();
		void getSolutions(int*,int);
		void getBestSolution(int*);
		void setFitness(int,double);
		
	private:
	
		Chromosome** population;
		Chromosome* bestChromosome;
		double avgFitness;
		
		int gene_num;
		int gene_type_num;
		int population_num;
		double cross_rate;
		double mutation_rate;
};

