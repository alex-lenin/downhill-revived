# Online protocol (version 1)

The wire protocol between the PC port (`port/net/`) and the online server (`online/server/`,
Cloudflare Workers + Durable Objects). The plan behind it is `docs/ONLINE-PLAN.md`.

- One WebSocket per player. Lobby control travels as JSON **text** frames, the race as compact
  little-endian **binary** frames.
- The server runs no game logic. In a race it relays inputs, compares state hashes and
  reports players who leave.
- `PROTOCOL = 1`. It is in the URL (`/v1/`) and in `hello`. Any incompatible change raises
  it; the server refuses clients with another number.

## Endpoints

`BASE` is `wss://<worker host>` (the official server, the client's default:
`wss://dhd-online.downhill-revived.workers.dev`; your own deployment:
`wss://dhd-online.<account>.workers.dev`; local `wrangler dev`: `ws://127.0.0.1:8787`). HTTP endpoints use `https://` / `http://` on the same
host.

| Method | Path | What |
|---|---|---|
| WebSocket | `/v1/rooms/new` | Create a room; this socket becomes its host. |
| WebSocket | `/v1/rooms/<CODE>` | Join the room with that join code (case-insensitive). |
| GET | `/v1/lobbies` | Public lobby list (JSON, below). |
| GET | `/v1/health` | `{"ok":true,"protocol":1,"minVersion":"0.1.0"}` |

Join codes are 6 characters from `23456789ABCDEFGHJKMNPQRSTUVWXYZ` (no `0 O 1 I L`). Clients
upper-case what the user types and reject characters outside that alphabet.

The client sends `hello` as its first message, within 10 seconds of connecting. Before `hello`
the server sends nothing.

## Refusals and errors

A refusal is one text frame followed by a close:

```json
{"type":"error","code":"iso_mismatch","message":"Your ISO does not match the host's (US SCUS-97177 expected)."}
```

| `code` | Close code | When |
|---|---|---|
| `bad_message` | 4000 | Unparseable JSON, unknown type, invalid `hello`, anything before `hello`. |
| `protocol_mismatch` | 4001 | `hello.protocol` is not the server's `PROTOCOL`. |
| `version_mismatch` | 4002 | `hello.version` differs from the host's. |
| `iso_mismatch` | 4003 | `hello.isoSha256` differs from the host's. |
| `room_full` | 4004 | No free slot. |
| `room_started` | 4005 | The race is running; late joins are not supported. |
| `room_not_found` | 4006 | No room has this code (or it was closed). |
| `kicked` | 4007 | The host removed this player. |
| `hello_timeout` | 4008 | No `hello` within 10 seconds. |
| `client_outdated` | 4009 | `hello.clientVersion` is below the server's `MIN_VERSION` (below). |

### Minimum client version

The server refuses releases older than its `MIN_VERSION` (a Worker variable in
`online/server/wrangler.jsonc`, `MAJOR.MINOR.PATCH`; default `0.1.0`, every release). Since
0.2.0 the client sends its release in `hello.clientVersion` (`port/include/version.h`). A
`hello` without it is 0.1.0, which did not send one; an unparseable one counts as outdated. The
refusal carries both versions:

```json
{"type":"error","code":"client_outdated","message":"A new version is out. Update to play online.",
 "minVersion":"0.2.0","clientVersion":"0.1.0"}
```

0.1.0 knows no `client_outdated`: like any refusal it shows `message` on the ONLINE screen.
0.2.0 and later show the same text and add an **Update Now** row (finding 054).

Non-fatal errors (a non-host sending a host command, a `start` while someone is not ready)
use the same `error` frame **without** a close; the client just shows `message`.

## Lobby (JSON text frames)

### Client → server

```jsonc
// First message, required. name: 1-24 characters.
{"type":"hello","protocol":1,"version":"dhd-port 0.2.0 1a2b3c4d5e6f7a8b","clientVersion":"0.2.0",
 "isoSha256":"<64 lowercase hex>","name":"Rider"}

// Host only. Every field optional; omitted fields keep their value.
{"type":"settings","track":3,"laps":2,"seed":12345,"public":true,"maxPlayers":4,"roomName":"Friday race"}

// Rider and bike for a member. Anyone may set their own; the host may set anyone's.
{"type":"pick","member":2,"rider":5,"bike":1}     // "member" omitted = yourself

{"type":"ready","ready":true}
{"type":"kick","member":3}                          // host only
{"type":"start"}                                    // host only; everyone (host included) must be ready
{"type":"pong","s":1734000000123}                   // echo of a server ping, see below
{"type":"ping","t":12345.6}                          // optional client-side ping; the server answers "pong"
{"type":"end"}                                       // host only: race over, back to the lobby
```

