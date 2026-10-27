//! dhd.ini, read and written the way the game does (port/hle/host.c): one "key = value" a line,
//! [sections] and ;/# comments ignored, a value in double quotes taken as it is. Saving replaces
//! the key's first line or adds one at the end; every other line stays as it was.

use std::collections::BTreeMap;
use std::fs;
use std::io::Write;
use std::path::{Path, PathBuf};

/// The first lines of a dhd.ini the launcher starts (the game writes its own longer template when
/// it saves first; either way the game's player defaults apply once the file exists).
const HEADER: &str = "; Downhill Revived settings: one \"key = value\" per line. The launcher and\n\
; OPTIONS > PC SETTINGS in the game write them here. DHD_* environment variables override them.\n";

/// dhd.ini's path: DHD_SETTINGS, else beside the game.
pub fn path(game_dir: &Path) -> PathBuf {
    match std::env::var_os("DHD_SETTINGS") {
        Some(p) => PathBuf::from(p),
        None => game_dir.join("dhd.ini"),
    }
}

fn split(line: &str) -> Option<(&str, &str)> {
    let s = line.trim();
    if s.is_empty() || s.starts_with([';', '#', '[']) {
        return None;
    }
    let (k, v) = s.split_once('=')?;
    Some((k.trim(), v.trim()))
}

/// A value in double quotes as it is, else up to a comment that follows a space.
fn value(v: &str) -> String {
    if let Some(rest) = v.strip_prefix('"') {
        if let Some(end) = rest.find('"') {
            return rest[..end].to_string();
        }
    }
    let b = v.as_bytes();
    for (i, c) in b.iter().enumerate() {
        if (*c == b';' || *c == b'#') && (i == 0 || b[i - 1] == b' ' || b[i - 1] == b'\t') {
            return v[..i].trim().to_string();
        }
    }
    v.to_string()
}

/// Every key's first value, keys lower-cased (the game compares them without case).
pub fn read(path: &Path) -> BTreeMap<String, String> {
    let mut out = BTreeMap::new();
    if let Ok(text) = fs::read_to_string(path) {
        for line in text.lines() {
            if let Some((k, v)) = split(line) {
                out.entry(k.to_ascii_lowercase()).or_insert_with(|| value(v));
            }
        }
    }
    out
}

pub fn save(path: &Path, key: &str, val: &str) -> std::io::Result<()> {
    let quote = if val.contains([';', '#', '"']) || val.starts_with(' ') { "\"" } else { "" };
    let line = format!("{key} = {quote}{val}{quote}");
    let old = fs::read_to_string(path).ok();
    let mut out = String::new();
    let mut written = false;
    match &old {
        None => out.push_str(HEADER),
        Some(text) => {
            for l in text.lines() {
                if !written && split(l).is_some_and(|(k, _)| k.eq_ignore_ascii_case(key)) {
                    out.push_str(&line);
                    written = true;
                } else {
                    out.push_str(l);
                }
                out.push('\n');
            }
        }
    }
    if !written {
        out.push_str(&line);
        out.push('\n');
    }
    let temp = path.with_extension("ini.new");
    {
        let mut f = fs::File::create(&temp)?;
        f.write_all(out.as_bytes())?;
        f.sync_all()?;
    }
    fs::rename(&temp, path).inspect_err(|_| {
        let _ = fs::remove_file(&temp);
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn keeps_other_lines() {
        let dir = std::env::temp_dir().join(format!("dhd-ini-test-{}", std::process::id()));
        fs::create_dir_all(&dir).unwrap();
        let p = dir.join("dhd.ini");
        fs::write(&p, "; comment\n[x]\nScale = 2 ; two\niso = \"D:\\a;b.iso\"\n").unwrap();
        save(&p, "scale", "4").unwrap();
        save(&p, "name", "SKUZA").unwrap();
        let text = fs::read_to_string(&p).unwrap();
        assert_eq!(text, "; comment\n[x]\nscale = 4\niso = \"D:\\a;b.iso\"\nname = SKUZA\n");
        let m = read(&p);
        assert_eq!(m["iso"], "D:\\a;b.iso");
        assert_eq!(m["scale"], "4");
        fs::remove_dir_all(dir).unwrap();
    }
}
