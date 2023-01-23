#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define USAGE_HELP \
	"Not enough arguments.\n" \
	"Type ./mexel <file_name.csv>\n"

char *read_file(int argc, char **argv)
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

	size_t size = st.st_size;
	char *buffer = malloc(sizeof(char)*size);

	size_t readed = fread(buffer, 1, size, file);
	if(readed != size) {
		fputs("Fail to read file.\n", stderr);
		exit(EXIT_FAILURE);
	}

	fclose(file);

	return buffer;
}

int main(int argc, char **argv)
{
	char *context = read_file(argc, argv);

	puts(context);

	free(context);
	return 0;
}