- `track`, `laps`, `rider`, `bike`: integers 0..255 whose meaning belongs to the game (track
  table index, rider and bike table indices). `laps` 0 = the track's default.
- `seed`: unsigned 32-bit integer. If the host never sets one, the server draws one at `start`.
- `maxPlayers`: 2..8, default 4. It can't go below the current member count.
- `public`: listed in `/v1/lobbies` while not started and not full. Default `false`
  (private, join code only).

### Server → client

```jsonc
// Answer to hello: who you are in this room.
{"type":"welcome","protocol":1,"code":"K7QX2M","memberId":2,"host":false,"serverTime":1734000000000}

// Full room snapshot. Sent after every change and every ~2 s with fresh ping numbers.
{"type":"room","code":"K7QX2M","hostId":0,"roomName":"Friday race","public":true,"maxPlayers":4,
 "started":false,"version":"dhd-port 0.4.0",
 "settings":{"track":3,"laps":2,"seed":null},
 "members":[{"id":0,"name":"Ann","ready":true,"rider":5,"bike":1,"rttMs":38},
            {"id":2,"name":"Bo","ready":false,"rider":0,"bike":0,"rttMs":null}],
 "inputDelay":3}

// Server-timestamped ping, every 2 s while in the lobby. Echo "s" back unchanged in a "pong".
{"type":"ping","s":1734000000123}

// Answer to a client "ping": "t" echoed, serverTime added (use it to estimate the clock offset).
{"type":"pong","t":12345.6,"serverTime":1734000000150}

// Race start, sent to every member at once.
{"type":"start","yourSlot":1,"track":3,"laps":2,"seed":2864434397,"inputDelay":3,
 "players":[{"slot":0,"memberId":0,"name":"Ann","rider":5,"bike":1},
            {"slot":1,"memberId":2,"name":"Bo","rider":0,"bike":0}],
 "serverTime":1734000005000,"startAt":1734000008000}

// A racer left or dropped. Their rider becomes AI from field "fromField" on, on every PC.
{"type":"left","slot":1,"memberId":2,"fromField":1843,"reason":"disconnect"}   // or "leave"/"kicked"

// State hashes differ (see HASH). Sent once per newly found earliest bad field.
{"type":"desync","field":1800,"hashes":{"0":"9f3a...","1":"77c0..."}}

// Host ended the race; the room is a lobby again (ready flags cleared).
{"type":"ended"}
```

- **Member ids** are fixed for the life of a connection and never reused in a room. **Slots**
  (`0..N-1`) exist only in a race: the server assigns them at `start` in join order (host
  first). All race frames use slots.
- Lobby members who leave just disappear from the next `room`. If the host leaves, the
  longest-present member becomes host.
- `startAt` is server time (ms since the Unix epoch) at which field 0 should run, about 3 s
  after `start`. Clients map it to local time with the offset from `ping`/`pong`
  (`offset ≈ serverTime - (tSend + rtt/2)`). Lockstep does not need it to be exact: a
  client that starts early just waits for inputs.

### Suggested input delay

The server keeps each member's last five round-trips (from its own pings) and uses their
median. A relayed input travels sender → server → receiver, so the worst one-way time is
half the sum of the two largest member RTTs (with one member: half its RTT). Then

`inputDelay = clamp(ceil(worstOneWayMs / 16.68) + 1, 1, 8)` fields,

with 16.68 ms = one NTSC field (59.94 Hz). Unknown RTTs count as 0 until measured. The value
in `start` is final for the race.

## Race (binary frames, little-endian)

After `start` each racer sends binary frames on the same socket. The server overwrites byte
1 (`slot`) with the sender's real slot, then forwards INPUT frames unchanged to every other
racer. Malformed frames, and binary frames from a socket that is not racing (for example
stragglers after `ended`), are dropped silently.

### INPUT, type 1 — client ↔ server ↔ clients

| Offset | Size | Field |
|---|---|---|
| 0 | u8 | `type` = 1 |
| 1 | u8 | `slot` of the rider these inputs belong to |
| 2 | u32 | `field`: the field number of the first entry |
| 6 | u8 | `count`: 1..16 entries, for fields `field .. field+count-1` |
| 7 | 6 × count | entries |

Entry (6 bytes):

