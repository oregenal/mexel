#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * TODO:
 * Parse formulas & do math
 *
 */

typedef struct {
	char *buffer;
	size_t index;
} content_t;

void check_arguments(int argc, char **argv)
{
	if(argc < 2) {
		fprintf(stderr, "Usage: %s <file.csv>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
}

content_t *get_content(char **argv)
{
	FILE *input = fopen(argv[1], "r");
	if(input == NULL) {
		perror("File open error");
		exit(EXIT_FAILURE);
	}

	fseek(input, 0L, SEEK_END);
	long file_size = ftell(input);
	rewind(input);

	content_t *content =malloc(sizeof(content_t));
	content->index = 0;
	content->buffer = malloc(file_size * sizeof(char));

	size_t readed = fread(content->buffer, 1, file_size, input);

	if((long)readed != file_size) {
		fprintf(stderr, "File read error.\n");
		exit(EXIT_FAILURE);
	}

	fclose(input);

	return content;
}

int get_row(content_t *content)
{
	int result;

	if(islower(content->buffer[content->index])) {
		result = content->buffer[content->index] - 'a' + 1;
	} else if(content->buffer[content->index]) {
		result = content->buffer[content->index] - 'A' + 1;
	} else {
		fprintf(stderr, "Get row error, UNREACHABLE");
		exit(EXIT_FAILURE);
	}

	++content->index;

	return result;
}

int get_column(content_t *content) 
{
	int result = 0;

	while(isdigit(content->buffer[content->index]))
	{
		result += result*10 + (content->buffer[content->index] - '0');
		++content->index;
	}

	return result;
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

void do_math(content_t *content)
{
	int row, col;

	while(content->buffer[content->index] != ',' 
			&& content->buffer[content->index] != '\n') {
		if(isalpha(content->buffer[content->index])) {
			row = get_row(content);
		} else if(isdigit(content->buffer[content->index])) {
			col = get_column(content);
		} else {
			//TODO: debug
			printf("row = %d; col = %d\n", row, col);
			get_math_sign(content->buffer[content->index]);

			++content->index;
		}
	}
}

void process_data(content_t *content)
{
	while(content->buffer[content->index]) {
		if(content->buffer[content->index] == '=') {
			++content->index;
			do_math(content);
		} else {
			putchar(content->buffer[content->index]);
			++content->index;
		}
	}
}

int main(int argc, char **argv)
{
	check_arguments(argc, argv);

	content_t *content = get_content(argv);

	process_data(content);

	free(content->buffer);
	free(content);

	return 0;
}
