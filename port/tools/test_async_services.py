"""Async host services regression (finding 082 task): file SHA-256 and lobby list.

Builds a small native harness against port/host/host_win32.c, port/net/* and
port/runtime/sha256.c, then runs it: the SHA-256 service hashes a temp file
(busy -> done, correct digest, failure paths) and the lobby service fetches
from a local HTTP stub (busy -> rows, correct parse, truncation, failure).
No game data, no visible windows; all output stays under build/.
"""
import hashlib
import http.server
import os
import socket
import subprocess
import sys
import threading
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / 'build' / 'port' / 'async-test'
LLVM = (Path.home() / 'scoop/apps/llvm/current/bin')
TARGET = 'x86_64-pc-windows-msvc'

LOBBIES = {
    "lobbies": [
        {"code": "ABC123", "roomName": "ALPINE CUP", "version": "v9-test",
         "hostName": "HOST1", "track": 3, "players": 2, "maxPlayers": 4},
        {"code": "XYZ789", "roomName": "MOAB DASH", "version": "v9-test",
         "hostName": "HOST2", "track": 1, "players": 4, "maxPlayers": 4},
    ]
}

HARNESS = r'''
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "host_api.h"
#include "net.h"
#include "guest.h"

uintptr_t guest_base;

#define DIGEST_AT 0x1000u
#define ROWS_AT 0x2000u

static int failures;
#define CHECK(cond, label) \
    do { \
        if (cond) printf("PASS %s\n", label); \
        else { printf("FAIL %s\n", label); failures++; } \
    } while (0)

static int poll_sha(uint32_t at)
{
    return host_service_file_sha256_poll(at);
}

/* Copies a host string into the guest arena; the services take guest addresses. */
static uint32_t put_string(const char *s)
{
    static uint32_t next = 0x100u;
    size_t n = strlen(s) + 1;
    char *at = (char *)GUEST(next);
    memcpy(at, s, n);
    next += (uint32_t)n;
    return next - (uint32_t)n;
}

static void sha_tests(const char *datafile, const char *expected)
{
    char missing[1024];
    uint8_t *digest = (uint8_t *)GUEST(DIGEST_AT);
    uint32_t data_at, missing_at;
    int r, waited, saw_busy;
    snprintf(missing, sizeof missing, "%s-no-such-file", datafile);
    data_at = put_string(datafile);
    missing_at = put_string(missing);
    CHECK(host_service_file_sha256_start(missing_at) == 1, "sha-missing-start");
    waited = 0;
    while ((r = poll_sha(DIGEST_AT)) == 0 && waited < 30000) {
        Sleep(5);
        waited += 5;
    }
    CHECK(r == -1, "sha-missing-failed");
    CHECK(poll_sha(DIGEST_AT) == -1, "sha-idle-failed");
    CHECK(host_service_file_sha256_start(data_at) == 1, "sha-start");
    r = poll_sha(DIGEST_AT);
    if (r == 0) {
        saw_busy = 1;
        CHECK(host_service_file_sha256_start(data_at) == 0, "sha-start-while-busy");
    } else {
        saw_busy = 0;
    }
    waited = 0;
    while (r == 0 && waited < 120000) {
        Sleep(5);
        waited += 5;
        r = poll_sha(DIGEST_AT);
    }
    CHECK(saw_busy, "sha-saw-busy");
    CHECK(r == 1, "sha-done");
    if (r == 1) {
        char got[65];
        for (int i = 0; i < 32; i++) snprintf(got + 2 * i, 3, "%02x", digest[i]);
        CHECK(!strcmp(got, expected), "sha-digest");
    }
    CHECK(poll_sha(DIGEST_AT) == -1, "sha-idle-after-done");
    CHECK(host_service_file_sha256_start(data_at) == 1, "sha-restart");
    waited = 0;
    r = 0;
    while (r == 0 && waited < 120000) {
        Sleep(5);
        waited += 5;
        r = poll_sha(DIGEST_AT);
    }
    CHECK(r == 1, "sha-redone");
}

static int poll_lobbies(uint32_t at, int max)
{
    return host_service_net_list_lobbies_poll(at, max);
}

static void lobby_tests(const char *base, const char *closed_base)
{
    NetLobbyInfo *rows = (NetLobbyInfo *)GUEST(ROWS_AT);
    int r, waited, saw_busy;
    CHECK(poll_lobbies(ROWS_AT, 8) == -1, "lobby-idle-failed");
    CHECK(host_service_net_list_lobbies_start(0) == 0, "lobby-start-zero");
    CHECK(host_service_net_list_lobbies_start(1000) == 0, "lobby-start-huge");
    net_init(base, "TESTER", "v9-test",
             "0123456789012345678901234567890123456789012345678901234567890123");
    CHECK(host_service_net_list_lobbies_start(8) == 1, "lobby-start");
    r = poll_lobbies(ROWS_AT, 8);
    saw_busy = (r == -2);
    if (r == -2)
        CHECK(host_service_net_list_lobbies_start(8) == 0, "lobby-start-while-busy");
    waited = 0;
    while (r == -2 && waited < 60000) {
        Sleep(10);
        waited += 10;
        r = poll_lobbies(ROWS_AT, 8);
    }
    CHECK(saw_busy, "lobby-saw-busy");
    CHECK(r == 2, "lobby-count");
    if (r == 2) {
        printf("ROW %s|%s|%s|%s|%d|%d|%d\n", rows[0].code, rows[0].room_name, rows[0].version,
               rows[0].host_name, rows[0].track, rows[0].players, rows[0].max_players);
        printf("ROW %s|%s|%s|%s|%d|%d|%d\n", rows[1].code, rows[1].room_name, rows[1].version,
               rows[1].host_name, rows[1].track, rows[1].players, rows[1].max_players);
        CHECK(!strcmp(rows[0].code, "ABC123") && !strcmp(rows[0].room_name, "ALPINE CUP") &&
              !strcmp(rows[0].version, "v9-test") && !strcmp(rows[0].host_name, "HOST1") &&
              rows[0].track == 3 && rows[0].players == 2 && rows[0].max_players == 4,
              "lobby-row0");
        CHECK(!strcmp(rows[1].code, "XYZ789") && !strcmp(rows[1].room_name, "MOAB DASH") &&
              !strcmp(rows[1].version, "v9-test") && !strcmp(rows[1].host_name, "HOST2") &&
              rows[1].track == 1 && rows[1].players == 4 && rows[1].max_players == 4,
              "lobby-row1");
    }
    CHECK(host_service_net_list_lobbies_start(1) == 1, "lobby-restart-1");
    waited = 0;
    r = -2;
    while (r == -2 && waited < 60000) {
        Sleep(10);
        waited += 10;
        r = poll_lobbies(ROWS_AT, 1);
    }
    CHECK(r == 1, "lobby-truncated");
    if (r == 1) CHECK(!strcmp(rows[0].code, "ABC123"), "lobby-truncated-row");
    net_init(closed_base, "TESTER", "v9-test",
             "0123456789012345678901234567890123456789012345678901234567890123");
    CHECK(host_service_net_list_lobbies_start(8) == 1, "lobby-fail-start");
    waited = 0;
    r = -2;
    while (r == -2 && waited < 60000) {
        Sleep(10);
        waited += 10;
        r = poll_lobbies(ROWS_AT, 8);
    }
    CHECK(r == -1, "lobby-fail-count");
}

int main(int argc, char **argv)
{
    if (argc != 5) {
        fprintf(stderr, "usage: %s <datafile> <expected-hex> <base-url> <closed-base-url>\n", argv[0]);
        return 2;
    }
    guest_base = (uintptr_t)calloc(1, 1 << 20);
    if (!guest_base) {
        fprintf(stderr, "no guest arena\n");
        return 2;
    }
    sha_tests(argv[1], argv[2]);
    lobby_tests(argv[3], argv[4]);
    printf(failures ? "RESULT FAIL %d\n" : "RESULT PASS\n", failures);
    return failures != 0;
}
'''


