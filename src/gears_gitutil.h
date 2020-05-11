#ifndef GEARS_GITUTIL_H_INC
#define GEARS_GITUTIL_H_INC

// define a max length
#define GEARS_GITREMOTE_MAX_LENGTH 256

typedef struct GitRemote
{
	char name[GEARS_GITREMOTE_MAX_LENGTH];
	char url[GEARS_GITREMOTE_MAX_LENGTH];
} GitRemote;

GitRemote gears_lookupRemote(const char* remote);

typedef struct GitConfigEntry
{
	char name[GEARS_GITREMOTE_MAX_LENGTH];
	char value[GEARS_GITREMOTE_MAX_LENGTH];
} GitConfigEntry;

GitConfigEntry gears_getConfigEntry(const char* name);
GitConfigEntry gears_getGearsConfigEntry(const char* hostname, const char* subname);
GitConfigEntry gears_getGearsConfigEntryRemoteOrURL(const char* remoteOrUrl, const char* subname);

#endif // GEARS_GITUTIL_H_INC
