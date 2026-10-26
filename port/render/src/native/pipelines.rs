//! The native renderer's draw pipelines, by key (task N1).
//!
//! A key (the retired `gpu_wgpu_native.inc` native_pipeline; `port/gs/native_keys.inc`; both last
//! at render/rust 3882d5576):
//! `blend_key` (14 bits) | colour write mask << 14 | depth << 18 | variant << 23, where
//! - blend_key = enable | src << 1 | dst << 6 | op << 11, the factor and operation numbers in
//!   D3D11's numbering;
//! - depth (5 bits) = attachment 16 | write 8 | function 0-4 (never, always, gequal, greater,
//!   equal; 5-7 = always);
//! - variant 0 plain, 1 dual (dual-source module and factors), 2 RGB with A = 128
//!   (`native_pixel_blend`), 3 alpha only (no blend state).
//!
//! The D3D11 numbers map to wgpu through EXPLICIT tables, never casts: the C path casts the
//! operation number into a `WGPUBlendOperation`, which wgpu-native then maps (0, Undefined, to Add;
//! 1-5 to Add, Subtract, ReverseSubtract, Min, Max; 6 and 7 are refused). The colour blend is
//! (src, dst, op); the alpha blend is (One, Zero, Add). Depth: Depth32Float, the stencil state
//! ignored (Always / Keep, masks 0), no bias, `Some(write)`, `Some(compare)`. Targets are
//! RGBA8Unorm. `tests/data/native_keys.json` holds the C code's tables for every key; the cache has
//! no 512-entry cap.
//!
//! Referee: `rustgs.py test ... -- --test native_keys`, and `--test device_smoke native`.
use crate::device::Gpu;
use crate::native::init::{scoped, Objects};
use std::collections::HashMap;

pub const VARIANT_PLAIN: u32 = 0;
pub const VARIANT_DUAL: u32 = 1;
pub const VARIANT_RGB_A128: u32 = 2;
pub const VARIANT_ALPHA_ONLY: u32 = 3;

/// What a key's pipeline is built with (the parts the key decides).
#[derive(Clone, Debug, PartialEq)]
pub struct PipelineDesc {
    /// Format RGBA8Unorm, the key's blend state (None when disabled or alpha-only) and write mask.
    pub color: wgpu::ColorTargetState,
    /// None without the depth attachment bit.
    pub depth: Option<wgpu::DepthStencilState>,
    /// The fragment stage is the dual-source module's.
    pub dual_module: bool,
    /// The fragment entry point: `native_pixel`, `native_pixel_dual` or `native_pixel_blend`.
    pub entry: &'static str,
}

/// A D3D11 blend factor number (0-31) as wgpu's, for the plain or the dual-source variant
/// (native_factor: the numbers the GS blend uses, every other one One).
pub fn blend_factor(d3d: u32, dual: bool) -> wgpu::BlendFactor {
    use wgpu::BlendFactor as F;
    match d3d {
        1 => F::Zero,                                                      // D3D11_BLEND_ZERO
        2 => F::One,                                                       // D3D11_BLEND_ONE
        5 => if dual { F::Src1Alpha } else { F::SrcAlpha },                // D3D11_BLEND_SRC_ALPHA
        6 => if dual { F::OneMinusSrc1Alpha } else { F::OneMinusSrcAlpha }, // D3D11_BLEND_INV_SRC_ALPHA
        7 => F::DstAlpha,                                                  // D3D11_BLEND_DEST_ALPHA
        8 => F::OneMinusDstAlpha,                                          // D3D11_BLEND_INV_DEST_ALPHA
        14 => F::Constant,                                                 // D3D11_BLEND_BLEND_FACTOR
        15 => F::OneMinusConstant,                                         // D3D11_BLEND_INV_BLEND_FACTOR
        _ => F::One,
    }
}

/// A D3D11 blend operation number (0-7) as wgpu's; None for the numbers wgpu-native refuses.
/// The C code casts the number to `WGPUBlendOperation` (0 Undefined, which wgpu-native takes as
/// Add; 1 Add, 2 Subtract, 3 ReverseSubtract, 4 Min, 5 Max); here the same map is written out.
pub fn blend_operation(d3d: u32) -> Option<wgpu::BlendOperation> {
    use wgpu::BlendOperation as O;
    match d3d {
        0 => Some(O::Add),             // WGPUBlendOperation_Undefined: wgpu-native's default, Add
        1 => Some(O::Add),             // D3D11_BLEND_OP_ADD
        2 => Some(O::Subtract),        // D3D11_BLEND_OP_SUBTRACT
        3 => Some(O::ReverseSubtract), // D3D11_BLEND_OP_REV_SUBTRACT
        4 => Some(O::Min),             // D3D11_BLEND_OP_MIN
        5 => Some(O::Max),             // D3D11_BLEND_OP_MAX
        _ => None,
    }
}

