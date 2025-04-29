@echo off

rmdir /s /q build
rmdir /s /q ets2la_AR

REM check if pybind11 is pip installed, if not install
python -c "import pybind11" || pip install pybind11

REM get pybind11 path and set env var so cmake can find it
for /f "tokens=*" %%i in ('python -c "import pybind11; import os; print(os.path.dirname(pybind11.get_include()))"') do set "pybind11_DIR=%%i"
set pybind11_DIR "%pybind11_DIR%"

REM build project
cmake --preset=x64-release -B build/x64-release
cmake --build build/x64-release --config Release

mkdir ets2la_AR
copy build\x64-release\Release\*.pyd ets2la_AR