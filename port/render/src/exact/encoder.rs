//! The open command encoder and compute pass, and the exact ops that record into them (task A3).
//!
//! As the retired `gpu_wgpu.c`'s op_* (and `gpu.js`'s web_gpu_*) did (last at render/rust
//! 3882d5576): one open encoder; `batch` dispatches main
//! in a `ComputePass<'static>` (`forget_lifetime`) kept open across calls (main set when the pass
//! opens, then set_bind_group with the dynamic offset and dispatch per batch); `copy` and `record`
//! end that pass first; `record` runs local_copy or scanout in a pass of its own (scanout first
//! sizes the display texture: `display.rs`); `submit` ends the pass and submits (nothing recorded:
//! nothing happens, as gpu.js's; queue writes then wait for the next submit, which still runs
//! them before its commands). `write` is a queue write. `destroy` submits pending work.
//! A buffer or pipeline number outside the header's is fatal (`guard::fatal`).
//!
//! Uses from `exact::pipelines` (A2): `Pipelines::create`, `Pipelines::main`, `local_copy` and
//! `scanout` (`wgpu::ComputePipeline`). From `exact::display` (A4): `Display::new`,
//! `Display::ensure` and `Display::scanout_group()` (`Option<&wgpu::BindGroup>`, scanout's group 0
//! for the current texture). For A4's capture this file offers `Encoder::encoder` (the open
//! encoder outside any pass) and `Encoder::submit`; `readback.rs` offers `map_blocking`.
//!
//! Referee: optrace_replay (A3).
use crate::abi::*;
use crate::device::Gpu;
use crate::exact::Exact;

pub struct Encoder {
    /// The open compute pass (main set), if any. Declared before `encoder`: a pass must end (drop)
    /// before its encoder finishes or drops.
    pub pass: Option<wgpu::ComputePass<'static>>,
    /// The open command encoder, if anything was recorded since the last submit.
    pub encoder: Option<wgpu::CommandEncoder>,
}

impl Encoder {
    pub fn new() -> Encoder {
        Encoder { pass: None, encoder: None }
    }

    /// Ends the open compute pass, if any.
    pub fn end_pass(&mut self) {
        self.pass = None;
    }

    /// The open encoder (opened if needed), with no compute pass open in it: copies and other
    /// passes go after what was recorded.
    pub fn encoder(&mut self, gpu: &Gpu) -> &mut wgpu::CommandEncoder {
        self.end_pass();
        self.encoder
            .get_or_insert_with(|| gpu.device.create_command_encoder(&wgpu::CommandEncoderDescriptor::default()))
    }

    /// The open compute pass with `main` set (opened, and main set, if needed).
    pub fn pass(&mut self, gpu: &Gpu, main: &wgpu::ComputePipeline) -> &mut wgpu::ComputePass<'static> {
        if self.pass.is_none() {
            let encoder = self
                .encoder
                .get_or_insert_with(|| gpu.device.create_command_encoder(&wgpu::CommandEncoderDescriptor::default()));
            let mut pass = encoder.begin_compute_pass(&wgpu::ComputePassDescriptor::default()).forget_lifetime();
            pass.set_pipeline(main);
            self.pass = Some(pass);
        }
        self.pass.as_mut().unwrap()
    }

    /// One dispatch of `pipeline` with `group` in a compute pass of its own, after the open pass.
    pub fn dispatch_alone(&mut self, gpu: &Gpu, pipeline: &wgpu::ComputePipeline, group: &wgpu::BindGroup, x: u32,
                          y: u32) {
        let mut pass = self.encoder(gpu).begin_compute_pass(&wgpu::ComputePassDescriptor::default());
        pass.set_pipeline(pipeline);
        pass.set_bind_group(0, group, &[]);
        pass.dispatch_workgroups(x, y, 1);
    }

    /// Ends the pass and submits the open encoder. False when nothing was recorded (no submit).
    pub fn submit(&mut self, gpu: &Gpu) -> bool {
        self.end_pass();
        match self.encoder.take() {
            Some(e) => {
                gpu.queue.submit([e.finish()]);
                true
            }
            None => false,
        }
    }
}

