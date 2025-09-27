cmake_minimum_required(VERSION 3.16)
project(SerialDebugAssistant
        VERSION 1.0.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 找到 Qt6（只找一次，子目录复用）
find_package(Qt6 REQUIRED COMPONENTS 
    Core
    Gui
    Widgets 
    SerialPort
)

# 让 Qt 自动处理 moc/uic/rcc
qt_standard_project_setup()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/lib)# 静态库
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/bin)# 可执行文件
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/bin)# 动态库

# 递归编译子目录
add_subdirectory(src)           # 源代码
#add_subdirectory(resources)     # 资源文件
#add_subdirectory(tests)         # 测试







# 安装规则（CMake 自带变量）
#include(GNUInstallDirs)
#install(TARGETS SerialDebugAssistant
        #RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})