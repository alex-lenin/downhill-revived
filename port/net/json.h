/* Just enough JSON for the lobby messages: reading values by path from a received text, and
   escaping strings for the ones we write with snprintf. */
#ifndef NET_JSON_H
#define NET_JSON_H

#include <stddef.h>

/* The value at path in a JSON text, e.g. "type", "settings.track", "members[2].name",
   "hashes.0". Returns where the value starts, or NULL when it is absent or the text malformed. */
const char *json_find(const char *json, const char *path);
/* 1 and *out when the value is a number (true/false count as 1/0 for json_bool). */
int json_number(const char *json, const char *path, double *out);
int json_int(const char *json, const char *path, long long *out);
int json_bool(const char *json, const char *path, int *out);
int json_is_null(const char *json, const char *path);
/* Copies a string value, unescaped (\uXXXX as UTF-8), truncated to cap - 1 bytes. 1 if found. */
int json_string(const char *json, const char *path, char *out, size_t cap);
/* The number of elements of an array, -1 when it isn't one. */
int json_count(const char *json, const char *path);
/* The value as text: its JSON source, copied (objects stay objects). 1 if found and it fits. */
int json_raw(const char *json, const char *path, char *out, size_t cap);
/* s as the inside of a JSON string (without quotes). Returns the length it needs. */
size_t json_escape(char *out, size_t cap, const char *s);

#endif
