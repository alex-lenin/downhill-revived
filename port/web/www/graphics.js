// The graphics worker of the threaded build (port/web/build.py --threads; finding 087, BS2): the GS
// reactor (VIF1/VU1, the GS front end) and the Rust renderer, which owns the WebGPU device and the
// canvas, on the core's shared memory. worker.js starts it and hands over the memory, the compiled
// reactor, the canvas and the environment; the core then sends every graphics call through the
// queue in that memory (port/web/web_queue.h) and this worker runs them in order: web_q_run until
// the queue is empty, a vsync was drawn (shown here, then the task ends so the canvas commits) or
// the environment changed. A readback suspends web_q_run (JSPI) while the core waits for it.
import { shareSafeDecoding, startRenderer, renderImports, Q, QI, STAT } from './graphics_common.js';

const post = (type, data) => postMessage({ type, ...data });
const say = text => post('log', { text });
onmessage = e => { if (e.data.type === 'start') start(e.data).catch(fail); };
let memory = null;
function fail(err) {
  // The core may be waiting for this worker: it traps instead of waiting for ever.
  if (memory) { const q = new Int32Array(memory.buffer, Q, 128); Atomics.store(q, QI.failed, 1); Atomics.notify(q, QI.consumed); }
  post('error', { text: 'graphics worker: ' + String(err?.stack || err) });
}

async function start(m) {
  shareSafeDecoding();
  memory = m.memory;
  const canvas = m.canvas, env = { ...m.env };
  let bytes, dv;
  const refresh = () => { bytes = new Uint8Array(memory.buffer); dv = new DataView(memory.buffer); };
  refresh();
  const renderer = await startRenderer(memory, canvas, say);
  say(`WebGPU: ${renderer.adapter} (${renderer.version}), in the graphics worker`);
  let gx;
  const enc = new TextEncoder(), envList = () => Object.entries(env).map(([k, v]) => enc.encode(`${k}=${v}\0`));
  const wasi = {
    environ_sizes_get: (count, size) => { refresh(); const l = envList(); dv.setUint32(count, l.length, true); dv.setUint32(size, l.reduce((s, b) => s + b.length, 0), true); return 0; },
    environ_get: (ptrs, buf) => { refresh(); for (const b of envList()) { dv.setUint32(ptrs, buf, true); ptrs += 4; bytes.set(b, buf); buf += b.length; } return 0; },
    fd_write: (fd, iovs, n, written) => { refresh(); let out = ''; let total = 0;
      for (let i = 0; i < n; i++) { const p = dv.getUint32(iovs + 8 * i, true), l = dv.getUint32(iovs + 8 * i + 4, true); out += new TextDecoder().decode(bytes.subarray(p, p + l)); total += l; }
      dv.setUint32(written, total, true); if (out.trim()) say(out.trimEnd()); return 0; },
    fd_close: () => 0, fd_fdstat_get: () => 8, fd_fdstat_set_flags: () => 8, fd_prestat_get: () => 8, fd_prestat_dir_name: () => 8,
    fd_read: () => 8, fd_seek: () => 8, path_open: () => 44, proc_exit: code => { throw Error(`graphics proc_exit(${code})`); },
  };
  const R = renderer.R;
  const gi = await WebAssembly.instantiate(m.module, {
    env: { memory, host_wall_seconds: () => gx.web_field() / 60,
      web_gpu_await: new WebAssembly.Suspending(() => R.dhdr_web_pending()) },
    dhd_render: renderImports(m.module, renderer.rx), wasi_snapshot_preview1: wasi });
  gx = gi.exports;
  gx._initialize?.();
  if (!gx.web_gpu_enable()) throw Error('GS: the Rust renderer did not start');
  if (gx.web_q_init() !== Q) throw Error('graphics: the reactor\'s queue is not at the address worker.js expects');
  if (gx.web_q_abi() !== m.abi) throw Error('graphics: the reactor and gqueue.wasm come from different builds');
  post('ready', { rust: true });

  // Each vsync's picture ends a task (the canvas commits then), as worker.js's pacing yield does.
  const channel = new MessageChannel(); let wake = null; channel.port1.onmessage = () => { const w = wake; wake = null; w?.(); };
  const yieldTask = () => new Promise(r => { wake = r; channel.port2.postMessage(0); });
  const run = WebAssembly.promising(gx.web_q_run);
  let shown = 0;
  for (;;) {
    const r = await run();
    const q = new Int32Array(memory.buffer, Q, 128);
    if (r === 0) {   // empty: wait for the core's next packet
      const w = Atomics.waitAsync(q, QI.written, Atomics.load(q, QI.consumed), 1000);
      if (w.async) await w.value;
    } else if (r === 1) {   // a vsync: show its picture, then end the task
      const size = q[QI.display] >>> 0;
      if (size && gx.web_q_present(canvas.width, canvas.height) === 1) shown++;
      const s = q.subarray(QI.stats);
      s[STAT.shown] = shown; s[STAT.size] = size;
      for (const [k, i] of [['draws', 0], ['dispatches', 1], ['reads', 2], ['submits', 3], ['scale', 4]]) s[STAT[k]] = gx.web_gpu_stats(i);
      await yieldTask();
    } else if (r === 2) {   // the core changed its environment (host_putenv): the reactor reads this one
      const text = new TextDecoder().decode(new Uint8Array(memory.buffer, q[QI.env_text] >>> 0, q[QI.env_bytes]).slice());
      const i = text.indexOf('=');
      env[text.slice(0, i)] = text.slice(i + 1);
    } else throw Error(`graphics: web_q_run answered ${r}`);
  }
}
