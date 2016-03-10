#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>


#define T19056_KEY       15678


union semun
{
	int setval;
	struct semid_ds *buf;
	unsigned short *array;
};
static int sem_id = 0;


static int semaphore_p()
{
	struct sembuf *sem_opa;
	sem_opa->sem_num = 0;
	sem_opa->sem_op = -1;
	sem_opa->sem_flg = SEM_UNDO;
	
	if (semop(sem_id, sem_opa, 1) < 0)
	{
		return 1;
	}
	return 0;
}

static int semaphore_v()
{
	struct sembuf *sem_opa;
	sem_opa->sem_num = 0;
	sem_opa->sem_op = 1;
	sem_opa->sem_flg = SEM_UNDO;
	
	if (semop(sem_id, sem_opa, 1) < 0)
	{
		return 1;
	}
	return 0;
}


static void del_semvalue()
{
	union semun sem_args;
	if (semctl(sem_id, 0, IPC_RMID, sem_args) < 0)
	{
		fprintf(stderr, "Failed to delete semaphore!\n");
	
	}
}

int main()
{
	/*create sem by process*/
	sem_id = semget((key_t)(T19056_KEY), 1, IPC_CREAT|0666);
	

	if (sem_id == -1)
	{
		perror("sem get failed!\n");
		return 1;
	}
	printf("sem_id = %d\n", sem_id);
	
	/*process must be init sem value*/
	union semun sem_args;
	sem_args.setval = 1;
	if (semctl(sem_id, 0, SETVAL, sem_args) < 0)
	{
		perror("semctl init failed!\n");
		return 1;	
	}
	
	int j = 0;

	while(1)
	{
		if(semaphore_p())
		{
			perror("-1 operation was failed!\n");	
			return 1;	
		}
		
		printf("server print %d\n", ++j);		
		
		
		if(semaphore_v())
		{
			perror("+1 operation was failed!\n");	
			return 1;	
		}		
		sleep(3);	
	}
	


	/*delete sem by process*/
	del_semvalue();	



	return 0;
}




