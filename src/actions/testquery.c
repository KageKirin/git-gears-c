#include "gears_gitutil.h"
#include "gears_giturl.h"
#include "gears_option.h"
#include "gears_util.h"
#include "gears_config.h"
#include "gears_constants.h"
#include "gears_request.h"

#include "requests/github_graphql.h"
#include "requests/github_graphqlutils.h"
#include "requests/gitlab_graphql.h"
#include "requests/gitlab_graphqlutils.h"

#include <assert.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <git2.h>
#include <unistd.h>


static struct
{
	char* remoteOrUrl;
} OptionValues = {
	.remoteOrUrl = NULL,
};

static Option options[] = {
	{.description = "remote or url to look up", &OptionValues.remoteOrUrl, gears_setOptionPositionalString},
};

int TestQuery(int argc, char** argv)
{
	// parse arguments
	if (parse_options(options, ARRAY_COUNT(options), argc, argv))
	{
		return 1;
	}
	gears_println("remote or url: %s", OptionValues.remoteOrUrl);

	// implementation of action
	GitUrl gurl = gears_parseRemoteOrUrl(OptionValues.remoteOrUrl);

	gears_println("test query for %s", OptionValues.remoteOrUrl);
	gears_println("host: %s", gurl.host);

	GitConfigEntry gce_type = gears_getGearsConfigEntry(gurl.host, kConfigAPIType);
	GitConfigEntry gce_grql = gears_getGearsConfigEntry(gurl.host, kConfigAPIEndpointGraphQL);
	GitConfigEntry gce_rest = gears_getGearsConfigEntry(gurl.host, kConfigAPIEndpointRest);
	GitConfigEntry gce_token = gears_getGearsConfigEntry(gurl.host, kConfigAPIToken);

	int validAPI = gears_checkConfig(gurl.host, kConfigAPIType);
	int validGQL = gears_checkConfig(gurl.host, kConfigAPIEndpointGraphQL);
	int validRST = gears_checkConfig(gurl.host, kConfigAPIEndpointRest);
	int validTOK = gears_checkConfig(gurl.host, kConfigAPIToken);

	// result display
	if (validAPI == 0)
	{
		gears_println("query: %s", g_github_query_viewer_graphql_data);

		// current user
		{
			char recvBuffer[65535] = {};
			int bufsize = snprintJsonRequest_GithubQueryViewerGraphql(NULL, 0);
			char buffer[bufsize+1];
			snprintJsonRequest_GithubQueryViewerGraphql(buffer, bufsize+1);
			gears_println("buffer: %s", buffer);
			int res = proto_postRequest(gce_grql.value, gce_token.value, buffer, recvBuffer, sizeof(recvBuffer));
			gears_println("query res: %i", res);
			gears_println("%s", recvBuffer);


			char* b2 = NULL;
			gears_println("ha?: %p | %p", b2, &b2);

			res = proto_postRequest2(gce_grql.value, gce_token.value, buffer, &b2);
			gears_println("query res: %i", res);
			gears_println("%s", b2);
			free(b2);
		}

		// owner (org or user)
		{
			char recvBuffer[65535] = {};
			GithubQueryOwnerGraphql qv = {
				.login = gurl.owner,
			};
			int bufsize = snprintJsonRequest_GithubQueryOwnerGraphql(NULL, 0, &qv);
			char buffer[bufsize+1];
			snprintJsonRequest_GithubQueryOwnerGraphql(buffer, bufsize+1, &qv);
			gears_println("buffer: %s", buffer);
			int res = proto_postRequest(gce_grql.value, gce_token.value, buffer, recvBuffer, sizeof(recvBuffer));
			gears_println("query res: %i", res);
			gears_println("%s", recvBuffer);
		}

		// owning user
		{
			char recvBuffer[65535] = {};
			GithubQueryUserGraphql qv = {
				.login = gurl.owner,
			};
			int bufsize = snprintJsonRequest_GithubQueryUserGraphql(NULL, 0, &qv);
			char buffer[bufsize+1];
			snprintJsonRequest_GithubQueryUserGraphql(buffer, bufsize+1, &qv);
			gears_println("buffer: %s", buffer);
			int res = proto_postRequest(gce_grql.value, gce_token.value, buffer, recvBuffer, sizeof(recvBuffer));
			gears_println("query res: %i", res);
			gears_println("%s", recvBuffer);
		}

		// owning organization
		{
			char recvBuffer[65535] = {};
			GithubQueryOrganizationGraphql qv = {
				.login = gurl.owner,
			};
			int bufsize = snprintJsonRequest_GithubQueryOrganizationGraphql(NULL, 0, &qv);
			char buffer[bufsize+1];
			snprintJsonRequest_GithubQueryOrganizationGraphql(buffer, bufsize+1, &qv);
			gears_println("buffer: %s", buffer);
			int res = proto_postRequest(gce_grql.value, gce_token.value, buffer, recvBuffer, sizeof(recvBuffer));
			gears_println("query res: %i", res);
			gears_println("%s", recvBuffer);
		}

		// repo info
		{
			char recvBuffer[65535] = {};
			GithubQueryRepositoryGraphql qv = {
				.owner = gurl.owner,
				.name  = gurl.reponame,
			};
			int bufsize = snprintJsonRequest_GithubQueryRepositoryGraphql(NULL, 0, &qv);
			char buffer[bufsize+1];
			snprintJsonRequest_GithubQueryRepositoryGraphql(buffer, bufsize+1, &qv);
			gears_println("buffer: %s", buffer);
			int res = proto_postRequest(gce_grql.value, gce_token.value, buffer, recvBuffer, sizeof(recvBuffer));
			gears_println("query res: %i", res);
			gears_println("%s", recvBuffer);
		}

		// repo's issues
		{
			char recvBuffer[65535] = {};
			GithubQueryIssuesGraphql qv = {
				.owner = gurl.owner,
				.name  = gurl.reponame,
				.count = 10,
			};
			int bufsize = snprintJsonRequest_GithubQueryIssuesGraphql(NULL, 0, &qv);
			char buffer[bufsize+1];
			snprintJsonRequest_GithubQueryIssuesGraphql(buffer, bufsize+1, &qv);
			gears_println("buffer: %s", buffer);
			int res = proto_postRequest(gce_grql.value, gce_token.value, buffer, recvBuffer, sizeof(recvBuffer));
			gears_println("query res: %i", res);
			gears_println("%s", recvBuffer);
		}

		// repo's PRs
		{
			char recvBuffer[65535] = {};
			GithubQueryPullrequestsGraphql qv = {
				.owner = gurl.owner,
				.name  = gurl.reponame,
				.count = 10,
			};
			int bufsize = snprintJsonRequest_GithubQueryPullrequestsGraphql(NULL, 0, &qv);
			char buffer[bufsize+1];
			snprintJsonRequest_GithubQueryPullrequestsGraphql(buffer, bufsize+1, &qv);
			gears_println("buffer: %s", buffer);
			int res = proto_postRequest(gce_grql.value, gce_token.value, buffer, recvBuffer, sizeof(recvBuffer));
			gears_println("query res: %i", res);
			gears_println("%s", recvBuffer);
		}

		// my gists
		{
			char recvBuffer[65535] = {};
			GithubQueryGistsGraphql qv = {
				.owner = gurl.owner,
				.count = 10,
			};
			int bufsize = snprintJsonRequest_GithubQueryGistsGraphql(NULL, 0, &qv);
			char buffer[bufsize+1];
			snprintJsonRequest_GithubQueryGistsGraphql(buffer, bufsize+1, &qv);
			gears_println("buffer: %s", buffer);
			int res = proto_postRequest(gce_grql.value, gce_token.value, buffer, recvBuffer, sizeof(recvBuffer));
			gears_println("query res: %i", res);
			gears_println("%s", recvBuffer);
		}

		// my repos
		{
			char recvBuffer[65535] = {};
			GithubQueryRepositoriesGraphql qv = {
				.owner = gurl.owner,
				.count = 10,
			};
			int bufsize = snprintJsonRequest_GithubQueryRepositoriesGraphql(NULL, 0, &qv);
			char buffer[bufsize+1];
			snprintJsonRequest_GithubQueryRepositoriesGraphql(buffer, bufsize+1, &qv);
			gears_println("buffer: %s", buffer);
			int res = proto_postRequest(gce_grql.value, gce_token.value, buffer, recvBuffer, sizeof(recvBuffer));
			gears_println("query res: %i", res);
			gears_println("%s", recvBuffer);
		}
	}

	gears_tag();
	return 0;
}
