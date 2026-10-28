//! The native present pass and the present filter (task N7).
//!
//! As the retired `gpu_wgpu_native.inc`'s native_present and native_present_options (last at
//! render/rust 3882d5576; with `gpu_wgpu.c`'s
//! surface_configure and the frame acquire, now `Surface::acquire`): the last scanned-out picture
//! (`scanout.rs`'s display texture) is drawn into the window surface's frame. The pass clears the
//! whole frame to opaque black, then draws native.wgsl's `present_pixel` (one full-screen
//! triangle) with `rect` as its viewport constants, scissored to `cut` clamped to the frame.
//! `DHDR_NATIVE_POINT_FILTER` samples the picture with a nearest sampler, else linear.
//!
//! What stays in C (`gpu_rust_native.inc` over `native_core.inc`): the client rectangle
//! (`window_w`, `window_h`), the rectangle that keeps the picture's shape and DHD_VIEW's quarter
//! (`native_present_rect`: `rect`, `cut`; a movie's 4:3 aspect too), the `resize` flag, pacing
//! (`pacing_present`, not under DHD_INTERP), `dhdr_surface_present` (which runs the presented test
//! hook first) and `native_note_presented`. DHD_INTERP paced by vsync (no DHD_INTERP_HZ) wants
//! Fifo, as C's wgpu_surface forced it: C asks for it with `DHDR_PRESENT_VSYNC` when it creates
//! the surface (`present/surface.rs` picks the mode).
//!
//! Return values as `dhdr_present_exact`: 1 drawn (C paces, then presents); 0 skipped (no picture
//! scanned out yet, minimised, Timeout / Occluded); -1 failed (logged).
//!
//! The pipeline (`present_pipe`, on the draw layout, in the surface's format) and the layouts are `init.rs`'s; the present
//! uniform, the sampler and the picture's bind group are this file's, made by the first `options`
//! call, which `Native::create` makes.
//!
//! Referee: `test_native_transfers.py --backend both --impl rust --presented` and
//! `test_gs_wgpu.py --impl rust --arch x86_64 --only present` (N7).
use crate::abi::{DhdrPresentRect, DHDR_NATIVE_POINT_FILTER};
use crate::device::Gpu;
use crate::native::pipelines::gputimes;
use crate::native::scanout::Display;
use crate::native::Native;
use crate::present::surface::Surface;

/// Stand-in for `scanout.rs`'s accessor (N5: `Display::view`, the display texture's view, None
/// before the first scanout), while render/rust still has the skeleton's `Display`. Inherent
/// methods take precedence over trait methods, so once N5's scanout.rs (with a
/// `pub fn view(&self) -> Option<&wgpu::TextureView>`) is merged the call below reaches it and this
/// default goes unused: delete the trait then. Until then there is no picture and present skips (0).
#[allow(dead_code)]
trait SkeletonDisplay {
    fn view(&self) -> Option<&wgpu::TextureView> {
        None
    }
}

impl SkeletonDisplay for Display {}

/// The present uniform (C: present_uniform, present_uniform_group).
struct Uniform {
    /// `present.rect` (16 bytes used of 256; uniform, copy dst).
    buffer: wgpu::Buffer,
    /// On `init.rs`'s uniform layout, 16 bytes at dynamic offset 0.
    group: wgpu::BindGroup,
    /// The bytes last written to `buffer` (None: nothing yet); the same rectangle is not written
    /// again.
    written: Option<[u8; 16]>,
}

pub struct Presenter {
    /// Made by the first `options` call (which `Native::create` makes).
    uniform: Option<Uniform>,
    /// The present filter's sampler (C's present_sampler).
    sampler: Option<wgpu::Sampler>,
    /// The bind group of the display view it was made for (C's present_group): remade when the
    /// display texture is remade (a new size) or the sampler changes.
    group: Option<(wgpu::TextureView, wgpu::BindGroup)>,
}

impl Presenter {
    pub fn new() -> Presenter {
        Presenter { uniform: None, sampler: None, group: None }
    }
}

impl Default for Presenter {
    fn default() -> Presenter {
        Presenter::new()
    }
}

