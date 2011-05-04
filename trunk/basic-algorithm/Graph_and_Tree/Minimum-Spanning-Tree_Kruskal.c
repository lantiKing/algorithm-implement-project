/*MST(Minimum Spanning Tree) 最小木
*kruskal法を使って
*
*@param[in] V_num : 頂点(Vertex)の数
*@param[in] E_num : 辺(Edge)の数
*@param[in] Graph_file : グラフを定義したファイル
* ex) test_graph.txt
*   A-B:1
*   B-C:5  
*   C-D:2  
*   B-D:2  
*   A-D:1  
*
*usage: kruskal 4 5 test_glaph.txt
*
*/


#include <stdio.h>
#include <stdlib.h>

#define maxV 26//node名はアルファベット大文字のみ
#define maxE 676//

_Bool debug_flag = 0;

struct edge{
  char v1;//node v1
  char v2;//node v2
  int w;//wait
};

struct pqueue {
  int data;//データ
  int priority;//優先度
  struct pqueue *next;
};
static struct pqueue *first;

int i,j,m,V,E;//Vは頂点の数、Eは辺の数
struct edge e[maxE];//隣接行列
int dad[maxE];//親ノードを格納

//番兵
static int unseen = sizeof(int);

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

	printf("-- create Minimum Tree --\n");
	Kruskal(argv[3]);
	
}

//Kruskalのアルゴリズムによる最小木の作成
Kruskal(char* glaphfile){
	FILE *fp;
	
	printf("vertex num[V]=%d edge num[E]=%d \n",V,E);
	printf("Glaph file : %s \n",glaphfile);
	char row[10];
	
	//ファイルを開く
	fp=fopen(glaphfile,"r");
	if(!fp){
		printf("can not open %s",glaphfile);
		exit(1);
	}else{
		if(debug_flag == 1) printf("success!! open file \n");
	}
	
	for(j=1; j<=E; j++){
		//ファイルを一行読み込み
		fgets((char*)row,10,fp);
		sscanf(row,"%c-%c:%d",&e[j].v1,&e[j].v2,&e[j].w);
		printf("set e[%d] : %c <-> %c  : %d \n",j,e[j].v1,e[j].v2,e[j].w);		
	}
	
	//Kruskal法
	printf("-- Start Kruskal --\n");
	//順位付きキューが空じゃないなら
	for(pqconstruct(), i=0; pqueueempty()!=1;){
		m=pqremove();
		
		//閉路ができないなら
		if( find( getNodeID(e[m].v1),getNodeID(e[m].v2),1 ) ){
			//edgefound(e[m].v1,e[m].v2);
			printf("e[%2d] : %c - %c \n",m,e[m].v1,e[m].v2);
			i++;
		}
	}
	printf("-- End Kruskal --\n");
	
}

//順序付きキュー関連

//辺をすべて順序付きキューに格納
pqconstruct(){
	for(j=1; j<=E; j++){
		if(debug_flag==1) printf("[degub:pqconstruct] insert e[%d] by priority %d \n",j,e[j].w);
		pqinsert(j,e[j].w);
		if(debug_flag==1) printf("[debug:pqconstruct] Dumping Priority Queue \n");
		if(debug_flag==1) pqdump();
	}
}

//データを順序付きキューに格納
pqinsert(int data,int priority){
	struct pqueue *lp=first;
	struct pqueue *my;
	
	_Bool flag=0;
	
	// 領域確保（新しい節を作成
    if((my = (struct pqueue *)malloc(sizeof(struct pqueue))) == NULL){
		printf("error : Can not allocate memory\n");
        exit(1);
    }
	
	my->data = data;
	my->priority = priority;
	my->next = NULL;
	
	//priorityが小さいのが最初にくるように挿入
	if(lp!=NULL){
		struct pqueue *dadqueue = first;
		if(debug_flag==1) printf("[debug:pqinsert] lp is not NULL first data=%d priority=%d \n",lp->data,lp->priority);
		//キューの最後にたどりつくまで
		while( lp->next!=NULL ){
			//もし自分のpriorityのほうがちいさかったら
			if( (lp->priority) > priority){
				//先頭なら
				if(lp==first){
					my->next=lp;
					//自分が新しい先頭になる。
					first=my;
					flag=1;
					break;
				}else{
					my->next=lp;
					//自分がlpとlpの親の間に入る
					dadqueue->next=my;
					flag=1;
					break;
				}
			}
			dadqueue=lp;
			lp=lp->next;//次のキュー
		}
		//途中に挿入しなかったなら最後に追加
		if(flag!=1) lp->next=my;
			
	}else{//キューが空なら
		if(debug_flag==1) printf("[debug:pqinsert] pqueue is NULL \n");
		//自分が先頭だ
		first= (struct pqueue *)malloc(sizeof(struct pqueue));
		first=my;
		
		if(debug_flag==1) printf("[debug:pqinsert] data = %d priority = %d \n",first->data,first->priority);
	}
	
}

//順序付きキューからデータを取り出す。
int pqremove(){
    struct pqueue *lp = first;
    int data;
    if(pqueueempty()){// データがキューにない
    	return -1;
    }
    data = lp->data;
    lp = lp->next;
    free(first);
    first=lp;
    return data;
}

//ヒープが空なら1
int pqueueempty(){
	struct pqueue *lp = first;
	if(lp==NULL) return 1;
	return 0;
}

//順序付きキューの内容を出力
pqdump(){
	int k;
	struct pqueue *lp=first;
	
	printf("-- dump pq --\n");
	
	if(lp!=NULL)while(lp->next!=NULL){
			printf("lp->data=%d  v1=%c v2=%c w=%d\n",lp->data,e[lp->data].v1,e[lp->data].v2,e[lp->data].w);
		lp=lp->next;
		}
	
	printf("-- dump end --\n");
}


//doit = 1 なら合併する
int find(int x, int y, int doit){
	if(debug_flag==1) printf("[debug:find] find %d %d union flag is %d \n",x,y,doit);
	int t, i=x, j=y;
	//親を調べる
	while(dad[i] > 0) i=dad[i];
	while(dad[j] > 0)j=dad[j];
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
