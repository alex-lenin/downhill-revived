# `.optrace`: the exact renderer's device-op trace

A change that breaks old traces bumps `version` and keeps the reader for the old one.

An `.optrace` records, in order, every device op that the exact front end (`port/gs/gpu_exact.inc`)
makes through its `op_*` seam (the plan's D1), plus what each read and capture returned on the C
path. A replayer runs the same ops on another implementation of the seam (Rust natively:
`port/render/tools/src/bin/optrace_replay.rs`; the browser: `port/tools/web_optrace_replay.mjs`) and
compares every returned byte. This tests the Rust device ops before any C glue exists.

- **Recorder:** the `DHD_GS_OPTRACE` block in `gpu_exact.inc` (task P0-4). It is off unless
  `DHD_GS_OPTRACE=<path>` is set. When off it costs one branch per op and changes nothing.
  The first exact instance of a process writes `<path>`; instance N (N >= 1, counted from 0)
  writes `<path>.N`.
- **Goldens:** `$GOLD/optrace/{test_gpu-warp-early0, test_gpu-hw, native_transfers-exact, g+0,
  g+3600}.optrace`. They are game-derived (g+0, g+3600) or adapter-tied, so they are never
  committed.
- **Determinism:** the same program, input, environment and adapter must record byte-identical
  traces (P0-4's referee records twice and compares). So the trace holds no time, pid, pointer,
  absolute path or uninitialised byte. Payload bytes are exactly the bytes passed to the op.

## Layout

All integers are little-endian. Offsets are in bytes. Every record starts on a 4-byte boundary.

### Header

| Offset | Type | Field |
|---|---|---|
| 0 | `u8[8]` | magic `"DHDOPTR\0"` |
| 8 | `u32` | `version` = 1 |
| 12 | `u32` | `header_bytes`: total header size, a multiple of 4, padding included |
| 16 | `u32` | `raster_flags`: `DHDR_RASTER_*` (1 early depth, 2 address shift, 4 exclusive word store), as the instance's pipelines were built (`DHD_GS_EARLY_DEPTH`, `DHD_GS_ADDRESS_SHIFT`, `DHD_GS_EXCLUSIVE_WORD_STORE`) |
| 20 | `u32` | `api`: 1 DX12, 2 Vulkan, 3 browser WebGPU |
| 24 | `u32` | `vendor_id` (PCI) |
| 28 | `u32` | `device_id` (PCI) |
| 32 | `u32` | `compiler`: 0 none / unknown, 1 FXC, 2 DXC |
| 36 | `u32` | `fallback_adapter`: 1 when the adapter is a CPU / fallback one (WARP), else 0 |
| 40 | `u64` | `driver_version`: DXGI UMD version, Vulkan `driverVersion`, or 0 |
| 48 | `u8[40]` | `recorder_sha`: ASCII hex git blob SHA-1 of the `gpu_exact.inc` that recorded (the build passes it in as `DHD_GS_OPTRACE_SHA`; 40 `'0'` when unknown) |
| 88 | `u32` | `adapter_bytes`, then that many bytes of UTF-8 adapter name (no NUL) |
| ... | `u32` | `source_bytes`, then that many bytes of UTF-8: the program's base name and the caller's label, for example `test_gpu warp-early0` (no paths) |
| ... | | zero padding up to `header_bytes` |

A replayer must check `magic`, `version` and `header_bytes`. It prints the header. It refuses (exit
2, not a pass) a trace with `version` it does not know.

### Records

Each record is a `u32 tag`, a `u32 payload_bytes`, then the payload, then zero padding up to a
multiple of 4. `payload_bytes` excludes the padding. Field names follow `dhd_render.h`'s
`dhdr_exact_*` functions.

| Tag | Op | Payload |
|---|---|---|
| 1 | `WRITE` | `u32 buffer, u32 offset, u32 bytes`, then `bytes` data bytes. Queue write: lands before the next submitted command buffer. |
| 2 | `COPY` | `u32 from, u32 from_offset, u32 to, u32 to_offset, u32 bytes`. Ends the open compute pass, records a buffer copy. |
| 3 | `BATCH` | `u32 dynamic_offset, u32 groups_x, u32 groups_y`. `main` over one batch record at `dynamic_offset` (a multiple of 256) in the open pass, opened if needed. |
| 4 | `RECORD` | `u32 pipeline, u32 x_word, u32 y_word`. Ends the open pass and dispatches `pipeline` (1 `local_copy`, 2 `scanout`) in its own pass. For scanout, `x_word = groups | width << 16` and `y_word = groups | height << 16`. |
| 5 | `SUBMIT` | none. Ends the pass and submits the open encoder (no-op when nothing is recorded, still traced). |
| 6 | `READ` | `u32 buffer` (7 readback or 8 staged), `u32 want[16]` (a page bitmap: page p is bit `p & 31` of word `p >> 5`; 512 pages of 8192 bytes), `u32 result` (1 ok, 0 failed), then, when `result` is 1, 8192 bytes for every set page in increasing page order: what the C path got back. |
| 7 | `CAPTURE` | `u32 width, u32 height` (0, 0 = no display texture), then `width * height * 4` bytes: the display texture's RGBA8 pixels as `dhdr_exact_capture_finish` returns them (rows unpadded). |
| 8 | `DESTROY` | none. The instance is destroyed (it submits pending work first). |
| 15 | `END` | `u64 records` (records before END), `u64 fnv`, `u32 counts[8]` (records with tags 0-7; 0 is unused). |

Buffer numbers are `DHDR_B_*`, the same as `gpu_web.c`'s:

| # | Buffer | Size |
|---|---|---|
| 0 | vram | 4 MiB |
| 1 | snapshot | 4 MiB |
| 2 | states | `MAX_STATES * sizeof(Constants)` (1024 x 3088) |
| 3 | primitives | `MAX_PRIMITIVES * sizeof(Primitive)` (16384 x 208) |
| 4 | bins | `MAX_BIN_WORDS * 4` (16 MiB) |
| 5 | display constants | 48 |
| 6 | copy constants | 48 |
| 7 | readback | 4 MiB (map read) |
| 8 | staged | 4 MiB (map read) |
| 9 | batches | `MAX_BATCHES * BATCH_STRIDE` (4096 x 256) |
| 10 | upload | 4 MiB |

The instance is created with the header's `raster_flags` before the first record. A buffer starts
zeroed. Only tags 1-8 and 15 are valid in version 1. Any other tag is an error.

### END and integrity

- `fnv` is FNV-1a 64 (offset basis `0xcbf29ce484222325`, prime `0x100000001b3`) over every byte
  from the first record's tag up to, but not including, END's tag. Padding is included.
- A trace without END (truncated), with bytes after END, with a wrong `records`, `counts` or
  `fnv`, or with a record that runs past the end of the file is invalid. The replayer fails it
  before running any op, so a reordered, dropped or edited record can never pass.
- Recording a READ or CAPTURE never changes what the program sees: the recorder copies the
  returned bytes after the op.

## Replay rule

A replay passes only when all of these hold:

1. The header and integrity checks pass.
2. Every op ran in trace order on one exact instance created with the header's `raster_flags`.
3. Every `READ` returned the same `result`, and for `result == 1` the same bytes on every set
   page. The replayer reads into a zero-filled 4 MiB image and compares only the set pages.
4. Every `CAPTURE` returned the same size and the same bytes.
5. It printed `compared N reads (P pages), M captures` with N, P and M equal to the trace's
   counts.

There is no tolerance option. On the first mismatch the replayer prints the record index, the op,
the page and the first differing word offset, then exits 1. It keeps going only with `--all`, and
still exits 1.

The exact results are designed to be adapter-independent (test_gpu shows WARP = hardware), so a
replay on another adapter must also match. For example, the browser replays the native traces on
Dawn. A replayer may print an adapter difference, but must not refuse on it. WARP replays still need
`raster_flags` without early depth (WARP's JIT fault, finding 084 §10.5.6). The replayer runs with
IEEE float control on every thread that calls the device (MXCSR `0x1F80` class). 
