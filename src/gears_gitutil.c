#include "gears_gitutil.h"
#include "gears_util.h"

#include <fcntl.h>
#include <git2.h>
#include <unistd.h>
#include <assert.h>



GitRemote gears_lookupRemote(const char* remotename)
{
	assert(remotename);
	char scrape[4096] = {0};

	GitRemote gr = {0};

	git_remote* remote = NULL;
	git_repository* repo = NULL;

	if (git_repository_open_ext(&repo, getcwd(scrape, sizeof(scrape)), GIT_REPOSITORY_OPEN_CROSS_FS, NULL) == 0)
	{
		int err = git_remote_lookup(&remote, repo, remotename);
		if (err == 0)
		{
			snprintf(gr.name, GEARS_GITREMOTE_MAX_LENGTH, "%s", git_remote_name(remote));
			snprintf(gr.url, GEARS_GITREMOTE_MAX_LENGTH,  "%s", git_remote_url(remote));
			git_remote_free(remote);
		}
		git_repository_free(repo);
	}

	return gr;
}

GitConfigEntry gears_getConfigEntry(const char* name)
{
	assert(name);
	char scrape[4096] = {0};

	GitConfigEntry gce = {0};


	git_config* config = NULL;
	git_repository* repo = NULL;
	if (git_repository_open_ext(&repo, getcwd(scrape, sizeof(scrape)), GIT_REPOSITORY_OPEN_CROSS_FS, NULL) == 0)
	{
		int err = git_repository_config(&config, repo);
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
		int err = git_config_open_default(&config);
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
