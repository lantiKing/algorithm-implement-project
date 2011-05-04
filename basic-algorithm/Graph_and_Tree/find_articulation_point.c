/*間接点(articulation point)を見つけるプログラム
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
*usage: DFS 3 5 test_graph.txt 
*
*/


#include <stdio.h>
#include <stdlib.h>

#define maxV 26//node名はアルファベット大文字のみ

_Bool debug_flag = 0;

struct node{
  int v;//node
  struct node *next;//次nodeへのポインタ
};

int j,x,y,V,E;//Vは頂点の数、Eは辺の数
struct node *t,*z;
struct node *adj[maxV];//隣接行列

int val[maxV];//訪問順番番号
int id=0;//訪問番号

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


  printf("-- creating adjacency list --\n");
  adjlist(argv[3]);

  printf("-- display adjacency list representation --\n");
  dumpadjList();

  printf("-- Find Articulation Point --\n");
  listdfs();

}

//隣接行列(adj)の生成
adjlist(char* glaphfile){

  FILE *fp;

  printf("vertex num[V]=%d edge num[E]=%d \n",V,E);
  printf("Glaph file : %s \n",glaphfile);
  char v1,v2,row[3];
  
  //ファイルを開く
  fp=fopen(glaphfile,"r");
  if(!fp){
    printf("can not open %s",glaphfile);
    exit(1);
  }


  z=(struct node *)malloc(sizeof(*z));
  z->next = z;

  for(j=1; j<=V; j++) adj[j]=z;

  for(j=1; j<=E; j++){
    //ファイルを一行読み込み
    fgets((char*)row,6,fp);
    sscanf(row,"%c-%c",&v1,&v2);

    x=getNodeID(v1); y=getNodeID(v2);
    if(debug_flag==1) printf("[debug:adjlist] get : %c(%d) <-> %c(%d) \n",v1,x,v2,y);

    t=(struct node *)malloc(sizeof(*t));
    t->v = x;
    t->next = adj[y];//今のadj[y]は新しいadj[y]のnext
    adj[y]=t;
 
    printf("set : %c(%d) <-",getNodeName(t->v),t->v);

    t=(struct node *)malloc(sizeof(*t));
    t->v = y;
    t->next = adj[x];
    adj[x]=t;

    printf("-> %c(%d)\n",getNodeName(t->v),t->v);

    if(debug_flag==1) printf("[debug:adjlist] adj[%d]->%c(%d)\n",y,getNodeName(adj[y]->v),adj[y]->v);
    if(debug_flag==1) printf("[debug:adjlist] adj[%d]->%c(%d)\n",x,getNodeName(adj[x]->v),adj[x]->v);
  }


}

//文字コードをIDに変換
int getNodeID(char *nodeName){
  return (int)(nodeName-'A'+1);
}

//IDを文字コードに直す
int getNodeName(int nodeID){
  return (int)(nodeID+'A'-1);
}


//深さ優先探索(DFS)を改造
listdfs(){
  int k;
  for(k=1; k<=V; k++) val[k]=0;//初期化
  for(k=1; k<=V; k++) if(val[k]==0) visit(k);
}

int visit(int k){
	int m,min;	
	struct node *t;
	
	val[k] = ++id;
	min=id;
	
	//間接点を探す
	for(t=adj[k]; t!=z; t=t->next)
    	if(val[t->v]==0){
		m=visit(t->v);
		if(m < min) min = m;
		if(m >= val[k]) printf(" %c is articulation point \n",getNodeName(k));
		}
	return min;
}


//隣接リストを出力
dumpadjList(){
  int i,k;
  struct node *n;

  for(k=1; k<=V; k++){
    for(i=0; i<=V; i++) val[i]=0;//初期化
    printf("%c(%d) : ",getNodeName(k),k);
    for(n=adj[k]; n!=z; n=n->next)
      printf("%c(%d) -> ",getNodeName(n->v),n->v);

    printf("end \n");
  }
}

