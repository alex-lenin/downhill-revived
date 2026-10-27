//! Starting the game beside the launcher and waiting for it.
//!
//! The game is told it was started by the launcher (DHD_LAUNCHER=1): it then neither looks for
//! updates itself (DHD_AUTO_UPDATE=off; the launcher did) nor starts the launcher again, and the
//! online menu's Update Now ends it with UPDATE_EXIT_CODE so the launcher installs the update.

use std::path::{Path, PathBuf};
use std::process::{Child, Command};

#[cfg(windows)]
pub const GAME_EXE: &str = "dhd.exe";
#[cfg(not(windows))]
pub const GAME_EXE: &str = "dhd";

/// "UPDT": the game asks for an update (port/hle/update.c).
pub const UPDATE_EXIT_CODE: u32 = 0x5550_4454;

/// The folder of the game: the launcher's own, or DHD_GAME_DIR (development).
pub fn dir() -> PathBuf {
    if let Some(d) = std::env::var_os("DHD_GAME_DIR") {
        return PathBuf::from(d);
    }
    std::env::current_exe()
        .ok()
        .and_then(|p| p.parent().map(Path::to_path_buf))
        .unwrap_or_else(|| PathBuf::from("."))
}

pub fn exe(dir: &Path) -> PathBuf {
    dir.join(GAME_EXE)
}

pub fn start(dir: &Path) -> std::io::Result<Child> {
    Command::new(exe(dir))
        .current_dir(dir)
        .env("DHD_LAUNCHER", "1")
        .env("DHD_AUTO_UPDATE", "off")
        .spawn()
}

/// The exit code as the game set it (a u32 on Windows, where UPDATE_EXIT_CODE is negative as i32).
pub fn exit_code(status: std::process::ExitStatus) -> Option<u32> {
    status.code().map(|c| c as u32)
}
