#include "gears_util.h"

#include <assert.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

static size_t write_passthtrough(void* data, size_t size, size_t nmemb, void* userp)
{
	return size * nmemb;
}

typedef struct ReceiveBuffer
{
	char* buffer;
	size_t maxSize;
	size_t filledSize;
} ReceiveBuffer;

static size_t write_receive(void* data, size_t size, size_t nmemb, void* userp)
{
	ReceiveBuffer* recv = (ReceiveBuffer*)userp;
	assert(recv);

	if (recv->filledSize + size * nmemb < recv->maxSize)
	{
		memcpy(recv->buffer + recv->filledSize, data, size * nmemb);
		recv->filledSize += size * nmemb;
	}
	return size * nmemb;
}

static size_t write_resize(void* data, size_t size, size_t nmemb, void* userp)
{
	gears_tag();
	char** recv = (char**)userp;
	assert(recv != NULL);

	if (*recv)
	{
		gears_tag();

		size_t len = strlen(*recv);
		size_t growth = len + size * nmemb;
		char* new = malloc(len + size * nmemb+1);
		memset(new, 0, growth+1);
		strncpy(new, *recv, len);
		strncpy(new + len, data, size * nmemb);
		free(*recv);
		*recv = new;
	}
	else
	{
		gears_tag();

		size_t growth = size * nmemb;
		char* new = malloc(size * nmemb+1);
		memset(new, 0, growth+1);
		strncpy(new, data, size * nmemb);
		*recv = new;
	}

	gears_tag();

	return size * nmemb;
}


int proto_getRequest(const char* url, const char* token)
{
	assert(url);

	CURL* curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_passthtrough);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
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


int proto_putRequest(const char* url, const char* token)
{
	assert(url);

	CURL* curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_PUT, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_passthtrough);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
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


int proto_postRequest(const char* url, const char* token, const char* postData, char* recvData, int maxSize)
{
	assert(url);

	CURL* curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);

		ReceiveBuffer buffer = {
			.buffer = recvData,
			.maxSize = maxSize,
			.filledSize = 0,
		};

		// on receive
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_receive);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

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


int proto_postRequest2(const char* url, const char* token, const char* postData, char** recvData)
{
	assert(url);
	gears_println("recvData: %p", recvData);
	assert(recvData != NULL);

	CURL* curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);

		// on receive
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_resize);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, recvData);

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
