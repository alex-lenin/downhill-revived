// The VU worker of the threaded build (port/web/build.py; finding 090, SP4): VIF1 and VU1
// (build/web/vu.wasm) on the core's shared memory, as the native build's vu thread. worker.js
// starts it after the GS worker (graphics.js) and hands over the memory, the compiled module and
// the environment. It runs the core's queue (port/web/web_queue.h): the VIF1 packets and the VU1
// calls here, everything else passed on, in order, to the GS worker's queue. It never needs its
// event loop, so it waits inside the module (memory.atomic.wait32) and comes back here only when
// the core changed the environment, or after 0.1 s without work.
import { shareSafeDecoding, Q, Q2, QI } from './graphics_common.js';

const post = (type, data) => postMessage({ type, ...data });
const say = text => post('log', { text });
onmessage = e => { if (e.data.type === 'start') start(e.data).catch(fail); };
let memory = null;
function fail(err) {
  // The core or the VU worker's own module may wait for the other workers: they trap instead.
  if (memory) for (const at of [Q, Q2]) { const q = new Int32Array(memory.buffer, at, 128); Atomics.store(q, QI.failed, 1); Atomics.notify(q, QI.consumed); }
  post('error', { text: 'VU worker: ' + String(err?.stack || err) });
}

async function start(m) {
  shareSafeDecoding();
  memory = m.memory;
  const env = { ...m.env };
  let bytes, dv;
  const refresh = () => { bytes = new Uint8Array(memory.buffer); dv = new DataView(memory.buffer); };
  refresh();
  let vx;
  const enc = new TextEncoder(), envList = () => Object.entries(env).map(([k, v]) => enc.encode(`${k}=${v}\0`));
  const wasi = {
    environ_sizes_get: (count, size) => { refresh(); const l = envList(); dv.setUint32(count, l.length, true); dv.setUint32(size, l.reduce((s, b) => s + b.length, 0), true); return 0; },
    environ_get: (ptrs, buf) => { refresh(); for (const b of envList()) { dv.setUint32(ptrs, buf, true); ptrs += 4; bytes.set(b, buf); buf += b.length; } return 0; },
    fd_write: (fd, iovs, n, written) => { refresh(); let out = ''; let total = 0;
      for (let i = 0; i < n; i++) { const p = dv.getUint32(iovs + 8 * i, true), l = dv.getUint32(iovs + 8 * i + 4, true); out += new TextDecoder().decode(bytes.subarray(p, p + l)); total += l; }
      dv.setUint32(written, total, true); if (out.trim()) say(out.trimEnd()); return 0; },
    fd_close: () => 0, fd_fdstat_get: () => 8, fd_fdstat_set_flags: () => 8, fd_prestat_get: () => 8, fd_prestat_dir_name: () => 8,
    clock_time_get: (id, precision, out) => { refresh(); dv.setBigUint64(out, BigInt(Math.round(performance.now() * 1e6)), true); return 0; },
    fd_read: () => 8, fd_seek: () => 8, path_open: () => 44, proc_exit: code => { throw Error(`VU proc_exit(${code})`); },
  };
  const vi = await WebAssembly.instantiate(m.module, {
    env: { memory, host_wall_seconds: () => vx.web_field() / 60 }, wasi_snapshot_preview1: wasi });
  vx = vi.exports;
  vx._initialize?.();
  if (vx.web_vq_init() !== Q) throw Error('VU: the queue is not at the address worker.js expects, or the GS worker\'s is not there');
  if (vx.web_q_abi() !== m.abi) throw Error('VU: vu.wasm and gqueue.wasm come from different builds');
  post('ready', {});
  for (;;) {
    const r = vx.web_vq_run();
    if (r === 0) { await new Promise(r => setTimeout(r, 0)); continue; }   // idle for 0.1 s: a turn of the event loop
    if (r !== 2) throw Error(`VU: web_vq_run answered ${r}`);
    // the core changed its environment (host_putenv): this worker's copy, read again at the next getenv
    const q = new Int32Array(memory.buffer, Q, 128);
    const text = new TextDecoder().decode(new Uint8Array(memory.buffer, q[QI.env_text] >>> 0, q[QI.env_bytes]).slice());
    const i = text.indexOf('=');
    env[text.slice(0, i)] = text.slice(i + 1);
    vx.web_vu_env_changed();
  }
}
