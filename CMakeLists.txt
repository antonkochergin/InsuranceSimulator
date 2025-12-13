cmake_minimum_required(VERSION 3.16)
project(MyWxApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Статическая линковка — обязательно!
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

# Пути к wxWidgets (убедитесь, что путь верный!)
set(wxWidgets_ROOT_DIR "C:/wxWidgets")
set(WX_LIB_DIR "${wxWidgets_ROOT_DIR}/build-mingw/lib/gcc_x64_lib")

# Заголовки
include_directories("${wxWidgets_ROOT_DIR}/include")
include_directories("${WX_LIB_DIR}")  # для wx/setup.h

# Библиотеки
link_directories("${WX_LIB_DIR}")

# Исходники
file(GLOB SOURCES "src/*.cpp")
add_executable(MyWxApp ${SOURCES})

# GUI-приложение (без консоли)
set_target_properties(MyWxApp PROPERTIES WIN32_EXECUTABLE TRUE)

# Линковка — ОБЯЗАТЕЛЬНО в этом порядке!
target_link_libraries(MyWxApp
    # wxWidgets (статические)
    wxmsw33u_core
    wxmsw33u_adv
    wxbase33u
    wxpng
    wxzlib
    wxjpeg
    wxregexu
    wxexpat

    # Системные зависимости Windows (должны быть В КОНЦЕ!)
    gdiplus      # GDI+ → Gdip*
    uxtheme      # темы → __imp_*Theme*
    comctl32     # Common Controls
    version      # GetFileVersionInfoW
    oleacc       # CreateStdAccessibleObject
    msimg32      # AlphaBlend
    imm32
    winmm
    shlwapi
    gdi32
    comdlg32
    ole32
    oleaut32
    uuid
    kernel32
    user32
)