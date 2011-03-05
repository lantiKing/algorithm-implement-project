/** @file
 * 
 * @brief ハフマン符号の計算をするプログラム
 *
 * + 画像が扱えるのはOpenCVを持っている時だけ
 * + CSVファイルは[(int)要素,(float)生起確率]で書かれていること
 *   - CSVファイルの要素は必ず0から始まる連番であること
 *   - 生起確率の合計は1
 [csvファイルの例]
 --data.csv
0,0.1
1,0.2333
2,0.2
3,0.2
4,0.1333
5,0.1333
 --
 *
 *参考
 *[伊藤誠 講義・研究 ノート]ハフマン符号化による圧縮・解凍
 *http://www.ccad.sist.chukyo-u.ac.jp/~mito/syllabi/compaction/huffman/index.htm
 *
 * @author yosilove (univ.Ryukyu)
 * @date 2009-12-4
 */
	
#include  <stdio.h>
#include  <stdlib.h>//exit用
#include <getopt.h>//オプション取得用
#include <math.h>
#include <string.h>
#include <stdbool.h>//bool用
#include <limits.h>//INT_MIN用
#include <float.h>//FLT_MAX用

#define MAX_CODE_LEN 100

struct Data {
	int data;//!< データ
	double priority;//!< 優先順位（小さい方が優先順位高い）
};

struct TreeNode {
	struct Data node;//!< ノード
	int parent;//!< 親のindex
	int left;//!< 左の子のindex
	int right;//!< 右の子のindex
};

struct CodeMap {
	int data;//!< データ
	char code[MAX_CODE_LEN];//!< 符号
	int length;//!< 符号長
};

#define READ_CSV_MODE 0//!< CSVを読み込むモード
#define READ_IMG_MODE 1//!< 画像ファイルを読み込むモード（OpenCVがないと使えない）
int mode=READ_CSV_MODE;

bool debug = false;

int main(int argc,char **argv);

//ハフマン符号化関連の関数
int  createHuffmanTree(struct TreeNode * HuffmanTree, struct Data * probDatas, int length);
void createCodeMap(struct CodeMap * maps, struct TreeNode * HuffmanTree, int root, int length);

//評価指標
double calcEntoropy(struct Data* probability,int  length);
double calcAvarageCodeLength(struct CodeMap * maps, struct Data * input_datas, int length);

//ソート
void BubbleSort(struct Data* stream, int length);

//ファイル読み込み
int readProbCSV(struct Data* weighted_array,char* fileName,int type_num);
int readProbIMG(struct Data* input_datas,char* fileName);
#ifdef OPEN_CV
	#include <highgui.h>
	int cvReadProbIMG(struct Data* input_datas,char* fileName);
#endif

//その他
double lg(double value);	
void ShowData(struct Data* stream, int length);
void usage(char *myname);

/** main関数
 * 入力ファイルをもとに、ハフマン符号を計算する
 * @param[in] argc 引数の数
 * @param[in] argv 引数
 */
