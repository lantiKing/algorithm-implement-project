/*
 *  buffon.h
 *  
 *
 *  Created by yosilove on 平成 20/12/22.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#include <unistd.h>
#include <getopt.h>//実行時オプションの判定
#include <time.h>//rand用seed

#define DEFAULT_LOOP 1
#define DEFAULT_NUM 1000
#define DEFAULT_DISTANCE 2.0
#define DEFAULT_NEEDLE 1.5
#define PI_MAX 6.0
#define THETA_MAX 180.0

void usage(char *myname);