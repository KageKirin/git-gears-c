//! git-gears-c's REST queries
//! as structures and snprintf-style functions
//! DO NOT EDIT - this file is autogenerated.
//! Run `genie generate` or `make generate` to re-generate.

#include "gitlab_restutils.h"
#include "stb_printf.h"

int snprintUrl_GitlabQueryPullrequestsRoute(char* buf, int count, const char* endpointUrl,
											const GitlabQueryPullrequestsRoute* vars)
{
	return stbsp_snprintf(buf, count, "%s/projects/%s/merge_requests?max_results=100&order_by=updated_at&sort=desc",
						  endpointUrl, vars->projectId);
}

int snprintUrl_GitlabQueryUserRoute(char* buf, int count, const char* endpointUrl, const GitlabQueryUserRoute* vars)
{
	return stbsp_snprintf(buf, count, "%s/users/%s?username=%s", endpointUrl, vars->userId, vars->login);
}

int snprintUrl_GitlabQueryOwnerRoute(char* buf, int count, const char* endpointUrl, const GitlabQueryOwnerRoute* vars)
{
	return stbsp_snprintf(buf, count, "%s/namespaces/%s", endpointUrl, vars->login);
}

int snprintUrl_GitlabQueryPullrequestRoute(char* buf, int count, const char* endpointUrl,
										   const GitlabQueryPullrequestRoute* vars)
{
	return stbsp_snprintf(buf, count,
						  "%s/projects/%s/merge_requests?source_branch=%s&max_results=1&order_by=updated_at&sort=desc",
						  endpointUrl, vars->projectId, vars->branch);
}

int snprintUrl_GitlabMutationCreateRepositoryRoute(char* buf, int count, const char* endpointUrl)
{
	return stbsp_snprintf(buf, count, "%s/projects", endpointUrl);
}

int snprintRestJson_GitlabMutationCreateRepositoryJson(char* buf, int count,
													   const GitlabMutationCreateRepositoryJson* vars)
{
	return stbsp_snprintf(buf, count,
						  "{"
						  "	\"name\": \"%s\","
						  "	\"namespace_id\": \"%s\","
						  "	\"description\": \"%s\n%s\","
						  "	\"visibility\": \"%s\","
						  "	\"lfs_enabled\": true,"
						  "	\"remove_source_branch_after_merge\": true,"
						  "	\"autoclose_referenced_issues\": true"
						  "}"
						  "",
						  vars->repoName, vars->ownerId, vars->description, vars->homepage, vars->visibility);
}

int snprintUrl_GitlabQueryOrganizationRoute(char* buf, int count, const char* endpointUrl,
											const GitlabQueryOrganizationRoute* vars)
{
	return stbsp_snprintf(buf, count, "%s/groups/%s", endpointUrl, vars->login);
}

int snprintUrl_GitlabMutationCreatePullrequestRoute(char* buf, int count, const char* endpointUrl,
													const GitlabMutationCreatePullrequestRoute* vars)
{
	return stbsp_snprintf(buf, count, "%s/projects/%s/merge_requests", endpointUrl, vars->projectId);
}

int snprintRestJson_GitlabMutationCreatePullrequestJson(char* buf, int count,
														const GitlabMutationCreatePullrequestJson* vars)
{
	return stbsp_snprintf(buf, count,
						  "{"
						  "	\"source_branch\": \"%s\","
						  "	\"target_branch\": \"%s\","
						  "	\"title\": \"%s\","
						  "	\"description\": \"%s\""
						  "}"
						  "",
						  vars->branch, vars->targetBranch, vars->title, vars->body);
}

int snprintUrl_GitlabMutationCreateIssueRoute(char* buf, int count, const char* endpointUrl,
											  const GitlabMutationCreateIssueRoute* vars)
{
	return stbsp_snprintf(buf, count, "%s/projects/%s/issues", endpointUrl, vars->projectId);
}

int snprintRestJson_GitlabMutationCreateIssueJson(char* buf, int count, const GitlabMutationCreateIssueJson* vars)
{
	return stbsp_snprintf(buf, count,
						  "{"
						  "	\"title\": \"%s\","
						  "	\"description\": \"%s\""
						  "}"
						  "",
						  vars->title, vars->body);
}