int main(int argc,char **argv)
{	
	int i;
	FILE * inputFile;
	char* inputFileName;
	//MAX_CODE_LEN桁の符号しか扱えないので、少なめに定義しとく。
	//-nオプションで変更可能
	int type_num=MAX_CODE_LEN;
	
	inputFileName="data.csv";
	
#ifdef OPEN_CV
	printf("can use OpenCV images\n");
#endif
	
	//オプション設定
	int option;
	while( (option=getopt(argc,argv,"hdi:n:g")) != -1 ){
		switch(option){
			case 'i'://入力ファイル
				inputFileName=optarg;
				break;
			case 'g'://入力ファイルが画像
				mode=READ_IMG_MODE;
				type_num=256;//8bit画像のみ対応
				break;
			case 'n'://読み込む要素数を指定（しなかったら全て読み込む）
				type_num=atoi(optarg);
				break;
			case 'd'://詳細な出力
				debug=true;
				break;
			case 'h'://Usageを表示
			default:
				usage(argv[0]);
				return 0;
				break;
		}
	}
	
	printf("input : %s\n",inputFileName);
	
	//! ファイルの読み込み
	struct Data input_datas[type_num];
	int length=0;
	printf("\n\nRead File -- ");
	if(mode == READ_CSV_MODE){//!- 要素と生起確率が書かれたCSVファイルを読み込む
		printf("CSV Mode\n");
		length = readProbCSV(input_datas,inputFileName,type_num);
	}else if(mode == READ_IMG_MODE){//!- 画像ファイルを読み込み、生起確率を計算
		printf("Image Mode\n\n");
		length = readProbIMG(input_datas,inputFileName);
	}
	
	if(length < 0){
		printf("[Error] Failed to Calculate Probability\n");
		exit(-1);
	}
	
	printf("\ninput datas ----------\n"); ShowData(input_datas, length);
	
	//! ハフマン木を作ります。
	struct TreeNode HuffmanTree[length*2+2];//!< ハフマン木、十分なサイズを確保
	//ハフマン木は余裕を持たせてある（使わないnodeもある）のでちゃんと初期化しましょう。
	for(i=0;i<length*2+2;i++){
		HuffmanTree[i].node.data = INT_MIN;
		HuffmanTree[i].node.priority = DBL_MAX;//負にしとくと、うっかり使ったときに先頭になってしまう
		HuffmanTree[i].parent = HuffmanTree[i].left = HuffmanTree[i].right = INT_MIN;
	}
	
	int root =  createHuffmanTree(HuffmanTree, input_datas, length);

	//! ハフマン木配列の中身を確認してみませう。
	printf("\n\nHuffmanTree Array [root(length):%d] --------\n",root);
	for(i=0; i<root; i++) 
		printf(" Huffman[%d]  %d(%f)  parent %d left %d right %d\n"
					,i,HuffmanTree[i].node.data,HuffmanTree[i].node.priority
					,HuffmanTree[i].parent,HuffmanTree[i].left,HuffmanTree[i].right);
	
	//! ハフマン符号を計算します
	printf("\n\nCalculate Huffman Code  (%d types)--------\n",length);
	struct CodeMap codeMaps[length];
	createCodeMap(codeMaps, HuffmanTree, root, length);
	
	int data;
	for(data=0;data<length;data++) 
		printf("%d -> %s\n",codeMaps[data].data,codeMaps[data].code);
	
	//! 結果を出力する・・・・・かも
	printf("\n\nResult--------\n");
	double entoropy =  calcEntoropy(input_datas, length);
	printf("etoropy = %f\n",entoropy);
	
	for(data=0;data<length;data++) printf("%d (%f) -> %s\n"
				,codeMaps[data].data,input_datas[data].priority,codeMaps[data].code);
	
	double acl = calcAvarageCodeLength(codeMaps, input_datas, length);
	printf("Avarage Code Length = %f\n",acl);
	
	
	return 0;
}

//** ハフマン木 **//
/** ハフマン木の作成
 * 生起確率の配列をもとに、ハフマン木を作成する
 * @param[out] HuffmanTree ハフマン木を格納する配列のポインタ、最低でも(length*length+2)の大きさ
 * @param[in] probDatas データと生起確率が入っている配列
 * @param[in] length probDatasの要素数
 * @retval root 完成したハフマン木のrootのindex
 */
