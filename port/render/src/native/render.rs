//! The native picture: the command loop, draw passes, submit (task N6).
//!
//! As the retired `gpu_wgpu_native.inc`'s (last at render/rust 3882d5576) native_picture /
//! native_render / native_draw_passes /
//! native_begin_pass / native_end_pass / native_set_pipeline / native_submit (the plan's
//! `:193-224,657-831`, now `:250-280,445-451,718-896,1561-1589`): `arenas.reserve` first, then
//! each command in order:
//! - ALPHA_UPLOAD, IMPORT, COPY: surfaces.rs's `transfer`, which counts them (an IMPORT or COPY
//!   on a surface without a texture, or an IMPORT on a Z surface, is skipped here, as in C);
//! - DRAW: the pass stays open while draws keep their target and Z surface (pass reuse); the
//!   texture is a surface (a copy in the scratch texture when it is the target), a replacement, a
//!   decoded texture or a conversion (run again whenever the conversion index changes); the draw
//!   is one draw (no As, or dual-source blending), or RGB with As / 128 then the alpha channel
//!   alone, the second in reverse triangle order for GREATER with Z writes;
//!
//! then the vertices, the scanout (or the movie frame; scanout.rs) and one submit. The counts go
//! to `DhdrPictureResult`.
//!
//! Surfaces as the picture sees them (depth, scale, texture size) come from the picture's
//! snapshot (`Picture::surfaces`, taken after C made or grew every surface through
//! `dhdr_native_surface`); a surface "has a texture" when its size there is not 0 x 0.
//!
//! The encoder: `Pass::encoder` opens one when none is open, so work recorded between pictures
//! (surfaces.rs growing a surface in `dhdr_native_surface`) goes into the encoder the next
//! picture continues and submits, before its draws, as in C where surfaces grow inside
//! native_render. Everything that records outside a draw pass (copies, uploads, conversions,
//! blits, scanout) takes the encoder through `Pass::encoder`, which ends the open pass first.
//!
//! From surfaces.rs (N4) it uses `Surfaces::{prepare, transfer, group, sample_target, get}`,
//! lending them the picture's encoder (with the draw pass ended), the arenas and the layouts
//! through a `Kit`; the grown surfaces' old contents go into the new textures (`prepare`) right
//! after `arenas.reserve`, before the commands, as C's native_prepare_surfaces. From N3 it uses
//! `Tex::group`, `Tex::view` and `NativeShared`'s dummies; from N5 `Converts::run` and
//! `Display::{scanout, movie, size}`.
//!
//! Referee: native_replay on the transfers traces, dual and no-dual (N6).
use crate::abi::*;
use crate::device::Gpu;
use crate::native::arenas::Arenas;
use crate::native::convert::{ConvertObjects, ConvertSource};
use crate::native::scanout::ScanoutObjects;
use crate::native::pipelines::{VARIANT_ALPHA_ONLY, VARIANT_DUAL, VARIANT_PLAIN, VARIANT_RGB_A128};
use crate::native::shared::NativeShared;
use crate::native::surfaces::Kit;
use crate::native::{Native, Picture};
use std::collections::HashSet;

/// D3D11's SRC_ALPHA and INV_SRC_ALPHA (native_core.inc's BLEND_SA, BLEND_ISA).
const BLEND_SA: u32 = 5;
const BLEND_ISA: u32 = 6;
/// D3D11's BLEND_FACTOR and INV_BLEND_FACTOR (pipelines.rs's `blend_factor`: the blend constant).
const BLEND_FACTOR: u32 = 14;
const BLEND_INV_FACTOR: u32 = 15;

