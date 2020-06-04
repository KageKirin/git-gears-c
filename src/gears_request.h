#ifndef GEARS_REQUEST_H_INC
#define GEARS_REQUEST_H_INC

#include <stdlib.h>

typedef struct GearsResponse
{
	char* buffer;
	size_t size;

	void* (*alloc)(size_t);
	void (*free)(void*);
} GearsResponse;

//! send HTTP GET request
//! returns curl error code
//! if passed, GearsResponse used to be filled with the response data
int gears_httpGet(const char* url, const char* token, GearsResponse* resp);

//! send HTTP POST request
//! returns curl error code
//! if passed, GearsResponse used to be filled with the response data
int gears_httpPost(const char* url, const char* token, const char* data, GearsResponse* resp);

#endif // GEARS_REQUEST_H_INC
