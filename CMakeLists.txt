set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
cmake_minimum_required(VERSION 3.16)
project(MyWxApp)

set(CMAKE_CXX_STANDARD 17)

# Отключить предупреждение о манифесте (опционально)
add_definitions(-DwxUSE_WARNING_FOR_COMMON_CONTROLS=0)

# Пути к wxWidgets
set(WX_ROOT "C:/wxWidgets")
set(WX_LIB_DIR "C:/wxWidgets/build-mingw/lib/gcc_x64_lib")

# Заголовки
include_directories("${WX_ROOT}/include")
include_directories("${WX_LIB_DIR}")  # для wx/setup.h

# Библиотеки
link_directories("${WX_LIB_DIR}")

# Исполняемый файл
add_executable(MyWxApp src/main.cpp)

# Все необходимые системные библиотеки
target_link_libraries(MyWxApp
    # Статические библиотеки wxWidgets
    wxmsw33u_core
    wxmsw33u_adv
    wxbase33u
    wxpng
    wxzlib
    wxjpeg
    wxregexu
    wxexpat

    # Системные Windows-библиотеки (ВСЕ, что нужно)
    gdi32
    comdlg32
    winspool
    winmm
    shell32
    comctl32
    ole32
    oleaut32
    uuid
    rpcrt4
    shlwapi
    advapi32
    wsock32
    gdiplus          # ← GDI+
    msimg32          # ← AlphaBlend
    version          # ← GetFileVersionInfoW
    imm32
    wininet
    uxtheme          # ← темы оформления
    dwmapi           # ← DWM (Aero)
    oleacc           # ← доступность
)