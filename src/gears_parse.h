#ifndef GEARS_PARSE_H_INC
#define GEARS_PARSE_H_INC

#include <stdbool.h>

bool gears_parseBool(const char* json, const struct jsmntok*);
int gears_parseInteger(const char* json, const struct jsmntok*);
float gears_parseFloat(const char* json, const struct jsmntok*);

#endif // GEARS_PARSE_H_INC
