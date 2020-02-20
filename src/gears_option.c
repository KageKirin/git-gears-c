#include "gears_option.h"
#include "gears_util.h"
#include <stdlib.h>
#include <string.h>

extern char* program_name;
char* action_name;

static int showHelp(const Option* options, unsigned options_count)
{
	// header
	gears_println("Help for %s %s", program_name, action_name);

	// call help
	gears_println("call:", NULL);
	gears_printf("%s %s", program_name, action_name);
	for (unsigned i = 0; i < options_count; ++i)
	{
		const char* val = options[i].consume == &gears_setOptionImplicit ? "" : " <value>";
		if (options[i].shortname && options[i].longname)
		{
			gears_printf(" [-%c|--%s%s]", options[i].shortname, options[i].longname, val);
		}
		else if (options[i].shortname)
		{
			gears_printf(" [-%c%s]", options[i].shortname, val);
		}
		else if (options[i].longname)
		{
			gears_printf(" [--%s%s]", options[i].longname, val);
		}
		else
		{
			gears_printf(" <%s>", options[i].description);
		}
	}
	gears_printf("\n\n", NULL);


	// detail help
	gears_println("details:", NULL);
	gears_println("  -%c    --%-20s\t%s", 'h', "help", "show this message");
	int positional_args = 0;
	for (unsigned i = 0; i < options_count; ++i)
	{
		if (options[i].shortname && options[i].longname)
		{
			gears_println("  -%c    --%-20s\t%s", options[i].shortname, options[i].longname, options[i].description);
		}
		else if (options[i].shortname)
		{
			gears_println("  -%c      %-20s\t%s", options[i].shortname, "", options[i].description);
		}
		else if (options[i].longname)
		{
			gears_println("        --%-20s\t%s", options[i].longname, options[i].description);
		}
		else
		{
			gears_println("positional %i%-16s\t%s", ++positional_args, "", options[i].description);
		}
	}
	return 0;
}

int parse_options(const Option* options, unsigned options_count, int argc, char** argv)
{
	// set action name
	action_name = argv[0];
	argc -= 1;
	argv += 1;

	// parse actual options (-, --) until '--' encountered or
	while (argc > 0)
	{
		size_t len = strlen(argv[0]);

		// special case for -h/--help
		if ((len == 2 && argv[0][0] == '-' && argv[0][1] == 'h') ||
			(len == 6 && STRING_EQUALS("--help", &argv[0][0])))
		{
			gears_tag();
			showHelp(options, options_count);

			argc -= 1;
			argv += 1;
			break;
		}
		// single char arg
		else if (len == 2 && argv[0][0] == '-' && argv[0][1] != '-')
		{
			char arg = argv[0][1];
			for (unsigned i = 0; i < options_count; ++i)
			{
				if (options[i].shortname == arg)
				{
					options[i].consume(options[i].value, &argc, &argv);
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
				if (options[i].longname && STRING_EQUALS(options[i].longname, arg))
				{
					options[i].consume(options[i].value, &argc, &argv);
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
				options[i].consume(options[i].value, &argc, &argv);
			}
		}
	}
	return argc;
}


int gears_setOptionImplicit(void* value, int* argc, char*** argv)
{
	gears_tag();
	*(int*)value = 1;
	*argc -= 1;
	*argv += 1;
	return 1;
}

int gears_setOptionExplicitInt(void* value, int* argc, char*** argv)
{
	gears_tag();
	*(int*)value = atoi((*argv)[1]);
	*argc -= 2;
	*argv += 2;
	return 2;
	gears_tag();
}

int gears_setOptionExplicitFloat(void* value, int* argc, char*** argv)
{
	gears_tag();
	*(float*)value = atof((*argv)[1]);
	*argc -= 2;
	*argv += 2;
	return 2;
	gears_tag();
}

int gears_setOptionExplicitString(void* value, int* argc, char*** argv)
{
	gears_tag();
	*(char**)value = (*argv)[1];
	*argc -= 2;
	*argv += 2;
	return 2;
}

int gears_setOptionPositionalInt(void* value, int* argc, char*** argv)
{
	gears_tag();
	*(int*)value = atoi((*argv)[0]);
	*argc -= 1;
	*argv += 1;
	return 1;
}

int gears_setOptionPositionalFloat(void* value, int* argc, char*** argv)
{
	gears_tag();
	*(float*)value = atof((*argv)[0]);
	*argc -= 1;
	*argv += 1;
	return 1;
}

int gears_setOptionPositionalString(void* value, int* argc, char*** argv)
{
	gears_tag();
	*(char**)value = (*argv)[0];
	*argc -= 1;
	*argv += 1;
	return 1;
}
