/*ネットワーク流問題
*
*@param[in] V_num : 頂点(Vertex)の数
*@param[in] E_num : 辺(Edge)の数
*@param[in] Graph_file : グラフを定義したファイル
* ex) test_graph.txt
*   A->B:4
*   B->C:1 
*   C->D:3
*   B->D:2   
*   A->D:1 
*
*usage: nf 3 5 test_graph.txt 
*
*/


#include <stdio.h>
#include <stdlib.h>

#define maxV 26//node名はアルファベット大文字のみ
#define maxE 266//

#define unseen 5000//「まだ見てないよ」な番兵

_Bool debug_flag = 0;

struct pqueue {
  int data;
  int priority;
  struct pqueue *next;
};
static struct pqueue *first;

struct node{
  int v;//node
  int s;//size
  int f;//flow
  struct node *next;//次nodeへのポインタ
};

int j,x,y,V,E;//Vは頂点の数、Eは辺の数
struct node *t,*z;
struct node *adj[maxV];//隣接行列

int val[maxV];//訪問順番番号
int id=0;//訪問番号
int dad[maxE];//親ノードを格納

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

  printf("-- creating Priority-First Search(PFS) tree --\n");
  listpfs();
  printf("-- PFS tree created --\n");

}

//隣接行列(adj)の生成
adjlist(char* glaphfile){

  FILE *fp;

  printf("vertex num[V]=%d edge num[E]=%d \n",V,E);
  printf("Glaph file : %s \n",glaphfile);
  char v1,v2,row[10];
	int w;
	
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
		fgets((char*)row,10,fp);
		sscanf(row,"%c-%c:%d",&v1,&v2,&w);

		x=getNodeID(v1); y=getNodeID(v2);
		if(debug_flag==1) printf("[debug:adjlist] get : %c(%d) -> %c(%d) : %d\n",v1,x,v2,y,w);

		t=(struct node *)malloc(sizeof(*t));
		t->v = x;
		t->w = w;
		t->next = adj[y];//今のadj[y]は新しいadj[y]のnext
		adj[y]=t;
 
		printf("[adjlist] %d set : %c(%d):%d <-",j,getNodeName(t->v),t->v,t->w);

		t=(struct node *)malloc(sizeof(*t));
		t->v = y;
		t->w = w;
		t->next = adj[x];
		adj[x]=t;
		
			printf("-> %c(%d):%d\n",getNodeName(t->v),t->v,t->w);
		
		if(debug_flag==1) printf("[debug:adjlist] adj[%d]->%c(%d):%d\n",y,getNodeName(adj[y]->v),adj[y]->v,adj[y]->w);
		if(debug_flag==1) printf("[debug:adjlist] adj[%d]->%c(%d):%d\n",x,getNodeName(adj[x]->v),adj[x]->v,adj[x]->w);
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


//順位優先探索(Priority-First Search)
listpfs(){
  int k;
  for(k=1; k<=V; k++) val[k]=-unseen;//初期化
  for(k=1; k<=V; k++) if(val[k]==-unseen) visit(k);
	
	for(k=1; k<= E; k++) if(dad[k]!=0) printf("%c -> %c \n",getNodeName(dad[k]),getNodeName(k));
	
}

visit(int k){
  struct node *t;
	
	if(debug_flag==1) printf("[debug:visit] visit %c :  \n",getNodeName(k));
	
	if( pqupdate(k,unseen) != 0) dad[k]=0;
	while(!pqueueempty()){
		id++;
		k=pqremove();
		val[k]=-val[k];
		if(val[k] = -unseen) val[k]=0;
		
		for(t=adj[k]; t!=z; t=t->next){
			if(val[t->v]<0){
				if(pqupdate(t->v, t->w)){
					if(debug_flag==1) printf("[debug:visit]check %c - %c \n",getNodeName(k),getNodeName(t->v));
					val[t->v]=-(t->w);
					dad[t->v]=k;
				}
			}
		}
		
	}	
}

//dataが存在して、かつpriorityが新しいpriorityより大きい->優先度を更新する
//dataが存在しない->insert
//どちらでもない->何もしない
int pqupdate(int data,int priority){
	struct pqueue *lp=first;
	
	if(debug_flag==1)  printf("[debug:pqupdate] %d:%d check \n",data,priority);
	
	if(lp!=NULL){
		while(lp->next!=NULL){
			
			//もし現在登録されているpriorityより新しいpriorityのほうが小さかったら
			if((lp->data == data) && (lp->priority > priority) ){ 
				if(debug_flag==1)  printf("[debug:pqupdate] chenge %d : %d->%d \n",lp->data,lp->priority,priority);
				pqchenge(data,priority);//priorityを更新
				return 1;
			
			//もし現在登録されているpriorityより新しいpriorityのほうが大きかったら
			}else if(lp->data == data && lp->priority <= priority){
				if(debug_flag==1)  printf("[debug:pqupdate] Nothing to do \n");
				return 0;
			}
		lp=lp->next;
		}
	}
	
	//同じデータが格納されてなかったら
	if(debug_flag==1)  printf("[debug:pqupdate] insert %d : %d \n",data,priority);
	pqinsert(data,priority);
	if(debug_flag==1)  pqdump();
	return 1;
}

//キュー内のdataのpriorityを更新する
//キュー内にdataがあることはpqupdateでチェック済み、、、のはず
pqchenge(int data,int newpriority){
	struct pqueue *lp=first;
	struct pqueue *dadqueue=first;
	
	if(lp!=NULL){
		while(lp->next!=NULL){
			if(lp->data == data){//priorityを更新したいデータが見つかったら
					//とりあえずそのデータを削除
					dadqueue->next = lp->next;
					//もいちど入れ直す
					pqinsert(data,newpriority);
					break;
			}
			dadqueue = lp;
			lp=lp->next;
		}
	}
}

//キューにデータを追加
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

		while( lp->next!=NULL ){//キューの最後にたどりつくまで
			if( (lp->priority) > priority){//もし自分のpriorityのほうがちいさかったら
				if(lp==first){//lp=firstなら
					my->next=lp;
					first=my;
					flag=1;
					break;
				}else{
					my->next=lp;
					dadqueue->next=my;
					flag=1;
					break;
				}
			}
			dadqueue=lp;
			lp=lp->next;//次のキュー
		}
		if(flag!=1) lp->next=my;//途中に挿入しなかったなら最後に追加
			
	}else{//キューが空なら
		if(debug_flag==1) printf("[debug:pqinsert] pqueue is NULL \n");
		//myがfirstだ。
		first= (struct pqueue *)malloc(sizeof(struct pqueue));
		first=my;
		
		if(debug_flag==1) printf("[debug:pqinsert] data = %d priority = %d \n",first->data,first->priority);
	}
	
}


//キューから１つ取り出す。
int pqremove(){
    struct pqueue *lp = first;
    int data;
    if(pqueueempty()){// データがキューにない
    	return -1;
    }
	
	if(debug_flag==1)  printf("[debug:pqremove] remove %d \n",lp->data);
	
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

//-- おまけ --//
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

//pqueueを出力
pqdump(){
	int k;
	struct pqueue *lp=first;
	
	printf("-- dump pq --\n");
	
	if(lp!=NULL)while(lp->next!=NULL){
		//printf("lp->data=%d  v1=%c v2=%c w=%d\n",lp->data,e[lp->data].v1,e[lp->data].v2,e[lp->data].w);
		printf("data=%d  priority=%d \n",lp->data,lp->priority);
		lp=lp->next;
	}
	
	printf("-- dump end --\n");
}