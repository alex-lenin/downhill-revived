"""The browser's threaded build (build.py --threads; finding 087, BS2): the module rewrites it needs.

The game core and the graphics reactor run in two Web Workers on one WebAssembly.Memory, which must
then be a shared memory (wasm threads). The core module itself stays as build_wasm.py links it (the
x64 build's wasm2c input and its offset scan are untouched): only the browser's copy gets the
shared flag, here, after postlink.py.

  share(src, dst)   every memory the module defines or imports becomes shared, with the maximum
                    the core already has (2 GiB). The code needs nothing else: plain loads and
                    stores are valid on a shared memory, and only the queue (web_queue.c) uses
                    atomics. Each module is still instantiated once (the core in the game's worker,
                    the reactor and the Rust renderer in the graphics worker), so their active data
                    segments are applied once, as before.
  glue(js)          wasm-bindgen's glue (the Rust renderer's dhd_render.js, built for an unshared
                    memory) renews a cached view only when its buffer was detached (byteLength 0).
                    A shared memory's buffer is never detached: after a memory.grow the view would
                    keep the old length. The views are renewed when the memory's buffer object
                    changes instead (what wasm-bindgen emits for shared memories).
"""
import re
import sys
from pathlib import Path

MAX_PAGES = 32768   # 2 GiB: build_wasm.py's MAX_MEMORY, build_graphics.py's --max-memory


def uleb(b, i):
    n = shift = 0
    while True:
        x = b[i]
        i += 1
        n |= (x & 0x7F) << shift
        shift += 7
        if not x & 0x80:
            return n, i


def enc(n):
    out = bytearray()
    while True:
        x = n & 0x7F
        n >>= 7
        out.append(x | (0x80 if n else 0))
        if not n:
            return bytes(out)


def _limits(b, i, what, maximum):
    flags = b[i]
    mn, i = uleb(b, i + 1)
    mx = None
    if flags & 1:
        mx, i = uleb(b, i)
    if flags & ~3:
        sys.exit('threads: %s: memory flags 0x%x (memory64?) not supported' % (what, flags))
    mx = maximum if mx is None else mx
    if mx > maximum or mn > mx:
        sys.exit('threads: %s: limits %d..%d pages do not fit a %d-page shared memory' % (what, mn, mx, maximum))
    return bytes([3]) + enc(mn) + enc(mx), i


def share(src, dst, maximum=MAX_PAGES):
    """dst = src with its memories shared. Returns how many memories it changed."""
    data = Path(src).read_bytes()
    if data[:8] != b'\0asm\x01\0\0\0':
        sys.exit('threads: %s is not a wasm module' % src)
    out, pos, count = bytearray(data[:8]), 8, 0
    while pos < len(data):
        sid = data[pos]
        size, j = uleb(data, pos + 1)
        body, end = data[j:j + size], j + size
        if sid == 2:
            n, i = uleb(body, 0)
            entries = []
            for _ in range(n):
                start = i
                length, i = uleb(body, i)
                i += length
                length, i = uleb(body, i)
                i += length
                kind = body[i]
                i += 1
                if kind == 2:
                    head = body[start:i]
                    limits, i = _limits(body, i, '%s import' % src, maximum)
                    entries.append(head + limits)
                    count += 1
                    continue
                if kind == 0:
                    _, i = uleb(body, i)
                elif kind == 1:
                    flags = body[i + 1]
                    _, i = uleb(body, i + 2)
                    if flags & 1:
                        _, i = uleb(body, i)
                elif kind == 3:
                    i += 2
                elif kind == 4:
                    _, i = uleb(body, i + 1)
                else:
                    sys.exit('threads: %s: import kind %d' % (src, kind))
                entries.append(body[start:i])
            body = enc(n) + b''.join(entries)
        elif sid == 5:
            n, i = uleb(body, 0)
            memories = []
            for _ in range(n):
                limits, i = _limits(body, i, '%s memory' % src, maximum)
                memories.append(limits)
                count += 1
            body = enc(n) + b''.join(memories)
        out += bytes([sid]) + enc(len(body)) + body
        pos = end
    Path(dst).write_bytes(bytes(out))
    return count


def glue(js):
    """Renews wasm-bindgen's cached memory views on a new buffer object, not on a detached one."""
    text = Path(js).read_text(encoding='utf-8')
    new, n = re.subn(r'\b(cached\w+Memory0)\.byteLength === 0\b', r'\1.buffer !== wasm.memory.buffer', text)
    if not n and 'byteLength === 0' in text:
        sys.exit('threads: %s: a cached view check this script does not know' % js)
    if 'byteLength === 0' in new:
        sys.exit('threads: %s: a view check is left unchanged' % js)
    Path(js).write_text(new, encoding='utf-8', newline='')
    return n


if __name__ == '__main__':
    for path in sys.argv[1:]:
        print(path, share(path, path))
