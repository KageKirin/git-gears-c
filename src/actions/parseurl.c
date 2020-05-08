#include <assert.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <git2.h>
#include <unistd.h>
#include <rure.h>


#include "gears_util.h"
#include "gears_giturl.h"
#include "gears_option.h"

static char scrape[4096] = {0};

#define GIT_BUF_INIT                                                                                                   \
	{                                                                                                                  \
		git_buf__initbuf, 0, 0                                                                                         \
	}
extern char git_buf__initbuf[];

char git_buf__initbuf[1];

static struct
{
	char* url;
} OptionValues = {
	.url = NULL,
};

static Option options[] = {
	{.description = "url to parse", &OptionValues.url, gears_setOptionPositionalString},
};

int parsetest(char*);

int ParseUrl(int argc, char** argv)
{
	// parse arguments
	int newargc = parse_options(options, ARRAY_COUNT(options), argc, argv);
	gears_println("url: %s", OptionValues.url);

	// implementation of action
	GitUrl gurl = gears_parseUrl(OptionValues.url);

	// result display
	gears_println("host: %s", gurl.host);
	gears_println("path: %s", gurl.path);
	gears_println("owner: %s", gurl.owner);
	gears_println("reponame: %s", gurl.reponame);

	gears_tag();
	return 0;
}
