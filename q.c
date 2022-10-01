#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

struct Deadlock{

    int PID;
    int FileThatIsLocked;
    int FileNeededToBeLocked;
    int time;
    bool lockCounted;
    bool requestCounted;
    bool visited;
    bool passed;

}Task[100],CycleDetector[100],Terminate[100];


int fileContents[24];
int fileContentsIndex = 0; //Number of values in a given file
int arrayIndex = 0; //Number of processes
int totalFiles = 0; //Number of files in the process
int totalExecutionTime = 0; //Minimum execution time
int terminateIndex = 0; //Number of cycles

void FileReadingTask1(char* inputFile) {

    FILE* filepointer;
    filepointer = fopen(inputFile, "r");

    int contentToRead;

    while(fscanf(filepointer, "%d", &contentToRead) > 0) {
        fileContents[fileContentsIndex] = contentToRead;
        fileContentsIndex++;
    }
    fclose(filepointer);
}



void sortingFileContents(){
    int fileIndex = 0;

    for(int i = 0; fileIndex!=fileContentsIndex; i++){
        Task[arrayIndex].PID = fileContents[fileIndex];
        fileIndex++;
        Task[arrayIndex].FileThatIsLocked = fileContents[fileIndex];
        fileIndex++;
        Task[arrayIndex].FileNeededToBeLocked = fileContents[fileIndex];
        fileIndex++;
        arrayIndex++;
    }

    for(int i = 0; i<arrayIndex;i++){
        Task[i].lockCounted = false;
        Task[i].requestCounted = false;
        Task[i].visited = false;
        Task[i].passed = false;
    }
}

void Task1(){

    printf("Processes %d\n",arrayIndex);
    bool flag = true;

    for(int i = 0; i < arrayIndex; i++){
        if(Task[i].lockCounted == false){
            totalFiles++;
            for(int j = 0; j < arrayIndex; j++){
                if(Task[i].FileThatIsLocked == Task[j].FileThatIsLocked && Task[j].lockCounted == false){
                    Task[j].lockCounted = true;
                }
                if(Task[i].FileThatIsLocked == Task[j].FileNeededToBeLocked && Task[j].requestCounted == false){
                    Task[j].requestCounted = true;
                }
                Task[i].lockCounted == true;
            }
        }

        if(Task[i].requestCounted == false){
            totalFiles++;
            for(int j = 0; j < arrayIndex; j++){
                if(Task[i].FileNeededToBeLocked == Task[j].FileThatIsLocked && Task[j].lockCounted == false){
                    Task[j].lockCounted = true;
                }
                if(Task[i].FileNeededToBeLocked == Task[j].FileNeededToBeLocked && Task[j].requestCounted == false){
                    Task[j].requestCounted = true;
                }
                Task[i].lockCounted == true;
            }
        }
    }
    printf("Files %d\n",totalFiles);
}

int Task2(){
    int count = 0;
    for(int i = 0; i < arrayIndex; i++){
        for(int j = 0; j < arrayIndex; j++){
            if(Task[i].FileNeededToBeLocked == Task[j].FileNeededToBeLocked){
                count++;
            }
            if(totalExecutionTime<count){
                totalExecutionTime = count;
            }
        }
        count = 0;
    }
    totalExecutionTime++; //For the first one to release all those that are locked
    return totalExecutionTime;
}


bool Task3(){
    int cycleIndex = 0;
    bool deadlockDetected = false;
    for(int i = 0; i < arrayIndex; i++){
        for(int j = 0; j < arrayIndex; j++){
            if(Task[i].FileNeededToBeLocked == Task[j].FileThatIsLocked){
                CycleDetector[cycleIndex]=Task[i];
                cycleIndex++;
                CycleDetector[cycleIndex]=Task[j];
                cycleIndex++;
                for(int k = 0; k < cycleIndex; k++){
                    if(Task[j].FileNeededToBeLocked == CycleDetector[k].FileThatIsLocked){
                        if(Task[j].PID <= CycleDetector[k].PID){
                            Terminate[terminateIndex] = Task[j];
                        }
                        else{
                            Terminate[terminateIndex] = CycleDetector[k];
                        }
                        terminateIndex++;
                        deadlockDetected = true;
                    }
                }

            }
        }
    }

    if(deadlockDetected){
        printf("Deadlock detected\n");
    }
    return deadlockDetected;
}


