// The page: picks the ISO, hands the canvas to the game worker, and feeds it keys, gamepads and
// sound through SharedArrayBuffers (the worker never returns to its event loop mid-field, so
// messages would wait). Needs a cross-origin isolated page: serve.mjs sends COOP/COEP.
import { readIni, lookup } from './settings.js';
const $ = id => document.getElementById(id);
const status = $('status'), log = $('log'), canvas = $('screen');
const params = new URLSearchParams(location.search);
const addLog = text => { log.textContent += text + '\n'; log.scrollTop = log.scrollHeight; };

if (!crossOriginIsolated) status.textContent = 'This page needs cross-origin isolation: start it with port/web/serve.mjs.';
const input = new SharedArrayBuffer(256 + 16 * 4);
const keys = new Uint8Array(input, 0, 256), pads = new Int32Array(input, 256, 16);
const audio = new SharedArrayBuffer(8 + 48000 / 4 * 4);  // 0.25 s of 48 kHz stereo

// KeyboardEvent.code -> the Windows virtual-key code keyboard.c's table uses (US places).
function vk(code) {
  const named = { ArrowLeft: 0x25, ArrowUp: 0x26, ArrowRight: 0x27, ArrowDown: 0x28, Space: 0x20, Enter: 0x0d,
    NumpadEnter: 0x0d, Escape: 0x1b, Tab: 0x09, Backspace: 0x08 };
  if (named[code]) return named[code];
  const m = /^Key([A-Z])$/.exec(code); return m ? m[1].charCodeAt(0) : 0;
}
let running = false;
addEventListener('keydown', e => { const k = vk(e.code); if (!k || !running || e.altKey || e.ctrlKey || e.metaKey) return;
  Atomics.store(keys, k, 1); e.preventDefault(); });
addEventListener('keyup', e => { const k = vk(e.code); if (k) Atomics.store(keys, k, 0); });
addEventListener('blur', () => keys.fill(0));

// Standard-mapping gamepads -> PS2 buttons (libpad bits) and sticks (0..255, 128 centred).
const GP = [16384, 8192, 32768, 4096, 1024, 2048, 256, 512, 1, 8, 2, 4, 16, 64, 128, 32];
function pollPads() {
  const list = [...(navigator.getGamepads?.() || [])].filter(p => p && p.mapping === 'standard').slice(0, 2);
  for (let port = 0; port < 2; port++) {
    const p = list[port], o = port * 8;
    if (!p) { Atomics.store(pads, o, 0); continue; }
    let b = 0; p.buttons.forEach((x, i) => { if (x.pressed && GP[i]) b |= GP[i]; });
    const axis = v => Math.max(0, Math.min(255, Math.round(128 + (Math.abs(v) < 0.12 ? 0 : v) * 127.5)));
    Atomics.store(pads, o + 1, b);
    Atomics.store(pads, o + 2, axis(p.axes[2] ?? 0)); Atomics.store(pads, o + 3, axis(p.axes[3] ?? 0));
    Atomics.store(pads, o + 4, axis(p.axes[0] ?? 0)); Atomics.store(pads, o + 5, axis(p.axes[1] ?? 0));
    Atomics.store(pads, o, 1);
  }
  requestAnimationFrame(pollPads);
}

// PC SETTINGS > Display (settings.js): Borderless and Fullscreen are the canvas in fullscreen, Window
// the page. The browser allows it only just after a click or key (the Start click, the key that
// changed the row); otherwise the page stays as it is.
const fullscreen = mode => /^(borderless|fullscreen)$/i.test(mode || '');
function display(mode) {
  if (fullscreen(mode) && !document.fullscreenElement) canvas.requestFullscreen?.().catch(e => addLog(`fullscreen: ${e.message}`));
  else if (!fullscreen(mode) && document.fullscreenElement) document.exitFullscreen().catch(() => {});
  window.dhdDisplay = mode;
}
let savedWindow = null;  // the stored Display, read before Start so its click can go fullscreen
readIni('dhd.ini').then(text => { savedWindow = lookup(text, 'window') ?? null; }, () => {});

async function start(source) {
  $('start').disabled = true; $('iso').disabled = true; running = true;
  if (!source.isoUrl && fullscreen(savedWindow)) display(savedWindow);
  try {
    const context = new AudioContext({ sampleRate: 48000, latencyHint: 'interactive' });
    await context.audioWorklet.addModule('audio.js');
    new AudioWorkletNode(context, 'dhd-ring', { outputChannelCount: [2], processorOptions: { ring: audio } }).connect(context.destination);
    context.resume();
  } catch (e) { addLog(`no sound: ${e}`); }
  const worker = new Worker('worker.js', { type: 'module' });
  const offscreen = canvas.transferControlToOffscreen();
  worker.onmessage = e => { const m = e.data;
    if (m.type === 'log') addLog(m.text);
    else if (m.type === 'error') { addLog('ERROR ' + m.text); status.textContent = 'stopped: ' + m.text.split('\n')[0]; }
    else if (m.type === 'running') status.textContent = 'running';
    else if (m.type === 'display') display(m.mode);
    else if (m.type === 'exit') status.textContent = `game exited (${m.status})`;
    else if (m.type === 'status') { status.textContent = `field ${m.field}  ${m.fps.toFixed(1)} fields/s  picture ${m.size}  draws ${m.draws}  batches ${m.dispatches}  readbacks ${m.reads}  sound ${m.sound} s${m.riding ? '  riding' : ''}`;
      document.title = `DHD ${m.fps.toFixed(1)} fps (field ${m.field})`; window.dhdStatus = m; (window.dhdHistory ??= []).push([m.field, +m.fps.toFixed(2), m.draws]); }
  };
  worker.onerror = e => addLog('worker error: ' + e.message);
  worker.postMessage({ type: 'start', ...source, canvas: offscreen, input, audio }, [offscreen]);
  canvas.focus();
  pollPads();
}

$('iso').onchange = () => { $('start').disabled = !$('iso').files.length; };
$('start').onclick = () => start({ file: $('iso').files[0], env });
// Test hook: serve.mjs --iso <path> serves that file as /test-iso to localhost only, and
// ?iso=test starts with it at once (headless checks; the ISO never leaves the machine).
// ?env.NAME=value sets a DHD_* variable for the game (tests: DHD_NAV, DHD_RACE_PAD scripts).
const env = Object.fromEntries([...params].filter(([k]) => k.startsWith('env.')).map(([k, v]) => [k.slice(4), v]));
// Tests read and save no settings unless they name them (?env.DHD_SETTINGS=<name>), as the tools'
// DHD_SETTINGS=none natively.
if (params.get('iso') === 'test') start({ isoUrl: '/test-iso', env: { DHD_SETTINGS: 'none', ...env } });
