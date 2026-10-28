/* Releases of Downhill Domination other than the USA disc (finding 100). Downhill Revived runs
   the USA disc (SCUS-97177) only; a player who chooses one of these is told which release it is.
   One table for the game (host.c disc_problem) and the launcher (port/launcher disc.rs reads
   this file with include_str!): keep one entry per line, in this form. Each entry names the boot
   file in SYSTEM.CNF and what the release is. The boot file is matched by its letters and digits
   only, so SCPS_150.62 also matches SCPS-15062. */
OTHER_RELEASE("SLES_522.02", "the European disc (SLES-52202)")
OTHER_RELEASE("SCPS_150.62", "the Japanese release, MT Bikers (SCPS-15062)")
OTHER_RELEASES_MORE("https://github.com/skuza-oss/downhill-revived/blob/main/docs/findings/100-other-releases.md")
