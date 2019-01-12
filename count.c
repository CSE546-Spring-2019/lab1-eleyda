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
#include <string.h>
#endif

#ifdef linux
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

const int MAX_SIZE = 100;

int total_strings(FILE *fp_input, char *input_string);

int bring_in_and_cmp(char *input_string, long int ebp, int first_letter, FILE *fp_input, int string_len);

/* FIXME MUST BE CROSS PLATFORM */
int main(int argc, char **argv){
	if(argc != 4){ 
		printf("Error, must be ./count <input_file> <string> <output_file>\n");
		exit(10);
	}
	
	size_t act_size;
	FILE *fp_input;
	FILE *fp_output;
	int offset = 0;
	char *input_string; 
	input_string = (char*)malloc(20 * sizeof(char));
	if(input_string == NULL){
		printf("Failed to malloc memory\n");
		exit(11);
	}
	strcpy(input_string, argv[2]);
	printf("()input_string = %s\n", input_string);

	/* open the files */
	fp_input = fopen(argv[1], "rb");
	if(fp_input == NULL){
		printf("Error file not found: %s\n", argv[1]);
		exit(20);
	}
	
	fp_output = fopen(argv[3], "wb");
	if(fp_output == NULL){
		printf("Error output file failed: %s\n", argv[3]);
		exit(30);
	}	

/* find the file size */
#ifdef linux 
	fseek(fp_input, 0, SEEK_END);
	act_size = ftell(fp_input);
	rewind(fp_input);
	printf("Size of the file is %i bytes\n", act_size);
#endif

#ifdef _WIN32
	act_size = GetFileSizeEx(fp_input, NULL);
#endif

/* write to the output file */	
	int ret_in = 1;
	int ret_out = 0;
	char buffer[1];
	ret_in = fread(buffer, 1, 1, fp_input);
	while(ret_in == 1){
		ret_out = fwrite(buffer, 1, 1, fp_output);
		ret_in = fread(buffer, 1, 1, fp_input);
	}

	/* count the number of times the string shows up */	
	
	int total_strings = count_of_strings(fp_input, input_string);
	printf("%i strings of %s were found in %s\n", total_strings, input_string, argv[1]);

	free(input_string);
	fclose(fp_input);
	fclose(fp_output);
return 0;
} 


int count_of_strings(FILE *fp_input, char *input_string){
	int count = 0;
	int offset_inside = 0, ret_fread = 0, loop = 0;
	char *buffer, *temp;
	int string_len = 1; /* start at 1 b/c '\0' needs a spot */
	
	for(string_len = 0; input_string[string_len] != '\0'; string_len++);

	buffer = (char*)calloc((MAX_SIZE), sizeof(char));
	if(buffer == NULL){
		printf("Failed to calloc memory\n");
		exit(40);
	}
	/*
	temp = (char*)calloc(string_len, sizeof(char));
	if(temp == NULL){
		printf("Failed to calloc memory\n");
		exit(50);
	}*/

	rewind(fp_input);
	while((ret_fread = fread(buffer, 1, MAX_SIZE, fp_input)) != 0){
		//printf("### buffer = %s\n", buffer);
		int i;
		long int spot_in_memory = ftell(fp_input); /* save spot in memory in case goes into bring_in_and_cmp call */

		for(i = 0; i < MAX_SIZE; i++){
//printf("input_string[0] = %c = %d\tinput_string[0] & 0xff = %c = %d\n", input_string[0], input_string[0], input_string[0]&0xff, input_string[0]&0xff);

			if((buffer[i] != 0) && (buffer[i] == (input_string[0] & 0xff))){
		//printf("+++%d == %d at %d\n", buffer[i], input_string[i], i);
				count += bring_in_and_cmp(input_string, spot_in_memory, ((loop*MAX_SIZE) + i), fp_input, string_len);
				/* hold current position but get the next string_len characters starting from i */
			}
		}
		loop++; /* used to calculate offset */
	}

	free(buffer);
	//free(temp);

return count;
}

int bring_in_and_cmp(char *input_string, long int ebp, int first_letter, FILE *fp_input, int string_len){
	int rax = 1; /* returns 1 if match, 0 if not */
	char *buffer;
	int i;
	
	fseek(fp_input, first_letter, SEEK_SET); /* go to the first letter that matched */ 
	buffer = (char*)calloc((20), sizeof(char));
	if(buffer == NULL){
		printf("Failed to calloc memory\n");
		exit(60);
	}

	fread(buffer, 1, 20, fp_input);
	//printf("--- buffer[0] = %d\n", buffer[0]); 
	
	for(i = 0; i < string_len && rax == 1; i++){
		if(buffer[i] != (0xff & input_string[i])){
			rax = 0;
			//printf("Failed here when i = %d\n", i);
		}
	}
	fseek(fp_input, ebp, SEEK_SET); /* return program to where it was */

	free(buffer);
return rax;
}



