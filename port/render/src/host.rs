//! The renderer's only way out to the host: log lines.
//!
//! Natively C provides `dhd_host_log` (gpu_rust.c: hle_log + stderr, dhd.log flushed; a test or a
//! tool defines it itself). In the browser the line goes to the console through a wasm-bindgen
//! import (module `wbg` after the build's rename), never through the C reactor. There are no other
//! host imports.

#[cfg(not(target_arch = "wasm32"))]
extern "C" {
    fn dhd_host_log(line: *const std::ffi::c_char, bytes: usize);
}

/// The crate's own unit tests (`cargo test`'s lib target) have no C host: stderr.
#[cfg(all(test, not(target_arch = "wasm32")))]
mod unit_test_host {
    #[no_mangle]
    pub unsafe extern "C" fn dhd_host_log(line: *const std::ffi::c_char, bytes: usize) {
        use std::io::Write;
        // SAFETY: (pointer, length) of a live str, from line_out.
        let text = unsafe { std::slice::from_raw_parts(line as *const u8, bytes) };
        let _ = std::io::stderr().lock().write_all(&[text, b"\n"].concat());
    }
}

#[cfg(all(target_arch = "wasm32", feature = "web"))]
mod console {
    use wasm_bindgen::prelude::*;
    #[wasm_bindgen]
    extern "C" {
        #[wasm_bindgen(js_namespace = console, js_name = log)]
        pub fn log(line: &str);
    }
}

/// Logs `text`, one host line per line of it (the host's lines have no newline).
pub fn log(text: &str) {
    for line in text.split('\n') {
        line_out(line.strip_suffix('\r').unwrap_or(line));
    }
}

fn line_out(line: &str) {
    #[cfg(not(target_arch = "wasm32"))]
    // SAFETY: (pointer, length) of a live str; the host copies it before returning.
    unsafe {
        dhd_host_log(line.as_ptr() as *const std::ffi::c_char, line.len())
    }
    #[cfg(all(target_arch = "wasm32", feature = "web"))]
    console::log(line);
    #[cfg(all(target_arch = "wasm32", not(feature = "web")))]
    let _ = line;
}
