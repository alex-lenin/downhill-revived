/* Tests of json.c on the lobby messages of docs/online-protocol.md. */
#include "json.h"

#include <stdio.h>
#include <string.h>

static int failures;
#define CHECK(c) do { if (!(c)) { printf("%s:%d: CHECK(%s) failed\n", __FILE__, __LINE__, #c); failures++; } } while (0)

int main(void)
{
    static const char room[] =
        "{\"type\":\"room\",\"code\":\"K7QX2M\",\"hostId\":0,\"roomName\":\"Friday \\\"race\\\"\",\"public\":true,"
        "\"maxPlayers\":4,\"started\":false,\"version\":\"dhd-port 0.4.0\",\n"
        " \"settings\":{\"track\":3,\"laps\":2,\"seed\":null},\n"
        " \"members\":[{\"id\":0,\"name\":\"Ann\",\"ready\":true,\"rider\":5,\"bike\":1,\"rttMs\":38},\n"
        "            {\"id\":2,\"name\":\"B\\u00f6 \\ud83d\\ude00\",\"ready\":false,\"rider\":0,\"bike\":0,\"rttMs\":null}],\n"
        " \"inputDelay\":3, \"hashes\":{\"0\":\"9f3a\",\"1\":\"77c0\"}, \"seed\":2864434397, \"neg\":-1.5e2}";
    char s[64], raw[128];
    long long v;
    double d;
    int b;
    CHECK(json_string(room, "type", s, sizeof s) && !strcmp(s, "room"));
    CHECK(json_string(room, "roomName", s, sizeof s) && !strcmp(s, "Friday \"race\""));
    CHECK(json_int(room, "maxPlayers", &v) && v == 4);
    CHECK(json_bool(room, "public", &b) && b == 1);
    CHECK(json_bool(room, "started", &b) && b == 0);
    CHECK(json_int(room, "settings.track", &v) && v == 3);
    CHECK(json_is_null(room, "settings.seed") && !json_int(room, "settings.seed", &v));
    CHECK(json_count(room, "members") == 2);
    CHECK(json_int(room, "members[1].id", &v) && v == 2);
    CHECK(json_string(room, "members[1].name", s, sizeof s) && !strcmp(s, "B\xC3\xB6 \xF0\x9F\x98\x80"));
    CHECK(json_int(room, "members[0].rttMs", &v) && v == 38);
    CHECK(json_is_null(room, "members[1].rttMs"));
    CHECK(!json_find(room, "members[2]") && !json_find(room, "members[2].id"));
    CHECK(json_string(room, "hashes.1", s, sizeof s) && !strcmp(s, "77c0"));
    CHECK(json_int(room, "seed", &v) && v == 2864434397ll);
    CHECK(json_number(room, "neg", &d) && d == -150);
    CHECK(!json_find(room, "nope") && !json_find(room, "settings.nope") && !json_find(room, "type.x"));
    CHECK(json_raw(room, "settings", raw, sizeof raw) && !strcmp(raw, "{\"track\":3,\"laps\":2,\"seed\":null}"));
    CHECK(!json_raw(room, "members", raw, 8));
    CHECK(json_string(room, "code", s, 4) && !strcmp(s, "K7Q"));  /* truncated */
    CHECK(json_count("{\"a\":[]}", "a") == 0 && json_count("{\"a\":1}", "a") == -1);
    CHECK(json_find("{}", "") != NULL && !json_find("{}", "a"));
    /* malformed texts give nothing */
    CHECK(!json_find("{\"a\":1", "a"));
    CHECK(!json_find("{\"a\":[1,2}", "a"));
    CHECK(!json_find("{\"a\" 1}", "a"));
    CHECK(!json_find("{\"a\":tru}", "a"));
    CHECK(!json_find("{\"a\":\"x", "a"));
    {
        char deep[200];
        memset(deep, '[', 100), memset(deep + 100, ']', 99);
        deep[199] = 0;
        CHECK(!json_find(deep, ""));
    }
    {
        char out[64];
        size_t n = json_escape(out, sizeof out, "A \"q\" \\ \n\x01");
        CHECK(!strcmp(out, "A \\\"q\\\" \\\\ \\u000a\\u0001") && n == strlen(out));
        CHECK(json_escape(out, 4, "abcdef") == 6 && !strcmp(out, "abc"));
    }
    if (failures) {
        printf("test_json: %d failures\n", failures);
        return 1;
    }
    printf("test_json: ok\n");
    return 0;
}
