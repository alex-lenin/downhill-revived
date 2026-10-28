//! The disc image, checked twice:
//! - to boot, as the game checks it before it starts (port/hle/host.c disc_problem), without its
//!   last step (the game's program and data hashed, game_data.c): an ISO 9660 volume whose
//!   SYSTEM.CNF boots SCUS_971.77. The launcher only refuses what the game would refuse; the game
//!   still checks the data itself. The game's other releases are named (finding 100).
//! - for online races, which need the USA disc's game files (finding 098): the game's own check,
//!   dhd_disc (port/render/disc), with the game's iso-check.txt, so the game doesn't hash the
//!   files again.

use std::fs::File;
use std::io::{Read, Seek, SeekFrom};
use std::path::{Path, PathBuf};

const SECTOR: usize = 2048;

/// The game's other releases (finding 100): the table the game reads too (host.c).
const OTHER_RELEASES: &str = include_str!("../../../hle/other_releases.inc");

/// The quoted arguments of each of the table's name(...) lines.
fn table(name: &'static str) -> impl Iterator<Item = Vec<&'static str>> {
    OTHER_RELEASES
        .lines()
        .filter_map(move |l| l.trim().strip_prefix(name)?.strip_prefix('('))
        .map(|args| args.split('"').skip(1).step_by(2).collect())
}

/// A boot file's letters and digits, upper case: SCPS_150.62 and SCPS-15062 alike.
fn serial_key(boot: &str) -> String {
    boot.chars().filter(char::is_ascii_alphanumeric).map(|c| c.to_ascii_uppercase()).collect()
}

/// What the release booting boot is, when it is one of the game's other releases.
fn other_release(boot: &str) -> Option<&'static str> {
    let key = serial_key(boot);
    if key.is_empty() {
        return None;
    }
    table("OTHER_RELEASE").find(|a| a.len() == 2 && serial_key(a[0]) == key).map(|a| a[1])
}

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
    if let Some(what) = id.as_deref().and_then(other_release) {
        let more = table("OTHER_RELEASES_MORE").next().and_then(|a| a.first().copied()).unwrap_or_default();
        return Some(format!("This is {what}. Downhill Revived needs the USA disc (SCUS-97177) for now. More: {more}"));
    }
    Some(match id {
        Some(id) => format!("It is another PlayStation 2 disc ({id}), not Downhill Domination (USA), SCUS-97177."),
        None => "It is another PlayStation 2 disc, not Downhill Domination (USA), SCUS-97177.".into(),
    })
}

/// The game's online cache, in the data folder (DHD_DATA_DIR: online_menu.c iso_check_path).
pub fn cache_path(data: &Path) -> PathBuf {
    data.join("iso-check.txt")
}

