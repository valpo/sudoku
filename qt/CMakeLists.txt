# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets)
qt5_wrap_ui(UI_HEADERS mainwindow.ui)

aux_source_directory(. SRC_LIST)
add_definitions(-std=c++11 -g -Wall -O0)
add_executable(qt_sudoku ${SRC_LIST} ${UI_HEADERS})

# Use the Widgets module from Qt 5.
target_link_libraries(qt_sudoku Qt5::Widgets)
