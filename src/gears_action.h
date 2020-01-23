#ifndef GITCGEARS_ACTION_H_INC
#define GITCGEARS_ACTION_H_INC

/// structures for 'action' parsing

//! callback function for action
//! like main()
typedef int(*ActionFuncPtr)(int, char**); //int argc, char** argv

//! structure for action
//! a 'named' callback
typedef struct Action
{
	const char* verb;   //< verb to call action witrh
	const char* desc;   //< description for help text
	ActionFuncPtr call; //< callback
} Action;

#endif  //GITCGEARS_ACTION_H_INC
