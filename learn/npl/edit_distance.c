#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage(int argc, char **argv)
{
	printf("give to word return the edit distance form word1 to word2\n");
	printf("%s word1 word2\n", argv[0]);
	exit(-1);
}

int word1_len, word2_len;

#define ITEM(m,i,j) m[(i) * (word2_len + 1) +j]

void dump(char *word1, char *word2, int *m)
{
	int i,j;
	printf("   - ");
	for (j=1; j < strlen(word2) + 1; j++) {
		printf("  %c ", word2[j-1]);
	}
	printf("\n");
	for (i=0; i < strlen(word1) + 1; i++) {
		if (i>0) printf("%c", word1[i-1]);
		else printf("-");
		for (j=0; j < strlen(word2) + 1; j++) {
			printf("%3d ", ITEM(m, i, j));
		}
		printf("\n");
	}
}

int edit_distance(char *word1, char *word2)
{
	word1_len = strlen(word1);
	word2_len = strlen(word2);

	int *m = (int *)malloc((word1_len + 1) * (word2_len + 1));

	// init
	int i,j;
	for (i=0; i < word1_len + 1; i++) {
		ITEM(m, i, 0) = i;
	}
	for (j=0; j < word2_len + 1; j++) {
		ITEM(m, 0, j) = j;
	}

	// do the work
	for (i=1; i < word1_len + 1; i++ ) 
		for (j=1; j < word2_len + 1; j++ ) {
			int a,b,c;
			a = ITEM(m, i, j-1) + 1;
			b = ITEM(m, i-1, j) + 1;
			if (word1[i-1] == word2[j-1]) {
				c = ITEM(m, i-1, j-1);
			} else {
				c = ITEM(m, i-1, j-1) + 2;
			}
			// be the minimal of the a,b,c
			ITEM(m, i, j) = a < b ? (a < c ? a : c) : (b < c ? b : c);
		}
	return ITEM(m, word1_len, word2_len);
}

int main(int argc, char **argv)
{
	if (argc < 3) {
		usage(argc, argv);
		return 0;
	}
	
	char *word1 = argv[1];
	char *word2 = argv[2];
	//dump(word1, word2, m);
	int distance = edit_distance(word1, word2);
	printf("%d\n", distance);
	return 0;
}
