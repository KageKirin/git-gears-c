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

#endif // GEARS_GITUTIL_H_INC
