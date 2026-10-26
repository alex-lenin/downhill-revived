// Independent controls for the fast-path certificate and bulk span classifier.
import fs from 'node:fs';
import assert from 'node:assert/strict';
const memory=new WebAssembly.Memory({initial:28864});
const {instance:{exports:e}}=await WebAssembly.instantiate(
  fs.readFileSync('build/web/memory.wasm'),{env:{memory}});
for(const a of [0x00100000,0x06000000,0x08000000,0x70100000])e.web_assert_unfolded(a,0,16);
for(const a of [0x00400000,0x20400000,0x28000000,0x30400000,0x38000000])
  assert.throws(()=>e.web_assert_unfolded(a,0,16),WebAssembly.RuntimeError);
assert.throws(()=>e.web_assert_unfolded(0x003fffff,0,16),WebAssembly.RuntimeError);
assert.throws(()=>e.web_assert_unfolded(0x003fffff,0,0x01c00002),WebAssembly.RuntimeError);
console.log('PASS unfolded certificate accepts canonical addresses and rejects mirrors/boundaries');
const u=new Uint8Array(memory.buffer);
u[0x00400000]=0x12;u[0x08400000]=0x34;
e.web_copy(0x10000000,0x003fffff,0x01c00002);
assert.equal(u[0x10000001],0x34);
e.web_fill(0x003fffff,0xab,0x01c00002);
assert.equal(u[0x00400000],0x12);assert.equal(u[0x08400000],0xab);
console.log('PASS bulk span containing an entire low alias folds every byte');
const {instance:{exports:f}}=await WebAssembly.instantiate(
  fs.readFileSync('build/web/memory-fixture.wasm'));
f.store_based(3,0xabcdef12);
assert.equal(f.load_based(3)>>>0,0xabcdef12);
assert.equal(new DataView(f.memory.buffer).getUint32(0x4000100c,true),0xabcdef12);
// Large fixed offset + an overflowing effective address must not wrap.
assert.throws(()=>f.load_based(0x3ffffff0),WebAssembly.RuntimeError);
console.log('PASS indexed known-base loads/stores and effective-address overflow');
