// Shared by worker.js and port/web/run_headless.mjs.
// A tiny wasm module of call_indirect thunks: the core imports these, and their table holds the
// GS reactor's web_x_* exports, so core -> reactor calls have no JS frame between them and a
// readback can suspend (JSPI) through both modules.
export function thunks(names, metadata) {
  const code = { u32: 0x7f, u64: 0x7e, f32: 0x7d, f64: 0x7c };
  const leb = n => { const o = []; do { let b = n & 127; n >>>= 7; if (n) b |= 128; o.push(b); } while (n); return o; };
  const str = s => [...leb(s.length), ...new TextEncoder().encode(s)];
  const section = (id, body) => [id, ...leb(body.length), ...body];
  const vec = items => [...leb(items.length), ...items.flat()];
  const sigs = [], typeOf = names.map(n => { const m = metadata[n];
    const key = JSON.stringify(m); let i = sigs.findIndex(s => s.key === key);
    if (i < 0) { i = sigs.length; sigs.push({ key, m }); } return i; });
  const types = vec(sigs.map(({ m }) => [0x60, ...vec(m.params.map(p => [code[p]])), ...(m.result === 'void' ? [0] : [1, code[m.result]])]));
  const imports = vec([[...str('env'), ...str('t'), 0x01, 0x70, 0x00, ...leb(names.length)]]);
  const funcs = vec(typeOf.map(t => leb(t)));
  const exports = vec(names.map((n, i) => [...str(n), 0x00, ...leb(i)]));
  const bodies = vec(names.map((n, i) => { const p = metadata[n].params;
    const body = [0, ...p.flatMap((_, j) => [0x20, ...leb(j)]), 0x41, ...leb(i), 0x11, ...leb(typeOf[i]), 0x00, 0x0b];
    return [...leb(body.length), ...body]; }));
  return new Uint8Array([0, 0x61, 0x73, 0x6d, 1, 0, 0, 0, ...section(1, types), ...section(2, imports),
    ...section(3, funcs), ...section(7, exports), ...section(10, bodies)]);
}