/// The picture's encoder and the open render pass's state (target, depth, pipeline) between
/// commands, and the picture's counts.
pub struct Pass {
    encoder: Option<wgpu::CommandEncoder>,
    /// The open draw pass (`forget_lifetime`: it holds the encoder until dropped, so it is always
    /// ended before the encoder is used or finished).
    pass: Option<wgpu::RenderPass<'static>>,
    /// The open pass's target and Z surface (-1: none); -2 when no pass is open.
    target: i32,
    depth: i32,
    /// The key of the pipeline set in the open pass (native_set_pipeline's pass_pipe).
    pipe: Option<u32>,
    /// The picture's counts (shown and the display size are filled at the end).
    drawn: DhdrPictureResult,
    /// Keys whose pipeline failed, reported once.
    failed: HashSet<u32>,
    /// What the open pass has set, so a draw sets only what changed (in the browser each WebGPU
    /// call is a call out to JavaScript; the blend constant builds a JS object). WebGPU keeps
    /// all of these for the rest of the pass, across pipeline changes. Cleared with the pass.
    state: PassState,
    /// The picture's last draw constants and their offset in the constants arena: a draw with
    /// the same bytes binds that offset again instead of a new copy (the arena is written once,
    /// at the submit, and nothing in it is overwritten during the picture).
    constants: Option<(DhdrNativeConstants, u32)>,
    /// A draw not issued yet (first vertex, count): see `draw`.
    pending: Option<(u32, u32)>,
}

/// The state set in the open draw pass (see `Pass::state`); None: not set yet.
#[derive(Default)]
struct PassState {
    /// The viewport's size (always at 0, 0 with depth 0..1).
    viewport: Option<[u32; 2]>,
    scissor: Option<[u32; 4]>,
    /// The blend constant's GS value (blend_fix: all four channels are blend_fix / 128).
    blend: Option<u32>,
    /// Group 0's dynamic offset (the group is the picture's constants arena group).
    offset: Option<u32>,
    /// Group 1, the draw's texture.
    group: Option<wgpu::BindGroup>,
}

impl Pass {
    pub fn new() -> Pass {
        Pass {
            encoder: None,
            pass: None,
            target: -2,
            depth: -2,
            pipe: None,
            drawn: DhdrPictureResult::default(),
            failed: HashSet::new(),
            state: PassState::default(),
            constants: None,
            pending: None,
        }
    }

    /// native_end_pass: ends the open draw pass, if any.
    pub fn end(&mut self) {
        self.flush();
        if let Some(pass) = self.pass.take() {
            drop(pass);
            self.target = -2;
            self.depth = -2;
            self.pipe = None;
            self.state = PassState::default();
        }
    }

    /// The constants' offset in the arena: the last draw's when the bytes are the same, else a
    /// new copy (`Arenas::constants`).
    fn constants(&mut self, arenas: &mut Arenas, c: &DhdrNativeConstants) -> u32 {
        if let Some((last, offset)) = &self.constants {
            if constant_bytes(last) == constant_bytes(c) {
                return *offset;
            }
        }
        let offset = arenas.constants(constant_bytes(c));
        self.constants = Some((*c, offset));
        offset
    }

    /// The viewport (0, 0, w, h, depth 0..1), the scissor and the blend constant (None: the draw's
    /// blend does not read it) in the open pass, each set only when it differs from what the pass
    /// has.
    fn set_state(&mut self, w: u32, h: u32, scissor: [u32; 4], blend_fix: Option<u32>) {
        let s = &self.state;
        if s.viewport != Some([w, h]) || s.scissor != Some(scissor)
            || blend_fix.is_some_and(|b| s.blend != Some(b)) {
            self.flush();
        }
        let Some(pass) = self.pass.as_mut() else { return };
        let s = &mut self.state;
        if s.viewport != Some([w, h]) {
            pass.set_viewport(0.0, 0.0, w as f32, h as f32, 0.0, 1.0);
            s.viewport = Some([w, h]);
        }
        if s.scissor != Some(scissor) {
            pass.set_scissor_rect(scissor[0], scissor[1], scissor[2], scissor[3]);
            s.scissor = Some(scissor);
        }
        if let Some(blend_fix) = blend_fix.filter(|&b| s.blend != Some(b)) {
            let fix = blend_fix as f64 / 128.0;
            pass.set_blend_constant(wgpu::Color { r: fix, g: fix, b: fix, a: fix });
            s.blend = Some(blend_fix);
        }
    }

