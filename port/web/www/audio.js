// The game's mixer output (48 kHz stereo 16-bit) from the worker's ring (worker.js host_sound_write):
// header [write, read] in frames, then the interleaved samples. Silence when the ring runs dry.
class Ring extends AudioWorkletProcessor {
  constructor(options) {
    super();
    const sab = options.processorOptions.ring;
    this.head = new Int32Array(sab, 0, 2); this.data = new Int16Array(sab, 8); this.frames = (sab.byteLength - 8) / 4;
  }
  process(_inputs, outputs) {
    const [left, right] = outputs[0], h = this.head, cap = this.frames;
    let r = Atomics.load(h, 1); const w = Atomics.load(h, 0);
    for (let i = 0; i < left.length; i++) {
      if (r === w) { left[i] = right[i] = 0; continue; }
      left[i] = this.data[r * 2] / 32768; right[i] = this.data[r * 2 + 1] / 32768; r = (r + 1) % cap;
    }
    Atomics.store(h, 1, r);
    return true;
  }
}
registerProcessor('dhd-ring', Ring);