/// The depth function of a key's depth field (0-4: never, always, gequal, greater, equal; 5-7
/// always).
fn depth_compare(function: u32) -> wgpu::CompareFunction {
    use wgpu::CompareFunction as C;
    match function {
        0 => C::Never,
        2 => C::GreaterEqual,
        3 => C::Greater,
        4 => C::Equal,
        _ => C::Always,
    }
}

/// A colour write mask (red 1, green 2, blue 4, alpha 8, as WGPUColorWriteMask) as wgpu's.
fn write_mask(mask: u32) -> wgpu::ColorWrites {
    let mut w = wgpu::ColorWrites::empty();
    if mask & 1 != 0 {
        w |= wgpu::ColorWrites::RED;
    }
    if mask & 2 != 0 {
        w |= wgpu::ColorWrites::GREEN;
    }
    if mask & 4 != 0 {
        w |= wgpu::ColorWrites::BLUE;
    }
    if mask & 8 != 0 {
        w |= wgpu::ColorWrites::ALPHA;
    }
    w
}

/// Depth32Float with the stencil ignored (Always / Keep, masks 0) and no bias, as the C path's
/// zeroed `WGPUDepthStencilState` with Always / Keep stencil faces. Also the fullscreen depth
/// pipeline's (`init.rs`).
pub(crate) fn depth_state(write: bool, compare: wgpu::CompareFunction) -> wgpu::DepthStencilState {
    wgpu::DepthStencilState {
        format: wgpu::TextureFormat::Depth32Float,
        depth_write_enabled: Some(write),
        depth_compare: Some(compare),
        stencil: wgpu::StencilState {
            front: wgpu::StencilFaceState::IGNORE,
            back: wgpu::StencilFaceState::IGNORE,
            read_mask: 0,
            write_mask: 0,
        },
        bias: wgpu::DepthBiasState::default(),
    }
}

/// The pipeline a key asks for; Err for a key no pipeline exists for (an operation of 6 or 7 on
/// a blending key).
pub fn describe(key: u32) -> Result<PipelineDesc, String> {
    let blend_key = key & 0x3FFF;
    let mask = key >> 14 & 15;
    let depth = key >> 18 & 31;
    let variant = key >> 23 & 3;
    let dual = variant == VARIANT_DUAL;
    let blend = if blend_key & 1 != 0 && variant != VARIANT_ALPHA_ONLY {
        let op = blend_key >> 11 & 7;
        let operation = blend_operation(op)
            .ok_or_else(|| format!("native pipeline {key:07X}: blend operation {op} is not one wgpu takes"))?;
        Some(wgpu::BlendState {
            color: wgpu::BlendComponent {
                src_factor: blend_factor(blend_key >> 1 & 31, dual),
                dst_factor: blend_factor(blend_key >> 6 & 31, dual),
                operation,
            },
            alpha: wgpu::BlendComponent {
                src_factor: wgpu::BlendFactor::One,
                dst_factor: wgpu::BlendFactor::Zero,
                operation: wgpu::BlendOperation::Add,
            },
        })
    } else {
        None
    };
    let entry = match variant {
        VARIANT_DUAL => "native_pixel_dual",
        VARIANT_RGB_A128 => "native_pixel_blend",
        _ => "native_pixel",
    };
    Ok(PipelineDesc {
        color: wgpu::ColorTargetState { format: wgpu::TextureFormat::Rgba8Unorm, blend, write_mask: write_mask(mask) },
        depth: (depth & 16 != 0).then(|| depth_state(depth & 8 != 0, depth_compare(depth & 7))),
        dual_module: dual,
        entry,
    })
}

/// The vertex layout of every draw (`DhdrVertex`, 32 bytes): position xyz, stqf, rgba bytes.
const VERTEX_ATTRIBUTES: [wgpu::VertexAttribute; 3] = [
    wgpu::VertexAttribute { format: wgpu::VertexFormat::Float32x3, offset: 0, shader_location: 0 },
    wgpu::VertexAttribute { format: wgpu::VertexFormat::Float32x4, offset: 12, shader_location: 1 },
    wgpu::VertexAttribute { format: wgpu::VertexFormat::Uint8x4, offset: 28, shader_location: 2 },
];
const _: () = assert!(std::mem::size_of::<crate::abi::DhdrVertex>() == 32);

/// Builds the pipeline of `key` (no cache, no scope).
fn build(gpu: &Gpu, objects: &Objects, key: u32, d: &PipelineDesc) -> Result<wgpu::RenderPipeline, String> {
    let fragment_module = if d.dual_module {
        objects
            .dual_module
            .as_ref()
            .ok_or_else(|| format!("native pipeline {key:07X}: dual-source blending is not in use"))?
    } else {
        &objects.module
    };
    let vertex = wgpu::VertexBufferLayout {
        array_stride: std::mem::size_of::<crate::abi::DhdrVertex>() as u64,
        step_mode: wgpu::VertexStepMode::Vertex,
        attributes: &VERTEX_ATTRIBUTES,
    };
    let targets = [Some(d.color.clone())];
    Ok(gpu.device.create_render_pipeline(&wgpu::RenderPipelineDescriptor {
        label: Some("native draw"),
        layout: Some(&objects.draw_layout),
        vertex: wgpu::VertexState {
            module: &objects.module,
            entry_point: Some("native_vertex"),
            compilation_options: crate::native::init::compilation_options(),
            buffers: &[vertex],
        },
        primitive: wgpu::PrimitiveState {
            topology: wgpu::PrimitiveTopology::TriangleList,
            cull_mode: None,
            ..Default::default()
        },
        depth_stencil: d.depth.clone(),
        multisample: wgpu::MultisampleState { count: 1, mask: !0, alpha_to_coverage_enabled: false },
        fragment: Some(wgpu::FragmentState {
            module: fragment_module,
            entry_point: Some(d.entry),
            compilation_options: crate::native::init::compilation_options(),
            targets: &targets,
        }),
        multiview_mask: None,
        cache: None,
    }))
}

