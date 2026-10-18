"""Stop a tool's own game process, and only it, on Windows (by verified executable path)."""
import base64
import os
from pathlib import Path
import subprocess


def stop_verified(process, executable):
    if process.poll() is not None:
        return
    if os.name!='nt':
        process.terminate();process.wait(timeout=10);return
    # The fixed-address launcher can have a same-image child. Verify the root,
    # collect its descendants, and verify EACH live PID's path before stopping.
    # No taskkill /T: it would kill unverified descendants as a side effect.
    script=r'''
$ErrorActionPreference='Stop'
$rootPid=[int]$env:DHD_STOP_PID
$expected=[IO.Path]::GetFullPath($env:DHD_STOP_IMAGE)
$root=Get-Process -Id $rootPid -ErrorAction SilentlyContinue
if (!$root) { exit 0 }
if ($root.Path -ne $expected) { throw 'Root PID executable mismatch; left untouched' }
$all=@(Get-CimInstance Win32_Process)
$owned=@($rootPid)
for ($i=0;$i -lt $owned.Count;$i++) {
  $owned+=@($all | Where-Object { $_.ParentProcessId -eq $owned[$i] } | ForEach-Object { [int]$_.ProcessId })
}
[array]::Reverse($owned)
foreach ($ownedPid in $owned) {
  $live=Get-Process -Id $ownedPid -ErrorAction SilentlyContinue
  if (!$live) { continue }
  if ($live.Path -ne $expected) { throw 'Descendant PID executable mismatch; left untouched' }
  Stop-Process -Id $ownedPid -ErrorAction Stop
}
'''
    env=dict(os.environ,DHD_STOP_PID=str(process.pid),DHD_STOP_IMAGE=str(Path(executable).resolve()))
    encoded=base64.b64encode(script.encode('utf-16-le')).decode('ascii')
    subprocess.run(['powershell','-NoProfile','-EncodedCommand',encoded],env=env,check=True,capture_output=True)
    process.wait(timeout=10)
