# Downhill Domination online launcher (findings 041, 047): host a race or join one with a code.
# Everything it asks for goes into dhd.ini beside dhd.exe (port/hle/host.c, host_settings_load),
# including the server address the owner gives each player privately: the package has none. It
# checks that the ISO is the USA disc every PC must use, then starts dhd.exe with the DHD_NET_*
# settings and shows the room code and progress from the game's log.
$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Windows.Forms, System.Drawing
[System.Windows.Forms.Application]::EnableVisualStyles()

# Downhill Domination (USA) SCUS-97177, redump disc 30393 (MD5 3654a6f4438c899be4a1f5236cfdcd74).
$UsIsoSha256 = '0fe2b77713d3440cc3030741ea0b18f49e3feaa09671a30a421e0d4888a7eb86'
# The game's level codes for the tracks that have a 2-4 player version (online.c mp_level).
$Tracks = [ordered]@{ 0 = 'ALPINE'; 1 = 'MOAB'; 2 = 'JUNGLE'; 3 = 'PERU'; 4 = 'PERUTDH'; 5 = 'GLACIER'; 6 = 'ALPINEMX'
    7 = 'MOABMX'; 8 = 'JUNGLEMX'; 9 = 'PERUMX'; 10 = 'JUNGLETD'; 11 = 'AUBERST'; 12 = 'FUJI'; 13 = 'MOABTDH'
    14 = 'GLACIERX'; 15 = 'AUBERMX'; 16 = 'AUBERSTD'; 19 = 'BC'; 20 = 'CITY'; 22 = 'GLACIERT'; 23 = 'MOSH'
    24 = 'ALPINET'; 25 = 'BCTDX' }
# The riders every save has (online.c regular[]): the game's rider index and name.
$Riders = @(@(0, 'Cosmo'), @(1, 'Mai'), @(2, 'Ajax'), @(3, 'Jonnie'), @(4, 'Kalolo'), @(5, 'T-bag'), @(8, 'Drud'),
    @(10, 'Rakel'), @(13, 'Jen'), @(14, 'Woody'))

$here = $PSScriptRoot
$ini = Join-Path $here 'dhd.ini'
$oldSettings = Join-Path $here 'online-settings.txt'  # earlier packages kept the server here
$isoCheck = Join-Path $here 'iso-check.txt'
$exe = Join-Path $here 'dhd.exe'
$log = Join-Path $here 'online.log'

function Read-Ini([string]$path) {
    $s = [ordered]@{}
    if (Test-Path $path) {
        foreach ($line in Get-Content $path) {
            if ($line -match '^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(.*?)\s*$') { $s[$Matches[1]] = $Matches[2] }
        }
    }
    return $s
}
$settings = Read-Ini $ini
if (!$settings['server'] -and (Test-Path $oldSettings)) {
    $old = Read-Ini $oldSettings
    foreach ($k in 'server', 'name', 'iso') { if ($old[$k] -and !$settings[$k]) { $settings[$k] = $old[$k] } }
}
function Get-Setting($key, $default) { if ($settings[$key]) { $settings[$key] } else { $default } }

function Save-Ini {
    $lines = @('; Downhill Domination PC settings (Play-online.cmd and Settings.cmd write them; keep the server private)')
    foreach ($k in $settings.Keys) { if ("$($settings[$k])" -ne '') { $lines += "$k = $($settings[$k])" } }
    Set-Content -Path $ini -Value $lines -Encoding ASCII
}

function Show-Error([string]$text) {
    [void][System.Windows.Forms.MessageBox]::Show($text, 'Downhill Domination online', 'OK', 'Error')
}

# The ISO's SHA-256, remembered by size and date: hashing 2.5 GB takes a while.
function Get-IsoHash([string]$path, $status) {
    $f = Get-Item -LiteralPath $path
    $key = "$($f.Length):$($f.LastWriteTimeUtc.Ticks)"
    if ((Test-Path $isoCheck) -and (Get-Content -Raw $isoCheck).Trim().StartsWith("$key`:")) {
        return (Get-Content -Raw $isoCheck).Trim().Split(':')[2]
    }
    $status.Text = 'Checking the ISO (about half a minute)...'
    [System.Windows.Forms.Application]::DoEvents()
    $hash = (Get-FileHash -LiteralPath $path -Algorithm SHA256).Hash.ToLowerInvariant()
    Set-Content -Path $isoCheck -Value "$key`:$hash" -Encoding ASCII
    return $hash
}

$form = New-Object System.Windows.Forms.Form
$form.Text = 'Downhill Domination online'
$form.ClientSize = New-Object System.Drawing.Size(440, 520)
$form.FormBorderStyle = 'FixedDialog'
$form.MaximizeBox = $false
$form.StartPosition = 'CenterScreen'
$form.Font = New-Object System.Drawing.Font('Segoe UI', 10)