/// The online check of the disc image at path (dhd_disc::check, with the game's iso-check.txt in
/// the data folder): true for the USA disc's game files, which online races take. progress as in
/// dhd_disc::hash; Err(None) when it stopped it.
pub fn online_check(data: &Path, path: &Path, progress: impl FnMut(u64, u64) -> bool) -> Result<bool, Option<String>> {
    match dhd_disc::check(Some(&cache_path(data)), path, progress) {
        Ok((files, _)) => Ok(files.is_usa()),
        // no file system to hash, or a cut dump: not the USA disc's files
        Err(dhd_disc::Error::Format(_)) => Ok(false),
        Err(dhd_disc::Error::Stopped) => Err(None),
        Err(dhd_disc::Error::Read(e)) => Err(Some(e.to_string())),
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::fs;

    fn temp(name: &str) -> PathBuf {
        std::env::temp_dir().join(format!("dhd-launcher-disc-{}-{name}", std::process::id()))
    }

    /// A minimal ISO: the volume descriptor at 16, the root directory at 20, SYSTEM.CNF at 21.
    fn iso(name: &str, cnf: &str) -> PathBuf {
        let mut img = vec![0u8; 22 * SECTOR];
        let pvd = 16 * SECTOR;
        img[pvd] = 1;
        img[pvd + 1..pvd + 6].copy_from_slice(b"CD001");
        img[pvd + 158..pvd + 162].copy_from_slice(&20u32.to_le_bytes());
        img[pvd + 166..pvd + 170].copy_from_slice(&(SECTOR as u32).to_le_bytes());
        let r = 20 * SECTOR;
        let file = b"SYSTEM.CNF;1";
        img[r] = (33 + file.len()) as u8;
        img[r + 2..r + 6].copy_from_slice(&21u32.to_le_bytes());
        img[r + 10..r + 14].copy_from_slice(&(cnf.len() as u32).to_le_bytes());
        img[r + 32] = file.len() as u8;
        img[r + 33..r + 33 + file.len()].copy_from_slice(file);
        img[21 * SECTOR..21 * SECTOR + cnf.len()].copy_from_slice(cnf.as_bytes());
        let path = temp(&format!("{name}.iso"));
        fs::write(&path, img).unwrap();
        path
    }

    #[test]
    fn checks_the_boot_file() {
        let good = iso("usa", "BOOT2 = cdrom0:\\SCUS_971.77;1\r\nVER = 1.00\r\n");
        assert_eq!(problem(&good), None);
        let other = iso("other", "BOOT2 = cdrom0:\\SLUS_200.02;1\r\n");
        assert!(problem(&other).unwrap().contains("(SLUS_200.02)"));
        assert!(problem(Path::new("no-such.iso")).unwrap().contains("isn't there"));
        let _ = fs::remove_file(good);
        let _ = fs::remove_file(other);
    }

    #[test]
    fn names_the_other_releases() {
        let more = "More: https://github.com/skuza-oss/downhill-revived/blob/main/docs/findings/100-other-releases.md";
        let eu = iso("eu", "BOOT2 = cdrom0:\\SLES_522.02;1\r\nVER = 1.01\r\nVMODE = PAL\r\n\r\n");
        assert_eq!(
            problem(&eu).unwrap(),
            format!("This is the European disc (SLES-52202). Downhill Revived needs the USA disc (SCUS-97177) for now. {more}")
        );
        let jp = iso("jp", "BOOT2 = cdrom0:\\SCPS_150.62;1\r\nVER = 1.00\r\nVMODE = NTSC\r\n");
        assert_eq!(
            problem(&jp).unwrap(),
            format!(
                "This is the Japanese release, MT Bikers (SCPS-15062). Downhill Revived needs the USA disc (SCUS-97177) \
                 for now. {more}"
            )
        );
        // matched by letters and digits only, in any case
        assert_eq!(other_release("scps-15062"), Some("the Japanese release, MT Bikers (SCPS-15062)"));
        assert_eq!(other_release("SLES_522.03"), None);
        assert_eq!(other_release(""), None);
        assert_eq!(table("OTHER_RELEASE").count(), 2);
        let _ = fs::remove_file(eu);
        let _ = fs::remove_file(jp);
    }

    #[test]
    fn checks_the_game_files_for_online() {
        let data = temp("data");
        std::fs::create_dir_all(&data).unwrap();
        // an image that boots, but whose files aren't the USA disc's
        let img = iso("online", "BOOT2 = cdrom0:\\SCUS_971.77;1\r\n");
        assert_eq!(online_check(&data, &img, |_, _| true), Ok(false));
        // the answer is in the game's iso-check.txt now: not hashed again
        assert!(std::fs::read_to_string(cache_path(&data)).unwrap().starts_with("files:"));
        assert_eq!(online_check(&data, &img, |_, _| panic!("hashed again")), Ok(false));
        let _ = std::fs::remove_file(cache_path(&data));
        // stopped when another disc is chosen
        assert_eq!(online_check(&data, &img, |_, _| false), Err(None));
        // not an ISO 9660 image: not the USA disc's files either
        let text = temp("text.iso");
        std::fs::write(&text, b"abc").unwrap();
        assert_eq!(online_check(&data, &text, |_, _| true), Ok(false));
        assert!(matches!(online_check(&data, Path::new("no-such.iso"), |_, _| true), Err(Some(_))));
        let _ = std::fs::remove_file(img);
        let _ = std::fs::remove_file(text);
        let _ = std::fs::remove_dir_all(data);
    }
}
