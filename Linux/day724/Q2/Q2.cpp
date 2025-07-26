#include <my_cpp_header.h>
#include <pthread.h>
int gNum=0;
pthread_mutex_t mutex;

void* thread1(void*){
    int cnt=10000000;
    printf("thread1 before lock!\n");
    pthread_mutex_lock(&mutex);
    while(cnt-->0){
        gNum++;
    }
    pthread_mutex_unlock(&mutex);
    printf("thread1 after lock!\n");
    return NULL;
}   
void *thread2(void *){
    int cnt=10000000;
    printf("thread2 before lock!\n");
    pthread_mutex_lock(&mutex);
    while(cnt-->0){
        gNum++;
    }
    pthread_mutex_unlock(&mutex);
    printf("thread2 after lock!\n");
    return NULL;
}   
int main()
{
    pthread_mutex_init(&mutex,NULL);
    pthread_t ph1;
    pthread_t ph2;
    pthread_create(&ph1,NULL,thread1,NULL);
    pthread_create(&ph2,NULL,thread2,NULL);
    pthread_join(ph1,NULL);
    pthread_join(ph2,NULL);
    printf("gNum result is %d\n",gNum);
    return 0;
}

