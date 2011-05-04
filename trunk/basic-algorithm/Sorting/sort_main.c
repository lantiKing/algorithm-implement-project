/** ソートアルゴリズムのサンプル
 *
 */

#include <stdio.h>
#include <stdlib.h> //rand用
#include <unistd.h> //getopt用
#include <time.h> //srandと実行時間の測定用


void help(char* filename);
void dump_array(int array[],int len);


void bouble_sort(int data[],int n);
void quick_sort(int data[],int n);
void quick_sort_body(int left,int right,int data[]);

enum SORT_TYPE{BOUBLE,QUICK,MARGE,COMB};
enum INPUT_TYPE{RAND,STDINPUT};


//
// ソート関数
//

/** バブルソート
 *
 * @param[in] array 並べ替えたい数列
 * @param[in] n 数列の数字の数
 *
 * 特性：安定
 * 計算量：O(n^2)
 */
void bouble_sort(int data[],int n){
  int i,temp,flag,last=0;
  do{
    flag=0;
    //!(1)左から探索、ただし後ろは整列済みなので省略
    for(i=0;i<n-1-last;i++){
      if(data[i]>data[i+1]){//!(2)左右の並びがおかしければ入れ替え
        flag=1;
        temp=data[i];
        data[i]=data[i+1];
        data[i+1]=temp;
      }
    }
    last++;
  }while(flag==1);//!(3)一度も入れ替えなかったら終了
}

/** クイックソート
 *
 * @param[in] array 並べ替えたい数列
 * @param[in] n 数列の数字の数
 *
 * 特性：安定
 * 計算量：O(n^2)
 */
void quick_sort(int data[],int n){
  quick_sort_body(0,n-1,data);
}
void quick_sort_body(int left,int right,int data[]){
  printf("quick [%d to %d]\n",left,right);
  dump_array(data,10);
  int l,r,pivot,temp;
  
  if(left>=right-1) return;
  
  //(1)適当に１つ選ぶ(とりあえず、調査区間の真ん中のやつ)
  pivot=data[(left + right) / 2];
  
  //(2)調査区間(topからbottomまで)を調べる
  for(l=left, r=right; l<r;){
  //while(1){
    
    //(3)左側でしきい値より大きい物を探し、その位置を左indexとする
    //while(l<=r && data[l]<=pivot){
    while(l<=r && data[l]<pivot){
      printf("%d <= %d\n",data[l],pivot);
      l++;
    }
    
    //(4)右側でしきい値より小さい物を探し、その位置を右indexとする
    while(l<=r && pivot<data[r]){
      printf("%d > %d\n",data[r],pivot);
      r--;
    }
    
    if(l<r){//左indexが右indexより小さかったら値を入れ替え
      temp=data[l];
      data[l]=data[r];
      data[r]=temp;
    }//else if(l>=r) break;//左indexが右indexより大きくなってしまったら終了
    
    //l++;
    //r--;
  }
  
  //最初に選択した値を真ん中に持ってく
  
  temp=data[left];
  data[left]=data[r];
  data[r]=temp;
  
  if(left < l-1)//左側にまだ2つ以上残ってたら
    quick_sort_body(left,l-1,data);
  
  if(r+1 < right)//右側にまだ2つ以上残ってたら
    quick_sort_body(r+1,right,data);
  
}


//
// メイン関数と補助関数
//

int main(int argc,char *argv[]){
  
	int i,result;
  clock_t time_start,time_end;
  srand((unsigned)time(NULL));
  
  int array_length=10;//!< 数列の長さ
  enum SORT_TYPE sort_type = BOUBLE;//!< ソートの種類
  enum INPUT_TYPE input_type = RAND;//!< 数列の入力方法
  
  /* ループしてgetoptを使用しオプションを一つずつ取得して行く
   （「:」がついてるのは引数あり）
   */
  while((result=getopt(argc,argv,"bqmchn:i"))!=-1){
    switch(result){//指定されたオプションで切り替え
        //ソート手法
      case 'b'://バブルソート
        sort_type=BOUBLE;
        printf("use Bouble sort\n");
        break;
      case 'q'://クイックソート
        sort_type=QUICK;
        printf("use Quick sort\n");
        break;
      case 'm'://マージソート
        sort_type=MARGE;
        printf("use Marge sort\n");
        break;
      case 'c'://コームソート
        sort_type=COMB;
        printf("use Comb sort\n");
        break;
        
        //その他のオプション
      case 'n'://数列の長さ（値を取る引数の場合は外部変数optargにその値を格納されてる。）
        array_length=atoi(optarg);
        printf("sorting %d numbers\n",array_length);
        break;
      case 'i'://数字を自分で指定する
        input_type=STDINPUT;
        printf("setting numbers by myself\n");
        break;
      case 'h'://ヘルプを表示
        help(argv[0]);
        exit(0);
        break;
    }
  }
  
  int numbers[array_length];
  
  if(input_type==STDINPUT){//自分で決めるとき
    for(i=0;i<array_length;i++){
      printf("numbers[%d]->",i);
      scanf("%d",&numbers[i]);
    }
  }else{//ランダムに決めるとき
    for(i=0;i<array_length;i++){
      numbers[i]=rand()%10;//乱数
    }
  }
  
  printf("first array (%d numbers) ======\n",array_length);
  dump_array(numbers,array_length);
  printf("======\n");
  time_start = clock();//実行時間測定開始
  
  switch(sort_type){
    case BOUBLE:
      bouble_sort(numbers,array_length);
      break;
    case QUICK:
      quick_sort(numbers,array_length);
      break;
  }
  
  time_end = clock();//実行時間測定終了
  printf("result ======\n");
  dump_array(numbers,array_length);
  
  printf("time = %lf[sec]\n", (double)(time_end-time_start)/CLOCKS_PER_SEC);
  printf("======\n");
  
  return 0;
}

/** ヘルプを表示
 * @param[in] array 実行ファイル名
 */
void help(char* myname){
  printf("usage: %s [h] [bqmc] [f csv_file]\n",myname);
  printf("\t-b\t\tbouble sort\n");
  printf("\t-q\t\tquick sort\n");
  printf("\t-m\t\tmarge sort\n");
  printf("\t-c\t\tcomb sort\n");
  
  printf("\t-f filename\tnumber list (csv file)\n");
  
  printf("\t-h\t\tprint this help\n");
}

/** 数字配列の中身を表示
 * @param[in] array 数字の配列
 * @param[in] len 配列の長さ
 */
void dump_array(int array[],int len){
  int i;
  for(i=0;i<len;i++) printf("%d,",array[i]);
  printf("\n");
}
