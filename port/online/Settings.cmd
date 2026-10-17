@echo off
rem Downhill Domination: the PC settings in dhd.ini, and a single-player start (Launcher.ps1).
start "" powershell -NoProfile -ExecutionPolicy Bypass -WindowStyle Hidden -File "%~dp0Launcher.ps1" -Exe "%~dp0dhd.exe"
