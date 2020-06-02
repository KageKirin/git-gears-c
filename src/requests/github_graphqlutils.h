//! git-gears-c's GraphQL queries
//! as structures and snprintf-style functions
//! DO NOT EDIT - this file is autogenerated.
//! Run `genie generate` or `make generate` to re-generate.

#ifndef GEARS_REQUESTS_GITHUB_GRAPHQLUTILS_H_INC
#define GEARS_REQUESTS_GITHUB_GRAPHQLUTILS_H_INC

#include <stdbool.h>

//! query data
typedef struct GithubQueryIssueGraphql
{
	const char* name;
	int number;
	const char* owner;
} GithubQueryIssueGraphql;

//! snprint variables JSON
int snprintJsonVars_GithubQueryIssueGraphql(char* buf, int count, const GithubQueryIssueGraphql* vars);

//! snprint request JSON
int snprintJsonRequest_GithubQueryIssueGraphql(char* buf, int count, const GithubQueryIssueGraphql* vars);

//! query data
typedef struct GithubQueryGistsGraphql
{
	int count;
	const char* owner;
} GithubQueryGistsGraphql;

//! snprint variables JSON
int snprintJsonVars_GithubQueryGistsGraphql(char* buf, int count, const GithubQueryGistsGraphql* vars);

//! snprint request JSON
int snprintJsonRequest_GithubQueryGistsGraphql(char* buf, int count, const GithubQueryGistsGraphql* vars);

//! query data
typedef struct GithubQueryOrganizationGraphql
{
	const char* login;
} GithubQueryOrganizationGraphql;

//! snprint variables JSON
int snprintJsonVars_GithubQueryOrganizationGraphql(char* buf, int count, const GithubQueryOrganizationGraphql* vars);

//! snprint request JSON
int snprintJsonRequest_GithubQueryOrganizationGraphql(char* buf, int count, const GithubQueryOrganizationGraphql* vars);

//! query data
typedef struct GithubQueryPullrequestGraphql
{
	const char* branch;
	const char* name;
	const char* owner;
} GithubQueryPullrequestGraphql;

//! snprint variables JSON
int snprintJsonVars_GithubQueryPullrequestGraphql(char* buf, int count, const GithubQueryPullrequestGraphql* vars);

//! snprint request JSON
int snprintJsonRequest_GithubQueryPullrequestGraphql(char* buf, int count, const GithubQueryPullrequestGraphql* vars);

//! query data
typedef struct GithubMutationCreateIssueGraphql
{
	const char* body;
	const char* mutationId;
	const char* repositoryId;
	const char* title;
} GithubMutationCreateIssueGraphql;

//! snprint variables JSON
int snprintJsonVars_GithubMutationCreateIssueGraphql(char* buf, int count,
													 const GithubMutationCreateIssueGraphql* vars);

//! snprint request JSON
int snprintJsonRequest_GithubMutationCreateIssueGraphql(char* buf, int count,
														const GithubMutationCreateIssueGraphql* vars);

//! query data
typedef struct GithubQueryPullrequestsGraphql
{
	int count;
	const char* name;
	const char* owner;
} GithubQueryPullrequestsGraphql;

//! snprint variables JSON
int snprintJsonVars_GithubQueryPullrequestsGraphql(char* buf, int count, const GithubQueryPullrequestsGraphql* vars);

//! snprint request JSON
int snprintJsonRequest_GithubQueryPullrequestsGraphql(char* buf, int count, const GithubQueryPullrequestsGraphql* vars);

//! snprint request JSON
int snprintJsonRequest_GithubQueryViewerGraphql(char* buf, int count);

//! query data
typedef struct GithubMutationCreatePullrequestGraphql
{
	const char* body;
	const char* branch;
	const char* mutationId;
	const char* repositoryId;
	const char* targetBranch;
	const char* title;
} GithubMutationCreatePullrequestGraphql;

