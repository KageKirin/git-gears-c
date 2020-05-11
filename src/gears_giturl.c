#include "gears_giturl.h"
#include "gears_util.h"

#include <rure.h>

#include <assert.h>

// generic patterns _adapted_ from git-url-parse (python)
const char* gears_giturlpatterns[] = {
	/// the most compatible regex
	"^(git\\+)?"																						 //
	"((?P<protocol>(https?)|(s?ftps?)|(git)|(ssh)|(rsync))(://))?"										 //
	"((?P<hostuser>([\\w\\d]+))@)?"																		 //
	"(?P<hostname>([a-z0-9_\\.\\-]+)|((\\d{1,3}\\.){4})|(\\[?(([\\d\\w]{0,4}:){1,7}[\\d\\w]{1,4})\\]?))" //
	"(:(?P<hostport>\\d+))?"																			 //
	"([/:]*((?P<hostpath>[\\~\\w\\d\\-_/]+)/)?(?P<owner>[\\w\\d\\-]+)/)"								 //
	"((?P<reponame>[\\w\\d\\-]+)(\\.git)?)"																 //
	"/?$",

	/// service specific patterns adapted from giturlparse
	/// - base
	"^(?P<hostuser>.+)s@(?P<hostname>.+)s:(?P<reponame>.+)s\\.git$",
	"^((?P<protocol>https?)://(?P<hostname>.+)s/(?P<reponame>.+)s\\.git)$",
	"^((?P<protocol>https?)://(?P<hostname>.+)s/(?P<reponame>.+)s\\.git)$",
	"^((?P<protocol>git)://(?P<hostname>.+)s/(?P<reponame>.+)s\\.git)$",

	/// - assembla
	"^((?P<hostuser>git)@(?P<hostname>.+):(?P<reponame>.+)\\.git)$",
	"^((?P<protocol>git)://(?P<hostname>.+)/(?P<reponame>.+)\\.git)$",

	/// - bitbucket
	"^((?P<protocol>https)://(?P<hostuser>.+)@(?P<hostname>.+)/(?P<owner>.+)/(?P<reponame>.+)\\.git)$",
	"^((?P<hostuser>git)@(?P<hostname>.+):(?P<owner>.+)/(?P<reponame>.+)\\.git)$",

	/// - friendcode
	"^((?P<protocol>https)://(?P<hostname>.+)/(?P<owner>.+)@user/(?P<reponame>.+)\\.git)$",

	/// - github
	"^((?P<protocol>https)://(?P<hostname>.+)/(?P<owner>.+)/(?P<reponame>.+)\\.git)$",
	"^((?P<hostuser>git)@(?P<hostname>.+):(?P<owner>.+)/(?P<reponame>.+)\\.git)$",
	"^((?P<protocol>git)://(?P<hostname>.+)/(?P<owner>.+)/(?P<reponame>.+)\\.git)$",

	/// - gitlab
	"^((?P<protocol>https)://(?P<hostname>.+)/(?P<owner>.+)/(?P<reponame>.+)\\.git)$",
	"^((?P<hostuser>git)@(?P<hostname>.+):(?P<owner>.+)/(?P<reponame>.+)\\.git)$",
	"^((?P<protocol>git)://(?P<hostname>.+)/(?P<owner>.+)/(?P<reponame>.+)\\.git)$",
};

static bool parseUrl(const char* url, const char* pattern, GitUrl* gurl);

static int check_regexes2(const char* url)
{

	static size_t patternSizes[ARRAY_COUNT(gears_giturlpatterns)] = {0};
	if (patternSizes[0] == 0)
	{
		for (int i = 0; i < ARRAY_COUNT(gears_giturlpatterns); ++i)
		{
			patternSizes[i] = strlen(gears_giturlpatterns[i]);
		}
	}


	rure_error* error = rure_error_new();
	rure_set* re =
		rure_compile_set((const uint8_t**)(gears_giturlpatterns), patternSizes, ARRAY_COUNT(gears_giturlpatterns),
						 RURE_FLAG_UNICODE | RURE_FLAG_CASEI, NULL, error);

	int rc = 0;
	if (!re)
	{
		gears_errln("compilation of expressions failed: %s", rure_error_message(error));
		rc = 1;
	}
	else if (url)
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
	for (int i = 0; i < ARRAY_COUNT(gears_giturlpatterns); ++i)
	{
		regexes[i] = rure_compile((const uint8_t*)gears_giturlpatterns[i], strlen(gears_giturlpatterns[i]),
								  RURE_FLAG_UNICODE | RURE_FLAG_CASEI, NULL, error);
		if (!regexes[i])
		{
			gears_errln("compilation of expression %i '%s' failed: %s", i, gears_giturlpatterns[i],
						rure_error_message(error));
			raisedError = true;
			break;
		}
	}
	rure_error_free(error);
	for (int i = 0; i < ARRAY_COUNT(gears_giturlpatterns); ++i)
	{
		rure_free(regexes[i]);
	}
	return (int)raisedError;
}


GitUrl gears_parseUrl(const char* url)
{
	assert(check_regexes2(NULL) == 0);
	assert(check_regexes() == 0);

	if (check_regexes2(url))
	{
	}

	GitUrl gurl = {0};

	for (int i = 0; i < ARRAY_COUNT(gears_giturlpatterns); ++i)
	{
		const char* pattern = i[gears_giturlpatterns];

		gears_dbgln("pattern %d", i);
		if (parseUrl(url, pattern, &gurl))
		{
			break;
		}
	}

	return gurl;
}


