#include "gears_util.h"

#include <assert.h>
#include <jsmn.h>


void gears_printJsmntok(const jsmntok_t* tok, const char* json)
{
	assert(tok);
	assert(json);
	switch (tok->type)
	{
	case JSMN_PRIMITIVE:
	case JSMN_STRING:
		gears_println("%.*s", (int)(tok->end - tok->start), json + tok->start);
		break;

	case JSMN_OBJECT:
	case JSMN_ARRAY:
		break;
	}
}