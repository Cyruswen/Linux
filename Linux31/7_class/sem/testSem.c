
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define PATH_NAME "/tmp"
#define PROJ_ID 0x5555

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO(Linux-specific) */
};

void initSem(int semid, int num, int val)
{
	union semun arg;
	arg.val = val;
	semctl(semid, num, SETVAL, arg);
}

static void PV(int semid, int num, int op)
{
	struct sembuf sf;
	sf.sem_num = num;
	sf.sem_op = op;
	sf.sem_flg = 0;
	semop(semid, &sf, 1);
}

void P(int semid, int num)
{
	PV(semid, num, -1);
}

void V(int semid, int num)
{
	PV(semid, num, 1);
}

int main()
{
	key_t k = ftok(PATH_NAME, PROJ_ID);
	if(k < 0){
		return 1;
	}
	int semid = semget(k, 1, IPC_CREAT|IPC_EXCL|0666);
	if(semid < 0){
		return 2;
	}
	initSem(semid, 0, 1);
	pid_t id = fork();
	if(id == 0){//child
		while(1){
	//		P(semid, 0);
			printf("A");
			usleep(12345);
			fflush(stdout);
			printf("A ");
			usleep(32345);
			fflush(stdout);
	//		V(semid, 0);
		}
	}else{ //father
		while(1){
	//		P(semid, 0);
			printf("B");
			usleep(10232);
			fflush(stdout);
			printf("B ");
			usleep(34252);
			fflush(stdout);
	//		V(semid, 0);
		}
	}
	semctl(semid, 0, IPC_RMID);
}

