#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_TOKEN_LEN 0x7F
#define MAX_DATA_MEM 0x3FFFFFFF

enum PROGRAM_RUN_TARGETS {IDLE, RUN, COMPILE};
int program_run_target = IDLE;
FILE* input_file = NULL;
FILE* output_file = NULL;

int input_character = 0;
unsigned long long input_characters_counter = 0;
char* input_chars = NULL;
char* output_chars_ptr = NULL;
bool data_mem[MAX_DATA_MEM] = {'0'};

enum CMDS {NOP = 0, SET = 1, TRY = 2, STR = 3, RIN = 4, FAN = 5, RIP = 6, FAP = 7, STP = 8, RES = 9};
// 0 - nop
// 1 - set
// 2 - try
// 3 - str
// 4 - rin
// 5 - fan
// 6 - rip
// 7 - fap
// 8 - stp
// 9 - res

void run(char* cmd_chars, char*  output_chars_ptr)
{
	unsigned long long cmd_char_pos = 0;
	unsigned long long data_mem_pos = 0;
	unsigned long long max_data_mem_poses = 1;
	unsigned long long output_chars_quantity = 0;
	unsigned long long output_chars_counter = 0;
	bool need_to_stop = false;
	bool need_to_step_next = false;
	bool need_to_step_prev = false;
	bool step_next_done = false;
	bool step_prev_done = false;
	int cmd = STP;
	bool reg = false;
	while(!need_to_stop)
	{
		if ('\0' == cmd_chars[cmd_char_pos]) cmd_char_pos = 0;
		if(cmd_chars[cmd_char_pos] >= '1' && cmd_chars[cmd_char_pos] <= '9') cmd = cmd_chars[cmd_char_pos] - '0';
		else cmd = NOP;
		switch(cmd)
		{
			case NOP:
				break;
			case SET:
				reg = true;
				break;
			case TRY:
				if(true == data_mem[data_mem_pos]) reg = false;
				break;
			case STR:
				data_mem[data_mem_pos] = reg;
				break;
			case RIN:
				need_to_step_next = true;
				break;
			case FAN:
				need_to_step_next = false;
				step_next_done = true;
				break;
			case RIP:
				need_to_step_prev = true;
				break;
			case FAP:
				need_to_step_prev = false;
				step_prev_done = true;
				break;
			case STP:
				if(true == data_mem[data_mem_pos]) need_to_stop = true;
				break;
			case RES:
				reg = false;
				need_to_step_next = false;
				step_next_done = true;
				need_to_step_prev = false;
				step_prev_done = true;
				break;
		}

		if(need_to_step_next && !step_next_done)
		{
			++data_mem_pos;
			step_next_done = true;
		}

		if(need_to_step_prev && !step_prev_done)
		{
			--data_mem_pos;
			step_prev_done = true;
		}

		if(max_data_mem_poses < (data_mem_pos + 1)) max_data_mem_poses = data_mem_pos + 1;
	}

	output_chars_quantity = max_data_mem_poses * 3 +1; // bit (0 or 1), \r\n and \0 in the end
	output_chars_ptr = (char*)malloc(sizeof(char) *  output_chars_quantity);
	printf("Error, bad output_chars_ptr allocation\n");
	for(unsigned long long output_data_counter = 0; output_data_counter < max_data_mem_poses; ++output_data_counter)
	{
		output_chars_ptr[output_chars_counter] = data_mem[output_data_counter] ? '1' : '0';
		++output_chars_counter;
		output_chars_ptr[output_chars_counter] = '\r';
		++output_chars_counter;
		output_chars_ptr[output_chars_counter] = '\n';
		++output_chars_counter;
	}
	output_chars_ptr[output_chars_counter] = '\0';
}

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
		//putchar(input_character);
		++input_characters_counter;
	}

	if(ferror(input_file))
	{
		fprintf(stderr, "Error while trst input file reading\n");
		return EXIT_FAILURE;
	}

	fseek(input_file, 0, SEEK_SET);
	input_chars = (char*)malloc((input_characters_counter + 1) * sizeof(char));
	if(NULL == input_chars)
	{
		printf("Error with input_chars memory allocation\n");
		return EXIT_FAILURE;
	}
	for(unsigned long long fill_input_characters_counter = 0; fill_input_characters_counter < input_characters_counter; ++fill_input_characters_counter)
	{
		input_chars[fill_input_characters_counter] = fgetc(input_file);
	}
	input_chars[input_characters_counter] = '\0';
	printf("%s\n", input_chars);

	if(RUN == program_run_target) run(input_chars, output_chars_ptr);

	fprintf(output_file, "%s", output_chars_ptr);
	free(output_chars_ptr);
	free(input_chars);
	fclose(input_file);
	fclose(output_file);

	return EXIT_SUCCESS;
}
