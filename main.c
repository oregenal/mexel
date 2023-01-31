#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define USAGE_HELP \
	"Not enough arguments.\n" \
	"Type ./mexel <file_name.csv>\n"

typedef struct context {
	char *buffer;
	size_t len;
} context_t;

context_t *read_file(int argc, char **argv)
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

void parse_file(context_t *context)
{
	fwrite(context->buffer, 1, context->len, stdout);
}

void delete_context(context_t* context)
{
	free(context->buffer);

	free(context);
}

int main(int argc, char **argv)
{
	context_t *context = read_file(argc, argv);

	parse_file(context);

	delete_context(context);

	return 0;
}
