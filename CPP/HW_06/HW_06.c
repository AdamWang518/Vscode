#include <pthread.h>
#include <stdio.h>
#include<stdlib.h>
//the shared address space
int acc;
int count;
pthread_mutex_t mylock;
struct arg_data
{
	int a;
	int b;
};
void *
	child_thread(void *arg)
{
	struct arg_data *data=(struct arg_data *)arg;
	int key1=data->a;
	int key2=data->b;
	//the critical section with mutual-exclusive synchronization
	pthread_mutex_lock (&mylock);
	{
		acc = acc + key1 * key2;
		count = count + 1;
	}
	pthread_mutex_unlock (&mylock);
}//child_thread

int main ()
{
	int i;
	int A[101], B[101];
	pthread_t thread_id[101];
	struct arg_data v;
	//initialize shared data
	acc = 0;
	count = 0;
	for (i = 0; i <= 100;i++)
	{
		A[i] = 100;
		B[i] = 100;
	}
	//initialize locks
	pthread_mutex_init(&mylock, NULL);

	//fork child threads
	for (i=0;i<=100;i++)
	{
		v.a = A[i];
		v.b = B[i];
		pthread_create(&(thread_id[i]), NULL, child_thread, (void *)&v);
	}
	while (count<=100);
	printf ("Parent: accumulated acc = %d\n", acc);
	return 0;
}//main ()



















