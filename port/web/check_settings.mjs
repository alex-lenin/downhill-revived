// The browser's dhd.ini against the native game's (finding 102). Serves the build (serve.mjs: the page,
// settings.js and build/dhd_settings.wasm), opens the page in headless Chrome (no window) and runs
// every case of the crate's corpus, port/render/settings/tests/web_corpus.json (whose results
// tests/web.rs keeps equal to what the crate does natively: the game's start and its saves), through
// settings.js with this browser's own storage:
// - the case's text stored in IndexedDB as bytes (a case without one: no entry);
// - createSettings, the game worker's start: the variables it sets must be the native game's
//   without settings.js's BROWSER_WITHOUT, the keys it logs as unknown the native ones;
// - one host_setting_save: the stored bytes afterwards must be the native save's.
// Then DHD_SETTINGS=none (nothing read or stored) and the locked rows.
// --iso PATH: then the game's own settings page, with the disc (served to this machine only): the
// first case's text stored, the page loaded with ?iso=test and a DHD_NAV route to OPTIONS > PC
// SETTINGS, Right on Resolution (scale 2 -> 3) and Cross; the stored bytes must become that case's
// native save of scale = 3, every other line kept. Exit 0 when all hold.
//   node port/web/check_settings.mjs [--iso PATH [--seconds 300]] [--port 8194] [--chrome PATH] [--user-data DIR]
import { spawn } from 'node:child_process';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { startChrome, connect } from './cdp.mjs';
const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '../..');
const args = process.argv.slice(2), option = (n, f) => { const i = args.indexOf(n); return i < 0 ? f : args[i + 1]; };
const port = Number(option('--port', 8194));
const chrome = option('--chrome', 'C:/Program Files/Google/Chrome/Application/chrome.exe');
const profile = path.resolve(option('--user-data', path.join(root, 'build/web/settings-check-profile')));
const iso = option('--iso', null), seconds = Number(option('--seconds', 300));
// finding 086's route: title, main menu (15) to OPTIONS, OPTIONS (2) to PC SETTINGS, PC SETTINGS (13)
const route = '0:triangle|cross,15:down,15:down,15:cross,2:up,2:cross,13:right,13:cross';
const corpus = JSON.parse(fs.readFileSync(path.join(root, 'port/render/settings/tests/web_corpus.json'), 'utf8'));
if (!fs.existsSync(path.join(root, 'build/web/dhd_settings.wasm'))) { console.error('no build/web/dhd_settings.wasm: python port/web/build.py'); process.exit(2); }

// In the page: one case through settings.js. Texts are bytes, one character a byte.
async function inPage(cases) {
  const s = await import('/settings.js');
  const bytes = t => Uint8Array.from(t, c => c.charCodeAt(0)), text = b => b ? String.fromCharCode(...b) : null;
  const db = await new Promise((ok, no) => { const r = indexedDB.open('downhill-revived-settings', 1);
    r.onupgradeneeded = () => r.result.createObjectStore('ini'); r.onsuccess = () => ok(r.result); r.onerror = () => no(r.error); });
  const done = t => new Promise((ok, no) => { t.oncomplete = ok; t.onerror = () => no(t.error); });
  const put = (name, b) => { const t = db.transaction('ini', 'readwrite'); b === null ? t.objectStore('ini').delete(name) : t.objectStore('ini').put(b, name); return done(t); };
  const get = name => new Promise((ok, no) => { const r = db.transaction('ini').objectStore('ini').get(name); r.onsuccess = () => ok(r.result); r.onerror = () => no(r.error); });
  const results = [];
  for (const [i, c] of cases.entries()) {
    const name = `check-${i}.ini`;
    await put(name, c.stored ? bytes(c.text) : null);
    const env = { DHD_SETTINGS: name, ...Object.fromEntries(c.set.map(n => [n, 'set'])) }, before = new Set(Object.keys(env)), logs = [];
    const settings = await s.createSettings(env, l => logs.push(l));
    const vars = Object.fromEntries(Object.entries(env).filter(([k]) => !before.has(k)));
    const ok = settings.save(c.save[0], c.save[1]);
    await settings.written();
    results.push({ vars, ok, without: [...s.BROWSER_WITHOUT],
      unknown: logs.filter(l => l.startsWith('settings: unknown key ')).map(l => l.slice(22)),
      saved: text(await get(name)), gameOptions: settings.gameOptions() ?? null,
      locked: Object.fromEntries(['DHD_FPS', 'DHD_WIDESCREEN', 'DHD_SCALE'].map(v => [v, settings.locked(v)])) });
  }
  // DHD_SETTINGS=none: no defaults read, nothing stored
  const env = { DHD_SETTINGS: 'none' }, none = await s.createSettings(env, () => {});
  results.push({ none: { save: none.save('scale', '3'), stored: (await get('none')) ?? null, scale: env.DHD_SCALE ?? null } });
  db.close();
  return results;
}

