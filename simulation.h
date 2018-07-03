#include"queue_ADT.h"

typedef struct call                                                         //Structure to store single call statistics
{
    int call_number;
    int arrival_time;
    int wait_time;
    int start_time;
    int service_time;
}CALL;

void init_call( CALL *current_call )                                        //Function to initialize call statistics to 0
{
    current_call->start_time=0;
    current_call->arrival_time=0;
    current_call->call_number=0;
    current_call->service_time=0;
    current_call->wait_time=0;
}

typedef struct statistics                                                   //Structure to store overall statistics
{
    int total_calls;
    int total_idle_time;
    int total_wait_time;
    int total_service_time;
    int maximum_queue_size;
    float average_wait_time;
    float average_service_time;
}STATISTICS;

void init_statistics( STATISTICS *stats )                                   //Function to initialize all overall statistics to 0
{
    stats->total_calls=0;
    stats->total_idle_time=0;
    stats->total_wait_time=0;
    stats->total_service_time=0;
    stats->maximum_queue_size=0;
    stats->average_wait_time=0.0;
    stats->average_service_time=0.0;
}

void display( QUEUE server, CALL current_call, int current_time, FILE *fp1 )//Function to display a single call statistics on screen & write the same in file
{
    printf("\n  |   %-5d |   %-5d |   %-6d|   %-6d|   %-6d|   %-6d|   %-6d|", current_time, current_call.call_number, current_call.arrival_time, current_call.wait_time, current_call.start_time, current_call.service_time, server.size );
    fprintf(fp1,"\n%d,%d,%d,%d,%d,%d,%d", current_time, current_call.call_number, current_call.arrival_time, current_call.wait_time, current_call.start_time, current_call.service_time, server.size );
}

void display_statistics( STATISTICS stats, FILE *fp2 )                                 //Function to display overall statistics on screen & write the same in file
{
    printf("\n\n\n\n\t              Total calls   :   %d"
           "\n\t          Total idle time   :   %d Mins"
           "\n\t          Total wait time   :   %d Mins"
           "\n\t       Total service time   :   %d Mins"
           "\n\t       Maximum queue size   :   %d"
           "\n\t        Average wait time   :   %f Mins"
           "\n\t     Average service time   :   %f Mins\n\n\n", stats.total_calls, stats.total_idle_time, stats.total_wait_time, stats.total_service_time, stats.maximum_queue_size, stats.average_wait_time, stats.average_service_time );
           fprintf(fp2,"         Total calls  :  %d\n     Total idle time  :  %d Mins\n     Total wait time  :  %d Mins\n  Total service time  :  %d Mins"
                       "\n  Maximum queue size  :  %d\n   Average wait time  :  %f Mins\nAverage service time  :  %f Mins", stats.total_calls, stats.total_idle_time, stats.total_wait_time, stats.total_service_time, stats.maximum_queue_size, stats.average_wait_time, stats.average_service_time);
}

int call_simulator( QUEUE *server, int max_service_time, int current_time, int *call_number, STATISTICS *stats )//Function to simulate call generation
{
    CALL *cNew = (CALL*)malloc(sizeof(CALL));
    int isCall = rand()%10;                                         //Get random number from 0-9 in isCall
    if(isCall < 5)
        return 0;                                                   //If isCall<5; no new call
    cNew->call_number = *call_number;
    cNew->arrival_time = current_time;
    cNew->wait_time = 0;
    cNew->start_time = 0;
    cNew->service_time = (rand()% max_service_time) + 1;            //Set random service time from 0-max_service_time
    stats->total_service_time += cNew->service_time;
    enqueue(server,cNew);                                           //Enqueue the new call to server queue
    (*call_number)++;
    return 1;
}

void start_call( QUEUE *server, CALL *current_call, int current_time, STATISTICS *stats, int *flg )//Function to start a new call from server queue
{
    *flg=1;                                                                 //As new call is started; set the flg
    *current_call = *(CALL*)dequeue(server);                                //Dequeue next waiting call from server queue
    current_call->start_time = current_time;
    current_call->wait_time = current_time - current_call->arrival_time;
    stats->total_wait_time += current_call->wait_time;
}

void call_processor( QUEUE *server, CALL *current_call, int *current_time, STATISTICS *stats, FILE *fp1, int *flg )//Function to check whether to end an active call or not
{
	if( *flg==1 )       //If a call is active
	{
		if( (*current_time) - current_call->start_time + 1 == current_call->service_time )      //If time for the call is over
		{
			display( *server, *current_call, *current_time, fp1 );
			if( stats->maximum_queue_size < server->size )
                stats->maximum_queue_size = server->size;
			*flg=0;                                                                             //As the call is ended; reset flg
		}
	}
}

void timing_loop( int total_working_time, int max_service_time )//Function to simulate overall simulation i.e.timing loop
{
    int flg=0;                      //Reset flg

    FILE *fp1, *fp2;                //Two file pointers for excel & text file

    QUEUE server;
    init(&server);                  //Create & initialize server queue

    CALL current_call;              //current_call holds the data of current active call
    init_call(&current_call);       //Initialize current_call

    STATISTICS stats;
    init_statistics(&stats);        //Create & initialize stats

    time_t t;
    time(&t);
    char *s=ctime(&t),filename[40];

    sprintf(filename,"call-data (%c%c-%c%c%c-%c%c%c%c) (%c%c.%c%c).csv", s[8],s[9],s[4],s[5],s[6],s[20],s[21],s[22],s[23],s[11],s[12],s[14],s[15]);
    fp1 = fopen(filename,"w");       //Open Excel file in write mode
    fprintf(fp1,"Clock,Call,Arrival,Wait,Start,Service,Queue\ntime,number,time,time,time,time,size\n");

    sprintf(filename,"call-statistics (%c%c-%c%c%c-%c%c%c%c) (%c%c.%c%c).txt", s[8],s[9],s[4],s[5],s[6],s[20],s[21],s[22],s[23],s[11],s[12],s[14],s[15]);
    fp2 = fopen(filename,"w"); //Open text file in write mode

    int current_time,call_number=1;         //current_time holds the current minute & call_number holds the current call's serial number

    for( current_time=1; current_time <= total_working_time || !isEmpty(server) || flg==1; current_time++ )//Iterate the timing_loop till "All the working time is over","server queue is empty" & "No active call remains"
    {
        if( current_time <= total_working_time )                                            //Take new call only if working time is available
            call_simulator( &server, max_service_time, current_time, &call_number, &stats);
        if( !isEmpty(server) && flg==0 )                                                    //Start new call only if server contains waiting call & no current active call
            start_call( &server, &current_call, current_time, &stats, &flg );
        if( flg == 0 )                                                                      //If no active call, consider it as idle minute
            stats.total_idle_time++;
        call_processor( &server, &current_call, &current_time, &stats, fp1, &flg );
    }

    //Generate overall statistics & write to text file
    stats.total_calls = call_number - 1;
    stats.average_wait_time = (float)stats.total_wait_time / stats.total_calls;
    stats.average_service_time= (float)stats.total_service_time / stats.total_calls;
    printf("\n  -----------------------------------------------------------------------");

    display_statistics(stats, fp2);

    fclose(fp1);
    fclose(fp2);
}
