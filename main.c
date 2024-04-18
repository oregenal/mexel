#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

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

int get_col(content_t *content, size_t *index)
{
	int result;

	if(islower(content->buffer[*index])) {
		result = content->buffer[*index] - 'a' + 1;
	} else if(content->buffer[*index]) {
		result = content->buffer[*index] - 'A' + 1;
	} else {
		fputs("Get col error, UNREACHABLE\n", stderr);
		exit(EXIT_FAILURE);
	}

	++*index;

	return result;
}

int parse_furmula(content_t *, size_t *);

int get_integer(content_t *content, size_t *index) 
{
	int result = 0;
	bool negative = false;

	if(content->buffer[*index] == '=') {
		++*index;
		return parse_furmula(content, index);
	}

	if(!isdigit(content->buffer[*index])
			&& content->buffer[*index] != '-') {
		fputs("Error in formula, cell data is not integer.\n", stderr);
		exit(EXIT_FAILURE);
	}

	if(content->buffer[*index] == '-') {
		negative = true;
		++*index;
	}

	while(isdigit(content->buffer[*index]))
	{
		result = result*10 + ((content->buffer[*index] - '0'));
		++*index;
	}

	return negative ? -result : result;
}

int get_row(content_t *content, size_t *index)
{
	return get_integer(content, index);
}

void find_row(int row, content_t *content, size_t *search_index)
{
	while(row - 1) {
		while(content->buffer[*search_index] != '\n') {
			++*search_index;
		}
		++*search_index;
		--row;
	}
}

void find_col(int col, content_t *content, size_t *search_index)
{
	while(col - 1) {
		while(content->buffer[*search_index] != ',') {
			++*search_index;
		}
		++*search_index;
		--col;
	}
}

int get_cell_data(int row, int col, content_t *content)
{
	size_t search_index = 0;
	find_row(row, content, &search_index);
	find_col(col, content, &search_index);
	return get_integer(content, &search_index);
}

int do_math(int first_cell, int second_cell, char math_sign)
{
	if(math_sign != '\0') {
		switch(math_sign) {
			case '+':
				second_cell += first_cell;
				break;
			case '-':
				second_cell = first_cell - second_cell;
				break;
			case '*':
				second_cell *= first_cell;
				break;
			case '/':
				second_cell = first_cell / second_cell;
				break;
			default:
				fprintf(stderr, "Math sign error, must be UNREACHABLE.\n");
		}
	}

	return second_cell;
}

int parse_furmula(content_t *content, size_t *index)
{
	int row, col, temp, result = 0;
	char math_sign = '\0';

	//=A1+B3+D6
	//   ^
	while(content->buffer[*index] != ',' 
			&& content->buffer[*index] != '\n') {
		if(isalpha(content->buffer[*index])) {
			col = get_col(content, index);
		} else if(isdigit(content->buffer[*index])) {
			row = get_row(content, index);
			temp = get_cell_data(row, col, content);
			result = do_math(result, temp, math_sign);
		} else { 
			math_sign = content->buffer[*index];
			++*index;
		} 
	}


	return result;
}

void process_data(content_t *content)
{
	while(content->buffer[content->index]) {
		if(content->buffer[content->index] == '=') {
			++content->index;
			int result = parse_furmula(content, &content->index);
			printf("%d", result);
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
