# 1. 静态库：纯代码
qt_add_library(Method STATIC
    SerialWatcher.cpp
    SerialWatcher.h
)

# 2. 链 Qt
target_link_libraries(Method PUBLIC 
    MainWindow
    Qt6::Core
    Qt6::Widgets
    Qt6::SerialPort
)

# 3. 暴露头文件
target_include_directories(Method PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)