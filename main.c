#include <stdio.h>
#include <stdlib.h>

/*
 * TODO:
 * Parse formulas & do math
 *
 */

void check_arguments(int argc, char **argv)
{
	if(argc < 2) {
		fprintf(stderr, "Usage: %s file.csv\n", argv[0]);
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

int main(int argc, char **argv)
{
	check_arguments(argc, argv);

	char *content = get_content(argv);

	printf("%s\n", content);

	free(content);

	return 0;
}