/* Eric Leyda */

/* This program reads in an arbitrary bindary file and then prints stats about the file
- size of the file
- number of times the serach string specfficied word in second argument appears
*/

/* 
- moves args[0] and args[1] to a seperate variable
- open the file
	- error check
- look for size and certain string at same time
	- loop around entire file
		- Each chunk will be added together for the entire size
		- Within the chunks, look for matches of the string
			- if start start of string is found, check (len - 1) to see if the rest is a match (keep track of where the start is for the size

*/
#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#endif

#ifdef linux
#include <stdio.h>
#include <stdlib.h>
#endif

/* FIXME MUST BE CROSS PLATFORM */
int main(int argc, char **argv){
	if(argc != 4){ 
		printf("Error, too many command line args, must be ./count <input_file> <string> <output_file>\n");
		exit(10);
	}
	int offset = 0;
	size_t size;
	FILE *fp_input, *fp_output;

/*	char output_file[100];
	char input_string[100];

	memset(input_string, '\0', 100);
	memset(output_file, '\0', 100);

	strcpy(input_string, argv[2]);
	strcpy(output_file, argv[3]);
*/
	

	fp_input = fopen(argv[1], "r");
	if(fp_input == NULL){
		printf("Error file not found: %s\n", argv[1]);
		exit(20);
	}
	
	

	

	fclose(fp_input);
return 0;
}
