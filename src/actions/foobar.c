#include <assert.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <git2.h>
#include <unistd.h>


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
	int int_value;
	int implicit_value;
	float float_value;
	char* string_value;
	int short_option;
	int long_option;
	char* argument1;
	char* argument2;
	int argument3;
	float argument4;
} OptionValues = {
	.int_value = 0,
	.implicit_value = 0,
	.float_value = 0.0f,
	.string_value = NULL,
	.short_option = 0,
	.long_option = 0,
	.argument1 = "origin",
	.argument2 = "master",
	.argument3 = 0,
	.argument4 = 0.0f,
};

static Option options[] = {
	{'e', "explicit", "explicitly set int value", &OptionValues.int_value, gears_setOptionExplicitInt},
	{'i', "implicit", "implicitly set int value", &OptionValues.implicit_value, gears_setOptionImplicit},
	{'f', "float", "explicitely set float value", &OptionValues.float_value, gears_setOptionExplicitFloat},
	{'s', "string", "explicitely set string value", &OptionValues.string_value, gears_setOptionExplicitString},
	{'x', 0, "short option only int", &OptionValues.short_option, gears_setOptionExplicitInt},
	{0, "longopt", "long option only int", &OptionValues.long_option, gears_setOptionExplicitInt},
	{.description = "argument value 1 (string)", &OptionValues.argument1, gears_setOptionPositionalString},
	{.description = "argument value 2 (string)", &OptionValues.argument2, gears_setOptionPositionalString},
	{.description = "argument value 3 (int)", &OptionValues.argument3, gears_setOptionPositionalInt},
	{.description = "argument value 4 (float)", &OptionValues.argument4, gears_setOptionPositionalFloat},
};


int Foobar(int argc, char** argv)
{
	gears_println("foobar doing stuff", NULL);

	gears_println("int_value: %i", OptionValues.int_value);
	gears_println("implicit_value: %i", OptionValues.implicit_value);
	gears_println("float_value: %f", OptionValues.float_value);
	gears_println("string_value: %s", OptionValues.string_value);
	gears_println("short_option: %i", OptionValues.short_option);
	gears_println("long_option: %i", OptionValues.long_option);
	gears_println("argument1: %s", OptionValues.argument1);
	gears_println("argument2: %s", OptionValues.argument2);
	gears_println("argument3: %i", OptionValues.argument3);
	gears_println("argument4: %f", OptionValues.argument4);

	// parse arguments
	int newargc = parse_options(options, ARRAY_COUNT(options), argc, argv);

	gears_println("int_value: %i", OptionValues.int_value);
	gears_println("implicit_value: %i", OptionValues.implicit_value);
	gears_println("float_value: %f", OptionValues.float_value);
	gears_println("string_value: %s", OptionValues.string_value);
	gears_println("short_option: %i", OptionValues.short_option);
	gears_println("long_option: %i", OptionValues.long_option);
	gears_println("argument1: %s", OptionValues.argument1);
	gears_println("argument2: %s", OptionValues.argument2);
	gears_println("argument3: %i", OptionValues.argument3);
	gears_println("argument4: %f", OptionValues.argument4);

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
