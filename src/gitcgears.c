#include <stdio.h>
#include <curl/curl.h>
#include <git2.h>

#include "gears_util.h"
#include "gears_action.h"

int Help(int argc, char** argv);
int Foobar(int argc, char** argv);

static Action actions[] = {
	{ "help", "prints this message", &Help }, //must be top
	{ "foobar", "foos the bar", &Foobar },
};

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		gears_println("argc: %i, argv[1]: %s", argc, argv[1]);
		for(int i = 0; i < ARRAY_COUNT(actions); ++i)
		{
			if (STRING_EQUALS(actions[i].verb, argv[1]))
			{
				return actions[i].call(argc-1, argv+1);
			}
		}

		gears_println("No such action '%s'", argv[1]);
		Help(argc, argv);
		return 1;
	}

	return Help(argc, argv);
}


int Help(int argc, char** argv)
{
	static char* argv_help[] = { "--help" };
	if (argc > 1)
	{
		for(int i = 1; i < ARRAY_COUNT(actions); ++i) //skip help
		{
			if (STRING_EQUALS(actions[i].verb, argv[1]))
			{
				return actions[i].call(1, argv_help);
			}
		}
		gears_println("No such action '%s'", argv[1]);
	}

	gears_println("generic help", NULL);
	for(int i = 0; i < ARRAY_COUNT(actions); ++i)
	{
		gears_println("%s\t%s", actions[i].verb, actions[i].desc);
	}

	return 0;
}

//--

int Foobar(int argc, char** argv)
{
	gears_println("foobar doing stuff", NULL);
	//getopt or my own implementation to parse arguments
	//implementation of action
	//result display
	return 0;
}
