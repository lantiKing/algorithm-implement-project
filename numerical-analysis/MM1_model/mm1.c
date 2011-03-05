

#include "mm1.h"

/*main（メイン関数）

事前準備
touch mm1.in mm1.out

入力ファイル(mm1.in)の書式
--
[(float)mean interva llimit （客の来る間隔[minute]の上限）] [(float)mean service limit（客一人当たりの作業時間[minute]の上限）] [(int)deleyed required（客の人数[peaple]）]
--

入力ファイル(mm1.in)の記入例
--
1.000 0.5000 1000
--
*/

//false->Customer Number Limit Mode
//true->Fixed Time Mode
bool limit_time_mode=false;


int main(int argc, char **argv){
    
    char *infilename = "mm1.in";
    char *outfilename = "mm1.out";
    
    //Read Exec options (実行オプションの読み込み)
    int option;
    while( (option = getopt(argc, argv,"i:o:th"))!=-1 ){
		switch(option){
			case 'i':
				infilename = optarg;
				break;
      case 'o':
				outfilename = optarg;
				break;
      case 't':
				limit_time_mode = true;
				break;
			case 'h':
			default:
				usage(argv[0]);
				return 0;
				break;
		}
	}
    
    /*Open input and output file（入力ファイルと出力ファイルを開く）*/
    infile  =   fopen(infilename,"r");
    outfile  =   fopen(outfilename,"w");
    
    /*Specify the number of events for the timing function（イベント数を定義）*/
    //Customer Number Limit Mode
    if(!limit_time_mode) num_events = 2;//到着(Arrival Event)と出発(Departure Event)
    //Time Limit Mode
    else num_events = 3;//到着(Arrival Event)と出発(Departure  Event)と終了(Simulation End  Event)
    
    /*Read input parameter.（パラメータの読み込み）*/
    if(!limit_time_mode) {//Customer Number Limit Mode
        fscanf(infile,"%f %f %d",&mean_interarrival,&mean_service,&num_deleyed_required);
        printf("input %f %f %d\n",mean_interarrival,mean_service,num_deleyed_required);
    }else{//Time Limit Mode
        fscanf(infile,"%f %f %f",&mean_interarrival,&mean_service,&time_end);
        printf("input %f %f %f\n",mean_interarrival,mean_service,time_end);
    }
    
    if(!limit_time_mode) printf("Exec Limit Costomer Number Mode\n");
    else printf("Exec Limit Time Mode\n");
    
    /*Write Report heading and input parameter.（入力されたパラメータを書き出し）*/
    fprintf(outfile,"Single-server queueing system \n");
    fprintf(outfile,"Mean interarrival time Limit %11.3f minutes \n",mean_interarrival);
    fprintf(outfile,"Mean service time Limit %16.3f minutes \n",mean_service);
    
    if(!limit_time_mode) fprintf(outfile,"Number of customers %14d \n",num_deleyed_required);
    else fprintf(outfile,"Length of the Simulation %9.3f minute \n",time_end);
    
    
    printf("mean interarrival %f \n",mean_interarrival);
    printf("mean service %f \n",mean_service);
    
    /*Initialize the simulation.（シミュレーションの初期化）*/
    initialize();
        
    bool sim_run=true;
    //int arrival_id=0,dep_id=0;
    while(sim_run){
        /*Determine the next event.（次のイベントを定義）*/
        timing();
        
        /*Update time-average statistical accumulators（のべ時間の更新）*/
        update_time_avg_status();
        
        /*Invoke the appropriate event function.（イベントの実行）*/
        switch(next_event_type){
            case 1:
                arrival_id++;
                printf("[%10.3f] Customer %d is Arrival\n",sim_clock,arrival_id);
                arrival();//Arrival Event(到着イベント)
                break;
            case 2:
                dep_id++;
                printf("[%10.3f] Customer %d is Departure\n",sim_clock,dep_id);
                depart();//Departure Event(出発イベント)
                break;
            case 3://for Limit Time Mode
                /*Invoke the report generator and end the simulation.（レポートを出力してシミュレーションを修了）*/
                report();
                break;
        }
        if(!limit_time_mode){//Limit Customer Number Mode
            /*Quit the simulation when more delays are not needed.（もう客が来なくてもいいならシミュレーションを終わる）*/
            if(num_custs_delayed > num_deleyed_required) sim_run=false;
        }else{//Limit Time Mode
            /*Quit the simulation when time to simulation end.（シミュレーションの終了時間になったらシミュレーションを終わる）*/
            if(next_event_type==3) sim_run=false;
        }
    }
    
    /*Invoke the report generator and end the simulation.（レポートを出力してシミュレーションを修了）*/
    if(!limit_time_mode) report();//Limit Customer Number Mode
    
    fclose(infile);
    fclose(outfile);
    
    return 0;
}

