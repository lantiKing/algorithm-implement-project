/*
 *  mci.c
 *
 *  Monte calro integral
 *
 *  Iy = integral_{1}^{2} cos(loge(x)) dx
 *
 *  Created by yosilove on 平成 20/12/25.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 */

/*  [参考]
 *  モンテカルロ法 http://ja.wikipedia.org/wiki/モンテカルロ法
 *  モンテカルロ積分法 http://www.akita-nct.ac.jp/yamamoto/lecture/2005/5E/integrate/integral_html/node5.html
 */

#include "mci.h"

int main(int argc, char **argv){
    
    srand((unsigned)time(NULL));
    
    /*Initialize*/
    double
    temp=0.0f,
    Ir=1.0f*1.0f,//Rectangle : y=0,y=1,x=1,x=2
    Iy,//Integral y : integral_{1}^{2} cos(loge(x)) dx
    //random points
    Px,Py,
    y;//y = cos(loge(Px))
    
    int
    loop = DEFAULT_LOOP,
    count,
    N = DEFAULT_NUM,//MAX Num
    n = 0,//counter
    Nin = 0;// number of internal dot
    
    //Read Exec options (実行オプションの読み込み)
    int option;
    while( (option = getopt(argc, argv,"n:L:h"))!=-1 ){
        switch(option){
			case 'n':
				N = atoi(optarg);
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
    
    printf("# N = %d\n",N);
    printf("# Px Py\n");
    //-- 面積の計算 --//
    for(count=0;count<loop;count++){
        for(n=1;n<=N;n++){
            //ランダムな点の生成(Generate random points)
            //1 <= Px <= 2
            Px = ((double)rand()/RAND_MAX)+1;
            //0 <= Py <= 1
            Py = ((double)rand()/RAND_MAX);
            
            //Point
            printf("%f %f\n",Px,Py);
            
            //calculate y
            y=cos(log(Px));
            
            //図形内に入っているかどうか？？
            //if Py <= y, increment Nin
            if(Py <= y){
                printf("# y=%f so [in]\n",y);
                Nin++;
            }else printf("# y=%f so [out]\n",y);
            
        }
        
        // Iy/Ir = Nin/N so Iy = (Nin/N)Ir
        Iy = ((double)Nin/(double)N)*Ir;
        
        printf("\n## Result ##\n");
        printf("# Ir=%f,Nin=%d,N=%d\n",Ir,Nin,N);
        printf("# Iy = %f\n",Iy);
        
        Nin=0;
        temp+=Iy;
    }
    //面積の近似値
    printf("# Iy average = %f\n",temp/count);
    
    
    return 0;
}

void usage(char *myname){
    printf("%s [nh]\n",myname);
    printf("\t -n <Loop Number>:Loop Number (default:10000)\n");
    printf("\t -h :show this message\n");
}