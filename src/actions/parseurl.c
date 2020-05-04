#include <assert.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <git2.h>
#include <unistd.h>
#include <rure.h>


#include "gears_util.h"
#include "gears_giturl.h"
#include "gears_option.h"

static char scrape[4096] = {0};

#define GIT_BUF_INIT                                                                                                   \
	{                                                                                                                  \
		git_buf__initbuf, 0, 0                                                                                         \
	}
extern char git_buf__initbuf[];

char git_buf__initbuf[1];

static struct {
	char* url;
} OptionValues = {
	.url = NULL,
};

static Option options[] = {
	{ .description = "url to parse", &OptionValues.url, gears_setOptionPositionalString },
};

int parsetest(char*);

int ParseUrl(int argc, char** argv)
{
	// parse arguments
	int newargc = parse_options(options, ARRAY_COUNT(options), argc, argv);

	gears_println("url: %s", OptionValues.url);
	//parsetest(OptionValues.url);
	gears_tag();

	// implementation of action
	// result display

	// config

	static const char* pattern =
		"^((git\\+)?(?P<protocol>https?|git|ssh|rsync)://)?"
		"(?:(?P<user>.+)@)?"
		"(?P<hostname>[a-z0-9_.-]+)"
		"[:/]*"
		"(?P<port>[\\d]+){0,1}"
		"(?P<path>/((?P<owner>[\\w\\-]+)/)?"
		"((?P<reponame>[\\w\\-\\.]+?)(\\.git|/)?)?)$";

	rure_error *err = rure_error_new();
	rure *re = rure_compile((const uint8_t*)pattern, strlen(pattern),
							RURE_FLAG_UNICODE | RURE_FLAG_CASEI, NULL, err);
	if (!re)
	{
		gears_errln("compilation of %s failed: %s", pattern, rure_error_message(err));
		rure_error_free(err);
		return -1;
	}
	rure_error_free(err);

	gears_tag();
	rure_iter_capture_names* capturenames = rure_iter_capture_names_new(re);

	rure_captures* caps = rure_captures_new(re);
	rure_iter* iter = rure_iter_new(re);

	size_t OptionValues_url_length = strlen(OptionValues.url);
	bool match = rure_iter_next_captures(iter, (const uint8_t*)OptionValues.url, OptionValues_url_length, caps);
	gears_println("match: %i", match);
	if(match)
	{
		gears_println("captured: %zu", rure_captures_len(caps));
		rure_match groups[rure_captures_len(caps)]; //VLA
		for (size_t i = 0; i < rure_captures_len(caps); ++i)
		{
			rure_captures_at(caps, i, &groups[i]);
			gears_println("group %zu [%zu, %zu]", i, groups[i].start, groups[i].end);
			if (groups[i].start < OptionValues_url_length && groups[i].end < OptionValues_url_length)
			{
				gears_println("%.*s", (int)(groups[i].end - groups[i].start), OptionValues.url + groups[i].start);
			}
		}

		int32_t idx = 0;
		idx = rure_capture_name_index(re, "protocol");
		if (idx > 0)
		{
			rure_match match;
			rure_captures_at(caps, idx, &match);
			if (match.start < OptionValues_url_length && match.end < OptionValues_url_length)
			{
				gears_println("protocol: %.*s", (int)(match.end - match.start), OptionValues.url + match.start);
			}
		}

		idx = rure_capture_name_index(re, "user");
		if (idx > 0)
		{
			rure_match match;
			rure_captures_at(caps, idx, &match);
			if (match.start < OptionValues_url_length && match.end < OptionValues_url_length)
			{
				gears_println("user: %.*s", (int)(match.end - match.start), OptionValues.url + match.start);
			}
		}

		idx = rure_capture_name_index(re, "hostname");
		if (idx > 0)
		{
			rure_match match;
			rure_captures_at(caps, idx, &match);
			if (match.start < OptionValues_url_length && match.end < OptionValues_url_length)
			{
				gears_println("hostname: %.*s", (int)(match.end - match.start), OptionValues.url + match.start);
			}
		}

		idx = rure_capture_name_index(re, "port");
		if (idx > 0)
		{
			rure_match match;
			rure_captures_at(caps, idx, &match);
			if (match.start < OptionValues_url_length && match.end < OptionValues_url_length)
			{
				gears_println("port: %.*s", (int)(match.end - match.start), OptionValues.url + match.start);
			}
		}

		idx = rure_capture_name_index(re, "path");
		if (idx > 0)
		{
			rure_match match;
			rure_captures_at(caps, idx, &match);
			if (match.start < OptionValues_url_length && match.end < OptionValues_url_length)
			{
				gears_println("path: %.*s", (int)(match.end - match.start), OptionValues.url + match.start);
			}
		}

		idx = rure_capture_name_index(re, "owner");
		if (idx > 0)
		{
			rure_match match;
			rure_captures_at(caps, idx, &match);
			if (match.start < OptionValues_url_length && match.end < OptionValues_url_length)
			{
				gears_println("owner: %.*s", (int)(match.end - match.start), OptionValues.url + match.start);
			}
		}

		idx = rure_capture_name_index(re, "reponame");
		if (idx > 0)
		{
			rure_match match;
			rure_captures_at(caps, idx, &match);
			if (match.start < OptionValues_url_length && match.end < OptionValues_url_length)
			{
				gears_println("reponame: %.*s", (int)(match.end - match.start), OptionValues.url + match.start);
			}
		}
	}
	gears_tag();

	rure_iter_free(iter);
	rure_captures_free(caps);
	rure_iter_capture_names_free(capturenames);
	rure_free(re);


	gears_parseUrl(OptionValues.url);
	gears_tag();
	return 0;
}


