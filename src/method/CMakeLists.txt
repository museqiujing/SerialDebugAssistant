# 1. 静态库：纯代码
qt_add_library(method STATIC
    SerialWatcher.cpp
    SerialWatcher.h
)

# 2. 链 Qt
target_link_libraries(method PUBLIC 
    Qt6::Core
    Qt6::Widgets
    Qt6::SerialPort
)

# 3. 暴露头文件
target_include_directories(method PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)