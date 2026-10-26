// Compile WGSL files with Chrome's WebGPU compiler (Tint) and print its messages (finding 084).
//
//   node port/tools/tint_check.mjs port/gs/raster.wgsl port/gs/present.wgsl
//
// Headless Chromium (port/web/cdp.mjs's runPage: Playwright's, DXC off; CHROME, CHROME_FLAGS and
// CHROME_LOG as there) loads an empty page from a localhost server (WebGPU needs a secure context),
// creates a shader module per file and reads getCompilationInfo(); then raster.wgsl's entry points are built as compute pipelines
// with the backend's override constants. Exit 1 on any error. The browser is this process's child,
// stopped by its PID; its profile stays under build/.
import { readFileSync } from 'node:fs';
import { basename } from 'node:path';
import { setTimeout as sleep } from 'node:timers/promises';
import { runPage } from '../web/cdp.mjs';

const files = process.argv.slice(2);
if (!files.length) { console.error('usage: tint_check.mjs FILE.wgsl ...'); process.exit(2); }

await runPage((req, res) => { res.setHeader('content-type', 'text/html'); res.end('<!doctype html><title>tint</title>'); },
  async ({ url, send: call, evaluate }) => {
  let status = 1;
  // The tab can still be about:blank (no WebGPU there): navigate and wait for the page itself.
  await call('Page.navigate', { url });
  for (let i = 0; i < 100; i++) {
    if (await evaluate('location.href') === url) break;
    await sleep(100);
  }
  const sources = Object.fromEntries(files.map(f => [basename(f), readFileSync(f, 'utf8')]));
  const script = `(async () => {
    const sources = ${JSON.stringify(sources)}, out = [];
    const adapter = await navigator.gpu?.requestAdapter();
    if (!adapter) return { error: 'no WebGPU adapter at ' + location.href + ' secure=' + isSecureContext + ' gpu=' + !!navigator.gpu };
    const device = await adapter.requestDevice();
    const info = adapter.info || {};
    out.push('adapter: ' + [info.vendor, info.architecture, info.description].filter(Boolean).join(' '));
    let errors = 0;
    for (const [name, code] of Object.entries(sources)) {
      const module = device.createShaderModule({ code });
      const messages = (await module.getCompilationInfo()).messages;
      for (const m of messages) { out.push(name + ':' + m.lineNum + ':' + m.linePos + ': ' + m.type + ': ' + m.message); errors += m.type === 'error'; }
      out.push(name + ': ' + messages.filter(m => m.type === 'error').length + ' errors');
      if (name === 'raster.wgsl' && !messages.some(m => m.type === 'error')) {
        const c = { EARLY_DEPTH: 1, ADDRESS_SHIFT: 1, EXCLUSIVE_WORD_STORE: 1 };
        for (const [entryPoint, constants] of [['main', c], ['local_copy', { ADDRESS_SHIFT: 1 }], ['scanout', {}], ['depth_probe', {}]]) {
          device.pushErrorScope('validation');
          await device.createComputePipelineAsync({ layout: 'auto', compute: { module, entryPoint, constants } })
            .catch(e => out.push(name + ' ' + entryPoint + ': ' + e.message));
          const e = await device.popErrorScope();
          out.push(name + ' pipeline ' + entryPoint + ': ' + (e ? 'ERROR ' + e.message : 'ok'));
          errors += !!e;
        }
      }
    }
    return { out, errors };
  })()`;
  const r = await call('Runtime.evaluate', { expression: script, awaitPromise: true, returnByValue: true });
  const v = r.result?.result?.value;
  if (!v || v.error) { console.log('failed:', JSON.stringify(v?.error || r.result?.exceptionDetails || r)); }
  else if (!v.out) console.log('unexpected:', JSON.stringify(r).slice(0, 2000));
  else { console.log(v.out.join('\n')); status = v.errors ? 1 : 0; }
  return status;
}, { open: true });
