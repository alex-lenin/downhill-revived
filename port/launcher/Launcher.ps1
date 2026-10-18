# Downhill Domination PC launcher: edits dhd.ini next to dhd.exe and starts the game.
# Usage: powershell -ExecutionPolicy Bypass -File Launcher.ps1 [-Exe path\to\dhd.exe]
# port/tools/build_clean.py --install copies it to build/port/fast-preview with Launcher.cmd.
# Every setting maps to a DHD_* environment variable (port/hle/host.c, host_settings_load);
# variables set in the environment still override the file. The game's own OPTIONS > PC SETTINGS
# page changes the same settings.
param([string]$Exe = (Join-Path $PSScriptRoot 'native\dhd.exe'))
$ErrorActionPreference = 'Stop'
Add-Type -AssemblyName System.Windows.Forms, System.Drawing
[System.Windows.Forms.Application]::EnableVisualStyles()

$ini = Join-Path (Split-Path $Exe) 'dhd.ini'
$settings = [ordered]@{}
if (Test-Path $ini) {
    foreach ($line in Get-Content $ini) {
        if ($line -match '^\s*([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(.*?)\s*$') { $settings[$Matches[1]] = $Matches[2] }
    }
}
if (-not $settings.Contains('iso')) {
    $local = Join-Path $PSScriptRoot '..\r27\local-iso.txt'
    if (Test-Path $local) { $settings['iso'] = (Get-Content -Raw $local).Trim() }
}
function Get-Setting($key, $default) { if ($settings.Contains($key)) { $settings[$key] } else { $default } }

$form = New-Object System.Windows.Forms.Form
$form.Text = 'Downhill Domination'
$form.FormBorderStyle = 'FixedDialog'; $form.MaximizeBox = $false
$form.ClientSize = New-Object System.Drawing.Size 460, 330
$form.StartPosition = 'CenterScreen'
$script:row = 14
function Add-Row($label, $control) {
    $l = New-Object System.Windows.Forms.Label
    $l.Text = $label; $l.Location = New-Object System.Drawing.Point 14, ($script:row + 3); $l.AutoSize = $true
    $control.Location = New-Object System.Drawing.Point 150, $script:row
    if (-not $control.Width -or $control.Width -lt 100) { $control.Width = 290 }
    $form.Controls.Add($l); $form.Controls.Add($control)
    $script:row += 32
}
# A drop-down whose items are (label, value) pairs; selects the item with the current value.
function New-Choice($pairs, $value) {
    $c = New-Object System.Windows.Forms.ComboBox
    $c.DropDownStyle = 'DropDownList'; $c.Width = 290
    foreach ($p in $pairs) { [void]$c.Items.Add($p[0]) }
    $c.Tag = $pairs
    $index = 0
    for ($i = 0; $i -lt $pairs.Count; $i++) { if ($pairs[$i][1] -eq $value) { $index = $i } }
    $c.SelectedIndex = $index
    $c
}
function Get-Choice($c) { $c.Tag[$c.SelectedIndex][1] }

$iso = New-Object System.Windows.Forms.TextBox
$iso.Text = Get-Setting 'iso' ''; $iso.Width = 210
Add-Row 'Disc image (ISO)' $iso
$browse = New-Object System.Windows.Forms.Button
$browse.Text = 'Browse...'; $browse.Location = New-Object System.Drawing.Point 366, ($script:row - 33); $browse.Width = 74
$browse.Add_Click({
    $d = New-Object System.Windows.Forms.OpenFileDialog
    $d.Filter = 'Disc images (*.iso)|*.iso|All files (*.*)|*.*'
    if ($d.ShowDialog() -eq 'OK') { $iso.Text = $d.FileName }
})
$form.Controls.Add($browse)

$scale = New-Choice @(@('1x (the PS2''s own, 512 x 448)', '1'), @('2x (recommended)', '2'), @('3x', '3'), @('4x (sharpest, slower)', '4'), @('Match the window', 'window')) (Get-Setting 'scale' '2')
Add-Row 'Resolution (native)' $scale
$window = New-Choice @(@('Window', 'windowed'), @('Borderless (whole screen)', 'borderless'), @('Fullscreen (exclusive)', 'fullscreen')) (Get-Setting 'window' 'windowed')
Add-Row 'Display' $window
$size = New-Choice @(@('896 x 672', '896x672'), @('1280 x 960', '1280x960'), @('1440 x 1080', '1440x1080'), @('1280 x 720', '1280x720'), @('1920 x 1080', '1920x1080')) ('{0}x{1}' -f (Get-Setting 'width' '896'), (Get-Setting 'height' '672'))
Add-Row 'Window size' $size
$aspect = New-Choice @(@('4:3 (as on a television)', '4:3'), @('16:9', '16:9'), @('Fill the window (stretched)', 'stretch'), @('Square pixels', 'pixels')) (Get-Setting 'DHD_ASPECT' '4:3')
Add-Row 'Picture shape' $aspect
$filter = New-Choice @(@('Smooth', 'linear'), @('Sharp pixels', 'point')) (Get-Setting 'DHD_PRESENT_FILTER' 'linear')
Add-Row 'Picture scaling' $filter
$wide = New-Object System.Windows.Forms.CheckBox
$wide.Text = 'Widescreen 16:9 (wider view, HUD stretched)'; $wide.Checked = (Get-Setting 'widescreen' '0') -ne '0'; $wide.AutoSize = $true
$wide.Add_CheckedChanged({ $aspect.SelectedIndex = if ($wide.Checked) { 1 } else { 0 } })
Add-Row '' $wide

$play = New-Object System.Windows.Forms.Button
$play.Text = 'Play'; $play.Location = New-Object System.Drawing.Point 254, 290; $play.Width = 90
$save = New-Object System.Windows.Forms.Button
$save.Text = 'Save'; $save.Location = New-Object System.Drawing.Point 350, 290; $save.Width = 90
$form.Controls.Add($play); $form.Controls.Add($save)
$form.AcceptButton = $play

function Save-Settings {
    $wh = (Get-Choice $size) -split 'x'
    $settings['iso'] = $iso.Text.Trim()
    $settings['renderer'] = 'native'  # the exact renderer is for developers: DHD_RENDER=exact
    $settings['scale'] = Get-Choice $scale
    # Preserve the offline real-step rate selected in PC SETTINGS (finding 067).
    $settings['window'] = Get-Choice $window
    $settings['width'] = $wh[0]; $settings['height'] = $wh[1]
    $settings['DHD_ASPECT'] = Get-Choice $aspect
    $settings['DHD_PRESENT_FILTER'] = Get-Choice $filter
    $settings['widescreen'] = if ($wide.Checked) { '1' } else { '0' }
    $lines = @('; Downhill Domination PC settings (written by Launcher.ps1; DHD_* environment variables override)')
    foreach ($k in $settings.Keys) { if ("$($settings[$k])" -ne '') { $lines += "$k = $($settings[$k])" } }
    Set-Content -Path $ini -Value $lines -Encoding ASCII
}
$save.Add_Click({ Save-Settings })
$play.Add_Click({
    if (-not (Test-Path $iso.Text.Trim())) {
        [void][System.Windows.Forms.MessageBox]::Show('Choose your Downhill Domination (USA) disc image first.', 'Downhill Domination')
        return
    }
    Save-Settings
    Get-ChildItem Env:DHD_* | Remove-Item
    Start-Process -FilePath $Exe -WorkingDirectory (Split-Path $Exe)
    $form.Close()
})
[void]$form.ShowDialog()
