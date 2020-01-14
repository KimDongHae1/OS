#include <pthread.h>
#include <stdio.h>


int data = 0; // 스레드들이 공유할 데이터

void *thread1(void *num){ // threadnum은 스레드 번호이다.
    int i;
    
    for(i = 0; i < 1000; i++){ //각 thread를 각자 1000번씩 data에 접근하도록 한다.
        printf("Thread%d data a value = %d\n", (int)num, --data);// 스레드가 data 부분을 사용하것을 로그하는 것이다
    }
    
    pthread_exit((void *) 0); // 다 사용된 스레드를 종료하는 것이다.
}

void *thread2(void *num){ // threadnum은 스레드 번호이다.
    int i;
    
    for(i = 0; i < 1000; i++){ // 각 thread들이 각자 1000번씩 data에 접근하도록 한다.
        printf("Thread%d data a value = %d\n", (int)num, ++data); // 스레드가 data 부분을 사용하것을 로그하는 것이다
    }
    pthread_exit((void *) 0); // 다 사용된 스레드를 종료하는 것이다.
}

int main(int argc, char *argv[]){
    
    pthread_t threads[100]; // 스레드에게 번호를 부여해주는 것이다.
    long int i;
    for(i = 0; i < 50; i++)
        pthread_create(&threads[i], NULL, thread2, (void*)i); // 스레드 50개 생성
    for(i = 50; i < 100; i++)
        pthread_create(&threads[i], NULL, thread1, (void*)i); // 스레드 50개 생성
    for(i = 100; i > 0; i--)
        pthread_join(threads[i], NULL); // 종료된 스레드들이 결합하는 것이다.
    
    printf("\n");
}




