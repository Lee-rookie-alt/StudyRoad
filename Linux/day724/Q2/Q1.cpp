#include <my_cpp_header.h>
#include <pthread.h>
int gNum=0;
pthread_mutex_t mutex;
pthread_cond_t cond;
void* thread1(void*){
    printf("A\n");
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond,&mutex);
    printf("C");
    return NULL;
}   
void *thread2(void *){
    printf("B\n");
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
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
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}