function Add-Label([string]$text, [int]$x, [int]$y) {
    $l = New-Object System.Windows.Forms.Label
    $l.Text = $text; $l.Location = New-Object System.Drawing.Point($x, $y); $l.AutoSize = $true
    $form.Controls.Add($l)
    return $l
}
function Add-Control($c, [int]$x, [int]$y, [int]$w) {
    $c.Location = New-Object System.Drawing.Point($x, $y); $c.Width = $w
    $form.Controls.Add($c)
    return $c
}
function New-List($items, [int]$selected) {
    $c = New-Object System.Windows.Forms.ComboBox
    $c.DropDownStyle = 'DropDownList'
    foreach ($i in $items) { [void]$c.Items.Add($i) }
    $c.SelectedIndex = $selected
    return $c
}

[void](Add-Label 'Your name' 16 18)
$name = Add-Control (New-Object System.Windows.Forms.TextBox) 120 15 300
$name.MaxLength = 16
$name.Text = Get-Setting 'name' $env:USERNAME

[void](Add-Label 'Server' 16 54)
$server = Add-Control (New-Object System.Windows.Forms.TextBox) 120 51 300
$server.UseSystemPasswordChar = $true  # it's private: shown as dots
$server.Text = Get-Setting 'server' ''

[void](Add-Label 'Game ISO' 16 90)
$iso = Add-Control (New-Object System.Windows.Forms.TextBox) 120 87 220
$iso.Text = Get-Setting 'iso' ''
$browse = Add-Control (New-Object System.Windows.Forms.Button) 346 86 74
$browse.Text = 'Browse...'
$browse.Add_Click({
        $d = New-Object System.Windows.Forms.OpenFileDialog
        $d.Filter = 'PS2 disc image (*.iso)|*.iso|All files (*.*)|*.*'
        if ($d.ShowDialog() -eq 'OK') { $iso.Text = $d.FileName }
    })

[void](Add-Label 'Rider' 16 126)
$riderIndex = 4
for ($i = 0; $i -lt $Riders.Count; $i++) { if ("$($Riders[$i][0])" -eq (Get-Setting 'rider' '4')) { $riderIndex = $i } }
$rider = Add-Control (New-List ($Riders | ForEach-Object { $_[1] }) $riderIndex) 120 123 300

$host_ = Add-Control (New-Object System.Windows.Forms.RadioButton) 16 162 200
$host_.Text = 'Create a race'
$host_.Checked = $true
$join = Add-Control (New-Object System.Windows.Forms.RadioButton) 220 162 200
$join.Text = 'Join with a code'

[void](Add-Label 'Track' 36 198)
$track = Add-Control (New-List ($Tracks.GetEnumerator() | ForEach-Object { "$($_.Key)  $($_.Value)" }) 0) 140 195 280
[void](Add-Label 'Start' 36 232)
$players = Add-Control (New-List @('when I press START (or Enter)', 'when 2 players are in', 'when 3 players are in',
        'when 4 players are in') 0) 140 229 280

[void](Add-Label 'Code' 36 268)
$code = Add-Control (New-Object System.Windows.Forms.TextBox) 140 265 120
$code.CharacterCasing = 'Upper'
$code.MaxLength = 6
$copy = Add-Control (New-Object System.Windows.Forms.Button) 268 264 152
$copy.Text = 'Copy the code'
$copy.Enabled = $false
$copy.Add_Click({ if ($code.Text) { [System.Windows.Forms.Clipboard]::SetText($code.Text) } })

# The picture: each player's own choice, it doesn't have to match the others'.
[void](Add-Label 'Resolution' 16 306)
$scales = @('1', '2', '3', '4')
$scaleIndex = [Math]::Max(0, [Array]::IndexOf($scales, (Get-Setting 'scale' '2')))
$scaleNames = @('1x (the PS2''s own)', '2x (recommended)', '3x', '4x (sharpest, slower)')
$scale = Add-Control (New-List $scaleNames $scaleIndex) 120 303 300
[void](Add-Label 'Display' 16 340)
$windows = @('windowed', 'borderless', 'fullscreen')
$windowIndex = [Math]::Max(0, [Array]::IndexOf($windows, (Get-Setting 'window' 'windowed')))
$windowNames = @('Window', 'Borderless (whole screen)', 'Fullscreen (exclusive)')
$window = Add-Control (New-List $windowNames $windowIndex) 120 337 300
$wide = Add-Control (New-Object System.Windows.Forms.CheckBox) 16 372 404
$wide.Text = 'Widescreen 16:9 (wider view)'
$wide.Checked = (Get-Setting 'widescreen' '0') -ne '0'

$start = Add-Control (New-Object System.Windows.Forms.Button) 16 408 404
$start.Text = 'Start'
$start.Height = 34
$status = Add-Label '' 16 452
$status.MaximumSize = New-Object System.Drawing.Size(404, 60)

function Update-Mode {
    $track.Enabled = $host_.Checked; $players.Enabled = $host_.Checked
    $code.ReadOnly = $host_.Checked
    if ($host_.Checked) { $code.Text = '' }
}
$host_.Add_CheckedChanged({ Update-Mode })
Update-Mode

