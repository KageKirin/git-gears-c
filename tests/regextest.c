#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <assert.h>
#include <sys/time.h>


int main(int argc, char** argv)
{
	assert(argc == 2);
	printf("argv[1]: %s\n", argv[1]);

	struct timeval start, end, after_regcomp, before_regexec, after_regexec;

	gettimeofday(&start, NULL);

	regex_t rex;
	int rc = regcomp(&rex, "\\(aaa\\)\\(bbb\\)", 0);
	gettimeofday(&after_regcomp, NULL);
	if(rc)
	{
		size_t length = regerror(rc, &rex, NULL, 0);
		char* buffer = malloc(length);
		(void) regerror (rc, &rex, buffer, length);
		printf("regcomp error: %s", buffer);
		free(buffer);
	}

	printf("rex: %zu\n", rex.re_nsub);
	regmatch_t* matches = malloc((rex.re_nsub+1) * sizeof(regmatch_t));

	gettimeofday(&before_regexec, NULL);
	int rx = regexec(&rex, argv[1], rex.re_nsub+1, matches, 0);
	gettimeofday(&after_regexec, NULL);

	//regexec(&rex, argv[1], 0, 0, 0);
	if(rx)
	{
		size_t length = regerror(rx, &rex, NULL, 0);
		char* buffer = malloc(length);
		(void) regerror (rx, &rex, buffer, length);
		printf("regexec error: %s", buffer);
		free(buffer);
	}
	else
	{
		//(void) regexec(&rex, argv[1], rex.re_nsub+1, matches, 0);
		for(size_t i = 0; i < rex.re_nsub+1; ++i)
		{
			printf("match %zu [%lld .. %lld]: ", i, matches[i].rm_so, matches[i].rm_eo);
			for(regoff_t cc = matches[i].rm_so; cc < matches[i].rm_eo; ++cc)
			{
				printf("%c", argv[1][cc]);
			}
			printf("\n");
		}
	}

	free(matches);
	regfree(&rex);
	gettimeofday(&end, NULL);

	printf("regcomp: %d ms\n", after_regcomp.tv_usec - start.tv_usec);
	printf("regexec: %d ms\n", after_regexec.tv_usec - before_regexec.tv_usec);
	printf("start to end: %d ms\n", end.tv_usec - start.tv_usec);

	return 0;
}
