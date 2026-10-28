// The memory card's folders and files (port/hle/mc.c: "<exe dir>/saves\cardN\...") in memory,
// kept in the browser's IndexedDB: loaded before the game starts, each changed file written back
// when the game closes or flushes it (the write finishes while the worker waits for the next field).
const DB = 'downhill-revived', STORE = 'files';
const key = p => p.replace(/\\/g, '/').replace(/\/+/g, '/').replace(/\/$/, '').toLowerCase();
const parent = k => k.slice(0, Math.max(0, k.lastIndexOf('/')));

function open() {
  return new Promise((resolve, reject) => {
    const r = indexedDB.open(DB, 1);
    r.onupgradeneeded = () => r.result.createObjectStore(STORE);
    r.onsuccess = () => resolve(r.result); r.onerror = () => reject(r.error);
  });
}

export async function createVfs(log) {
  let db = null;
  const nodes = new Map();  // key -> { name, dir } or { name, data: Uint8Array, time }
  try {
    db = await open();
    await new Promise((resolve, reject) => {
      const r = db.transaction(STORE).objectStore(STORE).openCursor();
      r.onsuccess = () => { const c = r.result; if (!c) return resolve(); nodes.set(c.key, c.value); c.continue(); };
      r.onerror = () => reject(r.error);
    });
  } catch (e) { log(`saves stay in memory only (${e})`); }
  const store = (k, value) => { if (!db) return;
    const t = db.transaction(STORE, 'readwrite'); value ? t.objectStore(STORE).put(value, k) : t.objectStore(STORE).delete(k); };
  const vfs = {
    count: () => [...nodes.values()].filter(n => !n.dir).length,
    isDir: p => !!nodes.get(key(p))?.dir,
    exists: p => nodes.has(key(p)),
    mkdir(p) { const k = key(p); if (nodes.has(k)) return false;
      const n = { name: p.replace(/\\/g, '/').split('/').pop(), dir: true, time: Date.now() }; nodes.set(k, n); store(k, n); return true; },
    // mode: host_api.h's HOST_FILE_* bits
    open(p, mode) { const k = key(p); let n = nodes.get(k);
      if (n?.dir) return null;
      if (mode & 4) { n = { name: p.replace(/\\/g, '/').split('/').pop(), data: new Uint8Array(0), time: Date.now() }; nodes.set(k, n); }
      else if (!n) return null;
      return { k, n, pos: 0, dirty: !!(mode & 4), write: !!(mode & 2) }; },
    read(f, out) { const d = f.n.data, n = Math.max(0, Math.min(out.length, d.length - f.pos)); out.set(d.subarray(f.pos, f.pos + n)); f.pos += n; return n; },
    write(f, src) { if (!f.write) return 0; const end = f.pos + src.length;
      if (end > f.n.data.length) { const grown = new Uint8Array(Math.max(end, f.n.data.length * 2)); grown.set(f.n.data); f.n.data = grown.subarray(0, end); }
      f.n.data.set(src, f.pos); f.pos = end; f.dirty = true; f.n.time = Date.now(); return src.length; },
    flush(f) { if (f.dirty) { f.n.data = f.n.data.slice(); store(f.k, f.n); f.dirty = false; } },
    size: f => f.n.data.length,
    // The folder's entries: ".", ".." then its children (mc.c counts them as the card does).
    list(p) { const k = key(p); if (!nodes.get(k)?.dir) return null;
      const self = nodes.get(k), out = [{ name: '.', dir: true, time: self.time }, { name: '..', dir: true, time: self.time }];
      for (const [ck, n] of nodes) if (parent(ck) === k) out.push(n);
      return out; },
  };
  return vfs;
}
