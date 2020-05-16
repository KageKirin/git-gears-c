#include "gears_constants.h"

const char* kConfigAPIToken = "token";
const char* kConfigAPIType = "api";
const char* kConfigAPIEndpointGraphQL = "url";
const char* kConfigAPIEndpointRest = "rest";

const char* kGearsAPIType[eGearsAPIType_max] = {
	[eGearsAPIType_github] = "github",
	[eGearsAPIType_gitlab] = "gitlab",
};
