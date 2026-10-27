//! The disc image check the game makes before it starts (port/hle/host.c disc_problem), without
//! its last step (the game's program and data hashed, game_data.c): an ISO 9660 volume whose
//! SYSTEM.CNF boots SCUS_971.77. The launcher only refuses what the game would refuse; the game
//! still checks the data itself.

use std::fs::File;
use std::io::{Read, Seek, SeekFrom};
use std::path::Path;

const SECTOR: usize = 2048;

fn le32(b: &[u8]) -> u32 {
    u32::from_le_bytes([b[0], b[1], b[2], b[3]])
}

fn read(f: &mut File, lsn: u32, buf: &mut [u8; SECTOR]) -> bool {
    f.seek(SeekFrom::Start(lsn as u64 * SECTOR as u64)).is_ok() && f.read_exact(buf).is_ok()
}

/// None when the file is Downhill Domination (USA); else why not, in the game's words.
pub fn problem(path: &Path) -> Option<String> {
    let Ok(mut f) = File::open(path) else {
        return Some(if path.exists() { "The file can't be opened." } else { "The file isn't there any more." }.into());
    };
    let mut s = [0u8; SECTOR];
    if !read(&mut f, 16, &mut s) || s[0] != 1 || &s[1..6] != b"CD001" {
        return Some("It isn't a disc image (no ISO 9660 volume in it).".into());
    }
    // the primary volume descriptor's root directory record (at 156): extent at +2, size at +10
    let (dir, dir_size) = (le32(&s[158..]), le32(&s[166..]));
    let mut cnf = None;
    let mut off = 0;
    while off < dir_size && off < 16 * SECTOR as u32 && cnf.is_none() {
        if !read(&mut f, dir + off / SECTOR as u32, &mut s) {
            break;
        }
        let mut i = 0;
        while i + 33 < SECTOR && s[i] != 0 {
            let len = s[i + 32] as usize;
            let name = &s[i + 33..(i + 33 + len).min(SECTOR)];
            if len >= 10 && name.len() == len && name[..10].eq_ignore_ascii_case(b"SYSTEM.CNF") && (len == 10 || name[10] == b';') {
                cnf = Some((le32(&s[i + 2..]), le32(&s[i + 10..])));
                break;
            }
            i += s[i] as usize;
        }
        off += SECTOR as u32;
    }
    let Some((_, size)) = cnf.filter(|&(at, _)| read(&mut f, at, &mut s)) else {
        return Some("It has no SYSTEM.CNF, so it isn't a PlayStation 2 game disc.".into());
    };
    let text = String::from_utf8_lossy(&s[..(size as usize).min(SECTOR)]).into_owned();
    if text.contains("SCUS_971.77") {
        return None;
    }
    // name the game it is: BOOT2 = cdrom0:\SLUS_123.45;1
    let id = text
        .find("BOOT2")
        .and_then(|b| text[b..].find(['\\', ':']).map(|i| b + i))
        .map(|i| text[i..].trim_start_matches(['\\', ':']))
        .map(|t| t[..t.find([';', '\r', '\n']).unwrap_or(t.len())].chars().take(31).collect::<String>())
        .filter(|id| !id.is_empty());
    Some(match id {
        Some(id) => format!("It is another PlayStation 2 disc ({id}), not Downhill Domination (USA), SCUS-97177."),
        None => "It is another PlayStation 2 disc, not Downhill Domination (USA), SCUS-97177.".into(),
    })
}

#[cfg(test)]
mod tests {
    use super::*;

    /// A minimal ISO: the volume descriptor at 16, the root directory at 20, SYSTEM.CNF at 21.
    fn iso(cnf: &str) -> std::path::PathBuf {
        let mut img = vec![0u8; 22 * SECTOR];
        let pvd = 16 * SECTOR;
        img[pvd] = 1;
        img[pvd + 1..pvd + 6].copy_from_slice(b"CD001");
        img[pvd + 158..pvd + 162].copy_from_slice(&20u32.to_le_bytes());
        img[pvd + 166..pvd + 170].copy_from_slice(&(SECTOR as u32).to_le_bytes());
        let r = 20 * SECTOR;
        let name = b"SYSTEM.CNF;1";
        img[r] = (33 + name.len()) as u8;
        img[r + 2..r + 6].copy_from_slice(&21u32.to_le_bytes());
        img[r + 10..r + 14].copy_from_slice(&(cnf.len() as u32).to_le_bytes());
        img[r + 32] = name.len() as u8;
        img[r + 33..r + 33 + name.len()].copy_from_slice(name);
        img[21 * SECTOR..21 * SECTOR + cnf.len()].copy_from_slice(cnf.as_bytes());
        let path = std::env::temp_dir().join(format!("dhd-launcher-disc-{}.iso", cnf.len()));
        std::fs::write(&path, img).unwrap();
        path
    }

    #[test]
    fn checks_the_boot_file() {
        let good = iso("BOOT2 = cdrom0:\\SCUS_971.77;1\r\nVER = 1.00\r\n");
        assert_eq!(problem(&good), None);
        let other = iso("BOOT2 = cdrom0:\\SLUS_200.02;1\r\n");
        assert!(problem(&other).unwrap().contains("(SLUS_200.02)"));
        assert!(problem(Path::new("no-such.iso")).unwrap().contains("isn't there"));
        let _ = std::fs::remove_file(good);
        let _ = std::fs::remove_file(other);
    }
}
