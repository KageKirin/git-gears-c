#include "gears_config.h"
#include "gears_constants.h"
#include "gears_gitutil.h"
#include "gears_request.h"
#include "gears_util.h"

#include <assert.h>
#include <fcntl.h>
//#include <rure.h>
#include <string.h>
#include <unistd.h>


int gears_checkConfig(const char* hostname, const char* subname)
{
	assert(hostname);

	if (subname == kConfigAPIType || STRING_EQUALS(subname, kConfigAPIType))
	{
		GitConfigEntry gce = gears_getGearsConfigEntry(hostname, kConfigAPIType);
		if (gce.value[0])
		{
			for (int e = eGearsAPIType_min; e < eGearsAPIType_max; e++)
			{
				if (STRING_EQUALS(gce.value, kGearsAPIType[e]))
				{
					return 0;
				}
			}
		}
		return -1;
	}

	if (subname == kConfigAPIEndpointGraphQL || STRING_EQUALS(subname, kConfigAPIEndpointGraphQL))
	{
		GitConfigEntry gce = gears_getGearsConfigEntry(hostname, kConfigAPIEndpointGraphQL);
		if (gce.value[0])
		{
			if (strstr(gce.value, hostname))
			{
				return gears_httpGet(gce.value, NULL, NULL);
			}
		}
		return -1;
	}

	if (subname == kConfigAPIEndpointRest || STRING_EQUALS(subname, kConfigAPIEndpointRest))
	{
		GitConfigEntry gce = gears_getGearsConfigEntry(hostname, kConfigAPIEndpointRest);
		if (gce.value[0])
		{
			if (strstr(gce.value, hostname))
			{
				return gears_httpGet(gce.value, NULL, NULL);
			}
		}
		return -1;
	}

	if (subname == kConfigAPIToken || STRING_EQUALS(subname, kConfigAPIToken))
	{
		GitConfigEntry gce = gears_getGearsConfigEntry(hostname, kConfigAPIToken);
		GitConfigEntry gce_rest = gears_getGearsConfigEntry(hostname, kConfigAPIEndpointRest);
		if (gce.value[0])
		{
			return gears_httpGet(gce_rest.value, gce.value, NULL);
		}
		return -1;
	}

	if (!subname)
	{
		int validAPI = gears_checkConfig(hostname, kConfigAPIType);
		int validGraphQL = gears_checkConfig(hostname, kConfigAPIEndpointGraphQL);
		int validRest = gears_checkConfig(hostname, kConfigAPIEndpointRest);
		int validToken = gears_checkConfig(hostname, kConfigAPIToken);

		if ((validAPI == 0) && (validGraphQL == 0) && (validRest == 0) && (validToken == 0))
		{
			return 0;
		}
	}

	return -2;
}