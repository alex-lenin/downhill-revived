# 070: the start window and the shader cache

Players saw nothing for many seconds after a double-click and thought the game was stuck.

## Where the time went

`port/hle/splash.c` logs each start step to dhd.log (`start: <seconds> s: <step>`). On the
development PC, idle:

| step | before | after (second start on) |
|---|---|---|
| update check (dhd.ini `auto_update`) | up to 6 s (`CHECK_SECONDS`) | same |
| disc check, relaunch, game data | about 0.3 s | same |
| graphics (`gs_gpu_init`) | 8 s, 20-25 s with a cold driver cache | 0.05 s |

Every start compiled raster.hlsl's shaders with `D3DCompile`: three compute shaders for the exact
backend's Z probe, ten for the native renderer and two for the hardware path.

## The fixes

- `shader_compile` (port/gs/gpu.c) replaces every `D3DCompile` of raster.hlsl. It keeps the
  bytecode in `%LOCALAPPDATA%\DownhillRevived\shaders`. The file name hashes the source, entry
  point, target, defines, flags and the system `d3dcompiler_47.dll`'s size and date. The bytecode
  is the compiler's own output, so drawing is unchanged. `DHD_NO_SHADER_CACHE` turns the cache off.
- The start window (port/hle/splash.c) belongs to the first dhd.exe. It runs on its own thread
  and shows the icon art, the version, the current step and a bar that creeps towards the next
  step. The relaunched dhd.exe sends its steps with WM_COPYDATA and closes it once the game's
  window is shown (gs/present.c). A step longer than 4 s adds "The first start takes a little
  longer." The window gives way to the update question. `DHD_HEADLESS` or `DHD_NO_SPLASH` hides it.
- The parent allows the child to take the foreground (`AllowSetForegroundWindow`), so the
  game's window comes to the front when the start window closes.

No outside code used. The art is the existing icon (port/res/icon.png). 