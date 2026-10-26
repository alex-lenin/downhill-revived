//! Shared pieces of the replay tools (`src/bin/optrace_replay.rs`, `src/bin/native_replay.rs`).
//!
//! The tools stand where the C host stands in the game: they provide `dhd_host_log`, the one C
//! function the renderer calls (`include/dhd_render.h`), and print its lines on stderr.

use std::ffi::c_char;
use std::io::Write;

/// The host's log line (`dhd_render.h`: one UTF-8 line of `bytes` bytes, no NUL, no newline).
///
/// # Safety
/// `line` points at `bytes` readable bytes (the renderer's own string).
#[no_mangle]
pub unsafe extern "C" fn dhd_host_log(line: *const c_char, bytes: usize) {
    // SAFETY: the renderer passes a valid (pointer, length) pair of one of its strings.
    let text = unsafe { std::slice::from_raw_parts(line as *const u8, bytes) };
    let mut err = std::io::stderr().lock();
    let _ = err.write_all(text);
    let _ = err.write_all(b"\n");
    let _ = err.flush();
}

/// Keeps `dhd_host_log` in every binary that links this crate (call once from `main`).
pub fn link_host_log() {
    let f: unsafe extern "C" fn(*const c_char, usize) = dhd_host_log;
    std::hint::black_box(f);
}

/// FNV-1a 64 over `bytes` from `state` (offset basis `0xcbf29ce484222325`; OPTRACE.md, NTRACE.md).
pub fn fnv1a(mut state: u64, bytes: &[u8]) -> u64 {
    for &b in bytes {
        state ^= b as u64;
        state = state.wrapping_mul(0x100000001b3);
    }
    state
}

/// The FNV-1a 64 offset basis.
pub const FNV_BASIS: u64 = 0xcbf29ce484222325;
