#include "gears_util.h"

#include <assert.h>
#include <jsmn.h>
#include <stdbool.h>


void proto_parse(const char* json, size_t size)
{
	jsmn_parser p;
	jsmn_init(&p);
	int tokCount = jsmn_parse(&p, json, size, NULL, 0);
	gears_println("%i tokens", tokCount);
	jsmntok_t tokens[tokCount];
	memset(tokens, 0, sizeof(jsmntok_t) * tokCount);
	jsmn_init(&p);
	int tokCount2 = jsmn_parse(&p, json, size, tokens, tokCount);
	assert(tokCount2 == tokCount);

	typedef enum
	{
		START,
		KEY,
		PRINT,
		VALUE,
		STOP
	} parse_state;
	parse_state state = START;
	size_t object_tokens = 0;
	char* KEYS[] = {"name", "location", "public_repos", "hireable"};

	for (size_t i = 0, j = 1; j > 0; i++, j--)
	{
		jsmntok_t* t = &tokens[i];

		if (t->type == JSMN_ARRAY || t->type == JSMN_OBJECT)
		{
			j += t->size;
		}

		switch (state)
		{
		case START:
			gears_tag();
			assert(t->type == JSMN_OBJECT); // Invalid response: root element must be an object.

			state = KEY;
			object_tokens = t->size;

			if (object_tokens == 0)
				state = STOP;

			break;

		case KEY:
			gears_tag();
			object_tokens--;
			assert(t->type == JSMN_STRING); // Invalid response: object keys must be strings.
			state = VALUE;

			printf("parsed key: ");
			gears_printJsmntok(t, json);
			break;

		case VALUE:
			gears_tag();
			gears_println("type: %i", t->type);

			assert(t->type == JSMN_STRING ||
				   t->type == JSMN_PRIMITIVE); // Invalid response: object values must be strings or primitives.
			object_tokens--;
			state = KEY;

			printf("skipping: ");
			gears_printJsmntok(t, json);

			if (object_tokens == 0)
				state = STOP;

			break;

		case PRINT:
			gears_tag();
			assert(t->type == JSMN_STRING ||
				   t->type == JSMN_PRIMITIVE); // Invalid response: object values must be strings or primitives.

			printf("printing: ");
			gears_printJsmntok(t, json);

			object_tokens--;
			state = KEY;

			if (object_tokens == 0)
				state = STOP;

			break;

		case STOP:
			gears_tag();
			// Just consume the tokens
			break;

		default:
			assert(0); // Invalid state
		}
	}
}


bool gears_parseBool(const char* json, jsmntok_t* t)
{
	assert(t->type == JSMN_PRIMITIVE);
	assert(json[t->start] == 't' || json[t->start] == 'f');
	if (json[t->start] == 't')
	{
		assert(t->end - t->start == 4);
		assert(json[t->start + 0] == 't');
		assert(json[t->start + 1] == 'r');
		assert(json[t->start + 2] == 'u');
		assert(json[t->start + 3] == 'e');
		return true;
	}

	if (json[t->start] == 'f')
	{
		assert(t->end - t->start == 5);
		assert(json[t->start + 0] == 'f');
		assert(json[t->start + 1] == 'a');
		assert(json[t->start + 2] == 'l');
		assert(json[t->start + 3] == 's');
		assert(json[t->start + 4] == 'e');
		return false;
	}

	assert(0); // complete failure at this point
}

int gears_parseInteger(const char* json, jsmntok_t* t)
{
	assert(t->type == JSMN_PRIMITIVE);

	char buf[t->end - t->start + 1];
	stbsp_snprintf(buf, "%*.s", t->end - t->start, json + t->start);
	int value;
	sscanf(buf, "%i", &value);
	return value;
}

float gears_parseFloat(const char* json, jsmntok_t* t)
{
	assert(t->type == JSMN_PRIMITIVE);

	char buf[t->end - t->start + 1];
	stbsp_snprintf(buf, "%*.s", t->end - t->start, json + t->start);
	float value;
	sscanf(buf, "%f", &value);
	return value;
}

typedef struct gears_fixed_string
{
	char buf[256];
} gears_fixed_string;

gears_fixed_string gears_parseString(const char* json, jsmntok_t* t)
{
	assert(t->type == JSMN_STRING);

	gears_fixed_string value;
	stbsp_snprintf(value.buf, "%*.s", t->end - t->start, json + t->start);
	return value;
}
