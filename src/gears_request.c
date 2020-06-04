#include "gears_request.h"
#include "gears_util.h"

#include <assert.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static size_t write_passthtrough(void* data, size_t size, size_t nmemb, void* userp)
{
	return size * nmemb;
}

static size_t write_response(void* data, size_t size, size_t nmemb, void* userp)
{
	gears_tag();

	GearsResponse* resp = (GearsResponse*)userp;
	assert(resp);

	const size_t dataSize = size * nmemb;

	if (resp->buffer)
	{
		size_t growth = resp->size + dataSize;
		char* new = resp->alloc(growth + 1);
		assert(new);
		gears_tag();
		memset(new, 0, growth + 1);
		memcpy(new, resp->buffer, resp->size);
		memcpy(new + resp->size, data, dataSize);
		resp->free(resp->buffer);
		resp->buffer = new;
		resp->size = growth;
		gears_tag();
	}
	else
	{
		char* new = resp->alloc(dataSize + 1);
		assert(new);
		gears_tag();
		memset(new, 0, dataSize + 1);
		memcpy(new, data, dataSize);
		resp->buffer = new;
		resp->size = dataSize;
		gears_tag();
	}

	return dataSize;
}


int gears_httpGet(const char* url, const char* token, GearsResponse* resp)
{
	assert(url);

	CURL* curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, resp ? write_response : write_passthtrough);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);
#ifdef SKIP_PEER_VERIFICATION
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif
#ifdef SKIP_HOSTNAME_VERIFICATION
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "git-gears-c/0.0.1");
		if (token)
		{
			curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);
			curl_easy_setopt(curl, CURLOPT_XOAUTH2_BEARER, token);
		}
		CURLcode res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		return res;
	}
	return -1;
}


int gears_httpPost(const char* url, const char* token, const char* postData, GearsResponse* resp)
{
	assert(url);

	CURL* curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);

		// on receive
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, resp ? write_response : write_passthtrough);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);

		// on send
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData); //<--- fill with post data
#ifdef SKIP_PEER_VERIFICATION
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif
#ifdef SKIP_HOSTNAME_VERIFICATION
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "git-gears-c/0.0.1");
		if (token)
		{
			curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);
			curl_easy_setopt(curl, CURLOPT_XOAUTH2_BEARER, token);
		}
		CURLcode res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		return res;
	}
	return -1;
}