/*Initialization function（初期化）*/
void initialize(void){
    /*Set rondom seed*/
    srand((unsigned int)time(NULL));

    /*Initialize the Simulation sim_clock（シミュレーション内の時計の初期化）*/
    sim_clock = 0.0f;
    
    /*Initialize the state variables（状態の初期化）*/
    server_status   =   IDLE;
    num_in_q        =   0;
    time_last_event =   0.0;
    
    /*Initialize the statical counters（カウンタの初期化）*/
    num_custs_delayed   =   0;
    total_of_delays     =   0.0;
    area_num_in_q       =   0.0;
    area_server_status  =   0.0;
    
    /*Initialize event list.（イベントリストの初期化）
        Since no customers are present,（客が存在しないなら）
        the departure (service completion) event is eliminated from consideration.
        （「departure」イベントは無視する）*/
    time_next_event[1]  =   sim_clock + expon(mean_interarrival);
    /*Guaranteening that first event will be an arrival
    (最初のイベントをArrivalにするために、（[timing()]min_time_next_eventより）大きな数を設定しておく)*/
    time_next_event[2]  =   1.0e+30;
    /*if Limit Time Mode, set Time to Simulation end*/
    if(limit_time_mode) time_next_event[3] = time_end;
}


/*Timing function*/
void timing(void){
    int i;
    float min_time_next_event = 1.0e+29;
    
    next_event_type = 0;
    
    /*Determine the event type of next event occur（次に発生するイベント生成する）*/
    for(i=1; i <= num_events; ++i){
        if(time_next_event[i] < min_time_next_event){
            min_time_next_event = time_next_event[i];
            next_event_type = i;
        }
    }
    
    /*Check to see whether the event list is empty（イベントリストが空でないかチェックする）*/
    if(next_event_type == 0){
        /*the event list is empty, so stop simulation（イベントリストが空なのでシミュレーション停止）*/
        fprintf(outfile,"\nEvent list empty at sim_clock %f",sim_clock);
        exit(1);
    }
    
    /*The event list is not empty, so advance the simulation sim_clock（イベントリストが空でないので、時計を進める）*/
    sim_clock = min_time_next_event;
}

/*Arrival event function（到着イベント）*/
void arrival(void){
    float delay;
    
    /*Schedule next arrival（次の到着をスケジュールに追加）*/
    time_next_event[1] = sim_clock + expon(mean_interarrival);
    
    /*Check the whether server is busy（店員が忙しいかどうかチェック）*/
    if(server_status == BUSY){
        /*server is busy, so increment number of customers in queue（店員が忙しいので、待ち行列内の客の数を増やす）*/
        ++num_in_q;
        
        /*Check to see whether an overflow condition status（客があふれていないかチェック）*/
        if(num_in_q > Q_LIMIT){
            /*the queue has overflowed, so stop the simulation（客があふれているのでシミュレーション停止）*/
            fprintf(outfile,"\nOverflow of the array time_arrival at sim_clock %f",sim_clock);
            exit(2);
        }
        
        /*there is still room in the queue,（まだ待ち行列内にいるので）
            so store the sim_clock of arrival of the arriving costomer at the (new) end of time_arrival
            （time_arrivalに客の到着時間を追加する）*/
        time_arrival[num_in_q] = sim_clock;
    
    }else{
        /*server is idle,（店員はヒマ）
            so arriving customer has delay of zero.（なので待ち時間はなし）
            
            the following two statement are for program clarity and do not affect the result of the simulation
            （一応明記しておくが、シミュレーションの結果には影響がない）*/
        delay = 0.0;
        total_of_delays += delay;
        
        /*Increment the number of customers delayed,（待っていた客の数を増やし）
            and make server busy（店員を仕事中にする）*/
        ++num_custs_delayed;
        server_status = BUSY;
        
        /*Schedule a dedeparture(service completion)　（出発イベントをスケジュールに追加）*/
        time_next_event[2] = sim_clock + expon(mean_service);
    }
}

