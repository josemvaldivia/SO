#include <sys/shm.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "tstruct.h"
using namespace std;

int main()
{
	key_t Clave;
	int Id_Memoria;
	int *Memoria = NULL;
	int i,j;
	TaskStruct* t_s;

	Clave = ftok ("/bin", 30);
	if (Clave == -1)
	{
		cout << "No consigo clave para memoria compartida" << endl;
		exit(0);
	}


	Id_Memoria = shmget (Clave, sizeof(TaskStruct)*2, 0777 | IPC_CREAT);
	if (Id_Memoria == -1)
	{
		cout << "No consigo Id para memoria compartida" << endl;
		exit (0);
	}

	
	t_s = (TaskStruct *)shmat (Id_Memoria, (char *)0, 0);
	if (t_s == NULL)
	{
		cout << "No consigo memoria compartida" << endl;
		exit (0);
	}
	
	t_s->state=1;
	t_s->pid=getpid();
	t_s->prio=0;
	for (i=1; i<=100; i++)
	{
		while(true)
		{
			
			if(t_s->state==1)
			{	
				t_s->dat = i;
				cout << "Escrito " << t_s->dat << endl;
				t_s->state=0;
				break;
			}
		}
	
	}
	


	shmdt ((char *)Memoria);
	shmctl (Id_Memoria, IPC_RMID, (struct shmid_ds *)NULL);
	return 0;
}