    /// Groups 0 (the constants arena's, at `offset`) and 1 in the open pass, each set only when it
    /// differs from what the pass has.
    fn set_groups(&mut self, uniforms: &wgpu::BindGroup, offset: u32, group: &wgpu::BindGroup) {
        if self.state.offset != Some(offset) || self.state.group.as_ref() != Some(group) {
            self.flush();
        }
        let Some(pass) = self.pass.as_mut() else { return };
        let s = &mut self.state;
        if s.offset != Some(offset) {
            pass.set_bind_group(0, uniforms, &[offset]);
            s.offset = Some(offset);
        }
        if s.group.as_ref() != Some(group) {
            pass.set_bind_group(1, group, &[]);
            s.group = Some(group.clone());
        }
    }

    /// The encoder, with the open draw pass ended; one is opened when none is (the next picture
    /// submits it).
    pub fn encoder(&mut self, gpu: &Gpu) -> &mut wgpu::CommandEncoder {
        self.split(gpu).0
    }

    /// The encoder (as `encoder`) and the picture's counts, for surfaces.rs's operations.
    fn split(&mut self, gpu: &Gpu) -> (&mut wgpu::CommandEncoder, &mut DhdrPictureResult) {
        self.end();
        let encoder = self.encoder.get_or_insert_with(|| {
            gpu.device.create_command_encoder(&wgpu::CommandEncoderDescriptor { label: Some("native picture") })
        });
        (encoder, &mut self.drawn)
    }

    /// Whether the open pass draws into this target and Z surface.
    fn open_on(&self, target: i32, depth: i32) -> bool {
        self.pass.is_some() && self.target == target && self.depth == depth
    }

    /// native_begin_pass (once `open_on` said no): a pass into `color` (and `z`), loading and
    /// storing both, with the whole vertex buffer at slot 0.
    #[allow(clippy::too_many_arguments)]
    fn begin(&mut self, gpu: &Gpu, target: i32, depth: i32, color: &wgpu::TextureView, z: Option<&wgpu::TextureView>,
             vertices: &wgpu::Buffer, vertex_bytes: u64) {
        let keep = wgpu::Operations { load: wgpu::LoadOp::Load, store: wgpu::StoreOp::Store };
        let encoder = self.encoder(gpu);
        let mut pass = encoder
            .begin_render_pass(&wgpu::RenderPassDescriptor {
                label: None,
                color_attachments: &[Some(wgpu::RenderPassColorAttachment {
                    view: color,
                    depth_slice: None,
                    resolve_target: None,
                    ops: wgpu::Operations { load: wgpu::LoadOp::Load, store: wgpu::StoreOp::Store },
                })],
                depth_stencil_attachment: z.map(|view| wgpu::RenderPassDepthStencilAttachment {
                    view,
                    depth_ops: Some(keep),
                    stencil_ops: None,
                }),
                timestamp_writes: None,
                occlusion_query_set: None,
                multiview_mask: None,
            })
            .forget_lifetime();
        pass.set_vertex_buffer(0, vertices.slice(0..vertex_bytes));
        self.pass = Some(pass);
        self.target = target;
        self.depth = depth;
        self.pipe = None;
    }

    /// native_set_pipeline in the open pass: false (reported once per key) when the key has no
    /// pipeline; the draw is then skipped.
    fn set_pipeline(&mut self, gpu: &Gpu, n: &mut crate::native::pipelines::Pipelines,
                    objects: &crate::native::init::Objects, key: u32) -> bool {
        if self.pipe == Some(key) {
            return true;
        }
        match n.get(gpu, objects, key) {
            Ok(pipe) => {
                self.flush();
                if let Some(pass) = self.pass.as_mut() {
                    pass.set_pipeline(pipe);
                }
                self.pipe = Some(key);
                true
            }
            Err(e) => {
                if self.failed.insert(key) {
                    crate::host::log(&format!("gs native: draw pipeline {key:07X}: {e}"));
                }
                false
            }
        }
    }

    /// One draw of `vertices` in the open pass. It waits in `pending`: a next draw that starts
    /// where it ends, with nothing set in the pass between them, extends it (draw(a..b) then
    /// draw(b..c) with the same state is draw(a..c): the triangles in the same order). Every
    /// call that sets state in the pass, and the pass's end, issues it first (`flush`).
    fn draw(&mut self, first: u32, count: u32) {
        if self.pass.is_none() {
            return;
        }
        if first.checked_add(count).is_some() {
            if let Some((f, c)) = self.pending.as_mut() {
                // whole triangles only, so no vertex of one draw joins a triangle of the other
                if c.is_multiple_of(3) && f.checked_add(*c) == Some(first) {
                    *c += count;
                    return;
                }
            }
        }
        self.flush();
        self.pending = Some((first, count));
    }

