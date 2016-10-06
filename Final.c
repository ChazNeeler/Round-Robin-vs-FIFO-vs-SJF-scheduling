/* Margarita Sanchez, Chaz Neeler
 * CSCE 4600.021
 * Homework 2 - Problem 3
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


struct tuple {
    int pid;
    int cycle;
    int cyclesdone;
    long int wait;
    int mem;
};

void roundRobin(struct tuple *, int );
void printRR(struct tuple *);
void fifo(struct tuple *, int );
void printfifo(struct tuple *);
void cleanStruct (struct tuple *);
void sjf(struct tuple *, int );


int main()
{
    int i=0, k=0;
    float rands;
    time_t t;

    srand((unsigned) time(&t));

    printf("Enter value for k: ");
    scanf("%d", &k);

    struct tuple process[k];

    printf("Process ID\tCycles\t\tMemory Footprint\n");
    printf("---------------------------------------------------------\n");
    for(i=0; i<k; i++)
    {
        process[i].cyclesdone = 0;
        process[i].wait = 0;
        process[i].pid = i+1;
        process[i].cycle = rand() % 10000 + 1000;
        rands = ((float)process[i].cycle + 1000.0) / 11000.0;
        process[i].mem = (rand() % 100) * rands; //to keep it around the same range
        printf("%d\t\t%d\t\t%d\n", process[i].pid, process[i].cycle, process[i].mem);

    }

    printf("\n\n Round Robin - 1 Processor\n");
    roundRobin(process, 0);
    printRR(process);
    cleanStruct (process);
    printf("\n\n Round Robin - 4 Processors\n");
    roundRobin(process, 1);
    printRR(process);
    cleanStruct (process);
    fifo(process, 0);
    printfifo(process);
    cleanStruct (process);
    printf("\n\n FIFO - 4 Processors\n");
    fifo(process, 1);
    printfifo(process);
    cleanStruct (process);
    printf("\n\n Shortest Job First - 1 Processor\n");
    sjf(process, 0);
    printRR(process);
    printf("\n\n Shortest Job First - 4 Processors\n");
    sjf(process, 1);
    printRR(process);


    return 0;
}

void cleanStruct(struct tuple process[])
{
    int x = 0;
    for (x = 0; x < 50; x ++)
    {
        process[x].cyclesdone = 0;
        process[x].wait = 0;
    }
}

void fifo(struct tuple process[], int processCt)
{

    int cyclect = 0, ct = 0, i = 0, j = 0, k=0, pmax=0, pcount=0, pdone = 0, junk = 0; //cyclect being the overall cycles needed to finish all processes. pdone is how many processes are done

    if(processCt == 0)
    {   for (i = 0; i < 50; i++)
        {
            ct = 0;


            if (pdone == 0)
            {
            ct = 0;
            ct = ct + process[i].cycle;
            cyclect = ct;
            process[i].cyclesdone += process[i].cycle;
            process[i].wait = 0;
            pdone ++;
            }

            else if (pdone > 0)
            {
                ct = 0;
                ct = (50 * pdone) + (pdone * 10);
                cyclect = cyclect + ct;
                process[i].wait = cyclect;
                process[i].cyclesdone += (process[i].cycle - process[i].cyclesdone);
                cyclect = cyclect + process[i].cyclesdone;
                pdone++;
            }


        }
    }
    else
    {   for(k=0; k<4; k++)
        {
            pcount = 0;

            for(i=k; i<50; i+=4)
            {
                ct = 0;

                if(pcount == 0)
                {   ct = 0;
                    ct = ct + process[i].cycle;
                    cyclect = ct;
                    process[i].cyclesdone += process[i].cycle;
                    process[i].wait = 0;
                    pcount++;

                }
                else
                {   ct = 0;
                    ct = (50 * pdone) + (pdone * 10);
                    cyclect = cyclect + ct;
                    process[i].wait = cyclect;
                    process[i].cyclesdone += (process[i].cycle - process[i].cyclesdone);
                    cyclect = cyclect + process[i].cyclesdone;
                    pdone++;
                }
            }
        }
    }
}

// 50 * place in time = startint point.   10 tick for switch


void printfifo(struct tuple process[])
{
    int i=0;
printf ("fifo\n");
    printf("---------------------------------------------------------\n");
    printf("\nProcess ID\tCycles\t\tCycles Done\tWait\n\n");
    printf("---------------------------------------------------------\n");
    for(i=0; i<50; i++)
        printf("%d\t\t%d\t\t%d\t\t%ld\n", process[i].pid, process[i].cycle, process[i].cyclesdone, process[i].wait);

}


void roundRobin(struct tuple process[], int processCt)
{
    int cyclect=0, ct=0, i=0, j=0, k=0, pdone=0, pmax = 0; //cyclect being the overall cycles needed to finish all processes. pdone is how many processes are done

    do{
        if(processCt == 0)
        {
            for(i=0; i<50; i++)
            {
                ct = 0;

                if(process[i].cyclesdone < process[i].cycle)
                {
                    //printf("4.%d\n", i);
                    if(pdone < 49)
                        ct = 10;
                    if((process[i].cycle - process[i].cyclesdone) > 50)
                    {   ct += 50;
                        cyclect += ct; //accounting for penalty
                        process[i].cyclesdone += 50;
                    }
                    else
                    {   ct = ct + (process[i].cycle - process[i].cyclesdone);
                        cyclect += ct;
                        process[i].cyclesdone += (process[i].cycle - process[i].cyclesdone);
                        pdone++;
                    }
                }
                for(j=0; j<=i; j++)
                    if(process[i].cyclesdone != process[i].cycle)
                        process[j].wait += ct;
            }
        }
        else
        {   for(i=0; i<4; i++)
            {   if(i<2)
                    pmax = 13;
                else
                    pmax = 12;


                for(k=i; k<50; k+=4)
                {   ct = 0;

                    if(process[i].cyclesdone < process[i].cycle)
                    {
                        if(pmax > 1)
                            ct = 10;
                        if((process[k].cycle - process[k].cyclesdone) > 50)
                        {   ct += 50;
                            cyclect += 50;
                            process[k].cyclesdone += 50;
                        }
                        else
                        {   ct = ct + (process[k].cycle - process[k].cyclesdone);
                            cyclect += ct;
                            process[k].cyclesdone += (process[k].cycle - process[k].cyclesdone);
                            pdone++;
                            pmax--;
                        }
                    }
                    for(j=i; j<=k; j+=4)
                        if(process[i].cyclesdone != process[i].cycle)
                            process[j].wait += ct;
                }
            }
        }
    } while (pdone<50);

    printf("\nTotal Cycles Done = %d\n\n", cyclect);
}

void printRR(struct tuple process[])
{
    int i=0;

    printf("---------------------------------------------------------\n");
    printf("\nProcess ID\tCycles\t\tCycles Done\tWait\n\n");
    printf("---------------------------------------------------------\n");
    for(i=0; i<50; i++)
        printf("%d\t\t%d\t\t%d\t\t%ld\n", process[i].pid, process[i].cycle, process[i].cyclesdone, process[i].wait);

}

void sjf(struct tuple process[], int processCt)
{
    int i=0, j=0, k=0, pct=0, ct=0, cyclect=0, pcycles=0, qSize=0, tempQ=0, pmax=0;
    int queue[50];

    do {
        pct++; //increase count of processes added in. 50 max
        qSize++; //increase process count in queue
        pcycles = process[pct-1].cycle; //the new cycle we're adding. for sorting

        if(pct == 1)
            queue[0] = 1;
        else
        {   queue[qSize-1] = pct;
            for(i=1; i<(qSize-2); i++) //SORTING using Instertion Sort
            {   if(pcycles > process[queue[i]+1].cycle) //if cycles of new process < cycles of queued processes, move up
                {   tempQ = queue[i];
                    queue[i] = queue[i+1];
                    queue[i+1] = tempQ;
                }
                else //if there's no need to switch, break out of for loop
                    break;
            }
        }
    } while(pct < 50);

    /*
    do {
        if((process[queue[0]-1].cycle - process[queue[0]-1].cyclesdone) > 50) //still working with increments of 50 as each process is added every 50 cycles
        {   cyclect += 50;
            process[queue[0]].cyclesdone += 50;
            ct += 50;
        }
        else
        {   if(qSize > 1)
            {   ct += 60; //50 for finishing current process and starting new process to complete the 50 mark. 10 for switch penalty
                process[queue[0]].cyclesdone = process[queue[0]].cycle;
                cyclect += ct;
            }
        }
    } while(qSize > 0);*/


    if(processCt == 0)
    {
        for(i=0; i<qSize; i++)
        {
            process[queue[i]-1].cyclesdone = process[queue[i]-1].cycle;
            if((i != (qSize-1)) && (i>0))
            {   for(j=i; j<qSize; j++)
                {   process[queue[j]-1].wait += (process[queue[i]-1].cycle + 10); //penalty
                }
            }
            else
                process[0].wait = 0;
            //printf("%d\t\t%d\t\t%d\t\t%ld\n", process[queue[i]-1].pid, process[queue[i]-1].cycle, process[queue[i]-1].cyclesdone, process[queue[i]-1].wait);
        }
    }
    else
    {   for(k=0; k<4; k++)
        {   if(k < 2)
                pmax = 13;
            else
                pmax = 12;

            for(i=k; i<qSize; i+=4)
            {
                process[queue[i]-1].cyclesdone = process[queue[i]-1].cycle;
                if(( pmax > 1 ) && (i > k))
                {
                    for(j=i; j<qSize; j+=4)
                    {   process[queue[j]-1].wait += (process[queue[i]-1].cycle +10);
                        //printf("process[%d-1].wait = %ld. pmax is %d\n", queue[j], process[queue[j]-1].wait, pmax);
                        pmax--;
                    }
                }
                else
                    process[k].wait = 0;
                //printf("%d\t\t%d\t\t%d\t\t%ld\n", process[queue[i]-1].pid, process[queue[i]-1].cycle, process[queue[i]-1].cyclesdone, process[queue[i]-1].wait);
            }
        }
    }
}



