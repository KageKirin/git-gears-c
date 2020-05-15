#include "gears_gitutil.h"
#include "gears_giturl.h"
#include "gears_util.h"

#include <assert.h>
#include <fcntl.h>
#include <git2.h>
#include <unistd.h>


GitBranch gears_getCurrentBranch()
{
	char scrape[4096] = {};
	GitBranch gb = {};

	git_repository* repo = NULL;

	int err = git_repository_open_ext(&repo, getcwd(scrape, sizeof(scrape)), GIT_REPOSITORY_OPEN_CROSS_FS, NULL);
	if (err == 0)
	{
		git_reference* ref;
		err = git_repository_head(&ref, repo);
		if (err == 0)
		{
			snprintf(gb.name, GEARS_GITREMOTE_MAX_LENGTH, "%s", git_reference_shorthand(ref));
			snprintf(gb.ref,  GEARS_GITREMOTE_MAX_LENGTH, "%s", git_reference_name(ref));
			git_reference_free(ref);
		}
		git_repository_free(repo);
	}

	return gb;
}


GitBranch gears_getCurrentUpstreamBranch()
{
	char scrape[4096] = {};
	GitBranch gb = {};

	git_repository* repo = NULL;
	int err = git_repository_open_ext(&repo, getcwd(scrape, sizeof(scrape)), GIT_REPOSITORY_OPEN_CROSS_FS, NULL);
	if (err == 0)
	{
		git_reference* ref;
		err = git_repository_head(&ref, repo);
		if (err == 0)
		{
			git_reference* upref;
			err = git_branch_upstream(&upref, ref);
			if (err == 0)
			{
				snprintf(gb.name, GEARS_GITREMOTE_MAX_LENGTH, "%s", git_reference_shorthand(upref));
				snprintf(gb.ref,  GEARS_GITREMOTE_MAX_LENGTH, "%s", git_reference_name(upref));
				git_reference_free(upref);
			}
			git_reference_free(ref);
		}
		git_repository_free(repo);
	}

	return gb;
}


GitRemote gears_lookupRemote(const char* remotename)
{
	char scrape[4096] = {};
	GitRemote gr = {};

	git_repository* repo = NULL;
	int err = git_repository_open_ext(&repo, getcwd(scrape, sizeof(scrape)), GIT_REPOSITORY_OPEN_CROSS_FS, NULL);
	if (err == 0)
	{
		if (!remotename)
		{
			git_reference* ref;
			err = git_repository_head(&ref, repo);
			if (err == 0)
			{
				git_buf buf = {};
				err = git_branch_upstream_remote(&buf, repo, git_reference_name(ref));
				if (err == 0)
				{
					gears_println("upstream remote: %s", buf.ptr);
					snprintf(&scrape[0], sizeof(scrape), "%s", buf.ptr);
					remotename = &scrape[0];
					git_buf_dispose(&buf);
				}
				git_reference_free(ref);
			}
		}

		if (!remotename)
		{
			git_remote* remote = NULL;
			err = git_remote_lookup(&remote, repo, remotename);
			if (err == 0)
			{
				snprintf(gr.name, GEARS_GITREMOTE_MAX_LENGTH, "%s", git_remote_name(remote));
				snprintf(gr.url, GEARS_GITREMOTE_MAX_LENGTH, "%s", git_remote_url(remote));
				git_remote_free(remote);
			}
		}
		git_repository_free(repo);
	}

	return gr;
}


GitConfigEntry gears_getConfigEntry(const char* name)
{
	assert(name);
	char scrape[4096] = {};
	GitConfigEntry gce = {};

	git_repository* repo = NULL;
	int err = git_repository_open_ext(&repo, getcwd(scrape, sizeof(scrape)), GIT_REPOSITORY_OPEN_CROSS_FS, NULL);
	if (err == 0)
	{
		git_config* config = NULL;
		err = git_repository_config(&config, repo);
		if (err == 0)
		{
			git_config_entry* entry;
			err = git_config_get_entry(&entry, config, name);
			if (err == 0)
			{
				snprintf(gce.name, GEARS_GITREMOTE_MAX_LENGTH, "%s", entry->name);
				snprintf(gce.value, GEARS_GITREMOTE_MAX_LENGTH, "%s", entry->value);
				git_config_entry_free(entry);
			}
			git_config_free(config);
		}
		git_repository_free(repo);
	}
	else
	{
		git_config* config = NULL;
		err = git_config_open_default(&config);
		if (err == 0)
		{
			git_config_entry* entry;
			err = git_config_get_entry(&entry, config, name);
			if (err == 0)
			{
				snprintf(gce.name, GEARS_GITREMOTE_MAX_LENGTH, "%s", entry->name);
				snprintf(gce.value, GEARS_GITREMOTE_MAX_LENGTH, "%s", entry->value);
				git_config_entry_free(entry);
			}
			git_config_free(config);
		}
	}

	return gce;
}


GitConfigEntry gears_getGearsConfigEntry(const char* hostname, const char* subname)
{
	assert(hostname);
	assert(subname);

	char buffer[GEARS_GITREMOTE_MAX_LENGTH] = {};
	snprintf(buffer, GEARS_GITREMOTE_MAX_LENGTH, "gears.%s.%s", hostname, subname);
	return gears_getConfigEntry(buffer);
}


GitConfigEntry gears_getGearsConfigEntryRemoteOrURL(const char* remoteOrUrl, const char* subname)
{
	assert(remoteOrUrl);
	assert(subname);

	GitRemote gr = gears_lookupRemote(remoteOrUrl);
	if (gr.url[0])
	{
		remoteOrUrl = gr.url;
	}

	GitUrl gurl = gears_parseUrl(remoteOrUrl);

	return gears_getGearsConfigEntry(gurl.host, subname);
}
