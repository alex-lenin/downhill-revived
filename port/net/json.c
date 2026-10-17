/* JSON by path (json.h). It scans the text each time: lobby messages are a few hundred bytes. */
#include "json.h"

#include <stdlib.h>
#include <string.h>

enum { MAX_DEPTH = 32 };

static const char *ws(const char *p)
{
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    return p;
}

static const char *skip_string(const char *p)  /* p at the opening quote */
{
    for (p++; *p != '"'; p++) {
        if (!*p || (unsigned char)*p < 0x20)
            return NULL;
        if (*p == '\\' && !*++p)
            return NULL;
    }
    return p + 1;
}

static const char *skip_value(const char *p, int depth)
{
    p = ws(p);
    if (depth > MAX_DEPTH)
        return NULL;
    if (*p == '"')
        return skip_string(p);
    if (*p == '{' || *p == '[') {
        char close = *p == '{' ? '}' : ']';
        p = ws(p + 1);
        if (*p == close)
            return p + 1;
        for (;;) {
            if (close == '}') {
                if (*p != '"' || !(p = skip_string(p)) || *(p = ws(p)) != ':')
                    return NULL;
                p++;
            }
            if (!(p = skip_value(p, depth + 1)))
                return NULL;
            p = ws(p);
            if (*p == close)
                return p + 1;
            if (*p != ',')
                return NULL;
            p = ws(p + 1);
        }
    }
    if (*p == '-' || (*p >= '0' && *p <= '9')) {
        char *end;
        strtod(p, &end);
        return end == p ? NULL : end;
    }
    if (!strncmp(p, "true", 4) || !strncmp(p, "null", 4))
        return p + 4;
    if (!strncmp(p, "false", 5))
        return p + 5;
    return NULL;
}

const char *json_find(const char *json, const char *path)
{
    const char *p = ws(json);
    if (!skip_value(p, 0))
        return NULL;  /* malformed: refuse the whole text */
    while (*path) {
        if (*path == '[') {
            long index = strtol(path + 1, (char **)&path, 10);
            if (*path++ != ']' || *p != '[')
                return NULL;
            p = ws(p + 1);
            if (*p == ']')
                return NULL;
            while (index-- > 0) {
                p = ws(skip_value(p, 0));
                if (*p != ',')
                    return NULL;
                p = ws(p + 1);
            }
        } else {
            size_t len = strcspn(path, ".[");
            if (*p != '{')
                return NULL;
            p = ws(p + 1);
            for (;;) {
                const char *key = p + 1, *end;
                if (*p != '"')
                    return NULL;
                end = skip_string(p);
                p = ws(ws(end) + 1);  /* past the colon */
                if ((size_t)(end - 1 - key) == len && !memcmp(key, path, len))
                    break;
                p = ws(skip_value(p, 0));
                if (*p != ',')
                    return NULL;
                p = ws(p + 1);
            }
            path += len;
        }
        if (*path == '.')
            path++;
    }
    return p;
}

int json_number(const char *json, const char *path, double *out)
{
    const char *p = json_find(json, path);
    if (!p || !(*p == '-' || (*p >= '0' && *p <= '9')))
        return 0;
    *out = strtod(p, NULL);
    return 1;
}

int json_int(const char *json, const char *path, long long *out)
{
    double v;
    if (!json_number(json, path, &v))
        return 0;
    *out = (long long)v;
    return 1;
}

int json_bool(const char *json, const char *path, int *out)
{
    const char *p = json_find(json, path);
    if (p && !strncmp(p, "true", 4))
        return *out = 1, 1;
    if (p && !strncmp(p, "false", 5))
        return *out = 0, 1;
    return 0;
}

int json_is_null(const char *json, const char *path)
{
    const char *p = json_find(json, path);
    return p && !strncmp(p, "null", 4);
}