/// Built pipelines by key (failures are kept too, so a bad key is reported once, not per draw).
pub struct Pipelines {
    built: HashMap<u32, Result<wgpu::RenderPipeline, String>>,
}

impl Pipelines {
    pub fn new() -> Pipelines {
        Pipelines { built: HashMap::new() }
    }

    /// Pipelines built (or failed) so far.
    pub fn len(&self) -> usize {
        self.built.len()
    }

    pub fn is_empty(&self) -> bool {
        self.built.is_empty()
    }

    /// The pipeline for `key`, built on first use (Err: the key has none, or it failed).
    pub fn get(&mut self, gpu: &Gpu, objects: &Objects, key: u32) -> Result<&wgpu::RenderPipeline, String> {
        let built = self.built.entry(key).or_insert_with(|| {
            #[cfg(not(target_arch = "wasm32"))]
            let _spike = SpikeTimer::new("pipeline", key);
            let d = describe(key)?;
            #[cfg(not(target_arch = "wasm32"))]
            let start = std::time::Instant::now();
            let pipe = scoped(gpu, &format!("native pipeline {key:07X}"), || build(gpu, objects, key, &d))??;
            #[cfg(not(target_arch = "wasm32"))]
            {
                let took = start.elapsed().as_secs_f64();
                if took > 0.25 {
                    crate::host::log(&format!("gs native: draw pipeline {key:07X} built in {took:.2} s"));
                }
            }
            Ok(pipe)
        });
        built.as_ref().map_err(|e| e.clone())
    }
}

/// DHD_NATIVE_PIPELOG (diagnostics, AND3): the keys each picture binds, one log line per submit.
#[cfg(not(target_arch = "wasm32"))]
pub(crate) mod pipelog {
    use std::sync::{Mutex, OnceLock};
    static KEYS: Mutex<Vec<u32>> = Mutex::new(Vec::new());
    static PICTURE: std::sync::atomic::AtomicU64 = std::sync::atomic::AtomicU64::new(0);
    fn on() -> bool {
        static ON: OnceLock<bool> = OnceLock::new();
        *ON.get_or_init(|| std::env::var_os("DHD_NATIVE_PIPELOG").is_some())
    }
    pub(crate) fn bind(key: u32) {
        if on() {
            let mut k = KEYS.lock().unwrap_or_else(|p| p.into_inner());
            if !k.contains(&key) {
                k.push(key);
            }
        }
    }
    pub(crate) fn submit() {
        if on() {
            let mut k = KEYS.lock().unwrap_or_else(|p| p.into_inner());
            let n = PICTURE.fetch_add(1, std::sync::atomic::Ordering::Relaxed);
            let list: Vec<String> = k.iter().map(|x| format!("{x:07X}")).collect();
            crate::host::log(&format!("pipelog {n}: {}", list.join(" ")));
            k.clear();
        }
    }
}

/// Opt-in spike attribution; no clocks or formatting when DHD_SPIKES is absent.
#[cfg(not(target_arch = "wasm32"))]
pub(crate) struct SpikeTimer {
    start: Option<std::time::Instant>,
    tag: &'static str,
    key: u32,
}
#[cfg(not(target_arch = "wasm32"))]
impl SpikeTimer {
    pub(crate) fn new(tag: &'static str, key: u32) -> Self {
        static ENABLED: std::sync::OnceLock<bool> = std::sync::OnceLock::new();
        Self { start: ENABLED.get_or_init(|| std::env::var_os("DHD_SPIKES").is_some())
                   .then(std::time::Instant::now), tag, key }
    }
}
#[cfg(not(target_arch = "wasm32"))]
impl Drop for SpikeTimer {
    fn drop(&mut self) {
        if let Some(start) = self.start {
            let ms = start.elapsed().as_secs_f64() * 1000.0;
            if ms >= 1.0 || self.tag == "pipeline" {
                let end = std::time::SystemTime::now().duration_since(std::time::UNIX_EPOCH)
                    .unwrap_or_default().as_secs_f64();
                crate::host::log(&format!("spike_gpu: end_unix={end:.6} ms={ms:.6} tag={} key={:X}", self.tag, self.key));
            }
        }
    }
}