| Offset | Size | Field |
|---|---|---|
| 0 | u16 | `buttons`: the PS2 digital button word as the game's pad buffer holds it (active low: a bit is 0 while its button is held; `0xFFFF` = nothing held) |
| 2 | u8 | `lx` (left stick X; 0x80 = centre) |
| 3 | u8 | `ly` |
| 4 | u8 | `rx` |
| 5 | u8 | `ry` |

Frame size = 7 + 6 × count bytes. Field numbers start at 0 at the race start and count game
fields (59.94 per second).

**Redundancy:** every frame carries the newest field plus the previous ones not yet known to
be safe; send each field in at least 3 consecutive frames (count 3 is the normal case). A
receiver keeps the first copy of each field and ignores repeats. Inputs for a field are
fixed once sent; a client must never send different inputs for a field it already sent.

With input delay `D`, the input a player makes on local field `F` is sent tagged `F + D`, and
every PC runs field `F` only once it holds all racers' inputs for `F`. Fields `0 .. D-1` use
neutral input (`buttons 0xFFFF`, sticks `0x80`) and are never sent.

### HASH, type 2 — client → server

| Offset | Size | Field |
|---|---|---|
| 0 | u8 | `type` = 2 |
| 1 | u8 | `slot` |
| 2 | u32 | `field` |
| 6 | u64 | `hash` of the game state after running `field` |

14 bytes. Send one every 60 fields (`field % 60 == 59`, i.e. after fields 59, 119, ...).
The hash function is the client's choice (all clients run the same build, which `version`
enforces). The server does not forward HASH frames; it compares the hashes of all racers
still connected for that field and broadcasts `desync` (JSON, above) naming the field and
each slot's hash as 16 lowercase hex digits. It reports a field only if it is earlier than
any field it reported before.

### PING, type 3 — client → server; PONG, type 4 — server → client

| Offset | Size | Field |
|---|---|---|
| 0 | u8 | `type` = 3 (4 in the answer) |
| 1 | u8 | `slot` |
| 2 | n | any payload, at most 32 bytes (e.g. a u32 sequence and an f64 send time) |

The server answers the sender only, with the same bytes and the type changed to 4. Use it to
watch the round-trip during a race; it is never forwarded.

### STATE, type 5, and EVENT, type 6 — client ↔ server ↔ clients

| Offset | Size | Field |
|---|---|---|
| 0 | u8 | `type` = 5 or 6 |
| 1 | u8 | `slot` of the sender |
| 2 | n | payload, the client's format (finding 044) |

State sync: a STATE frame is a snapshot of riders the sender owns, an EVENT frame something
it decided for another rider (a hit). The server forwards both, slot corrected, to every other
racer and keeps nothing.

### Leaving

A racer leaves by closing the socket (or sends `{"type":"leave"}` first, which only changes
`reason`). The server broadcasts `left` with `fromField` = one past the last field the server
had relayed for that slot (0 if none). All inputs up to `fromField - 1` reached the others
before the `left` message (same socket, ordered), so all PCs switch the rider to AI on the
same field.

`fromField` is `null` in one rare case: the room was evicted from memory mid-race (the
runtime keeps a room awake during a race, so this needs seconds with no traffic at all) and
that racer sent nothing since. Clients then use one past the last field they received from
that slot, which is the same on every PC because every one of them got the same relayed
frames.

## Lobby list

`GET /v1/lobbies` → `200 application/json`:

```json
{"protocol":1,"minVersion":"0.1.0","lobbies":[
  {"code":"K7QX2M","roomName":"Friday race","version":"dhd-port 0.2.0 1a2b3c4d5e6f7a8b","track":3,
   "players":2,"maxPlayers":4,"hostName":"Ann"}]}
```

Only public rooms that have not started and have a free slot are listed. Clients should
show only rooms whose `version` matches their own.

## Limits

- One room holds at most 8 members.
- Text frames: at most 4 KB. Binary frames: at most 256 bytes.
- A room with no members is deleted.
- No reconnect in version 1: a dropped racer is handed to AI for the rest of the race.

## Retail preview flow (finding 065)

The UI uses retail selection screens before READY. Rider/bike indexes still use
`pick`; the complete outfit and bike records still travel in EVENT_SETUP v2 when
the race loads. There is no new wire message or protocol revision. The host starts
when all present members are ready, with at least two members. Choosing again
clears readiness; a changed track requires guests to ready again. Exact executable
identity in `hello.version` refuses mixed builds. Raise MIN_VERSION to the release
that includes this UI at release time; no deployment is part of this change.
