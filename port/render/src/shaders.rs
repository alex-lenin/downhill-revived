//! The WGSL sources, embedded from `port/gs` as checked out. `.gitattributes` makes
//! `*.wgsl` `eol=lf`, so every checkout embeds the same bytes; a carriage return anywhere stops the
//! build here (the library must be byte-identical from every worktree). The C path embeds the
//! same files (`gs_wgsl.h`).

/// raster.wgsl: the exact renderer (`main`, `local_copy`, `scanout`, `depth_probe`).
pub const RASTER: &str = include_str!("../../gs/raster.wgsl");
/// present.wgsl: the exact present pass (`present_vertex`, `present_fragment`).
pub const PRESENT: &str = include_str!("../../gs/present.wgsl");
/// native.wgsl: the native renderer.
pub const NATIVE: &str = include_str!("../../gs/native.wgsl");
/// native_dual.wgsl: the dual-source blending entry points, appended to native.wgsl for the dual
/// module (`native/init.rs`).
pub const NATIVE_DUAL: &str = include_str!("../../gs/native_dual.wgsl");

const fn has_cr(s: &str) -> bool {
    let b = s.as_bytes();
    let mut i = 0;
    while i < b.len() {
        if b[i] == b'\r' {
            return true;
        }
        i += 1;
    }
    false
}

const _: () = assert!(!has_cr(RASTER), "port/gs/raster.wgsl has CR line endings (it must be LF)");
const _: () = assert!(!has_cr(PRESENT), "port/gs/present.wgsl has CR line endings (it must be LF)");
const _: () = assert!(!has_cr(NATIVE), "port/gs/native.wgsl has CR line endings (it must be LF)");
const _: () = assert!(!has_cr(NATIVE_DUAL), "port/gs/native_dual.wgsl has CR line endings (it must be LF)");
