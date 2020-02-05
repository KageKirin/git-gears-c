#ifndef GITCGEARS_OPTION_H_INC
#define GITCGEARS_OPTION_H_INC

/// structures for 'option' parsing

//! callback function for option
//! 'consumes' following arguments
//! modifies values pointed to by argc, argv passed into it
//! returns number of arguments consumed
typedef int (*OptionFuncPtr)(void* value, int*, char***); // int* argc, char*** argv

int gears_setOptionExplicit(void* value, int* argc, char*** argv);
int gears_setOptionImplicit(void* value, int* argc, char*** argv);
int gears_setOptionExplicitS(void* value, int* argc, char*** argv);
int gears_setOptionPositionalS(void* value, int* argc, char*** argv);

//! structure for option
//! a 'named' callback
typedef struct Option
{
	const char shortname;	 //< single char short option. e.g. '-o'
	const char* longname;	 //< multi-char long option. e.g. '--option'
	const char* description; //< description for help text
	void* const value;		 //< pointer to value
	OptionFuncPtr consume;	 //< callback to consume arguments coming after
} Option;

int parse_options(const Option* options, unsigned options_count, int argc, char** argv);

#endif // GITCGEARS_OPTION_H_INC