// In the page: the stored bytes of name (one character a byte), or put them first when given.
async function storedIni(name, put) {
  const db = await new Promise((ok, no) => { const r = indexedDB.open('downhill-revived-settings', 1);
    r.onupgradeneeded = () => r.result.createObjectStore('ini'); r.onsuccess = () => ok(r.result); r.onerror = () => no(r.error); });
  try {
    if (put !== undefined) await new Promise((ok, no) => { const t = db.transaction('ini', 'readwrite');
      t.objectStore('ini').put(Uint8Array.from(put, c => c.charCodeAt(0)), name); t.oncomplete = ok; t.onerror = () => no(t.error); });
    const v = await new Promise((ok, no) => { const r = db.transaction('ini').objectStore('ini').get(name); r.onsuccess = () => ok(r.result); r.onerror = () => no(r.error); });
    return v === undefined ? null : String.fromCharCode(...new Uint8Array(v));
  } finally { db.close(); }
}

const server = spawn(process.execPath, [path.join(root, 'port/web/serve.mjs'), '--port', String(port), ...(iso ? ['--iso', iso] : [])],
  { stdio: ['ignore', 'pipe', 'inherit'] });
await new Promise(r => server.stdout.once('data', r));
const { browser, wsUrl } = await startChrome({ chrome, profile, fresh: true,
  flags: iso ? ['--enable-unsafe-webgpu', '--autoplay-policy=no-user-gesture-required', '--window-size=1400,1150'] : [] });
