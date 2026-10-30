# Session rooms (0.7.4)

Session rooms keep the WebSocket room alive across races. The creator's hello
feature selects the new state machine; old rooms retain their previous behavior.
Grid membership, finish records, timers, running rule copies and last results
are persisted with the Durable Object. Its one alarm serves countdown, finish
deadline, intermission and ping/listing refresh. Relay sockets contain only
active grid entrants and rebuild after hibernation.

The client advertises session, parses room/results/list fields, and exposes
server epoch ms for comparison with host_service_net_server_ms(). Native imports
use the same host wrappers on Windows, Linux and Mac. The browser worker had
network stubs; it now implements the wire and writes the shared C ABI using
offsets generated from net.h. Four clang targets check those offsets.

The cherry-picked native serializers were retained. Their integer choices are:
next race 0 auto / 1 host; track action 0 same / 1 next / 2 random.
Rotation accepts up to 64 bytes; the host bridge checks the full guest span.
Finding 116's groundwork text was moved here because 116 was already occupied.

Decisions:
- Shared rider picks remain legal, following the existing issue #17 behavior.
- A race closes when all humans have finished, even before their final back.
  Otherwise a finished client stays attached to relay until back or closure.
- Empty or single-track rotations keep the track; absent current track selects
  the first list entry for next. Duplicate ids are accepted; screens send unique ids.
- Cancelling the last grid entrant restores AUTO with a fresh 30 seconds.
- A void also applies the running nextRace policy, without replacing lastRace.
- Session names replace C0 controls and unpaired UTF-16 surrogates with U+FFFD. This bounds JSON escaping while
  keeping snapshots below 4 KB with eight members and previous results.
- No production address, deployment, game data or game-screen changes are needed.

References: Cloudflare's [alarm API](https://developers.cloudflare.com/durable-objects/api/alarms/)
and [Workers best practices](https://developers.cloudflare.com/workers/best-practices/workers-best-practices/).
The implementation follows the existing persisted state and hibernated WebSocket
structure. No external code was copied. The full screen contract is in
docs/online-protocol.md, "Session rooms (0.7.4)".

Verification on this branch:
- Server: 46 tests passed (32 existing, 14 session tests); TypeScript check passed.
- Native: test_hash, test_json, test_loopback and test_session passed.
  Loopback covers three four-player latency/drop scenarios; test_session has
  17 session parsing/serialization checks. Nettool builds with --no-run.
- Browser: check_net.mjs passed (message serializers and shared snapshot writes).
- gen_net_layout.py --check passed all field offsets and sizes for four targets.
- Local Wrangler on port 8788: three native nettool connections across two
  completed races. Finisher and give-up players entered both grids. Late join
  waited during the first race and entered the second. All exited zero.
  Wrangler was stopped afterwards. Nothing was deployed.

Build note: this C-only work reused an existing F:-resident SDL3 build and Rust
renderer archive. build_tests.py accepts DHD_NET_TEST_RUST_LIB to select that
archive without rebuilding Rust; the normal path still calls rustgs.py.
Build output, dependencies and scratch are on F: through local junctions.
No full retail-data game build was needed. Screens remain the other lane's work.
