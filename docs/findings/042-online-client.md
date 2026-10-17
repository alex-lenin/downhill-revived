# 042 — Online client: lockstep core, lobby and WebSocket transport

2026-10-16. Step 2 of `docs/ONLINE-PLAN.md`: the game-side network module, `port/net/`. It
speaks protocol v1 (`docs/online-protocol.md`, server in finding 039) and is not wired into
the game yet; `pad.c` and `host.c` are untouched.

## What is there

| File | What |
|---|---|
| `net.h` | The API the port will use (lobby, race, hashes). |
| `lockstep.c/h` | The lockstep core. It doesn't depend on the transport and isn't thread-safe. |
| `hash.c` | XXH64, plus a chain over memory ranges for state hashes. |
| `json.c/h` | Reads lobby JSON by path (`members[2].name`) and escapes strings. |
| `client.c/h` | One player: lobby state machine, race frames, pings, clock offset, offline fallback. Thread-safe. |
| `ws_winhttp.c/h` | WebSocket over WinHTTP (Windows 8+): a receive thread and a send/tick thread. |
| `net.c` | `net.h` over one client and one connection. |
| `nettool.c`, `run_e2e.py` | A command-line player and the 4-process end-to-end test. |
| `test_lockstep.c`, `test_json.c`, `test_loopback.c`, `build_tests.py` | The tests. |

`python port/net/build_tests.py` builds everything with clang-cl (i686, VS x86 libraries) and
runs the three tests. It doesn't touch the port's own build.

## How it works

- **Fields.** Field 0 is the first field of the race. Local input sampled at field F is
  scheduled for F + delay. Fields 0 .. delay-1 are neutral for every slot and are never sent,
  as the protocol requires.
- **Frames.**
  - Each sample sends one INPUT frame with the last 3 scheduled fields.
  - Buttons are active-high in `NetInput` (hle.h's `PAD_*`) and active-low on the wire.
  - A receiver keeps the first copy of each field. It drops copies of played fields, fields
    more than 512 ahead, and its own echo.
- **Resend while stalled.** The relay is TCP, so frames are not lost; the redundancy only
  matters for a future UDP path. If a field still hasn't completed after 100 ms, the client
  resends its oldest fields that someone may lack, at most 16 per frame.
  - Each peer's input implicitly acknowledges ours. A player whose input reached field G
    sampled it at G - delay, after playing every earlier field, so it already held our input
    up to G - delay - 1.
  - The randomised simulation in `test_lockstep.c` recovers with this alone, even at 60% loss
    with reordering and duplicates.
- **Leaving.** `left` switches a slot to the AI from `fromField`. When `fromField` is null,
  the client uses one past the highest field any frame from that slot named; every PC got the
  same relayed frames, so every PC picks the same field.
  - If our own connection drops mid-race (protocol v1 has no reconnect), every other slot
    becomes the AI from the first field we lack. The local race goes on offline
    (`NetLobby.offline`).
- **Lobby reconnect.** A drop in the lobby (not a refusal, close codes 4000-4099) rejoins the
  room by its code, up to 5 times with backoff (0.5-8 s). It then repeats the pick and ready.
  The rejoined player is a new member, as the protocol defines.
- **Delay.** `ls_auto_delay` uses the server's formula (`ceil(worst one-way / 16.68) + 1`,
  1..8). The value in `start` is final. "Adjustable before the start" needs a protocol field
  (say `settings.inputDelay`); the core already accepts any delay, even mid-race (samples are
  dropped or repeated so every field is scheduled exactly once).
- **Pings and clock.**
  - Lobby: JSON `ping` every second. Its `pong` gives the round trip and the server-clock
    offset from the lowest-RTT sample, which maps `startAt` to `net_seconds()`.
  - Race: binary PING (type 3) every 0.5 s.
  - Server pings are echoed.
  - 10 s of silence drops the connection.
- **Hashes.** Sent after fields 59, 119, ... (`net_hash_due`). The server compares them. For a
  `desync`, the client compares the hash it sent for that field against each slot's and
  reports the slots that differ from it.
- **XXH64** was written here from the published specification (Cyan4973/xxHash,
  `doc/xxhash_spec.md`; BSD 2-clause). No code was copied. It matches the reference values
  for "", "abc" and a 39-byte string. The WinHTTP WebSocket calls follow Microsoft's
  documentation. No third-party code is included.

## Results

- **Unit tests.** Covered: loss, reorder, duplicates, malformed frames, window edges, a late
  player, disconnect to AI, `fromField` null, delay changes, hash checkpoints, auto delay.
  Randomised 4-player runs at 20% and 60% loss end with identical pad streams.
- **Loopback.** Four clients in one process through a fake relay that behaves like the
  server, with one-way latencies of 10, 25, 45 and 70 ms. 600 fields, identical state on
  every PC.
  - A late player makes the others wait.
  - A drop at field 300 hands that rider to the AI at field 308 (= 300 + delay 8) on every
    PC, both with and without `fromField`.
  - A wrong hash from field 400 is reported at field 419 with the right slots.
- **End to end, `wrangler dev` (local).** 4 nettool processes, 600 fields at 59.94/s. Clean,
  drop and desync all pass. Ping 1-2 ms, delay 2. After the drop, the others stalled 106 ms
  once, then had the AI from field 302.
- **End to end, the deployed server.** Same three scenarios, all pass.
  - Pings 30-60 ms, delay 4.
  - Drop: AI from field 308 on all three remaining PCs.
  - Desync: field 419, right slots.

## Field pacing (for the integration)

At first each test process paced its fields from a fixed `startAt`. A PC whose clock
estimate ran early stayed a field ahead for the whole race. It waited about 16 ms on almost
every field: 6-10 s of waiting in a 10 s race, while the other PCs waited almost nothing.

When `net_inputs_for_field` waits, the frame clock has to **slip** by the wait instead of
catching up. With that, the early PC waits once and then runs in step. Two live races then
waited 0-236 ms in total per PC, all of it single jitter waits of at most 67 ms. The port's
virtual frame clock needs the same rule during online races.

## For the server (finding 039)

In one live run the server chose input delay 8 although every member's ping was 53-60 ms; its
formula gives 5 for that. Early RTT samples taken while the connection opens may be inflating
the median of five. The other runs got 4, as expected.

## Integration still to do

- `pad.c` serves only port 0 slot 0. Online needs the 4 multitap pads, with
  `pads[i].kind` choosing human, AI or empty for each slot.
- Per field F, counted from the first `pad_vsync` at or after `start_at`:
  1. `net_submit_local(F, ...)`, from `host_pad_read(0)` mapped to a `NetInput`.
  2. Then `net_inputs_for_field(F, pads, timeout)`. While it returns `NET_WAIT`: pump
     window messages, show `net_waiting_for()`, and slip the frame clock.
  3. After the field's game logic: if `net_hash_due(F)`, call
     `net_submit_hash(F, net_hash_ranges(...))` over the state ranges the game-side work
     picks.
  4. Pressure buttons: derive them from the buttons (pressed = 255) on every PC.
- Link `lockstep.c hash.c json.c client.c ws_winhttp.c net.c` with `winhttp.lib`.
- The server comes from `net_init(url, ...)` or `DHD_NET_SERVER`. No address is built in;
  the deployed one is kept out of tracked files.
- The version string must be the build's (players must match the host's).
  `isoSha256` is the SHA-256 of the player's ISO.
- One human per PC (one local slot). `NET_MAX_SLOTS` is 8, as a room holds 8; the multitap
  uses the first 4.