let failed = 0;
try {
  const { send, evaluate, events } = await connect(wsUrl);
  const { result: { targetId } } = await send('Target.createTarget', { url: 'about:blank' });
  const { result: { sessionId } } = await send('Target.attachToTarget', { targetId, flatten: true });
  await send('Runtime.enable', {}, sessionId); await send('Page.enable', {}, sessionId);
  await send('Page.navigate', { url: `http://localhost:${port}/` }, sessionId);
  for (let t = 0; t < 100 && await evaluate('document.readyState', { sessionId }) !== 'complete'; t++) await new Promise(r => setTimeout(r, 100));
  const results = await evaluate(`(${inPage})(${JSON.stringify(corpus)})`, { sessionId, awaitPromise: true });
  if (!Array.isArray(results)) {
    for (const e of events) if (e.method === 'Runtime.exceptionThrown') console.log('exception', JSON.stringify(e.params.exceptionDetails).slice(0, 800));
    throw Error(`the page gave no results (${JSON.stringify(results)})`);
  }
  const sorted = o => JSON.stringify(Object.keys(o).sort().map(k => [k, o[k]]));
  for (const [i, c] of corpus.entries()) {
    const r = results[i], without = new Set(r.without), problems = [];
    const want = Object.fromEntries(c.vars.filter(([k]) => !without.has(k)));
    if (sorted(r.vars) !== sorted(want)) problems.push(`variables ${sorted(r.vars)}, the native game's ${sorted(want)}`);
    if (JSON.stringify(r.unknown) !== JSON.stringify(c.unknown)) problems.push(`unknown keys ${r.unknown}, native ${c.unknown}`);
    if (r.ok !== 1) problems.push(`save returned ${r.ok}`);
    if (r.saved !== c.saved) problems.push(`saved ${JSON.stringify(r.saved)}, native ${JSON.stringify(c.saved)}`);
    if (c.set.includes('DHD_FPS') && (r.locked.DHD_FPS !== 1 || r.locked.DHD_SCALE !== 0)) problems.push(`locked ${JSON.stringify(r.locked)}`);
    failed += problems.length > 0;
    console.log(`${problems.length ? 'FAIL' : 'ok  '} ${c.name}: ${Object.keys(r.vars).length} variables, save ${c.save.join(' = ')}`);
    for (const p of problems) console.log('     ' + p);
    if (i === 0) {
      // the round trip, line by line: only the saved setting's line differs
      const a = c.text.split('\n'), b = (r.saved ?? '').split('\n'), changed = a.map((l, k) => [l, b[k]]).filter(([x, y]) => x !== y);
      console.log(`     ${a.length - changed.length} of ${a.length} lines byte for byte, changed: ${changed.map(([x, y]) => `${JSON.stringify(x)} -> ${JSON.stringify(y)}`).join(', ')}`);
      console.log(`     game_options ${JSON.stringify(r.gameOptions)}`);
      if (b.length !== a.length || changed.length !== 1 || r.gameOptions !== '1 800') { failed++; console.log('FAIL the round trip changed more than one line'); }
    }
  }
  const n = results[corpus.length].none;
  const noneOk = n.save === 0 && n.stored === null && n.scale === null;
  failed += !noneOk;
  console.log(`${noneOk ? 'ok  ' : 'FAIL'} DHD_SETTINGS=none: ${JSON.stringify(n)}`);
  if (iso) {
    // the game's settings page: the first case's text, Resolution one step right
    const c = corpus[0], name = 'game-check.ini';
    if (c.save.join('=') !== 'scale=3' || !/^scale = 2\b/m.test(c.text)) throw Error('the first corpus case is no longer scale 2 -> 3');
    await evaluate(`(${storedIni})(${JSON.stringify(name)}, ${JSON.stringify(c.text)})`, { sessionId, awaitPromise: true });
    await send('Page.navigate', { url: `http://localhost:${port}/?iso=test&env.DHD_SETTINGS=${name}&env.DHD_NAV=${encodeURIComponent(route)}` }, sessionId);
    const t0 = Date.now(); let stored = c.text, status = {}, changedAt = 0;
    while ((Date.now() - t0) / 1000 < seconds) {
      await new Promise(r => setTimeout(r, 2000));
      stored = await evaluate(`(${storedIni})(${JSON.stringify(name)})`, { sessionId, awaitPromise: true });
      status = JSON.parse(await evaluate('JSON.stringify({s: window.dhdStatus, text: document.getElementById("status").textContent, log: document.getElementById("log").textContent})', { sessionId }) || '{}');
      if (/stopped|exited/.test(status.text || '')) break;
      if (stored !== c.text && !changedAt) changedAt = Date.now();
      if (changedAt && Date.now() - changedAt > 6000) break;   // the page's Cross and any later save
    }
    const lines = (status.log || '').split('\n').filter(l => /^(settings|pc settings)/.test(l));
    const ok = stored === c.saved;
    failed += !ok;
    console.log(`${ok ? 'ok  ' : 'FAIL'} the game's PC SETTINGS (field ${status.s?.field}, ${((Date.now() - t0) / 1000).toFixed(0)} s): ${lines.join(' | ')}`);
    if (!ok) console.log(`     stored ${JSON.stringify(stored)}, native ${JSON.stringify(c.saved)}`);
    else {
      const a = c.text.split('\n'), b = stored.split('\n'), changed = a.map((l, k) => [l, b[k]]).filter(([x, y]) => x !== y);
      console.log(`     ${a.length - changed.length} of ${a.length} lines byte for byte, changed: ${changed.map(([x, y]) => `${JSON.stringify(x)} -> ${JSON.stringify(y)}`).join(', ')}`);
    }
  }
} finally { browser.kill(); server.kill(); }
console.log(failed ? `check_settings: FAIL (${failed})` : `check_settings: OK (${corpus.length} corpus cases, the native game's results)`);
process.exit(failed ? 1 : 0);