/*Departure event function（出発イベント）*/
void depart(void){
    int i;
    float delay;
    
    /*Check to see whether the queue is empty（客が待っているか調べる）*/
    if(num_in_q == 0){
        /*queue is empty so make server idle and eliminate the departure event from consideration
        （待ってる人がいないので店員をヒマにし、出発イベントを無視する）*/
        server_status = IDLE;
        //min_time_next_eventより確実に大きいのでtimingで発生されなくなる
        time_next_event[2]= 1.0e+30;
    }else{
        /*queue is nonempty,（だれかが待っている）
            so decrement the number of customers in queue.（行列から一人減らす）*/
        --num_in_q;
        
        /*Compute the delay of customer who is beginning service and update the total delay accumulator
        （その人が待っていた時間を計算し、総待ち時間を増やす）*/
        delay = sim_clock - time_arrival[1];
        total_of_delays += delay;
        
        /*Increment the number of costomers delayed,（待っていた客の数を増やし）
            and schedule departure.（出発イベントをスケジュールに追加する）*/
        ++num_custs_delayed;
        time_next_event[2] = sim_clock + expon(mean_service);
        
        /*Move each customer in queue (if any) up one place.（待ち行列から客を削除し前につめる）*/
        for(i=1; i <= num_in_q; ++i) time_arrival[i] = time_arrival[i+1];
    }
}

/*Report generator function（結果表示）*/
void report(void){
    /*Compute and write estimates of desired mesures of perfomance.
    （）*/
    fprintf(outfile,"\nAvarage delay in queue %11.3f minutes",total_of_delays/num_custs_delayed);//平均待ち時間
    fprintf(outfile,"\nAvarage number in queue %10.3f",area_num_in_q/sim_clock);//平均待ち人数
    fprintf(outfile,"\nServer Utilization %15.3f",area_server_status/sim_clock);//店員の仕事率
    fprintf(outfile,"\ntime simulation ended %12.3f",sim_clock);//シミュレーション時間
    fprintf(outfile,"\nVisited %d customers",arrival_id);//来店客数
    fprintf(outfile,"\nServiced %d customers",dep_id);//来店客数
}

/*Update area acumulator for time-average（のべ時間の更新）*/
void update_time_avg_status(void){
    float sim_clock_since_last_event;
    
    /*Compute sim_clock since last event,（最終イベントからの経過時間を計算し）
        and update last-event-sim_clock marker.（最終イベント時間を更新）*/
    sim_clock_since_last_event = sim_clock - time_last_event;
    time_last_event = sim_clock;
        
    /*Update area under number-in-queue function.（のべ待ち時間更新）*/
    area_num_in_q += num_in_q * sim_clock_since_last_event;
    
    /*Update area under server-busy indicator function.（のべ作業時間の更新）*/
    area_server_status += server_status * sim_clock_since_last_event;
}


/*Exponential variate genaration function（指数の変量を計算）*/
float expon(float mean){
    /*Exponential function
        f(x) = (1/mean)e^(-x/mean) : (for x>=0)
            = 1-e^(-x/mean)
            = -mean*ln(U)
    */
    float u;
    
    /*Generate a U(0,1) random variate（0<=u<=1の乱数）*/
    //u = rand(1);//rand.h
    u = (float)rand()/RAND_MAX;
		
    /*Return an exponential random variate with mean "mean"
    （meanを基に計算したExponential Variateを返す）*/
    return -mean * log(u);

}

void usage(char *myname){
    printf("%s [hdt][-i Input_File_Name][-o Output_File_Name]\n",myname);
    printf("\t -t:Limit Time Mode (default:Limit Costomer Number Mode)\n");
    printf("\t -i <Input_File_Name>:set Input File (default:mm1.in)\n");
    printf("\t -o <output_File_Name>:set Output File (default:mm1.out)\n");
    printf("\t -h:show this usage\n");
    printf("\n");
    printf("Input File's format (Limit Costomer Number Mode)\n--- \n");
    printf("[(float)mean interva llimit[minute]] [(float)mean service limit[minute]] [(int)deleyed required[peaple]]\n");
    printf("ex)1.000 0.5000 1000\n");
    printf("\n");
    printf("Input File's format (Limit Time Mode)\n--- \n");
    printf("[(float)mean interva llimit[minute]] [(float)mean service limit[minute]] [(float)End time[minute]]\n");
    printf("ex)1.000 0.5000 100\n");
}
