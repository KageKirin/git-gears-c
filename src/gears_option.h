#ifndef GITCGEARS_OPTION_H_INC
#define GITCGEARS_OPTION_H_INC

/// structures for 'option' parsing

//! callback function for option
//! 'consumes' following arguments
//! modifies values pointed to by argc, argv passed into it
//! returns number of arguments consumed

typedef int (*OptionFuncPtr)(void* value, int*, char***); // int* argc, char*** argv
typedef struct OptionConsumer
{
	OptionFuncPtr function;
} OptionConsumer;
extern OptionConsumer gears_option_Explicit;
extern OptionConsumer gears_option_Implicit;
extern OptionConsumer gears_option_ExplicitString;
extern OptionConsumer gears_option_Positional;
extern OptionConsumer gears_option_PositionalString;

//! structure for option
//! a 'named' callback
typedef struct Option
{
	const char shortname;	 //< single char short option. e.g. '-o'
	const char* longname;	 //< multi-char long option. e.g. '--option'
	const char* description; //< description for help text
	void* const value;		 //< pointer to value
	OptionConsumer consume;	 //< callback to consume arguments coming after
} Option;

int parse_options(const Option* options, unsigned options_count, int argc, char** argv);

#endif // GITCGEARS_OPTION_H_INC
