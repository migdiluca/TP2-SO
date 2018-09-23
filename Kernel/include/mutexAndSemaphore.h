//despues paso a un .h, es para que no tengamos que ir abriendo los dos
#define O_CREAT 0
#define O_EXCL 1

typedef struct pid_list pid_list;

//SI ES UN MUTEX VALUE = -1 Y SE TRATA CASI IGUAL QUE UN SEMAFORO
typedef struct sem_t sem_t, mutex_t;

sem_t * sem_open(char * name, int flag, int initialValue);
mutex_t * mutex_open(char * name);
sem_t * semAndMutexOpen(char * name, int flag, int initialValue, int isSemaphore);
int trywait(sem_t * sem);
int wait(sem_t * sem);
int post(sem_t * sem);
int getStringID (char * name);
mutex_t * getMutex(int id);
sem_t * getSem(int id);
pid_list * listRemovePID(pid_list * pidList, int pid);
void freeMemoryPidLists(pid_list * pidList);
int close(sem_t * sem);
int unlink(sem_t * sem);
sem_t * unlinkR(sem_t * sem, int id);
int exists(sem_t * sem);
int cmp(int * pid1, int  * pid2);
void unlockTas(sem_t * sem);
