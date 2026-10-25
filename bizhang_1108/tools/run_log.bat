@echo off
cd /d "%~dp0"
title car log receiver  (Ctrl+C to stop)
python car_log.py
echo.
echo [stopped] press any key to close
pause >nul
