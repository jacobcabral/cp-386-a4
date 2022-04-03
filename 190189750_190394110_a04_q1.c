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
                available_ptr[i] += resources[i];
                *((allocation_ptr + cust_num * cols) + i) -= resources[i];
                *((need_ptr + cust_num * cols) + i) += resources[i]; //Resources get released and added back to available
            }
            return 1; // resource released
}
        else{
            return 0; // resource cannot be released
        }
    }
    return 0;
}

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
        valid = req[i] <= *(need_ptr + (customer_num * cols + i)); // request[i] <= need[customer_num][i]
    }
    if (valid == true){
        for (i = 0; i < cols && valid; i++)
        {
            valid = req[i] <= *(available_ptr + i); // request[i <= available[i]
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
                return 0; // not satsified
            }
        }
        else
        {
            return 0; // not satsified
        }
    }
    else
    {
        return 0; 
    }
}

bool safety(int *available, int *allocated, int *need)
{
    int work[cols];
    for (int i = 0; i < cols; i++) 
    {
        work[i] = *(available + i); 

    }
    printf("\n");

    bool finish[cols];
    for (int i = 0; i < cols; i++)
    {
        finish[i] = false;
    }

    int safe_seq[rows];

    int ind = 0;
    while (ind < rows)
    {
        bool found = false;
        for (int i = 0; i < rows; i++)
        {
            if (finish[i] == false)
            {
                int j = 0;
                for (j = 0; j < cols; j++)
                {
                    if (*((need + i * cols) + j) > work[j]) // need[i][j] > allocated[i][j] (why do I need to do this, I hate C)
                    {
                        break;
                    }
                }
                if (j == cols)
                {
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
        if (found == false)
        {
            return false;
        }
    }