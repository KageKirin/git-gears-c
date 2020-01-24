#include "gears_option.h"
#include "gears_util.h"
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
					options[i].consume(&argc, &argv);
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
					options[i].consume(&argc, &argv);
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
				options[i].consume(&argc, &argv);
			}
		}
	}
	return argc;
}
