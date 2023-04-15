#ifndef _TIME_TO_RUN_H
#define _TIME_TO_RUN_H
/* ---------------------------------------------------------------------------*/
#include <stdio.h>
#include <time.h>
/* ---------------------------------------------------------------------------*/
double time_to_run = 0;
clock_t timerToRun;
void start_time_param(clock_t *timer){
    *timer = clock();
}
/* ---------------------------------------------------------------------------*/
void start_time(){
    start_time_param(&timerToRun);
}
/* ---------------------------------------------------------------------------*/
void end_time_param(clock_t *timer, char *msg){
    *timer = clock() - *timer;
    time_to_run = ((double)*timer) / CLOCKS_PER_SEC;
    printf("%s => %f\n", msg, time_to_run);
}
/* ---------------------------------------------------------------------------*/
void end_time(char *msg){
    end_time_param(&timerToRun, msg);
}
/* ---------------------------------------------------------------------------*/
#endif