fn uniform(gpu: &Gpu, layout: &wgpu::BindGroupLayout) -> Uniform {
    let buffer = gpu.device.create_buffer(&wgpu::BufferDescriptor {
        label: Some("native present"),
        size: 256,
        usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
        mapped_at_creation: false,
    });
    let group = gpu.device.create_bind_group(&wgpu::BindGroupDescriptor {
        label: Some("native present uniform"),
        layout,
        entries: &[wgpu::BindGroupEntry {
            binding: 0,
            resource: wgpu::BindingResource::Buffer(wgpu::BufferBinding {
                buffer: &buffer,
                offset: 0,
                size: wgpu::BufferSize::new(16),
            }),
        }],
    });
    Uniform { buffer, group, written: None }
}

/// native_present_options' sampler: clamp to edge, `filter` for magnification and minification,
/// nearest mip, LOD 0..32, no anisotropy.
fn sampler(gpu: &Gpu, filter: wgpu::FilterMode) -> wgpu::Sampler {
    gpu.device.create_sampler(&wgpu::SamplerDescriptor {
        label: Some("native present"),
        address_mode_u: wgpu::AddressMode::ClampToEdge,
        address_mode_v: wgpu::AddressMode::ClampToEdge,
        address_mode_w: wgpu::AddressMode::ClampToEdge,
        mag_filter: filter,
        min_filter: filter,
        mipmap_filter: wgpu::MipmapFilterMode::Nearest,
        lod_min_clamp: 0.0,
        lod_max_clamp: 32.0,
        compare: None,
        anisotropy_clamp: 1,
        border_color: None,
    })
}

/// `cut` ([left, top, right, bottom) in the window) clamped to a w x h frame, as a scissor
/// rectangle (x, y, width, height); None when nothing of it is left.
fn scissor(cut: [i32; 4], w: u32, h: u32) -> Option<(u32, u32, u32, u32)> {
    let (w, h) = (w.min(i32::MAX as u32) as i32, h.min(i32::MAX as u32) as i32);
    let (left, top) = (cut[0].max(0), cut[1].max(0));
    let (right, bottom) = (cut[2].min(w), cut[3].min(h));
    (right > left && bottom > top).then(|| (left as u32, top as u32, (right - left) as u32, (bottom - top) as u32))
}

/// The 16 bytes of `present.rect` (x, y, width, height), little-endian f32.
fn rect_bytes(rect: [f32; 4]) -> [u8; 16] {
    let mut bytes = [0u8; 16];
    for (i, v) in rect.iter().enumerate() {
        bytes[i * 4..i * 4 + 4].copy_from_slice(&v.to_le_bytes());
    }
    bytes
}