int  createHuffmanTree(struct TreeNode * HuffmanTree, struct Data * probDatas, int length){
	int i;
	int freeNode=length;
	int nowLength=length;
	int root=0;
	
	//! ハフマン木の葉を作成
	for(i=0; i<length; i++) HuffmanTree[i].node=probDatas[i];
	
	//! 葉をソート
	struct Data sorting_array[length];
	for(i=0;i<length;i++) sorting_array[i] = HuffmanTree[i].node;
	BubbleSort(sorting_array,length);
	
	if(debug){printf("[createHuffmanTree] \n"); ShowData(sorting_array,length);}
	
	//! ハフマン木を作成
	while(nowLength > 0){
		if(debug) printf("\n sort array len %d free len is %d\n",nowLength,freeNode);
		
		//! 生起確率の小さい2つのノードを繋げて、Huffman木の新しい節を作成
		struct Data newData;
		//!- 新しい節のdataは他のdataとかぶらないようになってればおけ
		newData.data=(sorting_array[0].data+sorting_array[1].data)+length;
		newData.priority=((double)sorting_array[0].priority+(double)sorting_array[1].priority);
		//!- 親と子を初期化しとく（区別するために負の値で）
		//newData.parent = newData.left = newData.right = INT_MIN;
		HuffmanTree[freeNode].node = newData;
		
		if(debug) 
    		printf("new node %d(%f) <- %d(%f) + %d(%f)\n",
				newData.data,newData.priority
				,sorting_array[0].data,sorting_array[0].priority
				,sorting_array[1].data,sorting_array[1].priority);
		
		
		//! ハフマン木の中から左右の子を探し出す。
		//!- sorting_arrayとHuffmanTreeでは要素の入っている位置が異なるため、左右の子のdataとpriorityは分かってもHuffmanTree内での位置がわからない。
		if(debug) printf(" search %d(%f)  and %d(%f)\n",sorting_array[0].data,sorting_array[0].priority,sorting_array[1].data,sorting_array[1].priority);
		for(i=0; i<freeNode; i++){
			if(debug) printf(" Huffman %d(%f) \n",HuffmanTree[i].node.data,HuffmanTree[i].node.priority);
			if(HuffmanTree[i].node.data == sorting_array[0].data
						&& HuffmanTree[i].node.priority == sorting_array[0].priority){
				//!- 左の子を登録、左の子の親は新しい節
				if(debug) printf(" -- left %d parent %d \n",i,freeNode);
				HuffmanTree[freeNode].left = i;
				HuffmanTree[i].parent = freeNode;
				continue;
			}
			if(HuffmanTree[i].node.data == sorting_array[1].data
						&& HuffmanTree[i].node.priority == sorting_array[1].priority){
				//!- 右の子を登録、右の子の親は新しい節
				if(debug) printf(" -- right %d parent %d \n",i,freeNode);
				HuffmanTree[freeNode].right = i;
				HuffmanTree[i].parent = freeNode;
				continue;
			}
		}freeNode++; root = freeNode;

		//! 新しい順序を計算
		for(i=2;i<nowLength;i++) sorting_array[i-2] = sorting_array[i];
		sorting_array[nowLength-2] = newData;
		if(nowLength <=2) break;//残りが二つなら並び替える必要なし
		nowLength--;
		BubbleSort(sorting_array,nowLength);
	}
	if(debug) {
		printf("\n\n[createHuffmanTree] HuffmanTree Array [root(length):%d] --------\n",root);
		for(i=0; i<root; i++) printf(" Huffman %d(%f) \n",HuffmanTree[i].node.data,HuffmanTree[i].node.priority);
		printf("[createHuffmanTree] End --------\n");
	}
	return root;
}


/** ハフマンコードマップの作成
 * ハフマン木をもとに、ハフマン符号を求める
 * @param[out] maps ハフマン符号を格納する配列
 * @param[in] HuffmanTree ハフマン木
 * @param[in] root ハフマン木の根のindex
 * @param[in] dataTypeNum データの種類数
 */
void createCodeMap(struct CodeMap * maps, struct TreeNode * HuffmanTree, int root, int dataTypeNum){
	int i;
	int data,node_number;
	int bits[root];
	char result[root];
	int bitp,parent,bitp2;
	int code_len;
	int length;
	
	//! 葉の数（データの種類数）だけ繰り返す
	for(data=0;data<dataTypeNum;data++,code_len=0){
		//!- 目的のデータが入っている位置を探す
		for(i=0;i<dataTypeNum;i++){
			if(HuffmanTree[i].node.data == data){
				node_number = i;
				break;
			}
		}
		if(debug) printf("[createCodeMap]Start node num %d\n",node_number);
		
		//!- ハフマン木をもとにコードを生成
		for(i=0;i<root;i++) bits[i]=3;
		bitp=0;bitp2=0;
		do{
			//!-- 自分が親から見て左の子なら1、右の子なら0
			parent = HuffmanTree[node_number].parent;
			if(debug) printf("[createCodeMap]parent %d left  %d \n",parent, HuffmanTree[parent].left);
			bits[bitp++] = ( HuffmanTree[parent].left == node_number) ? 1 : 0;
			node_number=parent;

			if(i>root){printf("[Error] Failed to Calcurate Huffman Code\n"); exit(-1);}
		}while(node_number != root-1);
		
		//! 符号長を設定
		code_len = bitp;
		
		do {
			//!-- バックトレース
			if (bits[--bitp] == 1) result[bitp2++]='1';
			else   result[bitp2++]='0';
		} while (bitp > 0);
		result[bitp2]='\0';
		
		//!- マップ配列に格納
		maps[data].data = data;
		maps[data].length = code_len;
		strcpy(maps[data].code,result);
		if(debug) printf("[createCodeMap]%d : %d -> %s (%d)\n"
					,data,maps[data].data,maps[data].code,maps[data].length);
	}
}