impl Exact {
    /// Buffers (zeroed) and every exact pipeline, eagerly (`dhdr_exact_create`).
    pub fn create(gpu: &'static Gpu, raster_flags: u32) -> Result<Exact, String> {
        let pipelines = crate::exact::pipelines::Pipelines::create(gpu, raster_flags)?;
        let buffers = crate::exact::buffers::Buffers::create(gpu, &pipelines)?;
        Ok(Exact {
            gpu,
            raster_flags,
            pipelines,
            buffers,
            encoder: Encoder::new(),
            reading: crate::exact::readback::Reading::new(),
            display: crate::exact::display::Display::new(),
            stats: DhdrExactStats::default(),
        })
    }

    /// `dhdr_exact_write`
    pub fn write(&mut self, buffer: u32, offset: u32, data: &[u8]) {
        let b = self.buffers.get(buffer);
        if !data.is_empty() {
            self.gpu.queue.write_buffer(b, offset as u64, data);
        }
        self.stats.bytes_written += data.len() as u64;
    }

    /// `dhdr_exact_copy`
    pub fn copy(&mut self, from: u32, from_offset: u32, to: u32, to_offset: u32, bytes: u32) {
        let (src, dst) = (self.buffers.get(from), self.buffers.get(to));
        self.encoder
            .encoder(self.gpu)
            .copy_buffer_to_buffer(src, from_offset as u64, dst, to_offset as u64, Some(bytes as u64));
        self.stats.copies += 1;
    }

    /// `dhdr_exact_batch`
    pub fn batch(&mut self, dynamic_offset: u32, groups_x: u32, groups_y: u32) {
        let pass = self.encoder.pass(self.gpu, &self.pipelines.main);
        pass.set_bind_group(0, &self.buffers.main_group, &[dynamic_offset]);
        pass.dispatch_workgroups(groups_x, groups_y, 1);
        self.stats.dispatches += 1;
    }

    /// `dhdr_exact_record`: `DHDR_P_LOCAL_COPY` or `DHDR_P_SCANOUT` (sizes in the high halves).
    pub fn record(&mut self, pipeline: u32, x_word: u32, y_word: u32) {
        match pipeline {
            DHDR_P_LOCAL_COPY => {
                self.encoder.dispatch_alone(self.gpu, &self.pipelines.local_copy, &self.buffers.copy_group, x_word,
                                            y_word);
            }
            DHDR_P_SCANOUT => {
                let (width, height) = (x_word >> 16, y_word >> 16);
                if let Err(e) = self.display.ensure(self.gpu, &self.pipelines, &self.buffers, width, height) {
                    // The scanout is dropped (logged); op_record (gpu_rust_shared.inc) still reports 1.
                    crate::host::log(&format!("dhd_render: dhdr_exact_record: display {width} x {height}: {e}"));
                    return;
                }
                let Some(group) = self.display.scanout_group() else {
                    crate::host::log(&format!("dhd_render: dhdr_exact_record: no scanout group at {width} x {height}"));
                    return;
                };
                self.encoder.dispatch_alone(self.gpu, &self.pipelines.scanout, group, x_word & 0xFFFF, y_word & 0xFFFF);
            }
            _ => crate::guard::fatal(&format!("dhd_render: dhdr_exact_record: pipeline {pipeline} is not a DHDR_P_* number")),
        }
        self.stats.dispatches += 1;
    }

    /// `dhdr_exact_submit`
    pub fn submit(&mut self) {
        if self.encoder.submit(self.gpu) {
            self.stats.submits += 1;
        }
    }

    /// `dhdr_exact_stats`
    pub fn stats(&self) -> DhdrExactStats {
        self.stats
    }

    /// `dhdr_exact_destroy`: submits pending work, then frees everything.
    pub fn destroy(mut self) {
        self.submit();
    }
}