static void put_utf8(char **o, char *end, unsigned c)
{
    char buf[4];
    int n = 0, i;
    if (c < 0x80)
        buf[n++] = (char)c;
    else if (c < 0x800)
        buf[n++] = (char)(0xC0 | c >> 6), buf[n++] = (char)(0x80 | (c & 0x3F));
    else if (c < 0x10000)
        buf[n++] = (char)(0xE0 | c >> 12), buf[n++] = (char)(0x80 | (c >> 6 & 0x3F)),
        buf[n++] = (char)(0x80 | (c & 0x3F));
    else
        buf[n++] = (char)(0xF0 | c >> 18), buf[n++] = (char)(0x80 | (c >> 12 & 0x3F)),
        buf[n++] = (char)(0x80 | (c >> 6 & 0x3F)), buf[n++] = (char)(0x80 | (c & 0x3F));
    if (end - *o < n)
        return;
    for (i = 0; i < n; i++)
        *(*o)++ = buf[i];
}

int json_string(const char *json, const char *path, char *out, size_t cap)
{
    const char *p = json_find(json, path);
    char *o = out, *end = out + cap - 1;
    if (!cap || !p || *p != '"')
        return 0;
    for (p++; *p != '"'; p++) {
        unsigned c = (unsigned char)*p;
        int code = 0;  /* a \u escape: written as UTF-8 */
        if (c == '\\') {
            switch (*++p) {
            case 'n': c = '\n'; break;
            case 't': c = '\t'; break;
            case 'r': c = '\r'; break;
            case 'b': c = '\b'; break;
            case 'f': c = '\f'; break;
            case 'u': {
                char hex[5] = {0};
                code = 1;
                memcpy(hex, p + 1, 4);
                c = (unsigned)strtoul(hex, NULL, 16);
                p += 4;
                if (c >= 0xD800 && c < 0xDC00 && p[1] == '\\' && p[2] == 'u') {  /* a surrogate pair */
                    unsigned lo;
                    memcpy(hex, p + 3, 4);
                    lo = (unsigned)strtoul(hex, NULL, 16);
                    if (lo >= 0xDC00 && lo < 0xE000)
                        c = 0x10000 + ((c - 0xD800) << 10) + (lo - 0xDC00), p += 6;
                }
                break;
            }
            default: c = (unsigned char)*p;  /* \" \\ \/ */
            }
        }
        if (code)
            put_utf8(&o, end, c);
        else if (o < end)
            *o++ = (char)c;
    }
    *o = 0;
    return 1;
}

int json_count(const char *json, const char *path)
{
    const char *p = json_find(json, path);
    int n = 0;
    if (!p || *p != '[')
        return -1;
    p = ws(p + 1);
    if (*p == ']')
        return 0;
    for (;;) {
        n++;
        p = ws(skip_value(p, 0));
        if (*p != ',')
            return n;
        p = ws(p + 1);
    }
}

int json_raw(const char *json, const char *path, char *out, size_t cap)
{
    const char *p = json_find(json, path), *end;
    if (!p || !(end = skip_value(p, 0)) || (size_t)(end - p) >= cap)
        return 0;
    memcpy(out, p, (size_t)(end - p));
    out[end - p] = 0;
    return 1;
}

size_t json_escape(char *out, size_t cap, const char *s)
{
    static const char hex[] = "0123456789abcdef";
    size_t n = 0;
    for (; *s; s++) {
        unsigned char c = (unsigned char)*s;
        char esc[7];
        int len = 1, i;
        esc[0] = (char)c;
        if (c == '"' || c == '\\')
            esc[0] = '\\', esc[1] = (char)c, len = 2;
        else if (c < 0x20)
            memcpy(esc, "\\u00", 4), esc[4] = hex[c >> 4], esc[5] = hex[c & 15], len = 6;
        for (i = 0; i < len; i++, n++)
            if (n + 1 < cap)
                out[n] = esc[i];
    }
    if (cap)
        out[n < cap ? n : cap - 1] = 0;
    return n;
}