//** 評価指標 **//
/** エントロピーの計算
 * エントロピーを計算する
 * @param[in] probability 生起確率の配列
* @param[in] length 配列の長さ
 * @return エントロピー
 */
double calcEntoropy(struct Data* probability,int  length){
	int i;
	double entoropy=0.0;
	
	//! entoropy = sum -P*lg(P)
	for(i=0;i<length; i++){
		if(probability[i].priority != 0.0){//log(0)は計算できない
			entoropy += -(probability[i].priority * lg(probability[i].priority) );
		}else entoropy += 0;
	}
	
	return entoropy;
}

/** 平均符号長
 * 平均符号長を計算する
 * @param[in] maps ハフマン符号表
 * @param[in] input_datas 生起確率の配列
 * @param[in] length 配列の長さ
 * @return 平均符号長
 */
double calcAvarageCodeLength(struct CodeMap * maps, struct Data * input_datas, int length){
	int i;
	double acl=0.0;
	
	for(i=0;i<length;i++) acl += maps[i].length*input_datas[i].priority;
	
	return acl;
}

//** ソート **//
/** バブルソート
 * バブルソートを行う、入力配列がpriorityの小さい順序にソーティングされる。
 * @param[out] weighted_array 重み付き配列
 * @param[in] length 配列の長さ
 */
void BubbleSort(struct Data* weighted_array, int length)
{
	int i, j;
	struct Data temp;

    for (i = 0; i < length - 1; i++) {
        for (j = length - 1; j > i; j--) {
            if (weighted_array[j - 1].priority > weighted_array[j].priority) {  /* 前の要素の方が大きかったら */
                temp = weighted_array[j];
                weighted_array[j] = weighted_array[j - 1];
                weighted_array[j - 1]= temp;
            }
        }
    }
}

//** ファイルの読み込み **//
/** CSVファイルの読み込み
* [(int)要素,(float)生起確率]なCSVファイルを読み込み、配列に格納
* @param[out] input_datas データと生起確率が入っている配列
* @param[in] fileName CSVファイル名
* @param[in] type_num 要素数
* @retval length 完成した配列の要素数
* (注1) データ(一列目)は必ず0から始まること！（仕様です）
* (注2) 生起確率の合計は1じゃないとダメ！！（ハフマン符号の仕様です）
 */
int readProbCSV(struct Data* input_datas,char* fileName,int type_num){
	int length=0,ret;
	int data=0;
	float prob;
	
	//! ファイルを開く
	FILE* File = fopen(fileName,"r");
	if( File == NULL ){
		printf( "Can not Open [%s]\n", fileName);
		return -1;
	}
	
	//! 一行ごとに読み込み
	while( ( ret = fscanf( File,"%d,%f", &data, &prob) ) != EOF  && length <  type_num){
		if(debug) printf( "[readProbCSV] %d (%f)\n", data, prob);
		input_datas[length].data=(int)data;
		input_datas[length].priority=(double)prob;
		length++;
	}
	fclose( File );
	
	return length;
}

/** 画像ファイルの読み込み
 * 画像ファイルを読み込み、生起確率を計算して配列に格納する
* @param[out] input_datas データと生起確率が入っている配列
* @param[in] fileName 画像ファイル名
* @return length 完成した配列の要素数
* @retval -1 画像の読み込みに失敗したとき
 */
