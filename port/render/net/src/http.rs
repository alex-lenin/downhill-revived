//! http_get's client: one HTTP/1.1 GET with `Connection: close` on the WebSocket's stream (TCP,
//! rustls for https), its response parsed by httparse; Content-Length or chunked bodies.

use std::io::{Read, Write};
use std::time::Duration;

use tungstenite::http::Uri;

use crate::stream::{self, AGENT};

/// The longest response read (the lobby list is a few kB).
const LIMIT: usize = 4 << 20;

/// The body of a 200 response to GET url.
pub fn get(url: &str) -> Result<Vec<u8>, String> {
    let (secure, rest) = if let Some(r) = url.strip_prefix("https://").or_else(|| url.strip_prefix("wss://")) {
        (true, r)
    } else if let Some(r) = url.strip_prefix("http://").or_else(|| url.strip_prefix("ws://")) {
        (false, r)
    } else {
        return Err(format!("bad URL {url}"));
    };
    let uri: Uri = format!("{}://{rest}", if secure { "https" } else { "http" })
        .parse()
        .map_err(|_| format!("bad URL {url}"))?;
    let (mut s, tcp) = stream::open(&uri, secure)?;
    let _ = tcp.set_read_timeout(Some(Duration::from_secs(10)));
    let authority = uri.authority().map(|a| a.as_str()).unwrap_or_default();
    let path = uri.path_and_query().map(|p| p.as_str()).unwrap_or("/");
    let request = format!(
        "GET {path} HTTP/1.1\r\nHost: {authority}\r\nUser-Agent: {AGENT}\r\nAccept: */*\r\nConnection: close\r\n\r\n"
    );
    s.write_all(request.as_bytes()).and_then(|_| s.flush()).map_err(|e| e.to_string())?;
    let mut data = Vec::new();
    let mut buf = [0u8; 16384];
    loop {
        match s.read(&mut buf) {
            Ok(0) => break,
            Ok(n) => data.extend_from_slice(&buf[..n]),
            // a peer that closes without TLS's close_notify, or a read error after the body: the
            // length checks below decide
            Err(_) => break,
        }
        if data.len() > LIMIT {
            return Err("response too long".into());
        }
    }
    let mut headers = [httparse::EMPTY_HEADER; 64];
    let mut response = httparse::Response::new(&mut headers);
    let start = match response.parse(&data) {
        Ok(httparse::Status::Complete(n)) => n,
        _ => return Err("no HTTP response".into()),
    };
    if response.code != Some(200) {
        return Err(format!("HTTP {}", response.code.unwrap_or(0)));
    }
    let header = |name: &str| {
        response.headers.iter().find(|h| h.name.eq_ignore_ascii_case(name)).map(|h| String::from_utf8_lossy(h.value).trim().to_ascii_lowercase())
    };
    let body = &data[start..];
    if header("transfer-encoding").is_some_and(|v| v.contains("chunked")) {
        return dechunk(body);
    }
    match header("content-length").map(|v| v.parse::<usize>()) {
        Some(Ok(n)) if n <= body.len() => Ok(body[..n].to_vec()),
        Some(_) => Err("short body".into()),
        None => Ok(body.to_vec()),
    }
}

/// A chunked body's data (RFC 9112 section 7.1).
fn dechunk(mut body: &[u8]) -> Result<Vec<u8>, String> {
    let mut out = Vec::new();
    loop {
        let line = body.windows(2).position(|w| w == b"\r\n").ok_or("short chunk")?;
        let size = std::str::from_utf8(&body[..line]).ok().and_then(|l| usize::from_str_radix(l.split(';').next()?.trim(), 16).ok());
        let size = size.ok_or("bad chunk size")?;
        body = &body[line + 2..];
        if size == 0 {
            return Ok(out);
        }
        if body.len() < size + 2 {
            return Err("short chunk".into());
        }
        out.extend_from_slice(&body[..size]);
        body = &body[size + 2..];
    }
}
