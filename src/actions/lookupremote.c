#include "gears_gitutil.h"
#include "gears_option.h"
#include "gears_util.h"

#include <assert.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <git2.h>
#include <unistd.h>


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
	if (parse_options(options, ARRAY_COUNT(options), argc, argv))
	{
		return 1;
	}
	gears_println("remote: %s", OptionValues.remote);

	// implementation of action
	GitRemote gr = gears_lookupRemote(OptionValues.remote);

	// result display
	gears_println("name: %s", gr.name);
	gears_println("url: %s", gr.url);

	gears_tag();
	return 0;
}