    /// Issues the pending draw, if any.
    fn flush(&mut self) {
        if let Some((first, count)) = self.pending.take() {
            if let Some(pass) = self.pass.as_mut() {
                pass.draw(first..first.saturating_add(count), 0..1);
            }
        }
    }

    /// native_submit: ends the pass, writes the arenas the encoder's commands read, submits.
    fn submit(&mut self, gpu: &Gpu, arenas: &Arenas) {
        self.end();
        arenas.write(&gpu.queue);
        let encoder = self.encoder.take().unwrap_or_else(|| {
            gpu.device.create_command_encoder(&wgpu::CommandEncoderDescriptor { label: Some("native picture") })
        });
        gpu.queue.submit([encoder.finish()]);
    }
}

/// A surface of the picture's snapshot, or None for an absent index.
fn surface<'a>(p: &Picture<'a>, index: i16) -> Option<&'a DhdrSurfaceInfo> {
    if index < 0 {
        return None;
    }
    p.surfaces.get(index as usize)
}

/// The surface has a texture (its size is not 0 x 0).
fn has_texture(s: Option<&DhdrSurfaceInfo>) -> bool {
    s.is_some_and(|s| s.size != [0, 0])
}

/// Whether a draw's blend reads the blend constant: blending on, with BLEND_FACTOR or
/// INV_BLEND_FACTOR as the source or destination colour factor (pipelines.rs's `describe`; the
/// alpha factors are One and Zero, and the alpha-only second draw does not blend).
fn uses_constant(blend_key: u32) -> bool {
    let (src, dst) = (blend_key >> 1 & 31, blend_key >> 6 & 31);
    blend_key & 1 != 0 && (src == BLEND_FACTOR || src == BLEND_INV_FACTOR || dst == BLEND_FACTOR || dst == BLEND_INV_FACTOR)
}

/// The draw constants as the uniform block's bytes.
fn constant_bytes(c: &DhdrNativeConstants) -> &[u8] {
    // SAFETY: DhdrNativeConstants is repr(C) with 24 four-byte fields and no padding, so every
    // byte is initialised; the length is its size.
    unsafe { std::slice::from_raw_parts((c as *const DhdrNativeConstants).cast::<u8>(), std::mem::size_of_val(c)) }
}

impl Native {
    /// `dhdr_native_picture`: draws, scans out (or shows the movie) and submits.
    pub fn picture(&mut self, p: &Picture) -> Result<DhdrPictureResult, String> {
        let gpu = self.gpu;
        let shared = NativeShared::get(gpu)?;
        self.arenas.reserve(Some((gpu, &self.objects.uniform_layout)), p.commands, p.vertices.len() as u32)?;
        let uniforms = self.arenas.uniform_group().cloned().ok_or("gs native: no constants bind group")?;
        let vertices = self.arenas.vertex_buffer().cloned().ok_or("gs native: no vertex buffer")?;
        let vertex_bytes = self.arenas.vertex_capacity();
        self.pass.drawn = DhdrPictureResult::default();
        self.pass.constants = None; // reserve emptied the arena
        {
            // native_prepare_surfaces: grown surfaces' old contents first, in the picture's encoder.
            let encoder = self.pass.encoder(gpu);
            let mut kit = Kit::new(gpu, &self.objects, shared, &mut self.arenas, encoder);
            self.surfaces.prepare(&mut kit)?;
        }
        let views = Views { shared, uniforms, vertices, vertex_bytes };
        self.render(p, &views);
        self.pass.end();
        self.arenas.write_vertices(&gpu.queue, p.vertices);
        let shown = match p.movie {
            Some((w, h, pixels)) => self.display.movie(gpu, w, h, pixels),
            None => self.scan_out(p),
        };
        self.pass.submit(gpu, &self.arenas);
        let mut r = self.pass.drawn;
        r.shown = shown as u32;
        (r.display_width, r.display_height) = self.display.size();
        Ok(r)
    }

