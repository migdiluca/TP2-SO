////despues paso a un .h, es para que no tengamos que ir abriendo los dos
//#define O_CREAT 0
//#define O_EXCL 1
////Me tenia que ir y como habia puesto null antes puse esto para que no tire errores
//#define null 0
//#include "scheduler.h"
//#include "BuddyAllocationSystem.h"
//
//
//typedef struct pid_list {
//    int pid;
//    struct pid_list * next;
//} pid_list;
//
//typedef struct sem_t {
//    int value;
//    int mutex;
//    int id;
//    pid_list * waitListStart;
//    pid_list * waitListTail;
//    struct sem_t * next;
//} sem_t;
//
//sem_t * sem_open(char * name, int flag, int initialValue);
//int sem_trywait(sem_t * sem);
//int sem_wait(sem_t * sem);
//int sem_post(sem_t * sem);
//int getStringID (char * name);
//sem_t* getSem(int id);
//
//sem_t * semList = null;
///**
// * Abre un nuevo semaforo.
// * @param name. Nombre del semaforo.
// * @param flag. O_CREAT crea el semaforo o lo retorna si ya existe, O_EXCL lo crea unicamente si no existe.
// * @param initialValue
// * @return
// */
//sem_t * sem_open(char * name, int flag, int initialValue) {
//    if(name == null)
//        return null;
//
//    sem_t * newSem = mallocMemory(sizeof(sem_t));
//    if(newSem == null)
//        return null;
//
//    sem_t * searchedSem = getSem(getStringID(name));
//    if(searchedSem != null) {
//        if(flag != O_EXCL)
//            return searchedSem;
//        return null;
//    }
//
//    newSem->id = getStringID(name);
//    newSem->value = initialValue;
//    newSem->next = semList;
//    semList = newSem;
//    newSem->waitListStart = null;
//    newSem->waitListTail = null;
//
//    return newSem;
//}
//
///**
// * Pregunta si se bloquearia el semaforo al hacer un sem_wait.
// * @param El semaforo a probar.
// * @return -1 si el semaforo no existe.
// *          0 si el proceso pasaria a estar bloqueado al hacer un sem_wait.
// *          1 si el proceso no pasaria a estar bloqueado al hacer un sem_wait.
// */
//int sem_trywait(sem_t * sem) {
//    if(getSem(sem->id))
//        return -1;
//    if(sem->value > 0) {
//        return 1;
//    }
//    return 0;
//}
//
///**
// * Decrementa el valor del semaforo si no hay ningun proceso espearando, en caso contrario agrega el
// * proceso que lo llamo a la lista de espera.
// * @param El semaforo a usar.
// * @return -1 en caso de error, 0 en otro caso.
// */
//int sem_wait(sem_t * sem){
//    if(getSem(sem->id))
//        return -1;
//    if(sem->value > 0) {
//        (sem->value)--;
//        return 0;
//    }
//    else {
//        /* Hay que ponerlo en el scheduler al getCurrentProcessPid() y al blockProcess(), el problema es si hace una interupcion de timertick mientras
//         * hago esto, entonces si cambia el proceso agrego el de otro, no se si eso es asi. Si es asi
//         * tendria que conseguir el pid de otra forma, o ver si podemos hacer que consiga el pid antes
//         * de que haga la rutina de cambiar de contexto */
//        int currentPid = getCurrentProcessPid();
//        sem->waitListTail->next = malloc(sizeof(pid_list));
//        sem->waitListTail->next->pid = currentPid;
//        sem->waitListTail->next->next = null;
//        if(sem->waitListStart == null)
//            sem->waitListStart = sem->waitListTail;
//        blockProcess(currentPid);
//        return 0;
//    }
//}
//
///**
// * Incrementa el valor del semaforo si no hay nadie en la lista de espera, si hay alguien lo quita de la
// * lista de espera y el proceso pasa a preparado.
// * @param El semaforo a usar.
// * @return -1 en caso de error, 0 en otro caso.
// */
//int sem_post(sem_t * sem) {
//    if(getSem(sem->id)) {
//        return -1;
//    }
//    pid_list * firstWaiting = sem->waitListStart;
//    if(firstWaiting != null) {
//        /* IMPLEMENTAR SCHEDULER*/
//        unblockProcess(firstWaiting->pid);
//        sem->waitListStart = firstWaiting->next;
//        if(sem->waitListStart == null)
//            sem->waitListTail = null;
//        free(firstWaiting);
//    }
//    else
//        (sem->value)++;
//    return 0;
//}
//
//int getStringID (char * name) {
//    int h = 0;
//    for (int i = 0; name[i] != '\0'; i++) {
//        h = 31 * h + name[i];
//    }
//    return h;
//}
//
//sem_t* getSem(int id){
//    sem_t * aux = semList;
//    while(aux != null) {
//        if(aux->id == id)
//            return aux;
//        else
//            aux = aux->next;
//    }
//    return null;
//}