#include <stdio.h>
#include <curl/curl.h>
#include <git2.h>

#include "gears_util.h"

int main(int argc, char** argv)
{
	gears_println("hello world", NULL);
	gears_wrnln("hello world", NULL);
	gears_errln("hello world", NULL);
	return 0;
}
