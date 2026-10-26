// Compare every result bit, streaming SSE output through a bounded pipe buffer.
import fs from 'node:fs';
import {spawn} from 'node:child_process';
const kind=process.argv.includes('--double')?'double':'float';
const total=kind==='double'?1000000:20000000,ops=kind==='double'?7:10,stride=kind==='double'?8:4;
const {instance}=await WebAssembly.instantiate(fs.readFileSync(`build/web/${kind}-test.wasm`));
const p=spawn(`build/web/${kind}-native.exe`,['--stream'],{windowsHide:true,stdio:['ignore','pipe','inherit']});
const ended=new Promise((resolve,reject)=>{p.on('error',reject);p.on('exit',code=>code?reject(Error(`SSE exit ${code}`)):resolve());});
let op=0,index=0,seed=0x19770321,pending=Buffer.alloc(0),start=performance.now();
for await(const chunk of p.stdout){
  pending=Buffer.concat([pending,chunk]);
  while(op<ops){
    const n=Math.min(65536,total-index),size=n*stride;
    if(pending.length<size)break;
    seed=instance.exports.web_batch(op,seed,n)>>>0;
    const actual=Buffer.from(instance.exports.memory.buffer,0x01000000,size);
    const expected=pending.subarray(0,size);
    if(!actual.equals(expected)){
      for(let k=0;k<n*stride/4;k++)if(actual.readUInt32LE(k*4)!==expected.readUInt32LE(k*4)){
        console.error(`FAIL op=${op} pair=${index+Math.floor(k*4/stride)} SSE=${expected.subarray(k*4,k*4+8).toString('hex')} wasm=${actual.subarray(k*4,k*4+8).toString('hex')}`);
        p.kill();throw Error('float mismatch');
      }
    }
    pending=pending.subarray(size);index+=n;
    if(index===total){console.log(`PASS ${kind} op=${op} ${total}/${total} bit-identical SSE MXCSR=FFC0`);op++;index=0;seed=0x19770321;}
  }
}
await ended;
if(op!==ops||pending.length)throw Error('incomplete SSE corpus');
console.log(`PASS ${ops*total} results, ${((performance.now()-start)/1000).toFixed(2)} seconds`);
