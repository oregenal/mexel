#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * TODO:
 * Use struct for content
 * Parse formulas & do math
 *
 */

typedef struct {
	int first_cell_row, first_cell_col, second_cell_row, second_cell_col;
	char math_sign;
} cells_math_t;

void check_arguments(int argc, char **argv)
{
	if(argc < 2) {
		fprintf(stderr, "Usage: %s <file.csv>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
}

char *get_content(char **argv)
{
	FILE *input = fopen(argv[1], "r");
	if(input == NULL) {
		perror("File open error");
		exit(EXIT_FAILURE);
	}

	fseek(input, 0L, SEEK_END);
	long file_size = ftell(input);
	rewind(input);

	char *buffer = malloc(file_size * sizeof(char));

	size_t readed = fread(buffer, 1, file_size, input);

	if((long)readed != file_size) {
		fprintf(stderr, "File read error.\n");
		exit(EXIT_FAILURE);
	}

	fclose(input);

	return buffer;
}

void get_row()
{
}

void get_column() 
{
}

void get_math_sign(char content)
{
	switch(content) {
		case '+':
			//do_summ();
			break;
		case '-':
			//do_minus();
			break;
		case '*':
			//do_mult();
			break;
		case '/':
			//do_division();
			break;
		default:
			fprintf(stderr, "Cell parsing error,\
					must be UNREACHABLE\n");
	}
}

void do_math(char *content, int *i)
{
	cells_math_t cells_math;
	/* TODO: dummy */
	(void) cells_math;

	while(content[*i] != ',' && content[*i] != '\n') {
		if(isalpha(content[*i])) {
			get_row();
		} else if(isdigit(content[*i])) {
			get_column();
		} else {
			get_math_sign(content[*i]);
		}
		++*i;
	}
}

void process_data(char *content)
{
	int i = 0;
	while(content[i]) {
		if(content[i] == '=') {
			++i;
			do_math(content, &i);
			//TODO: math
			// do a structure wich have row/col for the both cells & math sign
		} else {
			putchar(content[i]);
			++i;
		}
	}
}

int main(int argc, char **argv)
{
	check_arguments(argc, argv);

	char *content = get_content(argv);

	process_data(content);

	free(content);

	return 0;
}
