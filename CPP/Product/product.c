#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

//the shared address space
int acc;
int count;
int msgq_id;
int msgq_flag;
pthread_mutex_t mylock;

child_thread (void *p)
{
	int Avalue;
	int Bvalue;
	//the critical section with mutual-exclusive synchronization
	pthread_mutex_lock(&mylock);
	{
		msgrcv (msgq_id, &Avalue, sizeof(int), 0, 0);
		msgrcv (msgq_id, &Bvalue, sizeof(int), 0, 0);
		printf("%d", Avalue);
		printf("%d", Bvalue);
		acc = acc + Avalue * Bvalue;
		count = count + 1;
	}
	pthread_mutex_unlock (&mylock);
} //child_thread

int main ()
{
	int i;
	int A[101];
	int B[101];
	msgq_flag = IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR;
	msgq_id = msgget (IPC_PRIVATE, msgq_flag);
	printf ("The message queue ID: %d\n", msgq_id);
	for (int i = 0; i <= 100;i++)
	{
		A[i] = 100;
		B[i] = 100;
	}
	pthread_t thread_id[101];
	acc = 0;
	//initialize shared data
	count = 0;

	//initialize locks
	pthread_mutex_init (&mylock, NULL);

	//fork child threads
	for (i=0;i<=100;i++)
		pthread_create (&(thread_id[i]), NULL, child_thread, NULL);
	while (count<=100)
	{
		msgsnd(msgq_id, &A[i], sizeof(int), 0);
		msgsnd(msgq_id, &B[i], sizeof(int), 0);
	}
	printf ("Parent: accumulated acc = %d\n", acc);

	return 0;
}//main ()



















