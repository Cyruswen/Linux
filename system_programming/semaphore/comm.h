#pragma once

#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<unistd.h>
#include<sys/wait.h>

#define PATHNAME "."
#define PROJ_ID 0x6666

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *_buf;
};

int createSemSet(int nums);
int initSem(int semid, int nums, int initVal);
int getSemSet(int nums);
int P(int semid,int who);
int V(int semid,int who);
int destroySemSet(int semid);
