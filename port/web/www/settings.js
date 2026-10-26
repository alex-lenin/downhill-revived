// PC settings in the browser: the text of a dhd.ini (port/hle/host.c's format and keys) kept in
// this browser's IndexedDB, under the name DHD_SETTINGS gives ("dhd.ini" for players; "none" or
// "0": no settings read or saved, the tools' rule, finding 056). The worker reads it before the
// game starts and sets the DHD_* variables it stands for, as host_settings_load does natively (a
// variable already set wins: the page's ?env.NAME and the browser's own fixed ones); each
// host_setting_save rewrites one line, keeps the others and stores the text again. The disc is
// never stored: it stays the player's own file, picked again at each start.
const DB = 'downhill-revived-settings', STORE = 'ini';

// host.c's apply_setting: dhd.ini keys and the variables they set.
const PLAIN = {
  iso: 'DHD_ISO', skip_movies: 'DHD_SKIP_MOVIES', window: 'DHD_WINDOW', width: 'DHD_WINDOW_WIDTH', height: 'DHD_WINDOW_HEIGHT',
  monitor: 'DHD_MONITOR', scale: 'DHD_SCALE', widescreen: 'DHD_WIDESCREEN', vibration: 'DHD_VIBRATION',
  memory_card: 'DHD_MEMCARD',
  sound_latency_ms: 'DHD_SND_LATENCY_MS', server: 'DHD_NET_SERVER', name: 'DHD_NET_NAME', rider: 'DHD_NET_RIDER',
  update_url: 'DHD_UPDATE_URL', auto_update: 'DHD_AUTO_UPDATE', texture_pack: 'DHD_TEXTURE_PACK', textures: 'DHD_TEXTURES',
  texture_cache_mb: 'DHD_TEXTURE_CACHE_MB', font: 'DHD_FONT', fps: 'DHD_FPS',
};
// The browser's OPTIONS > PC SETTINGS (pc_settings.c's DHD_PC_SETTINGS_* variables):
// - ROWS, the rows shown: Resolution (DHD_SCALE: 1x to DHD_SCALE_MAX, the reactor's NATIVE_SCALE_MAX,
//   or Window, the canvas's match), Scaling, Widescreen, Font and Frame Rate. Display, window size,
//   picture shape (the canvas has one shape: 16:9 with widescreen, else 4:3, main.js), texture
//   packs, skip movies (the browser never plays them) and vibration (no rumble here) are left out.
// - LIVE, the rows that apply at once here besides the core's live ones: Resolution (the reactor's
//   native_rescale, through web_env_changed). Scaling is live everywhere.
// - RESTART, the footer of the others (Widescreen, Font, Frame Rate): the page loaded again.
// - FPS_MAX: the browser's Frame Rate list and Monitor stop at 144 (the core's own cap is 240).
// Unset rows keep the core's defaults (2x, Smooth, the sharp font, 60) or BROWSER_DEFAULTS.
export const BROWSER_PC_SETTINGS = {
  DHD_PC_SETTINGS_ROWS: 'DHD_SCALE,DHD_PRESENT_FILTER,DHD_WIDESCREEN,DHD_FONT,DHD_FPS',
  DHD_PC_SETTINGS_LIVE: 'DHD_SCALE',
  DHD_PC_SETTINGS_RESTART: '* Applies when the page is loaded again',
  DHD_SCALE_MAX: '8',
  DHD_FPS_MAX: '144',
};
// A player's settings where the stored dhd.ini doesn't say, as host.c's player_defaults on the PC:
// widescreen on (widescreen.c then shows the picture "wide"), 60, 2x, the sharp font. Only with
// stored settings (DHD_SETTINGS=none keeps the core's defaults for checks); a stored line or
// ?env. wins (createSettings fills what is still unset, after the stored lines).
export const BROWSER_DEFAULTS = { DHD_WIDESCREEN: '1', DHD_FPS: '60', DHD_SCALE: '2', DHD_FONT: 'sharp' };
// The variables the reactor follows while the game runs (gpu_rust_web.c's web_env_changed).
export const RENDERER_LIVE = new Set(['DHD_SCALE', 'DHD_PRESENT_FILTER', 'DHD_ASPECT']);
const TEMPLATE = '; Downhill Revived settings (browser): one "key = value" per line, written by the game\n' +
  '; (OPTIONS > PC SETTINGS and the game\'s options). Kept in this browser only.\n';