$game = $null
$timer = New-Object System.Windows.Forms.Timer
$timer.Interval = 500
$timer.Add_Tick({
        if (Test-Path $log) {
            $text = Get-Content -Raw $log -ErrorAction SilentlyContinue
            if ($text -match 'online: room (\w+)') { $code.Text = $Matches[1]; $copy.Enabled = $true }
            if ($text -match 'online: result 1\.') { $status.Text = 'Race over: the results are on the game screen.' }
            elseif ($text -match 'online: countdown starts') { $status.Text = 'Racing. Close the game window to stop.' }
            elseif ($text -match 'online: room \w+ \(host\)') {
                $status.Text = 'Send your friends the code. The race starts as you chose (see the game window''s title).'
            } elseif ($text -match 'online: room') { $status.Text = 'In the room; waiting for the host to start.' }
            if ($text -match 'online: (Online play stopped: [^\r\n]*)') { $status.Text = $Matches[1] }
        }
        if ($game -and $game.HasExited) {
            $timer.Stop(); $start.Enabled = $true
            if ($status.Text -notmatch 'stopped') { $status.Text = "The game closed (exit $($game.ExitCode))." }
        }
    })

$start.Add_Click({
        if ($server.Text.Trim() -notmatch '^wss?://\S+$') {
            Show-Error "Paste the server address you were given (it starts with wss://) into the Server box."
            return
        }
        if (!(Test-Path $exe)) { Show-Error "dhd.exe is missing beside the launcher."; return }
        if (!$iso.Text -or !(Test-Path -LiteralPath $iso.Text)) { Show-Error 'Choose your Downhill Domination ISO.'; return }
        if ($join.Checked -and $code.Text -notmatch '^[A-Z0-9]{6}$') { Show-Error 'Type the 6-character code the host gave you.'; return }
        if (!$name.Text.Trim()) { Show-Error 'Type a name.'; return }
        # A game left running from this folder keeps online.log open: a new one couldn't write it
        # and closed at once (exit 1). Offer to close the old one.
        $running = @(Get-CimInstance Win32_Process -Filter "name='dhd.exe'" -ErrorAction SilentlyContinue |
            Where-Object { $_.ExecutablePath -and ((Resolve-Path -LiteralPath $_.ExecutablePath).Path -eq (Resolve-Path -LiteralPath $exe).Path) })
        if ($running.Count) {
            $answer = [System.Windows.Forms.MessageBox]::Show(
                "The game from this folder is still running (maybe hidden behind other windows). Close it and start a new one?",
                'Downhill Domination online', 'YesNo', 'Question')
            if ($answer -ne 'Yes') { return }
            foreach ($p in $running) { Stop-Process -Id $p.ProcessId -Force -ErrorAction SilentlyContinue }
            Start-Sleep -Milliseconds 500
        }
        $start.Enabled = $false
        $settings['name'] = $name.Text.Trim()
        $settings['server'] = $server.Text.Trim()
        $settings['iso'] = $iso.Text
        $settings['rider'] = "$($Riders[$rider.SelectedIndex][0])"
        $settings['scale'] = $scales[$scale.SelectedIndex]
        $settings['window'] = $windows[$window.SelectedIndex]
        $settings['widescreen'] = if ($wide.Checked) { '1' } else { '0' }
        if (!$settings['renderer']) { $settings['renderer'] = 'native' }
        if (!$settings['skip_movies']) { $settings['skip_movies'] = '1' }
        Save-Ini
        $hash = Get-IsoHash $iso.Text $status
        if ($hash -ne $UsIsoSha256) {
            $start.Enabled = $true
            $status.Text = ''
            Show-Error ("This ISO isn't the USA Downhill Domination disc (SCUS-97177). Online races need exactly " +
                "that disc image on every PC.`n`nSHA-256: $hash")
            return
        }
        # dhd.ini has the rest; the environment only says what to do this time
        Get-ChildItem Env: | Where-Object { $_.Name -like 'DHD_*' } | ForEach-Object { Remove-Item "Env:$($_.Name)" }
        $env:DHD_NET_ISO_SHA256 = $hash
        if ($host_.Checked) {
            $env:DHD_NET_CREATE = '1'
            $env:DHD_NET_TRACK = ($track.SelectedItem -split '\s+')[0]
            if ($players.SelectedIndex -gt 0) { $env:DHD_NET_PLAYERS = "$($players.SelectedIndex + 1)" }
        } else {
            $env:DHD_NET_JOIN = $code.Text
        }
        Remove-Item $log -ErrorAction SilentlyContinue
        $status.Text = 'Starting the game...'
        # cmd only to send the game's messages to online.log, which the timer reads
        $script:game = Start-Process -FilePath $env:ComSpec -ArgumentList "/c `"`"$exe`" 2> `"$log`"`"" `
            -WorkingDirectory $here -WindowStyle Hidden -PassThru
        $timer.Start()
    })

[void]$form.ShowDialog()
