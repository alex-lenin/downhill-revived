// Simulates a lockstep race against a running server (wrangler dev or deployed) and reports
// relay latency. Node 22+ (global WebSocket), no dependencies.
//
//   node tools/loadtest.mjs [--url ws://127.0.0.1:8787] [--players 4] [--fields 600] [--desync]
//
// Each player creates/joins one room, readies up, and after "start" runs fields at 59.94 Hz:
// on field F it sends its input for F + delay, together with the two previous fields (the
// protocol's redundancy), and every 60 fields a HASH. A player only advances when it holds every
// racer's input for the field (real lockstep), so stalls show up in the report. --desync makes
// the last player report a different hash from field 299 on.

const args = Object.fromEntries(
  process.argv.slice(2).map((a, i, all) => (a.startsWith("--") ? [a.slice(2), all[i + 1]?.startsWith("--") ? true : (all[i + 1] ?? true)] : null)).filter(Boolean),
);
const BASE = (args.url ?? "ws://127.0.0.1:8787").replace(/\/$/, "");
const PLAYERS = Number(args.players ?? 4);
const FIELDS = Number(args.fields ?? 600);
const FIELD_MS = 1000 / 59.94;
const DESYNC = args.desync === true || args.desync === "true";

const VERSION = "dhd-port loadtest";
const ISO = "c".repeat(64);

/** Deterministic test input for (slot, field). */
function inputFor(slot, field) {
  const x = (field * 2654435761 + slot * 40503) >>> 0;
  return [x & 0xffff, (x >>> 16) & 0xff, (x >>> 24) & 0xff, 0x80, 0x80];
}

function inputFrame(slot, field, count) {
  const b = new Uint8Array(7 + 6 * count);
  const v = new DataView(b.buffer);
  b[0] = 1;
  b[1] = slot;
  v.setUint32(2, field, true);
  b[6] = count;
  for (let i = 0; i < count; i++) {
    const [buttons, lx, ly, rx, ry] = inputFor(slot, field + i);
    v.setUint16(7 + 6 * i, buttons, true);
    b.set([lx, ly, rx, ry], 9 + 6 * i);
  }
  return b;
}

function hashFrame(slot, field, hash) {
  const b = new Uint8Array(14);
  const v = new DataView(b.buffer);
  b[0] = 2;
  b[1] = slot;
  v.setUint32(2, field, true);
  v.setBigUint64(6, hash, true);
  return b;
}

class Player {
  constructor(name) {
    this.name = name;
    this.queue = [];
    this.waiters = [];
    this.inputs = new Map(); // `${slot}:${field}` -> true
    this.latencies = []; // one-way relay ms (sender and receiver share this process's clock)
    this.mismatches = 0;
    this.duplicates = 0;
    this.events = [];
  }

  open(path) {
    return new Promise((resolve, reject) => {
      this.ws = new WebSocket(BASE + path);
      this.ws.binaryType = "arraybuffer";
      this.ws.onopen = () => resolve();
      this.ws.onerror = (e) => reject(new Error(`${this.name}: ${e.message ?? "socket error"}`));
      this.ws.onclose = (e) => (this.closed = e.code);
      this.ws.onmessage = (e) => this.onMessage(e.data);
    });
  }

  onMessage(data) {
    if (typeof data !== "string") return this.onBinary(new Uint8Array(data));
    const m = JSON.parse(data);
    if (m.type === "ping") this.send({ type: "pong", s: m.s });
    if (m.type === "desync" || m.type === "left" || m.type === "error") this.events.push(m);
    this.queue.push(m);
    for (const w of this.waiters.splice(0)) w();
  }

  onBinary(b) {
    const now = performance.now();
    if (b[0] === 4) {
      const v = new DataView(b.buffer);
      this.pingRtts.push(now - v.getFloat64(2, true));
      return;
    }
    if (b[0] !== 1) return;
    const v = new DataView(b.buffer);
    const slot = b[1];
    const first = v.getUint32(2, true);
    for (let i = 0; i < b[6]; i++) {
      const field = first + i;
      const key = `${slot}:${field}`;
      if (this.inputs.has(key)) {
        this.duplicates++;
        continue;
      }
      const want = inputFor(slot, field);
      const got = [v.getUint16(7 + 6 * i, true), ...b.subarray(9 + 6 * i, 13 + 6 * i)];
      if (want.some((x, j) => x !== got[j])) this.mismatches++;
      this.inputs.set(key, true);
      const sent = sendTimes.get(key);
      if (sent !== undefined) this.latencies.push(now - sent);
    }
  }

  send(obj) {
    this.ws.send(JSON.stringify(obj));
  }

  async next(type) {
    for (;;) {
      const i = this.queue.findIndex((m) => m.type === type);
      if (i >= 0) return this.queue.splice(i, 1)[0];
      if (this.closed !== undefined) throw new Error(`${this.name}: closed ${this.closed}`);
      await new Promise((r) => this.waiters.push(r));
    }
  }
}

/** When each (slot, field) input was first sent, for one-way latency. */
const sendTimes = new Map();

function pct(values, p) {
  if (!values.length) return NaN;
  const s = [...values].sort((a, b) => a - b);
  return s[Math.min(s.length - 1, Math.floor((p / 100) * s.length))];
}
const ms = (x) => `${x.toFixed(2)} ms`;

