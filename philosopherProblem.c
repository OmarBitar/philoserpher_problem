
/* 
    programmed by omar bitar
    to run: gcc philosopherProblem.c -pthread -o philosopherProblem; ./philosopherProblem
*/

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define maxSize 50

#define THINK 100 
#define HUNGRY 200 
#define EAT 300 

sem_t me; 
sem_t S[maxSize];
pthread_t pThreads[maxSize];
int pFlag[maxSize]; 
int p_total;

void getSemVal(sem_t s) {
    int value; 
    sem_getvalue(&s, &value); 
    printf("The value of the semaphors is %d ", value);
}

void printStates() {
    printf("=== \n");
    for (int index = 0; index < p_total; index++) {
        switch (pFlag[index]) {
            case THINK:
                printf("P[%d] is now THINKING \n",index);
                break;
            case HUNGRY:
                printf("P[%d] is now HUNGRY \n",index);
                break;
            case EAT:
                printf("P[%d] is now EATING \n",index);
                break;
            
            default:
                break;
        }
    }
}

void *test(int p_index) {   

    if( pFlag[p_index] == HUNGRY & 
        pFlag[(p_index + (p_total-1)) % p_total] != EAT & // test left
        pFlag[(p_index + 1) % p_total] != EAT) {          // test right

            pFlag[p_index] = EAT;   printStates();
            sleep(3); // EAT        
            sem_post(&S[p_index]);          //getSemVal(me);
            
        } else { 
            //printf("P[%d] CAN'T EAT \n",p_index); 
        }
}

void *pickUp_chopsticks(int p_index) {  
    sem_wait(&me);  
    pFlag[p_index] = HUNGRY;        printStates();
    test(p_index);
    sem_post(&me);
    sem_wait(&S[p_index]);   
}

void *drop_chopsticks(int p_index) { 
    sem_wait(&me);
    pFlag[p_index] = THINK;         printStates();
    test((p_index + (p_total-1)) % p_total);    // test left
    test((p_index + 1) % p_total);              // test right
    sem_post(&me);
}

void *philosopher(int p_index) {  

    while (1)
    {
        pFlag[p_index] = THINK;  

        sleep(2); // THINKING

        pickUp_chopsticks(p_index);

        sleep(3); // EAT   

        drop_chopsticks(p_index); 
    }
    
}

int main() {  
    
    printf("enter the number of processes you wish to create [50 is maximum]: ");
    scanf("%d",&p_total);  
    if (p_total > 50 || p_total < 1) {
        printf("please restart the program and enter an integer between 1 and 50 \n");
        return(0);
    } else {
        printf("you entered %d \nto end program hit Ctr+c \n", p_total);
    }
    
    sem_init(&me,0,1);

    //getSemVal();

    int  p_index;
    // The first for loop was put here to indicate that all states will 
    // become initilized as THINKING as shown int the philospoher() fucntion.   
    // The reason it was put in the main function not in the philosopher  
    // function is because all other functions involve loops and will 
    // result in an inacurate output to the user.  
    for(p_index = 0; p_index < p_total; p_index++) {
        printf("P[%d] is now THINKING \n",p_index);
    }
    for(p_index = 0; p_index < p_total; p_index++) {
        sem_init(&S[p_index],0,0);
        pthread_create(&pThreads[p_index],NULL,philosopher,p_index); 
    }
    for(p_index=0; p_index < p_total; p_index++) {
        pthread_join(pThreads[p_index],NULL); 
    }


}
