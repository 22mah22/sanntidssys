#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <x86intrin.h>
#include <string.h>
#include <sched.h>

struct timespec timespec_normalized(time_t sec, long nsec){
    while(nsec >= 1000000000){
        nsec -= 1000000000;
        ++sec;
    }
    while(nsec < 0){
        nsec += 1000000000;
        --sec;
    }
return (struct timespec){sec, nsec};
}
struct timespec timespec_sub(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec - rhs.tv_sec, lhs.tv_nsec - rhs.tv_nsec);
}
struct timespec timespec_add(struct timespec lhs, struct timespec rhs){
    return timespec_normalized(lhs.tv_sec + rhs.tv_sec, lhs.tv_nsec + rhs.tv_nsec);
}

int timespec_cmp(struct timespec lhs, struct timespec rhs){
    if (lhs.tv_sec < rhs.tv_sec)
        return -1;
    if (lhs.tv_sec > rhs.tv_sec)
        return 1;
    return lhs.tv_nsec - rhs.tv_nsec;
}

void busy_wait(struct timespec t){
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    struct timespec then = timespec_add(now, t);

    while(timespec_cmp(now, then) < 0){
        for(int i = 0; i < 10000; i++){}
        clock_gettime(CLOCK_MONOTONIC, &now);
    }
}

void busy_wait_with_times(){
    int clock_ticks_per_sec = sysconf(_SC_CLK_TCK);
    printf("Banan %i", clock_ticks_per_sec);
    static struct tms buf;
    clock_t start_time = times(&buf);
    while ((times(&buf) - start_time) < clock_ticks_per_sec){    }
    return;
}


/* PART B FROM HERE */

int plot_rdtsc(){
    int clock_ticks_per_sec = sysconf(_SC_CLK_TCK);
    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);
    for(int i = 0; i < 10*1000*1000; i++){
        int t1 = __rdtsc();
        int t2 = __rdtsc();
        int ns = (t2 - t1) *(1/1.992);
    if(ns >= 0 && ns < ns_max){
        histogram[ns]++;
            }
        }
    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
    return 0;
}

int plot_clock_gettime(){
    struct timespec t1;
    struct timespec t2;
    int clock_ticks_per_sec = sysconf(_SC_CLK_TCK);
    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);
    for(int i = 0; i < 10*1000*1000; i++){
        clock_gettime(CLOCK_MONOTONIC, &t1);
        clock_gettime(CLOCK_MONOTONIC, &t2);
    int ns = timespec_cmp(t1, t2);
    if(ns >= 0 && ns < ns_max){
        histogram[ns]++;
            }
        }
    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
    return 0;
}

int plot_times(){
    int clock_ticks_per_sec = sysconf(_SC_CLK_TCK);
    static struct tms buf;
    int ns_max = 50;
    int histogram[ns_max];
    memset(histogram, 0, sizeof(int)*ns_max);
    for(int i = 0; i < 10*1000*1000; i++){
        clock_t t1 = times(&buf);
        clock_t t2 = times(&buf);
    int ns = (t2 - t1) *(1/1.992);
    if(ns >= 0 && ns < ns_max){
        histogram[ns]++;
            }
        }
    for(int i = 0; i < ns_max; i++){
        printf("%d\n", histogram[i]);
    }
    return 0;
}

/* Part C begins here */

int measure_switch(){
    struct timespec t1;
    struct timespec t2;
    int clock_ticks_per_sec = sysconf(_SC_CLK_TCK);
    //int ns_max = 5000;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    sched_yield();
    clock_gettime(CLOCK_MONOTONIC, &t2);
    int ns = timespec_cmp(t1, t2);
    //if(ns >= 0 && ns < ns_max){
        printf("%i", ns);
    //    }
    return 0;
}

int main() {
    int clock_ticks_per_sec = sysconf(_SC_CLK_TCK);
    //struct timespec remaining = {1, 0};
    /*
    TASK A - 1:  nanosleep
    int response = nanosleep(&request, &remaining);
    struct timespec request = {1, 0}; */

    /* TASK A - 2: busy_wait() with clock_gettime .. 
    struct timespec remaining = {1, 0}; 
    busy_wait(remaining);*/ 
    
    /* TASK A - 3: busy_wait_with_times();*/ 

    /* TASK B - Access latency*/
    // struct timespec now;
    // static struct tms buf;
    // for(int i = 0; i < 10*1000*1000; i++){
    //     /*__rdtsc(); Dette tok 0,083s totalt. //Dvs 8,3 * 10^-8 s */ 
    //     /*clock_gettime(CLOCK_MONOTONIC, &now); //Dette tok 0,271s totalt. Dvs 2,71s * 10^-8*/
    //     /*times(&buf); // Dette tok 4,297s totalt. Dvs 4,297 * 10^-7*/
    // }
    //plot_clock_times();

    measure_switch();

    return 0; 
}
