@echo off
py setup.py
call .target\premake\win64\premake5.exe vs2022
PAUSE