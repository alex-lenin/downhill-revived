# Online play: plan

Decided with the project owner on 2026-10-16. Revised the same day: input lockstep is dropped
(finding 041 has what was built and why it stopped); online races use state sync.

## Goal

Internet races for a full field of human riders, with real contact (punches, bumping,
power-up hits). It starts with friends and then opens to strangers through public and private
lobbies.

## Decisions

| Topic | Decision |
|---|---|
| Sync model | **State sync** (2026-10-16, replaces input lockstep). Each PC runs a normal 1-player race and owns its rider: no input delay, it sends its rider's state 20-30 times a second. Remote humans drive AI rider slots as puppets (AI and physics off, interpolated about 100 ms behind). The host runs the real AI riders and sends theirs. Contact (punches, bumps, power-up hits) is an attacker-authoritative event the victim's PC applies. Each PC reports its finish time; the host publishes the results. Finding 044. |
| Input delay | None: the local rider responds at once. Remote riders are shown about 100 ms in the past. |
| Players | A full field: the local rider plus remote riders in the AI slots (10 riders). Never split screen or multitap. |
| Empty slots | AI riders, as in the original. |
| Disconnect | The puppet stops getting snapshots; the host's AI can take the slot over. |
| Modes | Single races only. |
| Lobby | The room uses the game's own rider/bike/outfit previews, then an explicit READY/waiting screen. The host picks the level and starts when everyone present is ready (minimum two). Finding 065. |
| Connecting | Public lobby list, private lobbies and join codes, through a small relay. |
| Server | Cloudflare Workers and Durable Objects: lobbies, join codes, relay. |
| Chat | None (players use Discord). |
| Game data | Each player supplies their own US (NTSC-U) ISO, checked by hash. Other regions are refused. |
| Frame rate | Each player picks their own (60/120/144); nothing has to stay in step. |
| Cheating | Basic: same version and ISO hash checks. No state hashes (no lockstep to check). |

## How it works (input lockstep, dropped)

The state-sync design is in finding 044; this section describes the lockstep design it replaced.

- Each PC runs the race with N local "pads", one per human; AI drives the rest. Each field,
  the PCs exchange that field's inputs (tagged with the field number) and advance only when
  they have everyone's inputs for field F - delay.
- The camera and HUD follow the local player's rider full screen, not split screen. This is
  render-side only.
- A hash of the game state every few fields detects desyncs. Determinism across PCs relies on
  the port's exact EE/VU semantics (MXCSR, FTZ, the fabsf fix in finding 038) and on removing
  every host-time input (the virtual frame clock).

## Open research (before building)

1. How the game stores players, pads and riders: player count, the controller to player
   mapping, the rider table (stride 0x7BD0 on PAL), and the AI versus player flag. Can an AI
   rider read a pad?
2. Field size per track: how many riders a race holds.
3. Starting a race directly with a chosen track, riders and bikes (no menus); the random
   seed setup.
4. Two-PC determinism: two different PCs, the same inputs and the same state hash for a whole
   race, including the native renderer and interpolation on one side and not on the other.
5. Rollback cost: the size of the state that has to be saved per field (EE RAM, VU, scratchpad,
   SPU2 state that affects the game), and how fast it saves and restores.
6. Rendering one rider's view when the game runs in a 2-4 player mode, full screen and without
   split.
