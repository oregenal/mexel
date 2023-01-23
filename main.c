#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>

#define FILE_PATH "./input.csv"

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	FILE *file = fopen(FILE_PATH, "r");
	if(file == NULL) {
		perror("Failed");
		exit(EXIT_FAILURE);
	}

	struct stat st;
	int file_size = stat(FILE_PATH, &st);
	if(file_size < 0) {
		perror("Failed");
		exit(EXIT_FAILURE);
	}

	size_t size = st.st_size;
	char *buffer = malloc(sizeof(char)*size);

	size_t readed = fread(buffer, 1, size, file);
	if(readed != size) {
		fprintf(stderr, "Fail to read file.\n");
		exit(EXIT_FAILURE);
	}

	puts(buffer);

	free(buffer);
	fclose(file);
	return 0;
}
