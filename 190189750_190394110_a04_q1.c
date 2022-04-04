/*
Author:  Jacob Cabral, 190689750 - cabr9750@mylaurier.ca - https://github.com/jacobcabral
         Christopher Rossi, 190394110 - ross4110@mylaurier.ca -  https://github.com/DuckyZero
Repository:
         https://github.com/jacobcabral/cp-386-a4

 -------------------------------------
 File:    Question1.c
 Description: Banker's Algorithm to control processes
 Version  
 -------------------------------------
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <ctype.h> 

int *available_ptr;  // number of available resources
int *max_ptr;        // a m by n matrix representing the max resources
int *allocation_ptr; // a m by n matrix representing the resources allocated to the current process
int *need_ptr;       // shows the remaining resources that the process needs
int rows = -1; // the number of customers
int cols = -1; // the number of resources

typedef struct thread{ 

	char tid[4]; //this is the id of the thread when it is read
	unsigned int startTime;
	int state;
	pthread_t handle;
	int retVal;
} Thread;

//This method reads the number of cusotmers from the file
int read_customers(char *fname){
        
        FILE *file = fopen(fname,"r");
        int count = 0;
        struct stat st;
	fstat(fileno(file), &st);
	char* content = (char*)malloc(((int)st.st_size+1)* sizeof(char));	
	while(!feof(file))
	{
		char line[100];
                count += 1;
		if(fgets(line,100,file)!=NULL)
		{
			strncat(content,line,strlen(line));
                        
		}
	}
        
        return count;
}

//THis function counts the number of resources
int count_resources(char *fname){
        
        int num_resource = 0;
        char *token;
        FILE *file = fopen(fname, "r");
        if(!feof(file)){
                char line[100];
                
                if(fgets(line,100,file)!=NULL){
        token = strtok(line, ",");
        while(token != NULL){
        num_resource += 1;
        token = strtok(NULL, ",");
                }                         
		    }
        }
        fclose(file);
        return num_resource;
}


//The The thread runner
void *execute_thread(){
        char ord[100];
        int resources[cols];
        int process[10];
        int ind = 0;
        int curr;
        //show safe sequence
        printf("Safe Seq: ");
        fgets(ord, 100, stdin);
        char *token = strtok(ord, " ");

        while(token != NULL){
                process[ind] = atoi(token);
                token = strtok(NULL, " ");
                ind += 1;

        }

        for (int i = 0; i < ind; i++){
                printf("--> Thread %d\n", process[i]);
                printf("Given: ");
                for (int x = 0; x < cols; x++){
                        curr =  allocation_ptr[process[i] * cols + x];
                        printf("%d ", curr); 
                        resources[x] = curr;
                }
                printf("\n");
                printf("Need: ");
                for (int r = 0; r < cols; r++){
                        curr = need_ptr[process[i] * cols + r];
                        printf("%d ", curr);
                }
                printf("\n");
                printf("Available: ");
                for (int r = 0; r < cols; r++){
                        printf("%d ", available_ptr[r]);
                }
                printf("\n");
                printf("Started...\n");
                printf("Finished...\n");
                printf("Releasing...\n");

                for (int x = 0; x < cols; x++){
                available_ptr[x] += resources[x];
                *((available_ptr+ process[i] * cols) + x) -= resources[x];
                *((need_ptr + process[i] * cols) + x) += resources[x]; 

                }

                printf("Available: ");
                for (int r = 0; r < cols; r++){
                printf("%d ", available_ptr[r]);
                }
                printf("\n");

    }
    return NULL;


        
}

//Function to release resource
int resource_release (int args[]){
int i;
    int cust_num = args[0];
    int resources[cols];
    bool valid = true;
    for (i = 0; i < cols; i++){
        resources[i] = args[i + 1];
    }
    for (i = 0; i < cols; i++)
    {
        if (resources[i] > *((allocation_ptr + cust_num * cols) + i)){
            valid = false;
        }
        if (valid == true){
            for (i = 0; i < cols; i++){
                    //clearing the resources
                available_ptr[i] += resources[i];
                *((allocation_ptr + cust_num * cols) + i) -= resources[i];
                *((need_ptr + cust_num * cols) + i) += resources[i]; 
            }
            return 1; // resource released
}
        else{
            return 0; // resource cannot be released
        }
    }
    return 0;
}

//Function to request resources
int resource_request (int args[]){
    int customer_num = args[0];
    int req[cols];  
    bool valid;
    for (int i = 0; i < cols; i++) 
    {
        req[i] = args[i + 1];
    }

    int i;
    for (i = 0; i < cols && valid; i++)
    {
        valid = req[i] <= *(need_ptr + (customer_num * cols + i)); 
    }
    if (valid == true){
        for (i = 0; i < cols && valid; i++)
        {
            valid = req[i] <= *(available_ptr + i);
        }
        if (valid == true){
            for (i = 0; i < cols; i++){
                available_ptr[i] -= req[i];
                *((allocation_ptr + customer_num * cols) + i) += req[i];
                *((need_ptr + customer_num * cols) + i) -= req[i];
            }
            if (safety(available_ptr, allocation_ptr, need_ptr)){
                return 1; // you can have your resources and eat them too
            }
            else{ //we must undo our last actions so the system remains safe
                for (i = 0; i < cols; i++){
                    available_ptr[i] += req[i];
                    *((allocation_ptr + customer_num * cols) + i) -= req[i];
                    *((need_ptr + customer_num * cols) + i) += req[i];
                }
                return 0; 
            }
        }
        else
        {
            return 0; // 0 = not satisified
        }
    }
    else
    {
        return 0; 
    }
}

//the saftey algorithm
bool safety(int *available, int *allocated, int *need){
    int work[cols];
    for (int i = 0; i < cols; i++) {
        work[i] = *(available + i); 

    }
    printf("\n");

    bool finish[cols];
    for (int i = 0; i < cols; i++){
        finish[i] = false;
    }

    int safe_seq[rows];
//begin checking
    int ind = 0;
    while (ind < rows){
        bool found = false;
        for (int i = 0; i < rows; i++){
            if (finish[i] == false){
                int j = 0;
                for (j = 0; j < cols; j++){
                    if (*((need + i * cols) + j) > work[j]) 
                    {
                        break;
                    }
                }
                if (j == cols){
                    for (int y = 0; y < cols; y++)
                    {
                        work[y] += *((allocated + i * cols) + y);
                    }
                    finish[i] = true;

                    found = true;
                    safe_seq[ind++] = i;
                }
            }
        }
        if (found == false){
            return false; // return false if the system is not safe
         }
        }
}

//main method
int main(int argc, char *argv[]){
        //we begin by getting the number of resources and customers on file. This helps to fill the matrix
        cols = count_resources("sample4_in.txt"); 
        rows = read_customers("sample4_in.txt");
        int max[rows][cols];
        int allocation[rows][cols];
        int need[rows][cols];
        //error checking
        if(argc > cols + 1){
                printf("excess amount of command line args\n");
                return 0;
        }
        if(argc < cols + 1){
                printf("lack of command line args\n");
                return 0;
        }
        for (int i = 1; i < argc; i++)
        {
            if (isalpha(*argv[i]) != 0)
            {
                printf("non-digit input detected\n");
                return 0;
            }
        }

        //we now need to set the matricies
        printf("Number of Customers: %d\n", rows);
       
        printf("Cuurently Available resources: ");
        for (int i = 1; i < cols; i++){
                printf("%s ", argv[i]);
        }
        printf("%s\n", argv[cols]);


        
        printf("Maximum resources from file:\n");
        //these lines will open and set the MAX matrix
        FILE *file = fopen("sample4_in.txt", "r");
        char *token;
        int x = 0;
        while(!feof(file))
	{
		char line[100];
                
		if(fgets(line,100,file)!=NULL)
		{
			printf("%s",line);
                        token = strtok(line, ",");
                        int y = 0;
                        while(token != NULL){
                                max[x][y] = atoi(token);
                                token = strtok(NULL,",");
                                y += 1;
                        }
                                
		}
                x += 1;
	}
        fclose(file);
        printf("\n");
        

        //Sets the available matrix
        int *available = (int *)malloc(cols * sizeof(int)); 
                for (int i = 0; i < cols; i++){
                        available[i] = atoi(argv[i + 1]);
                }
        
        
        //Sets the allocation matrix
        for (int x = 0; x < rows; x++){
                for (int y = 0; y < cols; y++)
                {
                    allocation[x][y] = 0;
                    
                }
        }


        for (int x = 0; x < rows; x++){
                for (int y = 0; y < cols; y++){
                        need[x][y] = max[x][y] - allocation[x][y];
                }
                
        }
        
        //inits
        allocation_ptr = &available[0];
        max_ptr = &max[0][0];
        allocation_ptr = &allocation[0][0];
        need_ptr = &need[0][0];

        char cmd[100];
    
        bool running = true;
        //main user control loop
        while (running){
                printf("Command: ");

                fgets(cmd, 100, stdin);

                char *token = strtok(cmd, " "); 
                //clean string

                int args[cols + 1];
                token = strtok(NULL, " ");
                int j = 0;
                while (token != NULL){
			args[j] = atoi(token);
			token = strtok(NULL, " ");
			j += 1;
                }

                for (char *cmd_lower = cmd; *cmd_lower; cmd_lower++){
                        *cmd_lower = tolower(*cmd_lower);
                }

                //STATUS command
                //This command will display the process' status
                if(strstr(cmd,"status")){
                        int current;
                        printf("Available resources: \n");
                        for (int i = 0; i < cols; i++){
                                current = available_ptr[i];
                                printf("%d ", current);
                        }
                        printf("\n");

                        printf("Max Resources:\n");
                        for (int i = 0; i < rows; i++){
                                for (int x = 0; x < cols; x++){
                                        current =  *((max_ptr + i * cols) + x);
                                        printf("%d ", current);
                                }
                                printf("\n");
                        }
                        printf("Resources Allocated:\n");
                        for (int i = 0; i < rows; i++){
                                for (int x = 0; x < cols; x++){
                                        current = *((allocation_ptr + i * cols) + x);
                                        printf("%d ", current);
                                }
                                printf("\n");
                        }
                        printf("This Processes needs:\n");
                        for (int i = 0; i < rows; i++){
                                for (int x = 0; x < cols; x++){
                                        current = *((need_ptr + i * cols) + x);
                                        printf("%d ", current);
                                }
                                printf("\n");
                        }
                }

                

                else if (strstr(cmd, "rq") != NULL){
                        valid_resc_request(args);               
                }
                else if (strstr(cmd, "run") != NULL){
                        execute_thread();
                }
                else if (strstr(cmd, "rl") != NULL)
                {
                        printf("Releasing Resources-\n");
                        resc_release(args);
                }
                
                else if (strstr(cmd, "exit") != NULL){
                        printf("Exiting program\n");
                        running= false;
                }
                else{
                        printf("INVALID INPUT\n");
                }
        }
 
        
}