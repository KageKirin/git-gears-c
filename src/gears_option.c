#include "gears_option.h"
#include "gears_util.h"
#include <stdlib.h>
#include <string.h>

int parse_options(const Option* options, unsigned options_count, int argc, char** argv)
{
	// parse actual options (-, --) until '--' encountered or
	while (argc > 0)
	{
		size_t len = strlen(argv[0]);

		//!!! TODO: special case for -h/--help

		// single char arg
		if (len == 2 && argv[0][0] == '-' && argv[0][1] != '-')
		{
			char arg = argv[0][1];
			for (unsigned i = 0; i < options_count; ++i)
			{
				if (options[i].shortname == arg)
				{
					options[i].consume.function(options[i].value, &argc, &argv);
					break;
				}
			}
		}
		// multi-char arg
		else if (len > 2 && argv[0][0] == '-' && argv[0][1] == '-')
		{
			char* arg = &argv[0][2];
			for (unsigned i = 0; i < options_count; ++i)
			{
				if (STRING_EQUALS(options[i].longname, arg))
				{
					options[i].consume.function(options[i].value, &argc, &argv);
					break;
				}
			}
		}
		// check for 'end of options' -> '--'
		else if (len == 2 && argv[0][0] == '-' && argv[0][1] == '-')
		{
			argc--;
			argv++;
			break;
		}
		// natural 'end of options', i.e. next arg is not prefixed with '-|--
		else
		{
			break;
		}
	}

	// consume remaining arguments
	for (unsigned i = 0; i < options_count; ++i)
	{
		if (argc > 0)
		{
			if (options[i].shortname == 0 && options[i].longname == NULL)
			{
				options[i].consume.function(options[i].value, &argc, &argv);
			}
		}
	}
	return argc;
}


static int gears_setOptionExplicit(void* value, int* argc, char*** argv)
{
	gears_tag();
	*(int*)value = atoi((*argv)[1]);
	*argc -= 2;
	*argv += 2;
	return 2;
	gears_tag();
}
OptionConsumer gears_option_Explicit = {.function = gears_setOptionExplicit};


static int gears_setOptionImplicit(void* value, int* argc, char*** argv)
{
	gears_tag();
	*(int*)value = 1;
	*argc -= 1;
	*argv += 1;
	return 1;
}
OptionConsumer gears_option_Implicit = {.function = gears_setOptionImplicit};


static int gears_setOptionExplicitString(void* value, int* argc, char*** argv)
{
	gears_tag();
	*(char**)value = (*argv)[1];
	*argc -= 2;
	*argv += 2;
	return 2;
}
OptionConsumer gears_option_ExplicitString = {.function = gears_setOptionExplicitString};


static int gears_setOptionPositional(void* value, int* argc, char*** argv)
{
	gears_tag();
	*(int*)value = atoi((*argv)[0]);
	*argc -= 1;
	*argv += 1;
	return 1;
}
OptionConsumer gears_option_Positional = {.function = gears_setOptionPositional};


static int gears_setOptionPositionalString(void* value, int* argc, char*** argv)
{
	gears_tag();
	*(char**)value = (*argv)[0];
	*argc -= 1;
	*argv += 1;
	return 1;
}
OptionConsumer gears_option_PositionalString = {.function = gears_setOptionPositionalString};
