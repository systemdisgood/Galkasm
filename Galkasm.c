#include <stdio.h>
#include <stdlib.h>

#define MAX_TOKEN_LEN 0x7F

enum PROGRAM_RUN_TARGETS {IDLE, RUN, COMPILE};
int program_run_target = IDLE;
FILE* input_file = NULL;
FILE* output_file = NULL;

int input_character = 0;

int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		fprintf(stderr, "Must be operation (compile, run) type, source path, and output file path.\n");
		return EXIT_FAILURE;
	}

	for(int args_counter = 1; args_counter < argc; ++args_counter)
	{
		printf("%s\n", argv[args_counter]);
	}

	if(!strcmp("compile", argv[1]) && IDLE == program_run_target)
	{
		program_run_target = COMPILE;
		printf("Arg is compile\n");
	}

	if(!strcmp("run", argv[1]) && IDLE == program_run_target)
	{
		program_run_target = RUN;
		printf("Args is run\n");
	}

	if(IDLE == program_run_target)
	{
		fprintf(stderr, "No such target %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	if(NULL == (input_file = fopen(argv[2], "r")))
	{	
		fprintf(stderr, "Can't open input file %s\n", argv[2]);
		return EXIT_FAILURE;
	}

	if(NULL == (output_file = fopen(argv[3], "w")))
	{
		fprintf(stderr, "Can't open output file %s\n", argv[3]);
		return EXIT_FAILURE;
	}

	while((input_character = fgetc(input_file)) != EOF)
	{
		putchar(input_character);
	}
	if(ferror(input_file))
	{
		fprintf(stderr, "Error while trst input file reading\n");
		return EXIT_FAILURE;
	}

	fclose(input_file);
	fclose(output_file);

	return EXIT_SUCCESS;
}