    /// native_render's command loop.
    fn render(&mut self, p: &Picture, v: &Views) {
        let gpu = self.gpu;
        let mut last_convert: i32 = -1;
        let mut converted: Option<wgpu::BindGroup> = None;
        for (i, k) in p.commands.iter().enumerate() {
            match k.kind {
                DHDR_CMD_ALPHA_UPLOAD => {
                    self.transfer(k, p.pixels(i), v.shared);
                    last_convert = -1;
                    continue;
                }
                DHDR_CMD_IMPORT => {
                    last_convert = -1;
                    let s = surface(p, k.target);
                    if !has_texture(s) || s.is_some_and(|s| s.depth != 0) {
                        continue;
                    }
                    self.transfer(k, p.pixels(i), v.shared);
                    continue;
                }
                DHDR_CMD_COPY => {
                    last_convert = -1;
                    if !has_texture(surface(p, k.source)) || !has_texture(surface(p, k.target)) {
                        continue;
                    }
                    self.transfer(k, None, v.shared);
                    continue;
                }
                _ => {}
            }
            if k.count == 0 {
                continue;
            }
            let Some(target) = surface(p, k.target).filter(|t| has_texture(Some(t)) && t.depth == 0) else { continue };
            let mut depth = if k.depth >= 0 && k.depth_key & 8 != 0 { surface(p, k.depth) } else { None };
            if depth.is_some_and(|d| !has_texture(Some(d)) || d.depth == 0 || d.size != target.size) {
                depth = None;
            }
            let (tw, th) = (target.size[0], target.size[1]);
            let big_t = target.scale;
            let mut big_s: u32 = 1;
            // Untextured, or a surface without a texture: zeros.
            let mut group = Some(v.shared.dummy_group.clone());
            if k.texture_surface >= 0 {
                let ts = k.texture_surface as usize;
                if let Some(s) = surface(p, k.texture_surface) {
                    big_s = s.scale;
                }
                if k.texture_surface == k.target {
                    // Sampling the target: draw from a copy of the region the texture can reach
                    // (in the scratch texture); none (C's `continue`) skips the draw.
                    let Some(g) = self.sample_target(ts, &k.constants, v.shared) else { continue };
                    group = Some(g);
                } else if let Some(g) = self.surface_group(ts, v.shared) {
                    group = Some(g);
                }
            } else if let Some(r) = p.replacement(i) {
                let c = &k.constants;
                let index = (c.flags >> 5 & 1) | ((c.wrap[0] != 0) as u32) << 1 | ((c.wrap[1] != 0) as u32) << 2;
                group = Some(r.group(index as usize).clone());
            } else if let Some(t) = p.texture(i) {
                group = Some(t.group(0).clone());
            } else if k.convert >= 0 {
                // Again for each run of draws: the surface may have changed since.
                if k.convert as i32 != last_convert {
                    converted = self.convert(p, k.convert as usize, v.shared);
                    last_convert = k.convert as i32;
                }
                if converted.is_none() {
                    continue;
                }
                group = converted.clone();
            }
            if k.convert < 0 {
                last_convert = -1;
            }
            let Some(group) = group else { continue };
            let depth_index = if depth.is_some() { k.depth as i32 } else { -1 };
            if !self.pass.open_on(k.target as i32, depth_index) {
                let Some(color) = self.surface_view(k.target as usize) else { continue };
                let z = match depth {
                    Some(_) => match self.surface_view(k.depth as usize) {
                        Some(z) => Some(z),
                        None => continue,
                    },
                    None => None,
                };
                self.pass.begin(gpu, k.target as i32, depth_index, &color, z.as_ref(), &v.vertices, v.vertex_bytes);
            }
            let mut constants = k.constants;
            constants.scale = big_t | big_s << 8 | big_s.min(big_t) << 16;
            // The attachment's size in GS pixels: native_vertex maps GS pixels onto it (D3D11 keeps
            // a 2048 x 2048 GS pixel viewport, larger than the target; WebGPU allows no such one).
            constants.target = [tw as f32 / big_t as f32, th as f32 / big_t as f32];
            let offset = self.pass.constants(&mut self.arenas, &constants);
            if self.pass.pass.is_none() {
                continue;
            }
            let t = big_t as i32;
            let left = k.scissor[0].wrapping_mul(t).max(0);
            let top = k.scissor[1].wrapping_mul(t).max(0);
            let right = k.scissor[2].wrapping_mul(t).min(tw as i32);
            let bottom = k.scissor[3].wrapping_mul(t).min(th as i32);
            if right <= left || bottom <= top {
                continue;
            }
            let scissor = [left as u32, top as u32, (right - left) as u32, (bottom - top) as u32];
            self.pass.set_state(tw, th, scissor, uses_constant(k.blend_key).then_some(k.blend_fix));
            self.draw_passes(k, p.vertices, depth.is_some(), offset, &v.uniforms, &group);
            self.pass.drawn.draws += 1;
        }
    }

