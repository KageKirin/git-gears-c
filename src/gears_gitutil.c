#include "gears_gitutil.h"
#include "gears_giturl.h"
#include "gears_util.h"

#include <assert.h>
#include <fcntl.h>
#include <git2.h>
#include <unistd.h>


GitRemote gears_lookupRemote(const char* remotename)
{
	char scrape[4096] = {0};

	GitRemote gr = {0};

	git_remote* remote = NULL;
	git_repository* repo = NULL;

	if (git_repository_open_ext(&repo, getcwd(scrape, sizeof(scrape)), GIT_REPOSITORY_OPEN_CROSS_FS, NULL) == 0)
	{
		if (!remotename)
		{
			git_reference* ref;
			int err = git_repository_head(&ref, repo);
			if (err == 0)
			{
				const char* branchname;
				err = git_branch_name(&branchname, ref);
				gears_println("branch %s [%s] {%s}", branchname, git_reference_name(ref), git_reference_shorthand(ref));

				git_buf buf_remote = {};
				err = git_branch_upstream_remote(&buf_remote, repo, git_reference_name(ref));
				gears_println("upstream remote: %s", buf_remote.ptr);

				git_buf buf_upstream = {};
				err = git_branch_upstream_name(&buf_upstream, repo, git_reference_name(ref));
				gears_println("upstream branch: %s", buf_upstream.ptr);

				git_reference* upref;
				err = git_branch_upstream(&upref, ref);
				if (err == 0)
				{
					err = git_branch_name(&branchname, upref);
					gears_println("upstream branch %s [%s] {%s}", branchname, git_reference_name(upref), git_reference_shorthand(upref));

					git_buf buf = {0};
					git_branch_remote_name(&buf, repo, git_reference_name(upref));
					gears_println("remote: %s", buf.ptr);
					snprintf(&scrape[0], sizeof(scrape), "%s", buf.ptr);
					remotename = &scrape[0];
				}
				git_reference_free(upref);
			}
			git_reference_free(ref);
		}

		assert(remotename);
		int err = git_remote_lookup(&remote, repo, remotename);
		if (err == 0)
		{
			snprintf(gr.name, GEARS_GITREMOTE_MAX_LENGTH, "%s", git_remote_name(remote));
			snprintf(gr.url, GEARS_GITREMOTE_MAX_LENGTH, "%s", git_remote_url(remote));
			git_remote_free(remote);
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
