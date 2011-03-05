/*
 *  buffon.c
 *
 *  Buffon's Needle Problem
 *
 *  Created by yosilove on 平成 20/12/22.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 */
 
/*  [参考]
 *  Buffon's Needle http://www.ie.u-ryukyu.ac.jp/~e035736/C_html/buffon_needle.html
 *  コロキウム室(ビュッフォンの針の実験) http://www.junko-k.com/cthema/38buffon.htm
 *  Buffon's Needle http://www.mste.uiuc.edu/reese/buffon/buffon.html
 */

#include "buffon.h"

int main(int argc, char **argv){

  srand((unsigned)time(NULL));
  
  /*Initialize*/
  double
    loop = DEFAULT_LOOP,//LoopCount
    d = DEFAULT_DISTANCE,// distance that between line and line
    l = DEFAULT_NEEDLE,// length of needle
    x = 0.0f,// distance that between needle and line
    theta = 0.0f,// corner that needle and line make
    Pr = 0.0f,// probability of needle cross line
    pi = (rand() * PI_MAX) / (RAND_MAX);//pi(first random)
  
  int
    N = DEFAULT_NUM,//MAX Num
    n = 0,//counter
    Nc = 0;// number of needle cross line
  
  //Read Exec options (実行オプションの読み込み)
  int option;
  while( (option = getopt(argc, argv,"n:d:l:L:h"))!=-1 ){
    switch(option){
			case 'n':
				N = atoi(optarg);
				break;
      case 'd':
				d = atoi(optarg);
				break;
      case 'l':
				l = atoi(optarg);
				break;
      case 'L':
				loop = atoi(optarg);
				break;
			case 'h':
			default:
				usage(argv[0]);
				return 0;
				break;
		}
	}
  
  if(l>d){
    printf("# Warning!! Needle length must be less than Line distance!!\n");
    d = DEFAULT_DISTANCE;
    l = DEFAULT_NEEDLE;
  }
  
  printf("# Loop Number %d\n",N);
  printf("# Line Dinstance %f\n",d);
  printf("# Needle Length %f\n",l);

  int count;
  double temp=0.0f;
  for(count=0;count<loop;count++){
  
    /*Start solve Buffon's Needle probrem*/
    printf("# n Probability pi\n");
    for(n=1;n<N;n++){
      // Generate random x and theta number
      //0 <= x <= Line distance/2
      x = ((rand()*(d/2))/(RAND_MAX));
      
      //0 <= theta <= 180 [deg]
      theta = (rand() * THETA_MAX) / (RAND_MAX);

      // convert theta to radian
      //(0 <= theta <= pi [rad])
      theta = (theta / 180.0) * pi;

      //Needle is crossing line?
      if(!(x <= (l/2)*sin(theta))) continue;


      // number of needle cross line
      Nc++;//Needle count ++

      // probability of needle cross line
      Pr = ((double)Nc / (double)n);

      // it is trouble when Pr = 0
      if(Pr == 0){printf("#trouble !! set Pr=0.01\n"); Pr = 0.01f;}

      // calculate pi
      pi = (double)((2 * l) / (d * Pr));

      //print pi
      printf("%d %f %f\n",n,Pr,pi);
    }
  
  temp+=pi;
  Nc=0;
  }
  
  printf("pi = %f\n",temp/count);
  
  return 0;
}

void usage(char *myname){
    printf("%s [ndlh]\n",myname);
    printf("\t -n <Loop Number>:Loop Number (default:10000)\n");
    printf("\t -d <Line distance>:Line distance (default:2.0)\n");
    printf("\t -l <Needle length>:Needle length (default:1.5)\n");
    printf("\t -h :show this message\n");
}
