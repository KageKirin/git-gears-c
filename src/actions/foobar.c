#include <assert.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <git2.h>
#include <unistd.h>


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
	int explicit_value;
	int implicit_value;
	char* string_value;
	char* argument1;
	char* argument2;
} OptionValues = {
	.explicit_value = 0,
	.implicit_value = 0,
	.string_value = NULL,
	.argument1 = "origin",
	.argument2 = "master",
};

static Option options[] = {
	{ 'e', "explicit", "explicitly set value", ^ int(int* argc, char*** argv){ OptionValues.explicit_value = atoi(*argv[1]); *argc -= 2; *argv+=2; return 2;}},
	{ 'i', "implicit", "implicitly set value", ^ int(int* argc, char*** argv){ OptionValues.implicit_value = 1; *argc -= 1; *argv+=1; return 1;}},
	{ 's', "string", "string value", ^ int(int* argc, char*** argv){ OptionValues.string_value = *argv[1]; *argc -= 2; *argv+=2; return 2;}},
	{ .description = "argument value 1", ^ int(int* argc, char*** argv){ OptionValues.argument1 = *argv[0]; *argc-=1; *argv+=1; return 1;}},
	{ .description = "argument value 2", ^ int(int* argc, char*** argv){ OptionValues.argument2 = *argv[0]; *argc-=1; *argv+=1; return 1;}},
};


int Foobar(int argc, char** argv)
{
	gears_println("foobar doing stuff", NULL);

	gears_println("explicit_value: %i", OptionValues.explicit_value);
	gears_println("implicit_value: %i", OptionValues.implicit_value);
	gears_println("string_value: %s", OptionValues.string_value);
	gears_println("argument1: %s", OptionValues.argument1);
	gears_println("argument2: %s", OptionValues.argument2);

	// parse arguments
	int newargc = parse_options(options, 5, argc-1, argv+1);

	gears_println("explicit_value: %i", OptionValues.explicit_value);
	gears_println("implicit_value: %i", OptionValues.implicit_value);
	gears_println("string_value: %s", OptionValues.string_value);
	gears_println("argument1: %s", OptionValues.argument1);
	gears_println("argument2: %s", OptionValues.argument2);

	// implementation of action
	// result display

	// config

	git_config* config = NULL;

	git_repository* repo = NULL;
	if (git_repository_open_ext(&repo, getcwd(scrape, sizeof(scrape)), GIT_REPOSITORY_OPEN_CROSS_FS, NULL) == 0)
	{
		int err = git_repository_config(&config, repo);
		assert(err == 0);
		git_repository_free(repo);
	}
	else
	{
		int err = git_config_open_default(&config);
		assert(err == 0);
	}


	if (config)
	{
		git_buf strval = GIT_BUF_INIT;
		if (git_config_get_string_buf(&strval, config, "user.name") == 0)
		{
			gears_println("'user.name': %s", strval.ptr);
		}
		if (git_config_get_string_buf(&strval, config, "user.email") == 0)
		{
			gears_println("'user.email': %s", strval.ptr);
		}
		if (git_config_get_string_buf(&strval, config, "gears.github.com.token") == 0)
		{
			gears_println("'token': %s", strval.ptr);
		}
		if (git_config_get_string_buf(&strval, config, "foobar.hoge") == 0)
		{
			gears_println("'foobar.hoge': %s", strval.ptr);
		}
		git_config_free(config);
	}
	else
	{
		gears_errln("no config", NULL);
	}

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