bool parseUrl(const char* url, const char* pattern, GitUrl* gurl)
{
	assert(url);
	assert(pattern);
	assert(gurl);

	rure_error* err = rure_error_new();
	rure* re = rure_compile((const uint8_t*)pattern, strlen(pattern), RURE_FLAG_UNICODE | RURE_FLAG_CASEI, NULL, err);
	if (!re)
	{
		gears_errln("compilation of %s failed: %s", pattern, rure_error_message(err));
		rure_error_free(err);
		return false;
	}
	rure_error_free(err);

	rure_iter_capture_names* capturenames = rure_iter_capture_names_new(re);

	rure_captures* caps = rure_captures_new(re);
	rure_iter* iter = rure_iter_new(re);

	size_t url_length = strlen(url);
	bool match = rure_iter_next_captures(iter, (const uint8_t*)url, url_length, caps);
	gears_dbgln("\"%s\" match: %s", pattern, match ? "true" : "false");
	if (match)
	{
		gears_dbgln("captured: %zu groups", rure_captures_len(caps));
		// rure_match groups[rure_captures_len(caps)]; // VLA
		// for (size_t i = 0; i < rure_captures_len(caps); ++i)
		//{
		//	rure_captures_at(caps, i, &groups[i]);
		//	gears_dbgln("group %zu [%zu, %zu]", i, groups[i].start, groups[i].end);
		//	if (groups[i].start < url_length && groups[i].end < url_length)
		//	{
		//		gears_dbgln("%.*s", (int)(groups[i].end - groups[i].start), url + groups[i].start);
		//	}
		//}

		int32_t idx = 0;
		idx = rure_capture_name_index(re, "protocol");
		if (idx > 0)
		{
			rure_match match;
			rure_captures_at(caps, idx, &match);
			if (match.start < url_length && match.end < url_length)
			{
				gears_dbgln("protocol: %.*s", (int)(match.end - match.start), url + match.start);
				snprintf(gurl->protocol, GEARS_GITURL_MAX_LENGTH, "%.*s", (int)(match.end - match.start),
						 url + match.start);
			}
		}

		idx = rure_capture_name_index(re, "hostuser");
		if (idx > 0)
		{
			rure_match match;
			rure_captures_at(caps, idx, &match);
			if (match.start < url_length && match.end < url_length)
			{
				gears_dbgln("user: %.*s", (int)(match.end - match.start), url + match.start);
				// snprintf(gurl->, GEARS_GITURL_MAX_LENGTH, "%.*s", (int)(match.end - match.start), url + match.start);
			}
		}

		idx = rure_capture_name_index(re, "hostname");
		if (idx > 0)
		{
			rure_match match;
			rure_captures_at(caps, idx, &match);
			if (match.start < url_length && match.end < url_length)
			{
				gears_dbgln("hostname: %.*s", (int)(match.end - match.start), url + match.start);
				snprintf(gurl->host, GEARS_GITURL_MAX_LENGTH, "%.*s", (int)(match.end - match.start),
						 url + match.start);
			}
		}

		idx = rure_capture_name_index(re, "hostport");
		if (idx > 0)
		{
			rure_match match;
			rure_captures_at(caps, idx, &match);
			if (match.start < url_length && match.end < url_length)
			{
				gears_dbgln("hostport: %.*s", (int)(match.end - match.start), url + match.start);
				// snprintf(gurl->, GEARS_GITURL_MAX_LENGTH, "%.*s", (int)(match.end - match.start), url + match.start);
			}
		}

		idx = rure_capture_name_index(re, "hostpath");
		if (idx > 0)
		{
			rure_match match;
			rure_captures_at(caps, idx, &match);
			if (match.start < url_length && match.end < url_length)
			{
				gears_dbgln("hostpath: %.*s", (int)(match.end - match.start), url + match.start);
				snprintf(gurl->path, GEARS_GITURL_MAX_LENGTH, "%.*s", (int)(match.end - match.start),
						 url + match.start);
			}
		}

		idx = rure_capture_name_index(re, "owner");
		if (idx > 0)
		{
			rure_match match;
			rure_captures_at(caps, idx, &match);
			if (match.start < url_length && match.end < url_length)
			{
				gears_dbgln("owner: %.*s", (int)(match.end - match.start), url + match.start);
				snprintf(gurl->owner, GEARS_GITURL_MAX_LENGTH, "%.*s", (int)(match.end - match.start),
						 url + match.start);
			}
		}

		idx = rure_capture_name_index(re, "reponame");
		if (idx > 0)
		{
			rure_match match;
			rure_captures_at(caps, idx, &match);
			if (match.start < url_length && match.end < url_length)
			{
				gears_dbgln("reponame: %.*s", (int)(match.end - match.start), url + match.start);
				snprintf(gurl->reponame, GEARS_GITURL_MAX_LENGTH, "%.*s", (int)(match.end - match.start),
						 url + match.start);
			}
		}
	}

	rure_iter_free(iter);
	rure_captures_free(caps);
	rure_iter_capture_names_free(capturenames);
	rure_free(re);

	return match;
}