void Task4(){
    printf("Terminate ");
    int minimum = 100;
    int count = 0;
    if(terminateIndex<=4){
        for(int i = 0; i < terminateIndex; i++){

            if(Terminate[i].PID < minimum){
                minimum = Terminate[i].PID;
            }
        }
        printf("%d \n",minimum);

    }
    else{
        int i, j;
        for (i = 0; i < terminateIndex; i++){
            for (j = 0; j < i; j++){
                if (Terminate[i].PID == Terminate[j].PID){
                    break;
                }
            }
            if (i == j){
                printf("%d ", Terminate[i].PID);
                count++;
            }
            if(count == 2){
                break;
            }
        }
    }
    printf("\n");
}


void Task5(){

    struct Deadlock Fix[arrayIndex];
    int pIndex = 0;
    int timer = -1;
    int count = 0;
    bool flag = true;
    
    for(int i = 0; i < arrayIndex; i++){
        if(Task[i].visited == false){
            flag = true;
            if(timer>0 && count<2){
            count++;
            }
            else{
            timer++;
            }
            Task[i].visited = true;
            Task[i].time = timer;
            Fix[pIndex] = Task[i];
            pIndex++;
            
            for(int j = 0; j < arrayIndex; j++){
        		for(int k = 0; k < pIndex; k++){
        			if(Fix[k].FileNeededToBeLocked==Task[j].FileThatIsLocked || Fix[k].FileNeededToBeLocked==Task[j].FileNeededToBeLocked 
        				|| Fix[k].FileThatIsLocked==Task[j].FileThatIsLocked || Fix[k].FileThatIsLocked==Task[j].FileNeededToBeLocked){
              			 	flag = false;
        			}
        		}
        	
   			
        		if(flag && Task[j].visited == false && Task[j].passed == false && i!=j){
        			Task[j].visited = true;
        			Task[j].time = timer;
                		Fix[pIndex] = Task[j];
                		pIndex++;
        		}
        		else{
        			flag = true;
        		}
       	 }
      	for(int l = 0; l < pIndex; l++){
        	if(Fix[l].visited == true){
        		Fix[l].passed = true;
        	}
        }
        
        for(int l = 0; l < arrayIndex; l++){
        	if(Task[l].visited == true){
        		Task[l].passed = true;
        	}
        }
        }
    }
    
    
    for(int i = 0; i < pIndex;  i++){
        printf("%d %d %d, %d\n",Fix[i].time, Fix[i].PID, Fix[i].FileThatIsLocked, Fix[i].FileNeededToBeLocked);
    }

    printf("Simulation time %d",timer+1);
}



void main(int argc, char* argv[]){

    bool hasE = false;
    bool hasC = false;
    char * inputFile;

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-f") == 0){
            inputFile = argv[i+1];
        }

        if(strcmp(argv[i], "-c") == 0){
            hasC = true;
        }
        else if(strcmp(argv[i], "-e") == 0){
            hasE = true;
        }
    }

    FileReadingTask1(inputFile);

    sortingFileContents();

    Task1();

    bool deadlock = Task3();


    if(!hasE && !hasC){

        if(deadlock){
            Task4();
        }
        else{
            printf("No deadlocks detected\n");
        }
    }
    else if(hasC){
        if(deadlock){
            Task5();
        }
        else{
            printf("No deadlocks detected\n");
        }
    }
    else if(hasE){
        if(!deadlock){
            Task2();
            printf("Execution time %d\n",totalExecutionTime);
        }
    }
}
