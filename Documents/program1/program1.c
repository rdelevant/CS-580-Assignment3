/*Rochell Delevante, WSU ID: a263z455 Program Assignment 1 
This program calls the fork system call to execute the Collatz conjecture for any positive integer-even or odd*/

#include <stdio.h>  //library used for printf
#include <unistd.h> //used for fork() system call
#include <sys/wait.h>   //used for wait() call
#include <stdlib.h>     //used for atoi call

int collatz(int n){ //function call to test whether n is even or odd
    if(n%2==0){
        return n/2; //n is even
    }
    else{
        return 3*n+1;   //n is odd
    }
}

int main(int argc, char **argv){
    if(argc!=2){        //Just the executable and the input integer
        fprintf(stderr, "Usage: %s <integer value>\n", argv[0]);
        return 1;
    }
    char* str=argv[1];
    int n=atoi(str); //all strings converted to integers
    if(n>50000 || n<2){ //tests should be between these numbers
        return 1;
    }
    int input=n;
    pid_t pid=fork();   //used to execute child process
    
    if(pid<0){      //error
        fprintf(stderr, "Failure");
        return 1;
    }
   
    
    else if(input==0){       //input==0
        while(input>1){ //child process
            printf("%d, ", input);  //outputs integers
            input=collatz(input);   //even/odd function call
            
           
        }
    printf("1\n"); //tests always converges to 1      
    }
     
    else{    //parent process
        wait(NULL); //wait for child to complete execution
    }
        
    return 0;
}