async function main() {
  const health = await (await fetch(BASE.replace(/^ws/, "http") + "/v1/health")).json();
  console.log(`server ${BASE}: protocol ${health.protocol}`);

  const players = [];
  const host = new Player("P0");
  await host.open("/v1/rooms/new");
  host.send({ type: "hello", protocol: 1, version: VERSION, isoSha256: ISO, name: "P0" });
  const { code } = await host.next("welcome");
  host.send({ type: "settings", maxPlayers: Math.max(2, PLAYERS), track: 1 });
  players.push(host);
  for (let i = 1; i < PLAYERS; i++) {
    const p = new Player(`P${i}`);
    await p.open(`/v1/rooms/${code}`);
    p.send({ type: "hello", protocol: 1, version: VERSION, isoSha256: ISO, name: p.name });
    await p.next("welcome");
    players.push(p);
  }
  console.log(`room ${code}: ${PLAYERS} players joined`);

  // Let a couple of server pings land so the delay is measured, then ready and start.
  await new Promise((r) => setTimeout(r, 2500));
  for (const p of players) p.send({ type: "ready", ready: true });
  for (;;) {
    const room = await host.next("room");
    if (room.members.length === PLAYERS && room.members.every((m) => m.ready)) break;
  }
  host.send({ type: "start" });
  const starts = await Promise.all(players.map((p) => p.next("start")));
  starts.forEach((s, i) => (players[i].slot = s.yourSlot));
  const delay = starts[0].inputDelay;
  console.log(`started: input delay ${delay} fields, seed ${starts[0].seed}`);

  // Lockstep loop, one per player, all on this process's event loop.
  const t0 = performance.now() + 200;
  let stallFields = 0;
  await Promise.all(
    players.map(async (p) => {
      p.pingRtts = [];
      let field = 0; // next field to run
      while (field < FIELDS) {
        const due = t0 + field * FIELD_MS;
        const wait = due - performance.now();
        if (wait > 1) await new Promise((r) => setTimeout(r, wait));
        // Send our input for field + delay with the two previous ones.
        const target = field + delay;
        const first = Math.max(delay, target - 2);
        const now = performance.now();
        for (let f = first; f <= target; f++) if (!sendTimes.has(`${p.slot}:${f}`)) sendTimes.set(`${p.slot}:${f}`, now);
        p.ws.send(inputFrame(p.slot, first, target - first + 1));
        // Run the field only once every other racer's input for it is here.
        let stalled = false;
        while (field >= delay && !players.every((o) => o === p || p.inputs.has(`${o.slot}:${field}`))) {
          stalled = true;
          await new Promise((r) => setTimeout(r, 1));
        }
        if (stalled) stallFields++;
        if (field % 60 === 59) {
          const bad = DESYNC && p === players[PLAYERS - 1] && field >= 299;
          p.ws.send(hashFrame(p.slot, field, BigInt(field) * 0x9e3779b97f4a7c15n % 2n ** 64n ^ (bad ? 1n : 0n)));
        }
        if (field % 30 === 0) {
          const b = new Uint8Array(10);
          b[0] = 3;
          new DataView(b.buffer).setFloat64(2, performance.now(), true);
          p.ws.send(b);
        }
        field++;
      }
    }),
  );
  const elapsed = performance.now() - t0;
  await new Promise((r) => setTimeout(r, 300));

  const lat = players.flatMap((p) => p.latencies);
  const rtt = players.flatMap((p) => p.pingRtts);
  const expected = FIELDS * (PLAYERS - 1); // fields delay .. FIELDS+delay-1 from each other racer
  console.log(`\n${FIELDS} fields in ${(elapsed / 1000).toFixed(2)} s (${((FIELDS / elapsed) * 1000).toFixed(2)} fields/s)`);
  for (const p of players)
    console.log(
      `  slot ${p.slot}: ${p.inputs.size}/${expected} inputs, ${p.mismatches} wrong, ${p.duplicates} redundant copies ignored`,
    );
  console.log(`relay one-way (sender -> server -> receiver), ${lat.length} inputs:`);
  console.log(`  min ${ms(Math.min(...lat))}  median ${ms(pct(lat, 50))}  p95 ${ms(pct(lat, 95))}  p99 ${ms(pct(lat, 99))}  max ${ms(Math.max(...lat))}`);
  console.log(`binary PING round-trip, ${rtt.length} samples: median ${ms(pct(rtt, 50))}  p99 ${ms(pct(rtt, 99))}`);
  console.log(`fields that had to wait for inputs: ${stallFields} of ${FIELDS * PLAYERS}`);
  const desyncs = host.events.filter((e) => e.type === "desync");
  console.log(`desync events: ${desyncs.length ? desyncs.map((d) => `field ${d.field}`).join(", ") : "none"}`);

  for (const p of players) p.ws.close(1000);
  const ok =
    players.every((p) => p.inputs.size === expected && p.mismatches === 0) &&
    (DESYNC ? desyncs.length === 1 && desyncs[0].field === 299 : desyncs.length === 0);
  console.log(ok ? "\nOK" : "\nFAILED");
  process.exit(ok ? 0 : 1);
}

main().catch((e) => {
  console.error(e);
  process.exit(1);
});
