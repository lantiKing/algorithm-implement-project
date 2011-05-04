/*UFT(Union-Find Tree) 合併-発見木
*重さ均衡法(Weight Balancing)+道短縮法(Path compression)を使用
*
*@param[in] V_num : 頂点(Vertex)の数
*@param[in] E_num : 辺(Edge)の数
*@param[in] Graph_file : グラフを定義したファイル
* ex) test_graph.txt
*   A-B    
*   B-C    
*   C-D    
*   B-D    
*   A-D    
*
*usage: UFT 3 5 test_graph.txt 
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define maxV 26//node名はアルファベット大文字のみ

_Bool debug_flag = 0;

int V,E;//Vは頂点の数、Eは辺の数

int dad[maxV];//親ノードを格納

main(int argc, char* argv[]) {
  int count;

  if(argc <= 2 || 4 < argc){
    printf("usage : %s V_num E_num Graph_file \n",argv[0]);
    exit(1);
  }

  if( ((V=atoi(argv[1]))<=0) || (E=atoi(argv[2]))<=0){
    printf("cant set graph size \n");
    printf("usage : %s V_num E_num Graph_file\n",argv[0]);
    exit(1);
  }
	
	FILE *fp;

	printf("vertex num[V]=%d edge num[E]=%d \n",V,E);
	printf("Glaph file : %s \n",argv[3]);
	char v1,v2,row[3];
	
	//ファイルを開く
	fp=fopen(argv[3],"r");
	if(!fp){
		printf("can not open %s",argv[3]);
		exit(1);
	}
	
	int temp,x,y;
	for(count=0; count<=maxV; count++) dad[count]=0;
	
	for(count=1; count<=E; count++){
		//ファイルを一行読み込み
		fgets((char*)row,6,fp);
		sscanf(row,"%c-%c",&v1,&v2);
		if(debug_flag==1) printf("[debug:main] read : %c <-> %c \n",v1,v2);
		
		x=getNodeID(v1); y=getNodeID(v2);
		if(debug_flag==1) printf("[debug:main] get : %c(%d) <-> %c(%d) \n",v1,x,v2,y);
		
		 temp=find(x,y,1);//union-find treeを作成
	}
	
	//木を出力してみる
	printf("-- view Union Find Tree --\n");
	for(count=1; count<=V; count++){
		if(debug_flag==1) printf("[debug:main] %d dad is %d  \n",count,dad[count]);
		
		//重さ均衡法により、dad[根]は-(子孫の数)になっている
		if(dad[count] < 0) dad[count]=count;
		printf("%c -> %c \n",getNodeName( dad[count] ),getNodeName(count));
	}
}


//doit = 1 なら合併する
int find(int x, int y, int doit){
  if(debug_flag==1) printf("[debug:find] find %d %d union flag is %d \n",x,y,doit);
  int t, i=x, j=y;
	
	//親を調べる。
  while(dad[i] > 0) i=dad[i];
  while(dad[j] > 0) j=dad[j];
  while(dad[x]>0){
    t=x;
    x=dad[x];
    dad[t]=i;
  }
  while(dad[y]>0){
    t=y;
    y=dad[y];
    dad[t]=j;
  }
	
	//合併する
  if( (doit != 0) && (i != j) ){
    if(dad[j] < dad[i]){
      dad[j] += (dad[i]-1);
      dad[i]=j;
    }else{
      dad[i] += (dad[j]-1);
      dad[j]=i;
    }
  }
  if(debug_flag==1) printf("[debug:find] dad[%d]=%d  dad[%d]=%d \n",i,dad[i],j,dad[j]);
  return (i != j);;
}

//文字コードをIDに変換
int getNodeID(char *nodeName){
  return (int)(nodeName-'A'+1);
}

//IDを文字コードに直す
int getNodeName(int nodeID){
  return (int)(nodeID+'A'-1);
}

