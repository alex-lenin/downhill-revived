// run_headless.mjs's graphics thread for a threaded build (port/web/build.py --threads): the GS
// reactor on the core's shared memory in a Node worker thread, running the queued graphics calls
// (port/web/web_queue.h) as www/graphics.js does in the browser, without WebGPU (the device ops are
// never enabled here, as in run_headless.mjs's one-thread mode).
import { parentPort, workerData } from 'node:worker_threads';
import { WASI } from 'node:wasi';
import { Q, QI } from './www/graphics_common.js';

const { memory, module, env, abi, preopens } = workerData;
let gx;
function fail(err) {
  const q = new Int32Array(memory.buffer, Q, 128);
  Atomics.store(q, QI.failed, 1); Atomics.notify(q, QI.consumed);
  parentPort.postMessage({ type: 'error', text: String(err?.stack || err) });
}
try {
  const wasi = new WASI({ version: 'preview1', args: [], env, preopens, returnOnExit: true });
  const noGpu = n => () => { throw Error(n + ' without WebGPU'); };
  const renderStubs = Object.fromEntries(WebAssembly.Module.imports(module).filter(i => i.module === 'dhd_render').map(i => [i.name, noGpu('dhd_render.' + i.name)]));
  const gi = await WebAssembly.instantiate(module, { env: { memory, host_wall_seconds: () => gx.web_field() / 60,
    web_gpu_await: noGpu('web_gpu_await') }, dhd_render: renderStubs, wasi_snapshot_preview1: wasi.wasiImport });
  gx = gi.exports;
  wasi.initialize(gi);
  if (gx.web_q_init() !== Q) throw Error('graphics: the reactor\'s queue is not at the address run_headless.mjs expects');
  if (gx.web_q_abi() !== abi) throw Error('graphics: the reactor and gqueue.wasm come from different builds');
  parentPort.on('message', m => { if (m.type === 'report') parentPort.postMessage({ type: 'report', vu_native: gx.web_vu_native?.(),
    vu_steps: gx.web_vu_stat?.(0), vu_blocks: gx.web_vu_stat?.(1),
    vu_ieee_probe: gx.web_vu_ieee_probe && [0, 1, 2, 3].map(i => gx.web_vu_ieee_probe(i)), probe: [0, 1, 2, 3].map(i => gx.web_vu_probe?.(i)) }); });
  parentPort.postMessage({ type: 'ready' });
  const run = WebAssembly.promising(gx.web_q_run);
  for (;;) {
    const r = await run();
    if (r === 0) {
      const q = new Int32Array(memory.buffer, Q, 128);
      const w = Atomics.waitAsync(q, QI.written, Atomics.load(q, QI.consumed), 1000);
      if (w.async) await w.value;
    } else if (r === 2) {
      // The reactor's environment (WASI's copy, read at its next environ_get) is fixed in Node's
      // WASI; DHD_* lines the core sets later do not reach it, as in the one-thread runner.
    } else if (r !== 1) throw Error(`graphics: web_q_run answered ${r}`);
  }
} catch (e) { fail(e); }
