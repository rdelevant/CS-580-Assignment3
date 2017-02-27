/*Rochell Delevante, WSU ID: a263z455 Program Assignment 2 
This program calls the fork system call to execute the Collatz conjecture for any positive integer-even or odd using shared memory. The parent process creates shared memory object and the child process will write the sequence to the shared memory. At the end parent process prints out the sequence.*/

#include <stdio.h>      //library used for printf
#include <unistd.h>     //used for fork() system call
#include <sys/wait.h>   //used for wait() call
#include <stdlib.h>     //used for atoi call
#include <string.h>     //strlen function
#include <sys/shm.h>    //using with shared memory
#include <sys/mman.h>   //using with shared memory
#include <fcntl.h>      //defines O_CREAT, O_RDWR, O_RDONLY


int main(int argc, char **argv){
    int x = 0;
    const int SIZE = 4096;          //bytes in shared memory
    const char *share = "Message";  //name of shared memory object
    int shm_fd;                     //shared memory file descriptor
    void *ptr = NULL;               //pointer to the shared memory object
    
    //strings written to shared memory-displayed
    const char *message_0 = "Operating";
    const char *message_1 = " Systems";
    const char *message_2 = " Is Fun!!";
    
    //kept in memory
    shm_fd = shm_open(share, O_CREAT | O_RDWR, 0666);
    
    //specify fixed size in number of bytes
    ftruncate(shm_fd, SIZE);
    
    //maps shared memory object as a file-parent process will read
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    
    
    //Just the executable and the input integer
    if(argc!=2){       
        fprintf(stderr, "Usage: %s <integer value>\n", argv[0]);
        return 1;
    }
    char* str=argv[1];
    int input=atoi(str);        //all strings converted to integers
    if(input>50000 || input<2){     //tests should be between these numbers
        printf("You entered an invalid number\n");
        return 1;
    }
    int collatz=input;
    pid_t pid=fork();       //used to execute child process
    
    //error had occurred pid is a negative value
    if(pid<0){              
        fprintf(stderr, "Failure");
        return 1;
    }
    
    //child process
    else if(pid==0){      
        
        //open shared memory object for reading and writing
        shm_fd = shm_open(share, O_RDWR, 0666); 
        
        //keep the shared memory object to the size the parent want
        ftruncate(shm_fd, SIZE);
        
        //memory map it
        ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
        
        //written into shared memory
        sprintf(ptr, "%s", message_0);
        ptr += strlen(message_0);
        sprintf(ptr, "%s", message_1);
        ptr += strlen(message_1);
        sprintf(ptr, "%s", message_2);
        
        //executable file
        while(collatz>1){ 
            x++;
            printf("%3d, ", collatz);  //outputs integers
            if(x%10==0){               //displays 10 integers per line 
                printf("\n");
            }
            //checks if integer is even
            if(collatz%2==0){
                collatz/=2;
            }
            //checks if integer is odd
            else{
                collatz = 3 * collatz +1;
            }
        }
             
    printf(" ");    
    printf("1\n");                  //tests always converges to 1    
    }
    
    //parent process   
    else{                          
        wait(NULL);                 //wait for child to complete execution
        
        //read from memory shared object
        printf("%s\n", (char *) ptr);
        
        //remove the shared memory object
        shmdt(share);
    }
       
    return 0;
}
    
