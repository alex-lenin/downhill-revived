// Headless Chrome over the DevTools protocol: the plumbing of check_browser.mjs and port/tools'
// tint_check.mjs and web_gs_replay.mjs (each keeps its own page and checks).
import { spawn } from 'node:child_process';
import { createServer } from 'node:http';
import { mkdirSync, rmSync } from 'node:fs';
import { fileURLToPath } from 'node:url';

// Headless Chrome, this process's child (browser.kill() stops it by its PID), on profile (emptied first when fresh),
// opening url; log copies its stderr to ours. { browser, wsUrl, port } once its DevTools endpoint listens; after 30 s
// without that, the browser is stopped and the promise rejects.
export async function startChrome({ chrome, profile, fresh = false, flags = [], url = 'about:blank', log = false }) {
  if (fresh) rmSync(profile, { recursive: true, force: true });
  mkdirSync(profile, { recursive: true });
  const browser = spawn(chrome, ['--headless=new', '--remote-debugging-port=0', `--user-data-dir=${profile}`, '--no-first-run',
    ...flags, url], { stdio: ['ignore', 'ignore', 'pipe'] });
  let text = '', done = false;
  const wsUrl = await new Promise((resolve, reject) => {
    const fail = e => { if (!done) { done = true; browser.kill(); reject(e); } };
    browser.stderr.on('data', d => { if (log) process.stderr.write(d); if (done) return; text += d;
      const m = /DevTools listening on (ws:\S+)/.exec(text); if (m) { done = true; resolve(m[1]); } });
    browser.on('error', fail); setTimeout(() => fail(Error('chrome did not start')), 30000).unref();
  });
  return { browser, wsUrl, port: new URL(wsUrl).port };
}

// A DevTools connection: send(method, params, sessionId) resolves with the reply (other messages go to events);
// evaluate(expression, { sessionId, awaitPromise }) with the expression's value.
export async function connect(wsUrl) {
  const ws = new WebSocket(wsUrl); await new Promise((r, j) => { ws.addEventListener('open', r); ws.addEventListener('error', j); });
  let id = 0; const pending = new Map(), events = [];
  ws.addEventListener('message', e => { const m = JSON.parse(e.data); if (m.id && pending.has(m.id)) { pending.get(m.id)(m); pending.delete(m.id); } else events.push(m); });
  const send = (method, params = {}, sessionId) => new Promise(r => { const i = ++id; pending.set(i, r); ws.send(JSON.stringify({ id: i, method, params, sessionId })); });
  const evaluate = async (expression, { sessionId, awaitPromise } = {}) =>
    (await send('Runtime.evaluate', { expression, returnByValue: true, awaitPromise }, sessionId)).result?.result?.value;
  return { ws, send, evaluate, events };
}

// The port/tools checks' run: handler served on 127.0.0.1 (WebGPU needs a secure context); Playwright's Chromium
// (CHROME overrides; no dxil.dll, so Dawn runs with DXC off; CHROME_FLAGS adds flags, CHROME_LOG shows its stderr)
// on a fresh build/tint-check-profile/<pid>, opening the page when open (else about:blank), its tab connected.
// body({ url, send, evaluate, ... }) returns the exit status; Chrome and the server are stopped whatever happens.
export async function runPage(handler, body, { open = false } = {}) {
  const server = createServer(handler); await new Promise(r => server.listen(0, '127.0.0.1', r));
  const url = `http://127.0.0.1:${server.address().port}/`;
  let status = 1, browser;
  try {
    const c = await startChrome({ chrome: process.env.CHROME || 'C:/Program Files/Google/Chrome/Application/chrome.exe',
      profile: fileURLToPath(new URL(`../../build/tint-check-profile/${process.pid}`, import.meta.url)), fresh: true,
      flags: ['--enable-unsafe-webgpu', '--disable-dawn-features=use_dxc', '--no-default-browser-check',
        ...(process.env.CHROME_FLAGS || '').split(' ').filter(Boolean)], url: open ? url : 'about:blank', log: !!process.env.CHROME_LOG });
    browser = c.browser;
    const page = (await (await fetch(`http://127.0.0.1:${c.port}/json/list`)).json()).find(t => t.type === 'page');
    const { ws, ...cdp } = await connect(page.webSocketDebuggerUrl);
    status = await body({ url, ...cdp });
    ws.close();
  } finally { browser?.kill(); server.close(); }
  process.exit(status);
}
