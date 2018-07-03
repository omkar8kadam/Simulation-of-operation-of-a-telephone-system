#include"simulation.h"

int main()
{
    srand(time(NULL));

    int total_working_time, max_service_time;

    printf("\n  Enter Total Working Time : ");                                                  //Get total working time
    scanf("%d", &total_working_time);

    printf("\n  Enter Maximum Service Time : ");                                                //Get maximum service time
    scanf("%d",&max_service_time);

    printf("\n  -----------------------------------------------------------------------"
           "\n  |  Clock  |  Call   | Arrival |   Wait  |  Start  | Service |  Queue  |"
           "\n  |  time   |  number |   time  |   time  |  time   |   time  |   size  |"
           "\n  -----------------------------------------------------------------------");

    timing_loop( total_working_time, max_service_time );                                        //Start timing loop for given working time & service time

    printf("\n  -----------------------------------------------------------------------");

    return 0;
}
