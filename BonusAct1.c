#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define READ_END	0
#define WRITE_END	1
#define BUFFER_SIZE 25

int
main(int argc, char *argv[])
{
		//Variable setup
		pid_t fork1;
		int fd[2];
		//-----------

		//Create the pipe
		if (pipe(fd) == -1) {
			fprintf(stderr,"ERROR: Pipe failed");
			return 1;
		}
		
		//Create the fork
		fork1 = fork();
		
		//Checks to see if fork failed
		if (fork1 < 0) 
		{
			// print that fork failed and exit program
			fprintf(stderr, "ERROR: Fork failed.\n");
			exit(1);
		} 
		
		
		
		/*----------PARENT----------*/
		else if (fork1 > 0)
		{
			//Variable setup
			
			//Create array of int to pass
			int mainSendArray[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
			
			int i, returnStatus;
			char read_msg[BUFFER_SIZE];
			
			//-----------

		
			//WRITE AND LOOP THROUGH EACH INT IN ARRAY
			for(i = 0; i<10; i++)
			{
				write(fd[WRITE_END], &mainSendArray[i], sizeof(mainSendArray[i]));
				printf("Parent(%d)| send value: %d\n", getpid(), mainSendArray[i]);
 
			}

			//CLOSE WRITE PIPE
			close(fd[WRITE_END]);
	
			//WAIT FOR CHILDREN TO RESPOND
			waitpid(fork1, &returnStatus, 0);

			if (returnStatus == 0)  // Verify child process terminated without error.  
			{
				printf("The child process terminated normally.\n");   
				//READ FROM CHILD
				read(fd[READ_END], read_msg, BUFFER_SIZE);
				printf("Parent(%d)| Child says: '%s'\n", getpid(), read_msg);
				

				//CLOSE READ PIPE 
				close(fd[READ_END]);
				
			}

			if (returnStatus == 1)      
			{
			  printf("The child process terminated with an error!.\n");    
			}
		}
		/*----------PARENT_END----------*/
		
		
		
		

		/*----------CHILD----------*/
		else
		{
			//Variable setup
			char write_msg[BUFFER_SIZE] = "ACK";
			int mainReceiveArray[10];
			int n, i;

			//-----------
			
			//Print message to prove child is ready
			printf("Child(%d) 'I'm ready for action!!'\n", getpid());
			
			//Read each int from the pipe and send to array
			//READ FROM PIPE EACH ITEM AND FORM ARRAY
			for(i = 0; i<10; i++)
			{
				read(fd[READ_END], &n, sizeof(n));
				mainReceiveArray[i] = n;
				printf("Child(%d)| receive value: %d\n", getpid(), n);
 
			}

			//CLOSE READ PIPE
			close(fd[READ_END]);
	
			//PRINT ARRAY FROM CHILD AS PROOF
			//printf("Child(%d)| received array: %d\n", getpid(), mainReceiveArray);

			//WRITE "AWK" to parent
			write(fd[WRITE_END], write_msg, strlen(write_msg)+1);

			//CLOSE WRITE PIPE
			close(fd[WRITE_END]);

			//EXIT
			exit(0);
    } 
	/*----------CHILD_END----------*/
	
    return 0;
}