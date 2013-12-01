#include <sys/shm.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include "tstruct.h"
using namespace std;

mutex my_mut;

template<typename T>
class ReviewProcces
{

	public:
		ReviewProcces();
		ReviewProcces(char* p, int x)
		{

			path=p;
			num_key=x;
			
		}

		~ReviewProcces()
		{
			if (Id_Memoria != -1)
			{
				shmdt ((char *)Memoria);
				shmctl (Id_Memoria, IPC_RMID, (struct shmid_ds *)NULL);
			}
		}
		
		void operator () ()
		{
			while(true)
			{
				Clave = ftok (path, num_key);
				Id_Memoria = shmget (Clave, sizeof(T)*2, 0777 );
				if (Id_Memoria!= -1)	break;
			}
			Memoria = (T*)shmat (Id_Memoria, (char *)0, 0);
			while (true)
			{
			if(Memoria->state==0)
				{	
				ID=Memoria->pid;
				break;
				}
			}
			for (int i=1; i<=100; i++)
				{
					while(true)
					{
						
						if(Memoria->state==0)
						{
							my_mut.lock();
							cout << "Leido de Proceso " <<ID<<"\t"<< Memoria->dat << endl;
							my_mut.unlock();
							Memoria->state=1;
							break;
						}
						
					}
				}
			return;
			}

	protected:
	private:
		char* path;
		int num_key;
		key_t Clave;
		int Id_Memoria;
		T* Memoria = NULL;
		bool status=1;
		int ID;
		

};

int main()
{
	ReviewProcces<TaskStruct> a ("/bin",30);
	ReviewProcces<TaskStruct> b ("/bin",33);
	ReviewProcces<TaskStruct> d ("/bin",40);
	ReviewProcces<TaskStruct> e ("/bin",37);


	thread f1(a);
	thread f2(b);
	thread  f4(d);
	thread  f5(e);


	f1.join();
	f2.join();
	f4.join();
	f5.join();


	return 0;
}