    /// A COPY, IMPORT or ALPHA_UPLOAD through surfaces.rs (`Surfaces::transfer`, which counts
    /// it), recorded after the open pass ends.
    fn transfer(&mut self, k: &DhdrCommand, pixels: Option<&[u32]>, shared: &NativeShared) {
        let gpu = self.gpu;
        let (encoder, drawn) = self.pass.split(gpu);
        let mut kit = Kit::new(gpu, &self.objects, shared, &mut self.arenas, encoder);
        self.surfaces.transfer(&mut kit, k, pixels, drawn);
    }

    /// native_surface_group: surface `index`'s draw bind group (init.rs's texture layout, the
    /// nearest sampler), made when first asked for; None for a Z surface or one without a texture.
    fn surface_group(&mut self, index: usize, shared: &NativeShared) -> Option<wgpu::BindGroup> {
        self.surfaces.group(self.gpu, &self.objects.texture_layout, &shared.nearest, index).cloned()
    }

    /// A draw sampling its own target, surface `index` (`Surfaces::sample_target`, which counts
    /// the self-copy): the scratch texture's bind group, or None to skip the draw. Recorded after
    /// the open pass ends.
    fn sample_target(&mut self, index: usize, c: &DhdrNativeConstants, shared: &NativeShared)
                     -> Option<wgpu::BindGroup> {
        let gpu = self.gpu;
        let (encoder, drawn) = self.pass.split(gpu);
        let mut kit = Kit::new(gpu, &self.objects, shared, &mut self.arenas, encoder);
        self.surfaces.sample_target(&mut kit, index, c, drawn).cloned()
    }

    /// Surface `index`'s texture view (a draw pass's colour or Z attachment, what conversions and
    /// the scanout sample); None when it has no texture.
    fn surface_view(&self, index: usize) -> Option<wgpu::TextureView> {
        self.surfaces.get(index).and_then(|s| s.view.clone())
    }

    /// native_run_convert through convert.rs: conversion `i` with its base texture and its four
    /// source surfaces as they stand, recorded after the open pass ends; its draw bind group.
    fn convert(&mut self, p: &Picture, i: usize, shared: &NativeShared) -> Option<wgpu::BindGroup> {
        let gpu = self.gpu;
        let v = p.converts.get(i)?;
        let base = p.convert_base(i).map(|t| &t.view);
        let mut views: [Option<wgpu::TextureView>; 4] = [None, None, None, None];
        let mut infos: [Option<&DhdrSurfaceInfo>; 4] = [None; 4];
        for k in 0..4 {
            if v.surface[k] >= 0 {
                infos[k] = surface(p, v.surface[k]);
                views[k] = infos[k].filter(|s| has_texture(Some(s))).and_then(|_| self.surface_view(v.surface[k] as usize));
            }
        }
        let sources: [Option<ConvertSource>; 4] = std::array::from_fn(|k| {
            (v.surface[k] >= 0).then(|| ConvertSource {
                view: views[k].as_ref(),
                depth: infos[k].is_some_and(|s| s.depth != 0),
                scale: infos[k].map_or(1, |s| s.scale),
            })
        });
        let o = ConvertObjects {
            pipeline: &self.objects.convert_pipe,
            layout: &self.objects.convert_layout,
            inverse: &self.objects.inverse,
            texture_layout: &shared.texture_layout,
            nearest: &shared.nearest,
            dummy_view: &shared.dummy_view,
            dummy_depth_view: &shared.dummy_depth_view,
        };
        let encoder = self.pass.encoder(gpu);
        self.converts.run(gpu, encoder, &mut self.arenas, &o, i, v, base, &sources)
    }