impl Native {
    /// `dhdr_native_present`: the last scanned-out picture into `surface`'s frame, as
    /// native_present. 1 drawn (C paces, then calls `dhdr_surface_present`), 0 skipped, -1 failed
    /// (logged).
    pub fn present(&mut self, surface: &mut Surface, r: &DhdrPresentRect) -> i32 {
        let whole = crate::native::render::rstat::T::new();
        let gpu = self.gpu;
        let log = |e: &str| crate::host::log(&format!("dhd_render: dhdr_native_present: {e}"));
        // No picture scanned out yet: nothing to show.
        let Some(display) = self.display.view().cloned() else { return 0 };
        let objects = &self.objects;
        let p = &mut self.presenter;
        let Some(uniform) = p.uniform.as_mut() else {
            log("no present uniform (options was not called)");
            return -1;
        };
        let Some(sampler) = p.sampler.as_ref() else {
            log("no present sampler");
            return -1;
        };
        // Minimised (0 x 0) skips; a new size or `resize` reconfigures; Outdated / Lost reconfigure
        // once; Timeout skips.
        #[cfg(not(target_arch = "wasm32"))]
        let acquiring = crate::native::pipelines::SpikeTimer::new("acquire", 0);
        let t = crate::native::render::rstat::T::new();
        let acquired = surface.acquire(r.window_w, r.window_h, r.resize != 0);
        t.add(crate::native::render::rstat::ACQUIRE);
        #[cfg(not(target_arch = "wasm32"))]
        drop(acquiring);
        let target = match acquired {
            Ok(Some(view)) => view,
            Ok(None) => return 0,
            Err(e) => {
                log(&e);
                return -1;
            }
        };
        // The frame's size (the configured one; the surface clamps it to the device's limit).
        let (w, h) = surface.frame_texture().map_or((r.window_w, r.window_h), |t| (t.width(), t.height()));
        if p.group.as_ref().is_none_or(|(view, _)| *view != display) {
            let group = gpu.device.create_bind_group(&wgpu::BindGroupDescriptor {
                label: Some("native present"),
                layout: &objects.texture_layout,
                entries: &[
                    wgpu::BindGroupEntry { binding: 0, resource: wgpu::BindingResource::TextureView(&display) },
                    wgpu::BindGroupEntry { binding: 1, resource: wgpu::BindingResource::Sampler(sampler) },
                ],
            });
            p.group = Some((display, group));
        }
        let group = &p.group.as_ref().expect("present bind group").1;
        let rect = rect_bytes(r.rect);
        if uniform.written != Some(rect) {
            gpu.queue.write_buffer(&uniform.buffer, 0, &rect);
            uniform.written = Some(rect);
        }
        let mut separate = (!self.pass.deferred).then(|| gpu.device.create_command_encoder(
            &wgpu::CommandEncoderDescriptor { label: Some("native present") }));
        let encoder = match separate.as_mut() {
            Some(encoder) => encoder,
            None => self.pass.encoder(gpu),
        };
        let t = crate::native::render::rstat::T::new();
        let times = gputimes::slot(gpu, gputimes::PRESENT);
        {
            crate::native::render::pass_trace::event("present:clear/store", -1, -1);
            let mut pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
                label: Some("native present"),
                color_attachments: &[Some(wgpu::RenderPassColorAttachment {
                    view: &target,
                    depth_slice: None,
                    resolve_target: None,
                    ops: wgpu::Operations {
                        load: wgpu::LoadOp::Clear(wgpu::Color { r: 0.0, g: 0.0, b: 0.0, a: 1.0 }),
                        store: wgpu::StoreOp::Store,
                    },
                })],
                depth_stencil_attachment: None,
                timestamp_writes: times.as_ref().map(gputimes::Slot::render),
                occlusion_query_set: None,
                multiview_mask: None,
            });
            if let Some((x, y, sw, sh)) = scissor(r.cut, w, h) {
                pass.set_pipeline(objects.present_pipe(gpu, surface.format()));
                pass.set_bind_group(0, &uniform.group, &[0]);
                pass.set_bind_group(1, group, &[]);
                pass.set_scissor_rect(x, y, sw, sh);
                pass.draw(0..3, 0..1);
            }
        }
        #[cfg(not(target_arch = "wasm32"))]
        let _spike = crate::native::pipelines::SpikeTimer::new("present_submit", 0);
        t.add(crate::native::render::rstat::PRESENT_RECORD);
        let t = crate::native::render::rstat::T::new();
        if let Some(encoder) = separate {
            let mut encoder = encoder;
            gputimes::end_frame(gpu, &mut encoder);
            gpu.queue.submit([encoder.finish()]);
        } else {
            gputimes::end_frame(gpu, self.pass.encoder(gpu));
            self.pass.submit_now(gpu, &self.arenas);
            self.zread_map();
        }
        t.add(crate::native::render::rstat::PRESENT_SUBMIT);
        whole.add(crate::native::render::rstat::PRESENT);
        gputimes::submitted(gpu);
        1
    }

    /// `dhdr_native_options`: the present filter, `DHDR_NATIVE_POINT_FILTER` nearest, else linear
    /// (native_present_options; C reads DHD_PRESENT_FILTER). The first call makes the present
    /// uniform too (native_objects).
    pub fn options(&mut self, flags: u32) {
        let gpu = self.gpu;
        let p = &mut self.presenter;
        if p.uniform.is_none() {
            p.uniform = Some(uniform(gpu, &self.objects.uniform_layout));
        }
        let filter = if flags & DHDR_NATIVE_POINT_FILTER != 0 { wgpu::FilterMode::Nearest } else { wgpu::FilterMode::Linear };
        p.sampler = Some(sampler(gpu, filter));
        p.group = None;
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn scissor_clamps_to_the_frame() {
        assert_eq!(scissor([-5, -3, 700, 500], 640, 480), Some((0, 0, 640, 480)));
        assert_eq!(scissor([80, 0, 560, 480], 640, 480), Some((80, 0, 480, 480)));
        assert_eq!(scissor([640, 0, 700, 480], 640, 480), None);
        assert_eq!(scissor([10, 10, 10, 20], 640, 480), None);
    }

    #[test]
    fn rect_is_little_endian_f32() {
        let b = rect_bytes([1.0, 2.0, 640.0, 480.0]);
        assert_eq!(&b[0..4], &1.0f32.to_le_bytes());
        assert_eq!(&b[12..16], &480.0f32.to_le_bytes());
    }
}
