///////////////////////////////////////////////////////////////////////
//  Course:                        CS 537                            //
//  Instructor:                    Remzi Arpaci-Dusseau              //
//  Project:                       1a. Fast Sort                     //
//  Author:                        Shixuan Fan                       //
//  Email:                         shixuan.fan@wisc.edu              //
///////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLEN 128

struct word_str {
	char word[129];
	char sentence[129];
};

int str_compare(const void* s1, const void* s2) {
	return strcmp(((const struct word_str*)s1)->word, ((const struct word_str*)s2)->word);
}

int main(int argc, char** argv) {
	// Incorrect number of arguments
	if (argc < 2 || argc > 3) {
		fprintf(stderr, "Error: Bad command line parameters\n");
		exit(1);
	}
  
	int i = 0, j = 0, compare_index = 0;
	// The second argument is not in "-#" format
	if (argc == 3) {
		if (argv[1][0] != '-') {
			fprintf(stderr, "Error: Bad command line parameters\n");
			exit(1);
		}
		for (i = 1; i < strlen(argv[1]); ++i) {
			if (argv[1][i] < '0' || argv[1][i] > '9') {
				fprintf(stderr, "Error: Bad command line parameters\n");
				exit(1);
			}
		}
    
		// Calculate which word should be used to do the sorting
		compare_index = - atoi(argv[1]) - 1;
	}
  
	// The number in the second argument is 0
	if (compare_index < 0) {
		fprintf(stderr, "Error: Bad command line parameters\n");
		exit(1);
	}
  
	// Get the line number of file
	FILE *in = fopen(argv[argc - 1], "r");
	if (in == NULL) {
		fprintf(stderr, "Error: Cannot open file %s\n", argv[argc - 1]);
		exit(1);
	}
  
	int line = 0;
	char *buffer = malloc((MAXLEN + 1) * sizeof(char)), *sentence = NULL, *word = NULL;
	while (fgets(buffer, MAXLEN + 1, in) != NULL) {
		// If the line is too long, return error
		if (strlen(buffer) == MAXLEN && buffer[MAXLEN - 1] != '\n') {
			fprintf(stderr, "Line too long\n");
			exit(1);
		}
		++line;
	}
  
	fclose(in);
	// Allocate memory for the array
	struct word_str* sort_array = malloc(line * sizeof(struct word_str));
	if (sort_array == NULL) {
		fprintf(stderr, "malloc failed\n");
		exit(1);
	}
  
	// Read the file and copy the sentences and words to be compared
	in = fopen(argv[argc - 1], "r");
	for (i = 0; i < line; ++i) {	
		// Copy sentence	
		sentence = fgets(buffer, MAXLEN + 1, in);
		strcpy(sort_array[i].sentence, sentence);
		// Copy word
		char* last;
		word = strtok(sentence, " \n");
		for (j = 1; j <= compare_index; ++j) {
			last = word;
			word = strtok(NULL, " ");
			// If the given sentence doesn't have enough words, get the last word
			if (word == NULL) {
				word = last;
				break;
			}
		}
    
		// Empty line
		if (word == NULL) {
			sort_array[i].word[0] = '\0';
			continue;
		}
    
		// Normal line
		else {
			strcpy(sort_array[i].word, word);
		}
	}
	fclose(in);
	free(buffer);
  
	// Sort
	qsort(sort_array, line, sizeof(struct word_str), str_compare);
  
	// Print
	for (i = 0; i < line; ++i) {
		printf("%s", sort_array[i].sentence);
	}
	free(sort_array);
	
	return (0);
}
