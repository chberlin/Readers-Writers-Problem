#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv, char ** envp) {

	if (argc < 4) {
		printf("incorrect command line parameters. Correct usage: \t\n %s <N value> <R-value> <W-value>\n", argv[0]);
		exit(1);
	}

	int num, numreaders, numwriters;
	num = atoi(argv[1]);
	numreaders = atoi(argv[2]);
	numwriters = atoi(argv[3]);

	if (num < 1 || num > 1000) {
		printf("incorrect N value, choose value between 1 and 1000\n");
		exit(1);
	}

	if (numreaders < 1 || numreaders > 9) {
		printf("incorrect R value, choose value between 1 and 9\n");
		exit(1);
	}

	if (numwriters < 1 || numwriters > 9) {
		printf("incorrect W value, choose value between 1 and 9\n");
		exit(1);
	}


}