export const enabled = name => !!name && !/^(none|0)$/i.test(name);
const unquote = v => { if (v[0] === '"') { const e = v.indexOf('"', 1); if (e > 0) return v.slice(1, e); }
  const c = /(^|[ \t])[;#]/.exec(v); return c ? v.slice(0, c.index).trim() : v; };
// The text's settings in order: [key, value] for each "key = value" line (host_settings_load's rules).
export function parse(text) {
  const out = [];
  for (const raw of (text || '').split('\n')) {
    const s = raw.trim(), eq = s.indexOf('=');
    if (!s || ';#['.includes(s[0]) || eq < 0) continue;
    const key = s.slice(0, eq).trim(), value = unquote(s.slice(eq + 1).trim());
    if (key && value) out.push([key, value]);
  }
  return out;
}
// The first line's value for key, case-insensitive (host_game_options_read).
export const lookup = (text, key) => parse(text).find(([k]) => k.toLowerCase() === key.toLowerCase())?.[1];
// host.c's settings_save: replaces the key's first line, else adds one; every other line stays.
function saveLine(text, key, value) {
  const quote = /[;#"]/.test(value) || value[0] === ' ' ? '"' : '', line = `${key} = ${quote}${value}${quote}`;
  const lines = (text || TEMPLATE).split('\n'); if (lines[lines.length - 1] === '') lines.pop();
  const at = lines.findIndex(l => { const s = l.trim(), eq = s.indexOf('=');
    return eq > 0 && !';#['.includes(s[0]) && s.slice(0, eq).trim().toLowerCase() === key.toLowerCase(); });
  if (at >= 0) lines[at] = line; else lines.push(line);
  return lines.join('\n') + '\n';
}

function open() {
  return new Promise((resolve, reject) => {
    const r = indexedDB.open(DB, 1);
    r.onupgradeneeded = () => r.result.createObjectStore(STORE);
    r.onsuccess = () => resolve(r.result); r.onerror = () => reject(r.error);
  });
}
const request = r => new Promise((resolve, reject) => { r.onsuccess = () => resolve(r.result); r.onerror = () => reject(r.error); });
// The stored text ('' when there is none); also for the page, which shapes the canvas at Start.
export async function readIni(name) {
  const db = await open();
  try { return (await request(db.transaction(STORE).objectStore(STORE).get(name))) || ''; } finally { db.close(); }
}

// The worker's settings: loads name's text into env (variables not set yet), then serves the game.
// overrides: the variables the page set (?env.NAME), which the settings page shows locked.
export async function createSettings(env, overrides, log) {
  const name = env.DHD_SETTINGS, on = enabled(name);
  let text = '', db = null;
  if (on) try { db = await open(); text = (await request(db.transaction(STORE).objectStore(STORE).get(name))) || ''; }
  catch (e) { log(`settings: kept for this session only (${e})`); }
  const set = (k, v) => { if (env[k] === undefined) env[k] = v; };
  for (const [key, value] of parse(text)) {
    const lower = key.toLowerCase();
    if (lower === 'game_options' || lower === 'iso') continue;  // read after the game's load; the disc is the page's
    if (lower.startsWith('dhd_')) set(key, value);
    else if (lower === 'renderer') set('DHD_RENDER', value.toLowerCase() === 'native' ? 'native' : 'exact');
    else if (PLAIN[lower]) set(PLAIN[lower], value);
    else log(`settings: unknown key ${key}`);
  }
  if (on) for (const [key, value] of Object.entries(BROWSER_DEFAULTS)) set(key, value);
  if (on) log(`settings: ${parse(text).length} from ${name} in this browser`);
  const store = () => { if (!db) return;
    const t = db.transaction(STORE, 'readwrite'); t.objectStore(STORE).put(text, name);
    t.onerror = () => log(`settings: not saved (${t.error})`); };
  return {
    // host_setting_save: nonzero when kept (the write finishes while the game goes on).
    save(key, value) {
      if (!on) return 0;
      if (key.toLowerCase() === 'iso') return 1;  // never the disc
      text = saveLine(text, key, value); store(); return 1;
    },
    locked: variable => +Object.prototype.hasOwnProperty.call(overrides || {}, variable),
    gameOptions: () => on ? lookup(text, 'game_options') : undefined,
  };
}
