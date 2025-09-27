set(UI_FILES MainWindow.ui)
qt6_wrap_ui(UI_HDRS ${UI_FILES})

#  静态库：包含生成的 ui 源
qt_add_library(MainWindow STATIC
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
)

#  库自身先链 Qt
target_link_libraries(MainWindow PUBLIC
    Qt6::Core
    Qt6::Widgets
    Qt6::SerialPort
)


target_include_directories(MainWindow PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src/MainWindow>
)



