/**
 *
 * This program copies files using a pipe.
 *
 * Usage:
 *	filecopy <input file> <output file>
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include<string.h>
#include <time.h>

#define READ_END 0
#define WRITE_END 1

int main( int argc, char* argv[] ) 
{
	int rv;  //unused
	pid_t pid;
	int c;   //unused
    char rb[50], wb[2];	/* bytes for reading/writing */
    //Made rb bigger to read more at a time
	int ffd[2];		/* file descriptor */
    int pfd[2];     //Pipeline File Descriptor
    
    /* Step1: Create pipe file descriptor */

    /* open the input file */

    ffd[READ_END] = open(argv[1], 0);

    if (ffd[READ_END] < 0) {
		fprintf(stderr,"Unable to open %s\n",argv[1]);
		return 1;
	}
	
    /* open the output file */
	ffd[WRITE_END] = open(argv[2], O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	if (ffd[1] < 0) {
		fprintf(stderr,"Unable to open %s\n",argv[2]);

		/* close the input file */
		close(ffd[0]);

		return 1;
	}

	/* Step2: set up the pipe */
	/* make sure your program closes file handles*/
    pipe(pfd);

	/* Step3: create the processes */
	/* read from the input file and write to the pipe */
	/* read from the pipe and write to the output file */
    pid = fork();

    if (pid == 0) {    //Check if we're the child or parent process
       //Child Process
        close(pfd[WRITE_END]);  //close the end of the pipe we don't need

        while (read(pfd[READ_END],rb, sizeof(rb)) > 0) {  //Child process read everything from the pipeline

            write(ffd[WRITE_END], rb, strlen(rb) - 1);  //Child process write everything from the pipeline to the output.txt

        }

        /*Step4: Add a timestamp in the child process to record the time when the reading-writing is done.
        7Print the timestamp to console output. */
        time_t currentTime;

        currentTime = time(NULL);

        printf("Child Process completed at: %s",asctime(localtime(&currentTime)) );  //print the current time

        close(pfd[READ_END]);   //close the pipeline
        close(ffd[WRITE_END]);  //close the output txt file
    }
    else {
        // Parent Process
        close(pfd[READ_END]);   //close the end of the pipe we don't need

        while (read( ffd[READ_END], rb, sizeof(rb)) > 0) { //Parent process read everything from the input file

            write(pfd[WRITE_END], rb, sizeof(rb)); //Parent process write everything from the input file to the pipeline
           
        }

        close(pfd[WRITE_END]); //close the pipeline
        close(ffd[READ_END]);   //close the input txt file
    }
}