//! snprint variables JSON
int snprintJsonVars_GithubMutationCreatePullrequestGraphql(char* buf, int count,
														   const GithubMutationCreatePullrequestGraphql* vars);

//! snprint request JSON
int snprintJsonRequest_GithubMutationCreatePullrequestGraphql(char* buf, int count,
															  const GithubMutationCreatePullrequestGraphql* vars);

//! query data
typedef struct GithubQueryRepositoriesGraphql
{
	int count;
	const char* owner;
} GithubQueryRepositoriesGraphql;

//! snprint variables JSON
int snprintJsonVars_GithubQueryRepositoriesGraphql(char* buf, int count, const GithubQueryRepositoriesGraphql* vars);

//! snprint request JSON
int snprintJsonRequest_GithubQueryRepositoriesGraphql(char* buf, int count, const GithubQueryRepositoriesGraphql* vars);

//! query data
typedef struct GithubQueryOwnerGraphql
{
	const char* login;
} GithubQueryOwnerGraphql;

//! snprint variables JSON
int snprintJsonVars_GithubQueryOwnerGraphql(char* buf, int count, const GithubQueryOwnerGraphql* vars);

//! snprint request JSON
int snprintJsonRequest_GithubQueryOwnerGraphql(char* buf, int count, const GithubQueryOwnerGraphql* vars);

//! query data
typedef struct GithubQueryGistGraphql
{
	const char* name;
	const char* owner;
} GithubQueryGistGraphql;

//! snprint variables JSON
int snprintJsonVars_GithubQueryGistGraphql(char* buf, int count, const GithubQueryGistGraphql* vars);

//! snprint request JSON
int snprintJsonRequest_GithubQueryGistGraphql(char* buf, int count, const GithubQueryGistGraphql* vars);

//! query data
typedef struct GithubQueryUserGraphql
{
	const char* login;
} GithubQueryUserGraphql;

//! snprint variables JSON
int snprintJsonVars_GithubQueryUserGraphql(char* buf, int count, const GithubQueryUserGraphql* vars);

//! snprint request JSON
int snprintJsonRequest_GithubQueryUserGraphql(char* buf, int count, const GithubQueryUserGraphql* vars);

//! query data
typedef struct GithubQueryIssuesGraphql
{
	int count;
	const char* name;
	const char* owner;
} GithubQueryIssuesGraphql;

//! snprint variables JSON
int snprintJsonVars_GithubQueryIssuesGraphql(char* buf, int count, const GithubQueryIssuesGraphql* vars);

//! snprint request JSON
int snprintJsonRequest_GithubQueryIssuesGraphql(char* buf, int count, const GithubQueryIssuesGraphql* vars);

//! query data
typedef struct GithubMutationCreateRepositoryGraphql
{
	const char* description;
	const char* homepageUrl;
	const char* mutationId;
	const char* name;
	const char* ownerId;
	const char* visibility;
} GithubMutationCreateRepositoryGraphql;

//! snprint variables JSON
int snprintJsonVars_GithubMutationCreateRepositoryGraphql(char* buf, int count,
														  const GithubMutationCreateRepositoryGraphql* vars);

//! snprint request JSON
int snprintJsonRequest_GithubMutationCreateRepositoryGraphql(char* buf, int count,
															 const GithubMutationCreateRepositoryGraphql* vars);

//! query data
typedef struct GithubQueryRepositoryGraphql
{
	const char* name;
	const char* owner;
} GithubQueryRepositoryGraphql;

//! snprint variables JSON
int snprintJsonVars_GithubQueryRepositoryGraphql(char* buf, int count, const GithubQueryRepositoryGraphql* vars);

//! snprint request JSON
int snprintJsonRequest_GithubQueryRepositoryGraphql(char* buf, int count, const GithubQueryRepositoryGraphql* vars);


#endif // GEARS_REQUESTS_GITHUB_GRAPHQLUTILS_H_INC