// Raileen Del Rosario
// Unix Shell Project

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define MAX_LINE 80 //The maximum number of characters for the command should be 80. 

char hist[10][MAX_LINE]; //This array stores the history commands.
int counter = 0;  //This initializes the counter variable, which keeps track of the number of commands. 

//History function from Silberschatz textbook. 
void history(){
    int i;
    int j = 0;
    int histCount = counter;
    for (i = 0; i<10; i++){ 
       	printf("%d.  ", histCount);
        while (hist[i][j] != '\n' && hist[i][j] != '\0'){
            printf("%c", hist[i][j]);
            j++;
        }
        printf("\n");
        j = 0;
        histCount--;
        if (histCount ==  0)
        break;
    }
}

//This function reads the input and splits it into tokens.
void readIn(char in[], char *args[],int *isAmp)
{
    int charDex = 0; //Index for char that helps determine where to put the characters in arg
	int l; //Length of command 		
    int next; //Next command	

    l = read(STDIN_FILENO, in, MAX_LINE);

    next = -1;
    if (l == 0)
        exit(0);
    if (l < 0){
        perror("Invalid command");
	exit(-1);
    }

    for (int i = 0; i < l; i++) {
        switch (in[i]){
        case ' ':
        case '\t' :
            if(next != -1){
                args[charDex] = &in[next];
                charDex++;
            }
            in[i] = '\0';
            next = -1;
            break;

        case '\n':
            if (next != -1){
                args[charDex] = &in[next];
                charDex++;
            }
            in[i] = '\0';
            args[charDex] = NULL;
            break;

        case '&': 	
            *isAmp = 1;
            in[i] = '\0';
            break;

        default :
            if (next == -1)
                next = i;
	}
    }
    args[charDex] = NULL;

    //This if statement checks to see if the user has inputted the history command, as well as if there is an existing command history.
    if(strcmp(args[0],"history") == 0){
            	if(counter>0){
            		history();
    		} else{
    			printf("\nNo Commands within history of time\n");
    			}
    }

    	else if (**args == '!'){
    		int check2 = args[0][1]- '0';
    		int check3 = args[0][2];
    		if(check2 > counter){
    		printf("\nCommand not found\n");
    		strcpy(in,"Invalid Command");
    		}else if (check3 != 0){
    				printf("\n Your input is too large.  History only stores the last 10 commands. \n");
    				strcpy(in,"Invalid Command");
    		}	else{
    				if(check2==-15){
    					 strcpy(in,hist[0]);

    				}	else if(check2 == 0){
    						printf("You cannot enter 0. Enter ! followed by single integer 1-9");
    						strcpy(in,"Invalid Command");
    					}	else if(check2 >= 1){
    							strcpy(in,hist[counter-check2]);
    						}

    				}
    		}
 
	// This loop updates the history array. 
    for (int i = 9; i>0; i--)
    	strcpy(hist[i], hist[i-1]);
        strcpy(hist[0],in);
        counter++;
    	if(counter>10){
    	counter=10;
    	}
}

// This function is an infinite loop to accept commands. 
int cmdInput(void){
    char in[MAX_LINE]; 			
    char *args[MAX_LINE/2+1];
    int isAmp;             		
    pid_t pid;
    int loops;					
    int continueLoop = 1;

    while (continueLoop){
	isAmp = 0;
	printf("osh>");
    fflush(stdout);
    readIn(in, args, &isAmp);   
    pid = fork();

   	if (pid < 0) {  			
   		printf("Fork failed.\n");
   		exit (1);
 	} 		else if (pid == 0){
 		 				if (execvp(args[0], args) == -1 && strcmp(in, "history") != 0)  //In the case of invalid command. 
 		 				{
 		 					printf("Invalid Command (ctrl^c to exit)\n");
 		 				}
    		}	else{
    			loops++;
    				if (isAmp == 0){	//Wait for child process to terminate if '&' follows command
    					loops++;
    					wait(NULL);
    				}

    		}
	}
	return 0;
}

// Initiate loop. 
int main(void){
cmdInput();
return 0;
}