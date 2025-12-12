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
    wxmsw33u_core
    wxmsw33u_adv
    wxbase33u
    wxpng
    wxzlib

    # GDI+
    gdiplus

    # Темы Windows (UxTheme)
    uxtheme
    comctl32

    # Доступность
    oleacc

    # Основные системные
    kernel32
    user32
    gdi32
    winspool
    comdlg32
    advapi32
    shell32
    ole32
    oleaut32
    uuid
    odbc32
    odbccp32
    winmm
    shlwapi
    version
    msimg32
    imm32
)