    /// native_scanout through scanout.rs: the picture's display surface (with a texture) into the
    /// display texture, recorded after the open pass ends; false when nothing was scanned out.
    fn scan_out(&mut self, p: &Picture) -> bool {
        let gpu = self.gpu;
        if p.display_surface < 0 {
            return false;
        }
        let Some(info) = p.surfaces.get(p.display_surface as usize).filter(|s| has_texture(Some(s))) else {
            return false;
        };
        let Some(view) = self.surface_view(p.display_surface as usize) else { return false };
        let o = ScanoutObjects { pipeline: &self.objects.scanout_pipe, layout: &self.objects.scanout_layout };
        let encoder = self.pass.encoder(gpu);
        self.display.scanout(gpu, encoder, &mut self.arenas, &o, &view, info.scale, &p.display)
    }

    /// native_draw_passes: one draw command into the open pass. GS blending wants As / 128 for SA
    /// and ISA; with dual-source blending that is the second output, as on D3D11. Without it, a
    /// draw whose blend uses As and whose alpha channel is written goes twice: RGB blended with
    /// As / 128 as the source alpha, then the alpha channel alone, unblended, on the pixels the
    /// first draw wrote. The second draw's Z test picks those pixels: with Z writes, EQUAL against
    /// what the first draw left (in reverse triangle order for GREATER, where a later triangle at
    /// an equal Z lost); without, the same test on the unchanged Z buffer.
    fn draw_passes(&mut self, k: &DhdrCommand, vertices: &[DhdrVertex], has_depth: bool, offset: u32,
                   uniforms: &wgpu::BindGroup, group: &wgpu::BindGroup) {
        let gpu = self.gpu;
        let (blend_key, mask, dk) = (k.blend_key, k.color_mask as u32, k.depth_key as u32);
        let (src, dst) = (blend_key >> 1 & 31, blend_key >> 6 & 31);
        let uses_as = blend_key & 1 != 0 && (src == BLEND_SA || src == BLEND_ISA || dst == BLEND_SA || dst == BLEND_ISA);
        let depth = if has_depth { 16 | if dk & 4 != 0 { 8 } else { 0 } | (dk & 3) } else { 0 };
        let base = blend_key | depth << 18;
        self.pass.set_groups(uniforms, offset, group);
        if !uses_as || (self.objects.dual && self.objects.dual_module.is_some()) {
            let variant = if uses_as { VARIANT_DUAL } else { VARIANT_PLAIN };
            if self.pass.set_pipeline(gpu, &mut self.pipelines, &self.objects, base | mask << 14 | variant << 23) {
                self.pass.draw(k.first, k.count);
            }
            return;
        }
        if mask & 7 != 0
            && self.pass.set_pipeline(gpu, &mut self.pipelines, &self.objects,
                                      base | (mask & 7) << 14 | VARIANT_RGB_A128 << 23)
        {
            self.pass.draw(k.first, k.count);
        }
        if mask & 8 == 0 {
            return;
        }
        self.pass.drawn.two_pass_draws += 1;
        let (mut second, mut first) = (0u32, k.first);
        if has_depth {
            second = 16 | if dk & 4 != 0 { 4 } else { dk & 3 }; // no write; EQUAL after a write
            let in_range = (k.first as u64 + k.count as u64) <= vertices.len() as u64;
            if dk & 7 == (4 | 3) && k.count.is_multiple_of(3) && k.count > 3 && in_range {
                // GREATER with writes
                first = self.arenas.reverse_triangles(vertices, k.first, k.count);
            }
        }
        if self.pass.set_pipeline(gpu, &mut self.pipelines, &self.objects,
                                  8 << 14 | second << 18 | VARIANT_ALPHA_ONLY << 23) {
            self.pass.draw(first, k.count);
        }
    }
}

/// The picture-wide objects the draws bind: the shared dummies, the constants arena's group and
/// the vertex buffer (as `reserve` left them).
struct Views {
    shared: &'static NativeShared,
    uniforms: wgpu::BindGroup,
    vertices: wgpu::Buffer,
    vertex_bytes: u64,
}
