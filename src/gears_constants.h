#ifndef GEARS_CONSTANTS_H_INC
#define GEARS_CONSTANTS_H_INC

const char* kConfigAPIToken;
const char* kConfigAPIType;
const char* kConfigAPIEndpointGraphQL;
const char* kConfigAPIEndpointRest;

typedef enum eGearsAPIType
{
	eGearsAPIType_min = 0,
	eGearsAPIType_github = 0,
	eGearsAPIType_gitlab,
	eGearsAPIType_max
} eGearsAPIType;

const char* kGearsAPIType[eGearsAPIType_max];

#endif // GEARS_CONSTANTS_H_INC
