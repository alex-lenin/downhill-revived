//! port/hle/settings_table.h is the crate's table for the game core's PC SETTINGS page. This test
//! fails while the file differs; with DHD_SETTINGS_BLESS=1 it writes it.

#[test]
fn settings_table_h_is_current() {
    let path = std::path::Path::new(env!("CARGO_MANIFEST_DIR")).join("../../hle/settings_table.h");
    let want = dhd_settings::table::c_table();
    if std::env::var_os("DHD_SETTINGS_BLESS").is_some() {
        std::fs::write(&path, &want).unwrap();
    }
    let have = std::fs::read_to_string(&path).unwrap_or_default().replace("\r\n", "\n");
    assert!(have == want, "port/hle/settings_table.h is not the table's: DHD_SETTINGS_BLESS=1 cargo test -p dhd_settings");
}
