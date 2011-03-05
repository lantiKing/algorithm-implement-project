#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <getopt.h>//実行時オプションの判定
#include <time.h>//random関数のシ−ド値を決めるのに使う
#include <stdbool.h>//bool型を使う

#define Q_LIMIT 100 //queueの上限

/*server state*/
#define BUSY 1
#define IDLE 0

int next_event_type,//次のイベントの種類、1->Arrival(到着) 2->Departure(出発) （3->End(終了)（Limit Time Mode））
    num_custs_delayed,//待たされた客の数
    num_deleyed_required,//客の上限（Limit Costomer Number mode）
    num_events,//イベント数
    num_in_q,//待っている客の数
    server_status;//店員の状態（BUSY or IDLE）

int arrival_id=0,//到着した客のID
    dep_id=0;//出発した客の人数
    
float   area_num_in_q,//のべ待ち時間
        area_server_status,//店員ののべ仕事時間
        mean_interarrival,//次の客が来るまでの時間
        mean_service,//対応にかかる時間
        sim_clock,//時計
        time_end,//終了時間（Fixed Time mode）
        time_arrival[Q_LIMIT+1],//到着時間
        time_last_event,//最終イベント時間
        time_next_event[4],//次のイベントの時間、1->出発時間 2->到着時間 (3->終了時間（Limit Time Mode）)
        total_of_delays;//総待ち時間
        
FILE    *infile,//入力ファイル（mm1.in）
        *outfile;//出力ファイル（mm1.out）
        
/*functions*/

void initialize();
void timing(void);
void arrival(void);
void depart(void);
void report(void);
void update_time_avg_status(void);
float expon(float mean);

void usage(char *myname);