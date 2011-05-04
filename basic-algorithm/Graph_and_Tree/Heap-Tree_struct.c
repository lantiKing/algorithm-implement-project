/*構造体でheapを実装してみるテスト*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct binary {         /* 2分木のデータ構造 */
    int  data;           /* 節（データ） */
	int priority;
	struct binary *dad;//親の番地
    struct binary   *left;//左の枝の節の番地
    struct binary   *right;//右の枝の節の番地
};

//struct binary  root = {-1,100,&root, &root};    /* 最初の節の番地 */
struct binary  *root;    /* 最初の節の番地 */
struct binary  *last;

int nodenum = 0;

main(){
	printf("start!! \n");
	
	//last = &root;
	
	insert(2,2);
	insert(6,6);
	insert(9,9);
	insert(3,3);
	insert(1,1);
	
	
	printf("printting tree \n");
	tree_print(root);
	printf("end tree \n");
}

insert(int data,int priority){
	
	nodenum++;
	
	printf("[debug:insert] %d(%d)\n",data,priority);
	struct binary  *new;
	
	
	//新しいnode、newを作成
	if((new = (struct binary *)malloc(sizeof(struct binary))) == NULL){
        printf("領域を確保できません\n");
        exit(1);
    }
	
	if(root == NULL) root=new;
	if(last == NULL){
		last=new;
		return;
	}
	new->data = data;
	new->priority = priority;
	new->dad=last;
	new->left=NULL;
	new->right=NULL;
	
	
	
	//左から順番につめる
	new->dad = last;
	if(last->left == NULL){
		last->left=new;
	}else if(last->right == NULL){
		last->right=new;
		last=new;
	}
	shiftup(new);
}

shiftup(struct binary *node){
	
	
	//printf("[debug:shiftup]dad (%d)  node (%d) \n",node->dad->priority,node->priority);
	
	//親のほうが優先度が高かったら
	
	if(  node != root  && node->dad != NULL && (node->dad->priority) > (node->priority)){
	//if(  (node->dad->priority) > (node->priority)){
		//親子を入れ替える
		swap(node->dad,node);
		printf("shuft up \n");
		//親をshiftup
		shiftup(node->dad);
	}
}

swap(struct binary *node1, struct binary *node2){
	printf("[debug:swap] %d(%d) <-> %d(%d)\n",node1->data,node1->priority,node2->data,node2->priority);
	//struct binary  *temp;
	int tempdata,temppriority;
	//いったんとっとく
	tempdata=node1->data;
	temppriority=node1->priority;
	node1->data=node2->data;
	node1->priority=node2->priority;
	node2->data=tempdata;
	node2->priority=temppriority;
	
	if(node1== root) root = node2;
	if(node1== last) last = node2;
	
}

tree_print(struct binary *p){
	
	//printf("[debug:tree_print] %d(%d) \n",p->data,p->priority);
	
	
    static int  depth = 0;

    if(p->right != NULL){
        ++depth;
        tree_print(p->right);
        --depth;
    }
	
	if(nodenum < 1) exit(1);
	int i;
	for(i=depth; i>0 ;i--) printf("  ");
	printf("%d\n",p->data);
	nodenum--;
	
    if(p->left != NULL){
		//printf("left %d\n",depth);
        ++depth;
        tree_print(p->left);
        --depth;
    }
}
