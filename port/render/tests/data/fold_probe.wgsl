// Spike S3: constants whose value depends on the MXCSR of whoever folds them.
// naga parses WGSL literals in software (exact) and writes them to HLSL with Rust's shortest
// round-trip text ("0.1"); FXC parses that text with the host's floating-point unit, so under
// round-toward-zero it can land one ulp low. naga's own constant evaluator does f32 arithmetic on
// the host too. `zero` is a uniform (0 at run time), so the control words are computed on the GPU.
@group(0) @binding(0) var<storage, read_write> out: array<u32>;
@group(0) @binding(1) var<uniform> zero: vec4<f32>;

const third: f32 = 1.0 / 3.0;          // folded by naga
const tiny: f32 = 1e-20 * 1e-20;        // folded by naga: 1e-40 (subnormal) unless flushed

@compute @workgroup_size(1)
fn fold() {
    out[0] = bitcast<u32>(0.1);                       // FXC parses "0.1"
    out[1] = bitcast<u32>(third);
    out[2] = bitcast<u32>(tiny);
    out[3] = bitcast<u32>(0.7);                       // FXC parses "0.7"
    out[4] = bitcast<u32>(4294967295.0);              // naga: 2^32; HLSL text "4294967300.0"
    out[5] = bitcast<u32>(1e-40);                     // a subnormal literal (FXC under DAZ)
    out[6] = bitcast<u32>(0.1 + zero.x);              // run time: the GPU adds 0
    out[7] = bitcast<u32>(1.0 / (3.0 + zero.x));      // run time on the GPU (not compared)
}