int readProbIMG(struct Data* input_datas,char* fileName){
	
#ifdef OPEN_CV	
	return cvReadProbIMG(input_datas,fileName);
#endif
	
	return -1;
}

#ifdef OPEN_CV
/** OpenCVを使って画像を読み込む関数
 * 画像を読み込み、階調ごとの生起確率を計算
 * @param[out] input_datas 生起確率を格納する配列
 * @param[in] fileName 画像ファイル名
 * @retval 256 8bit画像（256階調）を扱うので配列サイズは256
 * @retval -1 読み込み失敗したとき
 */
int cvReadProbIMG(struct Data* input_datas,char* fileName){
	int i;
	int x,y,ch;
	int gradation=0;
	double prob=0.0;
	
	int counter[256];//8bit画像だけ扱うよ
	for(i=0; i<256; i++) counter[i]=0;
	
	//! 画像の読み込み（チャンネル数は任意、グレースケール、8bit:256階調）
	IplImage * srcImg=cvLoadImage(fileName, CV_LOAD_IMAGE_GRAYSCALE | IPL_DEPTH_8U);
	
	if(!srcImg){
	  printf("Could not load image file: %s\n",fileName);
	  return -1;
	}
	
	//! 画像の情報を取得
	int height    = srcImg->height;
	int width     = srcImg->width;
	int channels  = srcImg->nChannels;
	int step      = srcImg->widthStep;
	int depth      = srcImg->depth;
	uchar *imgdata  = (uchar *)srcImg->imageData;
	printf("Processing a %dx%d image with %d channels : depth=%d step=%d\n"
			,height,width,channels,depth,step);
	
	//! 画素にアクセスして階調ごとの出現数を計算
	for(y=0;y<height; y++){ 
		for(x=0;x<width; x++){
			for(ch=1; ch<=channels; ch++){
				gradation = (int)imgdata[step*y+x*channels+ch];
				counter[gradation] += 1 ;
			}
		}
	}
	
	if(debug) for(i=0; i<256; i++) printf("%d - %d\n",i,counter[i]);
		
	//! 各階調の生起確率を計算
	for(gradation=0; gradation<256; gradation++){
		prob = (double)((double)counter[gradation]/(double)(x*y*channels));
		if(counter[gradation]!=0) 
			printf("%d - %f <- %d/%d\n",gradation,prob,counter[gradation],(x*y*channels));
		input_datas[gradation].data=(int)gradation;
		input_datas[gradation].priority=(double)prob;
	}
	
	if(debug){
		cvNamedWindow("\nPress any Key to continue\n",1);
		
		cvNamedWindow("Source Image",1);
		cvShowImage("Source Image",srcImg);
		cvWaitKey(0);
		cvDestroyWindow("Source Image");
	}
	
	cvReleaseImage(&srcImg);
		
	return 256;
}
#endif
	
/* その他 */	
/** 底が2の対数を計算する
 * log2(x)を計算する
 * @param[in] x 値
 * @return log2(x)
 */
double lg(double x){
	return log(x)/log(2);
}

/** 重み付き配列の内容表示
 * 重み付き配列の内容を表示
 * @param[in] stream 重み付き配列
 * @param[in] length 配列の長さ
 */
void ShowData(struct Data* stream, int length)
{
    int i;
	for (i = 0; i < length;  i++) printf("%d(%f) , ", stream[i].data,stream[i].priority);
	printf("\n");
}

/** 使い方表示
 * プログラムの使用法を表示
 * @param[in] myname プログラム名
 */
void usage(char *myname){
#ifdef OPEN_CV
	printf("%s [hdig]\n",myname);
#else
	printf("%s [hdi]\n",myname);
#endif	
	
	printf("\t -h : show this help\n");
	printf("\t -d : show detail\n");
	
	printf("\t -n <data num> : data type num (default %d)\n",MAX_CODE_LEN);
	printf("\t -i <input File> : Input File\n");
#ifdef OPEN_CV
	printf("\t -g : when Input File is Image\n");
	printf("\t\t can use bmp/jpg/png/ppm/tiff file\n");
#else
	printf("\t if you install OpenCV, you can reading image file\n");
#endif
}