class Stub(http.server.BaseHTTPRequestHandler):
    delay = 2.0
    body = b'{}'

    def do_GET(self):
        if self.path != '/v1/lobbies':
            self.send_response(404)
            self.end_headers()
            return
        time.sleep(self.delay)
        data = self.body
        self.send_response(200)
        self.send_header('Content-Type', 'application/json')
        self.send_header('Content-Length', str(len(data)))
        self.end_headers()
        self.wfile.write(data)

    def log_message(self, *args):
        pass


def free_port():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind(('127.0.0.1', 0))
        return s.getsockname()[1]


def main():
    import json as jsonlib
    OUT.mkdir(parents=True, exist_ok=True)
    data = OUT / 'async-blob.bin'
    data.write_bytes(os.urandom(40 << 20))
    expected = hashlib.sha256(data.read_bytes()).hexdigest()
    Stub.body = jsonlib.dumps(LOBBIES).encode()
    server = http.server.ThreadingHTTPServer(('127.0.0.1', 0), Stub)
    port = server.server_address[1]
    thread = threading.Thread(target=server.serve_forever, daemon=True)
    thread.start()
    closed = free_port()
    harness = OUT / 'async-harness.c'
    harness.write_text(HARNESS, newline='')
    exe = OUT / 'async-harness.exe'
    # Inherit the environment (explicit env dicts break spawning here): extend
    # it in place instead.
    os.environ['PATH'] = str(LLVM) + os.pathsep + os.environ.get('PATH', '')
    os.environ.update(TMP=str(OUT), TEMP=str(OUT), TMPDIR=str(OUT))
    sources = [harness, ROOT / 'port/host/host_win32.c', ROOT / 'port/runtime/sha256.c',
               ROOT / 'port/net/net.c', ROOT / 'port/net/client.c', ROOT / 'port/net/json.c',
               ROOT / 'port/net/hash.c',
               ROOT / 'port/net/ws_winhttp.c']
    cmd = ['clang', '--target=' + TARGET, '-O1', '-w',
           '-I' + str(ROOT / 'port' / 'include'), '-I' + str(ROOT / 'port' / 'runtime'),
           '-I' + str(ROOT / 'port' / 'host'), '-I' + str(ROOT / 'port' / 'net'),
           *[str(s) for s in sources], '-o', str(exe), '-lwinhttp', '-luser32']
    r = subprocess.run(cmd, capture_output=True, text=True, encoding='utf-8',
                       errors='replace', timeout=300)
    if r.returncode:
        raise SystemExit('harness build failed:\n' + r.stdout + r.stderr)
    base = f'http://127.0.0.1:{port}'
    args = [str(exe), str(data), expected, base, f'http://127.0.0.1:{closed}/']
    r = subprocess.run(args, capture_output=True, text=True, encoding='utf-8',
                       errors='replace', timeout=300)
    print(r.stdout, end='')
    print(r.stderr, end='', file=sys.stderr)
    server.shutdown()
    if r.returncode:
        raise SystemExit('harness failed')
    if '\nFAIL' in ('\n' + r.stdout) or 'RESULT FAIL' in r.stdout:
        raise SystemExit('harness reported failures')
    if 'RESULT PASS' not in r.stdout:
        raise SystemExit('harness did not report PASS')
    print(f'PASS: async services against {data.name} and {base}')


if __name__ == '__main__':
    main()
