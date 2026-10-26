import fs from 'node:fs';
import assert from 'node:assert/strict';
const get=async name=>(await WebAssembly.instantiate(fs.readFileSync(`build/web/${name}.wasm`))).instance.exports;
const raw=await get('memory-fixture-raw'),web=await get(process.argv[2]??'memory-fixture');
const u=new Uint8Array(web.memory.buffer),d=new DataView(web.memory.buffer);
d.setUint32(0x07800000,0xffc0,true);
assert.equal(web.leaf(1.5,2),raw.leaf(1.5,2),'inserted helper preserves unpublished leaf frame');
for(const address of [0x08001000,0x28001000,0x38001000]){web.store(address,0xabcdef12);assert.equal(web.load(0x08001000)>>>0,0xabcdef12);}
for(const address of [0x00401000,0x20401000,0x30401000]){web.store(address,0xdead1234);assert.equal(web.load(0x08401000)>>>0,0xdead1234);}
web.store(0x11001000,0x28001000);assert.equal(d.getUint32(0x11001000,true),0x28001000);
// Unaligned scalar crossing the low-view start/end and complete-mirror end.
for(const [a,left,right] of [[0x003ffffe,0x003ffffe,0x08400000],[0x01fffffe,0x09fffffe,0x02000000],
  [0x29fffffe,0x09fffffe,0x2a000000],[0x303ffffe,0x303ffffe,0x08400000]]){
  u.set([1,2],left);u.set([3,4],right);assert.equal(web.load(a)>>>0,0x04030201);
  web.store(a,0xaabbccdd);assert.deepEqual([...u.slice(left,left+2)],[0xdd,0xcc]);assert.deepEqual([...u.slice(right,right+2)],[0xbb,0xaa]);
}
for(let i=0;i<16;i++)u[0x08002000+i]=i;
web.copy(0x28002004,0x08002000,12);
assert.deepEqual([...u.slice(0x08002004,0x08002010)],Array.from({length:12},(_,i)=>i));
u.set([1,2],0x003ffffe);u.set([3,4],0x08400000);
web.copy(0x08003000,0x003ffffe,4);assert.deepEqual([...u.slice(0x08003000,0x08003004)],[1,2,3,4]);
web.fill(0x003ffffe,0xaa,4);assert.deepEqual([...u.slice(0x003ffffe,0x00400000)],[0xaa,0xaa]);assert.deepEqual([...u.slice(0x08400000,0x08400002)],[0xaa,0xaa]);
u.set(Array.from({length:8},(_,i)=>i),0x09fffff8);u.set(Array.from({length:8},(_,i)=>i+8),0x2a000000);
web.simd_copy(0x08005000,0x29fffff8);assert.deepEqual([...u.slice(0x08005000,0x08005010)],Array.from({length:16},(_,i)=>i));
web.simd_copy(0x39fffff8,0x08005000);assert.deepEqual([...u.slice(0x3a000000,0x3a000008)],Array.from({length:8},(_,i)=>i+8));
web.simd_lane(0x08005000,0x29fffffe);assert.equal(d.getUint32(0x08005008,true),0x09080706);
assert.throws(()=>web.offset(0xfffffff8),WebAssembly.RuntimeError);
assert.throws(()=>web.load(u.length-2),WebAssembly.RuntimeError);
console.log('PASS mirrors, low views, non-RAM, pointer values, scalar/bulk boundaries, physical overlap, effective-address overflow and OOB');
const times={},hashes={};
for(const [label,e] of [['raw',raw],['fold',web]]){
  const samples=[];for(let trial=0;trial<7;trial++){
    new Uint8Array(e.memory.buffer,0x08004000,1024).fill(1);
    const t=performance.now();const hash=e.bench(0x08004000,10000000);const ns=(performance.now()-t)*1e6/10000000;
    if(trial)samples.push(ns);hashes[label]=hash;
  }samples.sort((a,b)=>a-b);times[label]=samples[Math.floor(samples.length/2)];
}
assert.equal(hashes.raw,hashes.fold);
console.log(JSON.stringify({...times,ratio:times.fold/times.raw}));
