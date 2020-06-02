//! git-gears-c's REST queries
//! as structures and snprintf-style functions
//! DO NOT EDIT - this file is autogenerated.
//! Run `genie generate` or `make generate` to re-generate.

#ifndef GEARS_REQUESTS_GITLAB_RESTUTILS_H_INC
#define GEARS_REQUESTS_GITLAB_RESTUTILS_H_INC

#include <stdbool.h>

//! url data
typedef struct GitlabQueryPullrequestsRoute
{
	const char* projectId;
} GitlabQueryPullrequestsRoute;

//! snprint url
int snprintUrl_GitlabQueryPullrequestsRoute(char* buf, int count, const char* endpointUrl,
											const GitlabQueryPullrequestsRoute* vars);

//! url data
typedef struct GitlabQueryUserRoute
{
	const char* login;
	const char* userId;
} GitlabQueryUserRoute;

//! snprint url
int snprintUrl_GitlabQueryUserRoute(char* buf, int count, const char* endpointUrl, const GitlabQueryUserRoute* vars);

//! url data
typedef struct GitlabQueryOwnerRoute
{
	const char* login;
} GitlabQueryOwnerRoute;

//! snprint url
int snprintUrl_GitlabQueryOwnerRoute(char* buf, int count, const char* endpointUrl, const GitlabQueryOwnerRoute* vars);

//! url data
typedef struct GitlabQueryPullrequestRoute
{
	const char* branch;
	const char* projectId;
} GitlabQueryPullrequestRoute;

//! snprint url
int snprintUrl_GitlabQueryPullrequestRoute(char* buf, int count, const char* endpointUrl,
										   const GitlabQueryPullrequestRoute* vars);

//! snprint url
int snprintUrl_GitlabMutationCreateRepositoryRoute(char* buf, int count, const char* endpointUrl);

//! rest data
typedef struct GitlabMutationCreateRepositoryJson
{
	const char* description;
	const char* homepage;
	const char* ownerId;
	const char* repoName;
	const char* visibility;
} GitlabMutationCreateRepositoryJson;

//! snprint rest JSON
int snprintRestJson_GitlabMutationCreateRepositoryJson(char* buf, int count,
													   const GitlabMutationCreateRepositoryJson* vars);

//! url data
typedef struct GitlabQueryOrganizationRoute
{
	const char* login;
} GitlabQueryOrganizationRoute;

//! snprint url
int snprintUrl_GitlabQueryOrganizationRoute(char* buf, int count, const char* endpointUrl,
											const GitlabQueryOrganizationRoute* vars);

//! url data
typedef struct GitlabMutationCreatePullrequestRoute
{
	const char* projectId;
} GitlabMutationCreatePullrequestRoute;

//! snprint url
int snprintUrl_GitlabMutationCreatePullrequestRoute(char* buf, int count, const char* endpointUrl,
													const GitlabMutationCreatePullrequestRoute* vars);

//! rest data
typedef struct GitlabMutationCreatePullrequestJson
{
	const char* body;
	const char* branch;
	const char* targetBranch;
	const char* title;
} GitlabMutationCreatePullrequestJson;

//! snprint rest JSON
int snprintRestJson_GitlabMutationCreatePullrequestJson(char* buf, int count,
														const GitlabMutationCreatePullrequestJson* vars);

//! url data
typedef struct GitlabMutationCreateIssueRoute
{
	const char* projectId;
} GitlabMutationCreateIssueRoute;

//! snprint url
int snprintUrl_GitlabMutationCreateIssueRoute(char* buf, int count, const char* endpointUrl,
											  const GitlabMutationCreateIssueRoute* vars);

//! rest data
typedef struct GitlabMutationCreateIssueJson
{
	const char* body;
	const char* title;
} GitlabMutationCreateIssueJson;

//! snprint rest JSON
int snprintRestJson_GitlabMutationCreateIssueJson(char* buf, int count, const GitlabMutationCreateIssueJson* vars);


#endif // GEARS_REQUESTS_GITLAB_RESTUTILS_H_INC
