#include "gears_giturl.h"
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
	{.description = "remote to look up and parse", &OptionValues.remote, gears_setOptionPositionalString},
};

int ParseRemote(int argc, char** argv)
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

	if (gr.url[0])
	{
		GitUrl gurl = gears_parseUrl(gr.url);

		// result display
		gears_println("host: %s", gurl.host);
		gears_println("path: %s", gurl.path);
		gears_println("owner: %s", gurl.owner);
		gears_println("reponame: %s", gurl.reponame);
	}
	else
	{
		gears_println("the remote '%s' is not valid", OptionValues.remote);
	}

	gears_tag();
	return 0;
}
