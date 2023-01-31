#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#define USAGE_HELP \
	"Type ./mexel <file_name.csv>\n"

typedef struct context {
	char *buffer;
	size_t len;
} context_t;

context_t *read_context(int argc, char **argv)
{
	if(argc != 2) {
		fputs(USAGE_HELP, stderr);
		exit(EXIT_FAILURE);
	}

	const char *file_path = argv[1];

	char *extention_chek = strcasestr(file_path, ".csv");
	if(extention_chek == NULL) {
		fputs("File must be *.csv", stderr);
		exit(EXIT_FAILURE);
	}

	FILE *file = fopen(file_path, "r");
	if(file == NULL) {
		perror("Failed");
		exit(EXIT_FAILURE);
	}

	struct stat st;
	int error_indicator = stat(file_path, &st);
	if(error_indicator < 0) {
		perror("Failed");
		exit(EXIT_FAILURE);
	}

	context_t *result = malloc(sizeof(context_t));

	result->len = st.st_size;
	result->buffer = malloc(sizeof(char)*result->len);

	size_t readed = fread(result->buffer, 1, result->len, file);
	if(readed != result->len) {
		fputs("Fail to read file.\n", stderr);
		exit(EXIT_FAILURE);
	}

	fclose(file);

	return result;
}

void delete_context(context_t *context)
{
	free(context->buffer);

	free(context);
}

void find_cell(size_t row, size_t col, context_t *context)
{
	char *buffer = context->buffer;
	while(row-1) {
		while(*(buffer++) != '\n');
		--row;
	}

	while(col-1) {
		while(*(buffer++) != ',');
		--col;
	}

	printf("=");
	while(*buffer != ',' && *buffer != '\n') fwrite(buffer++, 1, 1, stdout);
}

char *do_math(char *current, context_t *context)
{
	(void) context;

	printf("f(");
	for(;;) {
		size_t column = *(current++)-'A'+1;
		size_t row = *(current++)-'1'+1;
		printf("%zu:%zu", column, row);
		find_cell(row, column, context);
		if(*current == ',' || *current == '\n')
			break;
		fwrite(current++, 1, 1, stdout);
	}
	printf(")");

	return current;
}

void parse_context(context_t *context)
{
	char *current = context->buffer;

	bool new_cell = true;
	
	while(*current) {
		if(new_cell) {
			if(*current == '=') {
				current = do_math(++current, context);
			}
		}

		if(*current == '\n') new_cell = true;
		if(*current == ',') new_cell = true;

		fwrite(current++, 1, 1, stdout);
	}

	//fwrite(context->buffer, 1, context->len, stdout);
}

int main(int argc, char **argv)
{
	context_t *context = read_context(argc, argv);

	parse_context(context);
	//parse_context(context);

	delete_context(context);

	return 0;
}
