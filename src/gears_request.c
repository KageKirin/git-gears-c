#include "gears_util.h"

#include <assert.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <unistd.h>

static size_t write_passthtrough(void* data, size_t size, size_t nmemb, void* userp)
{
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


int proto_postRequest(const char* url, const char* token)
{
	assert(url);

	CURL* curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);

		// on receive
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_passthtrough);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);

		// on send
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ""); //<--- fill with post data
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
