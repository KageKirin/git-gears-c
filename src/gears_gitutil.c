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
			strncpy(gr.name, git_remote_name(remote), GEARS_GITREMOTE_MAX_LENGTH);
			strncpy(gr.url, git_remote_url(remote), GEARS_GITREMOTE_MAX_LENGTH);
			git_remote_free(remote);
		}
		git_repository_free(repo);
	}

	return gr;
}
