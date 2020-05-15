#ifndef GEARS_GITURL_H_INC
#define GEARS_GITURL_H_INC

// define a max length
// as gitlab max to be on the safe side
// github only allows up 100 characters on the webpage, and 64 in requests.
// we use the same size to allow to long hostnames, as well as path names
#define GEARS_GITURL_MAX_LENGTH 256

typedef struct GitUrl
{
	char protocol[GEARS_GITURL_MAX_LENGTH];
	char host[GEARS_GITURL_MAX_LENGTH];
	char path[GEARS_GITURL_MAX_LENGTH];
	char owner[GEARS_GITURL_MAX_LENGTH];
	char reponame[GEARS_GITURL_MAX_LENGTH];
} GitUrl;

GitUrl gears_parseUrl(const char* url);
GitUrl gears_parseRemoteOrUrl(const char* remoteOrUrl);

#endif // GEARS_GITURL_H_INC
