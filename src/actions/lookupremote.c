#include <assert.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <git2.h>
#include <rure.h>
#include <unistd.h>


#include "gears_giturl.h"
#include "gears_option.h"
#include "gears_util.h"

static char scrape[4096] = {0};

#define GIT_BUF_INIT                                                                                                   \
	{                                                                                                                  \
		git_buf__initbuf, 0, 0                                                                                         \
	}
extern char git_buf__initbuf[];

char git_buf__initbuf[1];

static struct
{
	char* remote;
} OptionValues = {
	.remote = NULL,
};

static Option options[] = {
	{.description = "remote to look up", &OptionValues.remote, gears_setOptionPositionalString},
};

int LookupRemote(int argc, char** argv)
{
	// parse arguments
	int newargc = parse_options(options, ARRAY_COUNT(options), argc, argv);
	gears_println("remote: %s", OptionValues.remote);

	// implementation of action

	gears_tag();
	return 0;
}
