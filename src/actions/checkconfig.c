#include "gears_config.h"
#include "gears_constants.h"
#include "gears_giturl.h"
#include "gears_gitutil.h"
#include "gears_option.h"
#include "gears_util.h"

static struct
{
	char* remoteOrUrl;
	int showDetails;
} OptionValues = {
	.remoteOrUrl = NULL,
	.showDetails = 0,
};

static Option options[] = {
	{'v', "verbose", "verbose. show details", &OptionValues.showDetails, gears_setOptionImplicit},
	{.description = "remote or url to look up", &OptionValues.remoteOrUrl, gears_setOptionPositionalString},
};

static const char u_check[] = gearsio_green "\u2713" gearsio_reset;
static const char u_cross[] = gearsio_red "\u2717" gearsio_reset;

int CheckConfig(int argc, char** argv)
{
	// parse arguments
	if (parse_options(options, ARRAY_COUNT(options), argc, argv))
	{
		return 1;
	}
	gears_println("remote or url: %s", OptionValues.remoteOrUrl);

	GitUrl gurl = gears_parseRemoteOrUrl(OptionValues.remoteOrUrl);

	gears_println("checking configuration for %s", OptionValues.remoteOrUrl);
	gears_println("host: %s", gurl.host);

	GitConfigEntry gce_type = gears_getGearsConfigEntry(gurl.host, kConfigAPIType);
	GitConfigEntry gce_grql = gears_getGearsConfigEntry(gurl.host, kConfigAPIEndpointGraphQL);
	GitConfigEntry gce_rest = gears_getGearsConfigEntry(gurl.host, kConfigAPIEndpointRest);
	GitConfigEntry gce_token = gears_getGearsConfigEntry(gurl.host, kConfigAPIToken);

	int validAPI = gears_checkConfig(gurl.host, kConfigAPIType);
	int validGQL = gears_checkConfig(gurl.host, kConfigAPIEndpointGraphQL);
	int validRST = gears_checkConfig(gurl.host, kConfigAPIEndpointRest);
	int validTOK = gears_checkConfig(gurl.host, kConfigAPIToken);

	if (OptionValues.showDetails)
	{
		gears_println("type: %s %s", gce_type.value, validAPI == 0 ? u_check : u_cross);
		gears_println("endpoint (GraphQL): %s %s", gce_grql.value, validGQL == 0 ? u_check : u_cross);
		gears_println("endpoint (REST): %s %s", gce_rest.value, validRST == 0 ? u_check : u_cross);
		gears_println("token: %s %s", gce_token.value, validTOK == 0 ? u_check : u_cross);
	}
	else
	{
		gears_println("type: %s", validAPI == 0 ? u_check : u_cross);
		gears_println("endpoint (GraphQL): %s", validGQL == 0 ? u_check : u_cross);
		gears_println("endpoint (REST): %s", validRST == 0 ? u_check : u_cross);
		gears_println("token: %s", validTOK == 0 ? u_check : u_cross);
	}

	return 0;
}
