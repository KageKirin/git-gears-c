#include <assert.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <git2.h>
#include <unistd.h>
#include <regex.h> //<-- POSIX Regex (egrep)


#include "gears_util.h"
#include "gears_option.h"

static char scrape[4096] = {0};

#define GIT_BUF_INIT                                                                                                   \
	{                                                                                                                  \
		git_buf__initbuf, 0, 0                                                                                         \
	}
extern char git_buf__initbuf[];

char git_buf__initbuf[1];

static struct {
	char* url;
} OptionValues = {
	.url = NULL,
};

static Option options[] = {
	{ .description = "url to parse", &OptionValues.url, gears_setOptionPositionalString },
};

int parsetest(char*);

int ParseUrl(int argc, char** argv)
{
	// parse arguments
	int newargc = parse_options(options, ARRAY_COUNT(options), argc, argv);

	gears_println("url: %s", OptionValues.url);
	//parsetest(OptionValues.url);
	gears_tag();

	// implementation of action
	// result display

	// config


	regex_t regex = {0};
	int reti = regcomp(&regex,
		"^(https?|git|ssh|rsync)\\://"	  //
		//"(?:(.+)@)*"					  //
		//"([a-z0-9_.-]*)"				  //
		//"[:/]*"							  //
		//"([\\d]+){0,1}"					  //
		//"(\\/(([\\w\\-]+)\\/)?"			  //
		//"(([\\w\\-\\.]+?)(\\.git|\\/)?)?)$"
		, REG_EXTENDED);
	assert(reti == 0);

	reti = regexec(&regex, OptionValues.url, 0, NULL, 0);
	gears_println("regex: %i [%zu]", reti, regex.re_nsub);
	assert(reti == 0);

	regmatch_t matches[10] = {0};
	reti = regexec(&regex, OptionValues.url, 10, matches, 0);
	if (reti == 0)
	for (size_t i = 0; i < ARRAY_COUNT(matches); ++i)
	{
		gears_println("%zu -> %lld - %lld", i, matches[i].rm_so, matches[i].rm_eo);
		if (matches[i].rm_so >= 0)
		for(int cc = matches[i].rm_so; cc < matches[i].rm_eo; ++cc)
		{
			gears_printf("%c", OptionValues.url[cc]);
		}
		gears_println("", NULL);
	}
	gears_println("regex: %i", reti);
	regfree(&regex);

	return 0;
}
