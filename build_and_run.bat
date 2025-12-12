@echo off
setlocal

:: === НАСТРОЙКИ (измените, если нужно) ===
set PROJECT_DIR=C:\MyApp
set MSYS2_ROOT=C:\tools

:: === Проверка наличия MSYS2 ===
if not exist "%MSYS2_ROOT%\msys2_shell.cmd" (
    echo [!] Ошибка: MSYS2 не найден по пути "%MSYS2_ROOT%"
    echo     Установите MSYS2 или исправьте путь в этом .bat-файле.
    pause
    exit /b 1
)

:: === Создаём build, если нет ===
if not exist "%PROJECT_DIR%\build" mkdir "%PROJECT_DIR%\build"

:: === Запускаем сборку в MSYS2 MinGW 64-bit ===
echo [+] Запускаем сборку в MSYS2 MinGW 64-bit...
"%MSYS2_ROOT%\msys2_shell.cmd" -defterm -no-start -c ^
"cd /c/MyApp && cd build && cmake .. -G 'MinGW Makefiles' -DCMAKE_BUILD_TYPE=Release && mingw32-make && echo '[OK] Сборка успешна. Запуск...' && ./MyWxApp.exe"

:: === Завершение ===
if %ERRORLEVEL% EQU 0 (
    echo [✓] Готово.
) else (
    echo [✗] Ошибка при сборке или запуске.
)

pause