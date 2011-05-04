/*2分木(Binary Tree)
*http://www005.upp.so-net.ne.jp/h-masuda/cl/CAlgo/btree02.html
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct binary {         /* 2分木のデータ構造 */
    char        *key;           /* 節（データ） */
    struct binary   *left;      /* 左の枝の節の番地 */
    struct binary   *right;     /* 右の枝の節の番地 */
};
struct binary  root = {NULL, &root, &root};    /* 最初の節の番地 */

void tree_print(struct binary *);
int tree_insert(char *);

main()
{
    char    buff[256];

    while(1){
        printf("文字列データ : ");
        gets(buff);
        if(!strcmp(buff, "end"))
            break;
        tree_insert(buff);
        tree_print(&root);
    }
}


void tree_print(struct binary *p)
{
    static int  depth = 0;

    if(p->right != NULL){
        ++depth;
        tree_print(p->right);
        --depth;
    }
    printf("%*c%s\n", 5 * depth, ' ', p->key);
    if(p->left != NULL){
        ++depth;
        tree_print(p->left);
        --depth;
    }
}


int tree_insert(char *str)      /* 2分木構造に格納する */
{
    struct binary  *pp, *new;
    int     cmp;
    char    *s;

    pp = &root;         /* ルートの番地を格納 */
	
    /* 文字列データを格納する領域を確保 */
    if((s = (char *)malloc(strlen(str) + 1)) == NULL){
        printf("領域を確保できません\n");
        exit(1);
    }
    strcpy(s, str);         /* 確保した領域に文字列データを格納 */

	
    if(pp->left == &root || pp->right == &root){    /* 最初のデータ格納 */
        pp->key = s;
        pp->left = NULL;
        pp->right = NULL;
        return(1);
    }

    /* 領域確保（新しい節を作成） */
    if((new = (struct binary *)malloc(sizeof(struct binary))) == NULL){
        printf("領域を確保できません\n");
        exit(1);
    }
    while(1){           /* 枝をたどって節を探す */
        cmp = strcmp(pp->key, str);
        if(cmp == 0){       /* 登録済み */
            free(s);            /* 領域開放 */
            free(new);          /* 領域開放 */
            return(0);
        }else if(cmp > 0){
            if(pp->left == NULL){
                pp->left = new;
                break;
            }
            pp = pp->left;      /* 次の節を探す */
        }else{
            if(pp->right == NULL){
                pp->right = new;
                break;
            }
            pp = pp->right;     /* 次の節を探す */
        }
    }
    new->key = s;       /* 新しい節にデータを登録 */
    new->left = NULL;
    new->right = NULL;
    return(1);
}