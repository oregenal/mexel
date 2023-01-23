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
	int res = stat(FILE_PATH, &st);
	if(res < 0) {
		perror("Filed");
		exit(EXIT_FAILURE);
	}

	size_t size = st.st_size;
	char *buffer = malloc(sizeof(char)*size);

	fread(buffer, 1, size, file);

	puts(buffer);

	fclose(file);
	return 0;
}