/*

"^(https?|git|ssh|ftps?|rsync)://(?:(.+)@)*([a-z0-9_.-]*)[:/]*([\d]+){0,1}(/(([\w\-]+)/)?(([\w\-\.]+?)(\.git|/)?)?)$"

"^(https?|git|ssh|ftps?|rsync)://" \
"(?:(.+)@)*" \
"([a-z0-9_.-]*)" \
"[:/]*" \
"([\d]+){0,1}" \
"(/(([\w\-]+)/)?" \
"(([\w\-\.]+?)(\.git|/)?)?)$",
RegexOptions.Compiled | RegexOptions.IgnoreCase),

"(git\+)?" \
"((\w+)://)" \
"((\w+)@)?" \
"(([\w\.\-]+))" \
"(:(\d+))?" \
"((/(\w+)/)?" \
"(/?([\w\-]+)(\.git|/)?)?)$",
RegexOptions.Compiled | RegexOptions.IgnoreCase),

"^(?:(.+)@)*" \
"([a-z0-9_.-]*)[:]*" \
"([\d]+){0,1}" \
"(/?(.+)/(.+).git)$",
RegexOptions.Compiled | RegexOptions.IgnoreCase),

"((\w+)@)?" \
"(([\w\.\-]+))" \
"[\:/]{1,2}" \
"(((\w+)/)?" \
"(([\w\-]+)(\.git|/)?)?)$",
RegexOptions.Compiled | RegexOptions.IgnoreCase),

// service specific patterns adapted from giturlparse
// base

"(.+)s@(.+)s:(.+)s.git",
RegexOptions.Compiled | RegexOptions.IgnoreCase),

"(http://(.+)s/(.+)s.git)", RegexOptions.Compiled | RegexOptions.IgnoreCase),

"(http://(.+)s/(.+)s.git)", RegexOptions.Compiled | RegexOptions.IgnoreCase),

"^(git://(.+)s/(.+)s.git)$", RegexOptions.Compiled | RegexOptions.IgnoreCase),

// assembla

"^(git@(.+):(.+).git)$", RegexOptions.Compiled | RegexOptions.IgnoreCase),

"^(git://(.+)/(.+).git)$", RegexOptions.Compiled | RegexOptions.IgnoreCase),

// bitbucket

"^(https://(.+)@(.+)/(.+)/(.+).git)$",
RegexOptions.Compiled | RegexOptions.IgnoreCase),

"^(git@(.+):(.+)/(.+).git)$",
RegexOptions.Compiled | RegexOptions.IgnoreCase),

// friendcode

"^(https://(.+)/(.+)@user/(.+).git)$",
RegexOptions.Compiled | RegexOptions.IgnoreCase),

// github

"^(https://(.+)/(.+)/(.+).git)$",
RegexOptions.Compiled | RegexOptions.IgnoreCase),

"^(git@(.+):(.+)/(.+).git)$",
RegexOptions.Compiled | RegexOptions.IgnoreCase),

"^(git://(.+)/(.+)/(.+).git)$",
RegexOptions.Compiled | RegexOptions.IgnoreCase),

// gitlab

"^(https://(.+)/(.+)/(.+).git)$",
RegexOptions.Compiled | RegexOptions.IgnoreCase),

"^(git@(.+):(.+)/(.+).git)$",
RegexOptions.Compiled | RegexOptions.IgnoreCase),

"^(git://(.+)/(.+)/(.+).git)$",
RegexOptions.Compiled | RegexOptions.IgnoreCase),

*/