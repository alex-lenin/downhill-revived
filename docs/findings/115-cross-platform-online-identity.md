# 115: executable identity blocks cross-platform online rooms

2026-10-28. Cross-platform lobby compatibility; no server deployment.

## Symptom and cause

A macOS player sees a Windows player's room labelled `(other version)` and cannot select it,
even when both players use release 0.7.0.

`port/hle/online.c`, `build_version`, hashes the running native executable with SHA-256 and
formats the lobby identity as `dhd-port <DHD_VERSION> <first eight digest bytes>`. Windows PE
and macOS Mach-O executables have different contents, so a shared release number does not
give them a shared lobby identity. Separate builds on one platform can differ as well.
This is a compatibility policy based on executable bytes, not an explicit OS check.

`port/hle/online_menu.c`, `page_build`'s LIST case, compares the complete identity strings
with `strcmp`. A mismatch adds `(other version)` and disables the room row.

`online/server/src/room.ts`, `onHello`, independently requires `hello.version` to equal the
host's stored identity and refuses a mismatch with `version_mismatch`. Joining with a code
therefore does not bypass the check. `hello.clientVersion` separately passes the minimum-release
gate. The disc-files hash is checked separately too.

## Implementation

`port/tools/online_identity.py` hashes source files under `src/`, `include/`, `config/` and
`port/`, in relative-path order, with length-prefixed path/content pairs and CRLF normalised
to LF. The identity is `src1-` plus the first 128 bits of SHA-256. No native executable bytes,
absolute paths, file times, Git commit IDs or index contents participate. New source files
and uncommitted edits count. Build products, dependencies, Python caches, Gradle caches and
Tauri's generated schemas are skipped; documents and binary assets are outside the source
extension allowlist. The policy is conservative: source edits to graphics or tools also
separate clients. This is a compatibility identifier, not proof of deterministic simulation
or an anti-cheat mechanism; external toolchain differences still need gameplay validation.

`build_wasm.py` writes `build/wasm/port/include/online_build.h` after mirroring the port.
The new module unit `hle/online_version.c` formats
`dhd-port <DHD_VERSION> src1-<fingerprint>` for both the menu and the scripted online path.
A missing generated header fails compilation; there is no `unknown` fallback. The object
cache preprocesses headers, so a changed fingerprint invalidates the formatter's cached object.

The menu and server still compare identities exactly. Release minimums, protocol and disc
checks are unchanged. The log's first line keeps its native executable hash for diagnostics.
No server deployment is needed. Both players must rebuild: existing executable-hash clients
are deliberately incompatible, even with the same release number.

## Validation

- `python port/tools/test_online_identity.py`: 8 tests pass. Equivalent LF/CRLF trees in
  different locations agree; local edits, added/removed/renamed code and version/protocol
  changes separate builds; generated outputs are ignored; missing roots fail closed.
  The actual C formatter runs in two distinct native executables with different SHA-256
  hashes and returns the same identity, within the protocol's 64-byte string buffer.
  A native syntax check of the complete online unit makes implicit calls fatal, protecting
  the declarations of main's new high-rate timing helpers during integration.
- `npm test` in `online/server`: all 28 tests pass. Two new cases test a shared identity
  with Windows/macOS host/guest names in both directions, its public listing, and rejection
  of different source identities, legacy executable identities and different disc hashes.
  These are protocol tests, not actual Windows/macOS game sessions.
- `npm run typecheck`: passes. No production server code changed.
- `git diff --check`: passes.

No full game build was run: this checkout lacks extracted retail inputs, and this machine
does not have LLVM/WASI/WABT/SIMDe/Rust installed. A real race in both host directions remains
to be validated after rebuilding both clients.

## Checking on different platforms

Build each client using `docs/BUILDING.md`. Before building, run
`python port/tools/online_identity.py` on each checkout: the `src1-...` output must agree.
Rebuild the core after source changes. Test public rooms, joining by code and an actual race
in both host directions. Published executable-hash clients cannot join these rooms.

## Evidence and limits

Sources inspected locally: `port/hle/online.c:357-378`,
`port/hle/online_menu.c:441-449`, `online/server/src/room.ts:348-379`,
`online/server/test/room.test.ts` (different-version refusal), and
`docs/online-protocol.md` (`hello.version`, `clientVersion`, `version_mismatch`).
Research before implementation:
[Reproducible Builds: deterministic build systems](https://reproducible-builds.org/docs/deterministic-build-systems/),
[timestamps](https://reproducible-builds.org/docs/timestamps/) and
[build-environment variations](https://reproducible-builds.org/docs/env-variations/)
inform the use of stable inputs and exclusion of timestamps and absolute paths. No code was
copied from those references.
