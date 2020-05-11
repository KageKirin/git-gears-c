#include <assert.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <git2.h>
#include <unistd.h>


#include "gears_gitutil.h"
#include "gears_option.h"
#include "gears_util.h"
#include "gears_constants.h"

static char scrape[4096] = {0};

#define GIT_BUF_INIT                                                                                                   \
	{                                                                                                                  \
		git_buf__initbuf, 0, 0                                                                                         \
	}
extern char git_buf__initbuf[];

char git_buf__initbuf[1];

static struct
{
	int int_value;
} OptionValues = {
	.int_value = 0,
};

static Option options[] = {
	{'e', "explicit", "explicitly set int value", &OptionValues.int_value, gears_setOptionExplicitInt},
};


int Foobar(int argc, char** argv)
{
	gears_println("foobar doing stuff", NULL);

	// parse arguments
	int newargc = parse_options(options, ARRAY_COUNT(options), argc, argv);

	gears_println("int_value: %i", OptionValues.int_value);

	// implementation of action
	// result display

	// config
	GitConfigEntry user_name = gears_getConfigEntry("user.name");
	gears_println("'user.name': %s", user_name.value);
	GitConfigEntry user_email = gears_getConfigEntry("user.email");
	gears_println("'user.email': %s", user_email.value);
	GitConfigEntry token = gears_getConfigEntry("gears.github.com.token");
	gears_println("'token': %s", token.value);
	GitConfigEntry foobar_hoge = gears_getConfigEntry("foobar.hoge");
	gears_println("'foobar.hoge': %s", foobar_hoge.value);
	GitConfigEntry foobar_fuba = gears_getConfigEntry("foobar.fuba");
	gears_println("'foobar.fuba': %s", foobar_fuba.value);

	GitConfigEntry hostname_token = gears_getGearsConfigEntry("github.com", kConfigAPIToken);
	gears_println("'hostname_token': %s", hostname_token.value);
	GitConfigEntry url_token =
		gears_getGearsConfigEntryRemoteOrURL("https://github.com/KageKirin/git-gears-c.git", kConfigAPIToken);
	gears_println("'url_token': %s", url_token.value);
	GitConfigEntry origin_token = gears_getGearsConfigEntryRemoteOrURL("origin", kConfigAPIToken);
	gears_println("'origin_token': %s", origin_token.value);

	return 0;
}

#if 0 // scrape
int	git_config_find_global	git_buf * out
int	git_config_find_programdata	git_buf * out
int	git_config_find_system	git_buf * out
int	git_config_find_xdg

git_repository_config

git_config_open_default
git_config_get_string
git_repository_discover
#endif
