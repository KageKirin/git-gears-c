#include "gears_giturl.h"
#include "gears_util.h"

#include <rure.h>

#include <assert.h>

const char * gears_giturlpatterns[] = {
	// generic patterns _adapted_ from git-url-parse (python)
	"^(?P<protocol>https?|git|ssh|rsync)://"  //
	"(?:(?P<user>.+)@)*" 				  //
	"(?P<hostname>[a-z0-9_.-]*)" 		  //
	"[:/]*" 							  //
	"(?P<port>[\\d]+){0,1}" 				  //
	"(?P<path>/((?P<owner>[\\w\\-]+)/)?" 	  //
	"((?P<reponame>[\\w\\-\\.]+?)(\\.git|/)?)?)$",

	"(git\\+)?" 						   //
	"((?P<protocol>\\w+)://)" 	   //
	"((?P<user>\\w+)@)?" 			   //
	"((?P<hostname>[\\w\\.\\-]+))" 	   //
	"(:(?P<port>\\d+))?" 			   //
	"(?P<path>(/(?P<owner>\\w+)/)?"  //
	"(/?(?P<reponame>[\\w\\-]+)(\\.git|/)?)?)$",

	"(?:(?P<user>.+)@)*"				 //
	"(?P<hostname>[a-z0-9_.-]*)" //
	"[:]*(?P<port>[\\d]+){0,1}"			 //
	"(?P<path>/?(?P<owner>.+)/(?P<reponame>.+)\\.git)$",

	"((?P<user>\\w+)@)?"				 //
	"((?P<hostname>[\\w\\.\\-]+))"	 //
	"[:/]{1,2}"				 //
	"(?P<path>((?P<owner>\\w+)/)?" //
	"((?P<reponame>[\\w\\-]+)(\\.git|/)?)?)$",

	// service specific patterns adapted from giturlparse
	// base
"(?P<user>.+)s@(?P<hostname>.+)s:(?P<reponame>.+)s\\.git",
"(http://(?P<hostname>.+)s/(?P<reponame>.+)s\\.git)",
"(http://(?P<hostname>.+)s/(?P<reponame>.+)s\\.git)",
"^(git://(?P<hostname>.+)s/(?P<reponame>.+)s\\.git)$",

// assembla
"^(git@(?P<hostname>.+):(?P<reponame>.+)\\.git)$",
"^(git://(?P<hostname>.+)/(?P<reponame>.+)\\.git)$",

// bitbucket
"^(https://(?P<user>.+)@(?P<hostname>.+)/(?P<owner>.+)/(?P<reponame>.+)\\.git)$",
"^(git@(?P<hostname>.+):(?P<owner>.+)/(?P<reponame>.+)\\.git)$",

// friendcode
"^(https://(?P<hostname>.+)/(?P<owner>.+)@user/(?P<reponame>.+)\\.git)$",

// github
"^(https://(?P<hostname>.+)/(?P<owner>.+)/(?P<reponame>.+)\\.git)$",
"^(git@(?P<hostname>.+):(?P<owner>.+)/(?P<reponame>.+)\\.git)$",
"^(git://(?P<hostname>.+)/(?P<owner>.+)/(?P<reponame>.+)\\.git)$",

// gitlab
"^(https://(?P<hostname>.+)/(?P<owner>.+)/(?P<reponame>.+)\\.git)$",
"^(git@(?P<hostname>.+):(?P<owner>.+)/(?P<reponame>.+)\\.git)$",
"^(git://(?P<hostname>.+)/(?P<owner>.+)/(?P<reponame>.+)\\.git)$",
};



static int check_regexes2(const char* url)
{
	gears_tag();

	static size_t patternSizes[ARRAY_COUNT(gears_giturlpatterns)] = {0};
	if (patternSizes[0] == 0)
	{
		for (int i = 0; i < ARRAY_COUNT(gears_giturlpatterns); ++i)
		{
			patternSizes[i] = strlen(gears_giturlpatterns[i]);
		}
	}


	rure_error* error = rure_error_new();
	rure_set* re = rure_compile_set((const uint8_t **)(gears_giturlpatterns),
						   patternSizes,
						   ARRAY_COUNT(gears_giturlpatterns),
						   RURE_FLAG_UNICODE | RURE_FLAG_CASEI, NULL, error);

	int rc = 0;
	if (!re)
	{
		gears_tag();
		gears_errln("compilation of expressions failed: %s", rure_error_message(error));
		rc = 1;
	}
	else if(url)
	{
		rc = rure_set_is_match(re, (const uint8_t*)(url), strlen(url), 0);
	}

	rure_set_free(re);
	rure_error_free(error);

	return rc;
}

static int check_regexes()
{
	bool raisedError = false;
	rure_error* error = rure_error_new();
	rure* regexes[ARRAY_COUNT(gears_giturlpatterns)] = {0};
	for(int i = 0; i < ARRAY_COUNT(gears_giturlpatterns); ++i)
	{
		regexes[i] = rure_compile((const uint8_t*)gears_giturlpatterns[i], strlen(gears_giturlpatterns[i]),
						RURE_FLAG_UNICODE | RURE_FLAG_CASEI, NULL, error);
		if (!regexes[i])
		{
			gears_tag();
			gears_errln("compilation of expression %i '%s' failed: %s", i, gears_giturlpatterns[i], rure_error_message(error));
			raisedError = true;
			break;
		}
	}
	rure_error_free(error);
	for(int i = 0; i < ARRAY_COUNT(gears_giturlpatterns); ++i)
	{
		rure_free(regexes[i]);
	}
	return (int)raisedError;
}

GitUrl gears_parseUrl(const char* url)
{
	gears_tag();
	assert(check_regexes2(NULL) == 0);
	assert(check_regexes() == 0);

	if(check_regexes2(url))
	{
		gears_tag();
	}

	GitUrl gurl = {0};

	for(int i = 0; i < ARRAY_COUNT(gears_giturlpatterns); ++i)
	{
		const char* pattern = i[gears_giturlpatterns];
		rure_error *err = rure_error_new();
		rure *re = rure_compile((const uint8_t*)pattern, strlen(pattern),
							RURE_FLAG_UNICODE | RURE_FLAG_CASEI, NULL, err);
		
		rure_error_free(err);
		rure_free(re);
	}


	return gurl;
}
