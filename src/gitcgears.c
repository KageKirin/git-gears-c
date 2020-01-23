#include <stdio.h>
#include <curl/curl.h>
#include <git2.h>

#include "gears_util.h"
#include "gears_action.h"

int Help(int argc, char** argv);
int CreateGist(int argc, char** argv);
int CreateIssue(int argc, char** argv);
int CreateProject(int argc, char** argv);
int CreatePullRequest(int argc, char** argv);
int GetGist(int argc, char** argv);
int GetIssue(int argc, char** argv);
int GetProject(int argc, char** argv);
int GetPullRequest(int argc, char** argv);
int ListGists(int argc, char** argv);
int ListIssues(int argc, char** argv);
int ListProjects(int argc, char** argv);
int ListPullRequests(int argc, char** argv);
int Foobar(int argc, char** argv);


static Action actions[] = {
	{ "help",               "prints this message",                                 &Help }, //must be top
	{ "foobar",             "foos the bar",                                        &Foobar },

	{ "create-gist",        "creates a new gist",                                  &CreateGist },
	{ "create-project",     "creates a new project",                               &CreateProject },
	{ "create-issue",       "creates a new issue",                                 &CreateIssue },
	{ "create-pullrequest", "creates a new pullrequest",                           &CreatePullRequest },
	{ "get-gist",           "retrieves the gist given its ID",                     &GetGist },
	{ "get-project",        "retrieves the project info given its remote",         &GetProject },
	{ "get-issue",          "retrieves the issue given its number",                &GetIssue },
	{ "get-pullrequest",    "retrieves the pull request given its number",         &GetPullRequest },
	{ "list-gists",         "lists up the gists available to the user",            &ListGists },
	{ "list-projects",      "lists up the projects available to the user",         &ListProjects },
	{ "list-issues",        "lists up the issues for this project/remote",         &ListIssues },
	{ "list-pullrequests",  "lists up the pull requests for this project/remote",  &ListPullRequests },
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
		gears_println("%-30s\t%s", actions[i].verb, actions[i].desc);
	}

	return 0;
}
