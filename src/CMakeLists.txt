# 主程序：加生成的 ui 头 + 源
qt_add_executable(main
    main.cpp     
)

#  链接子目录库
target_link_libraries(main PUBLIC
    MainWindow 
    method    
    Qt6::Core
    Qt6::Widgets
    Qt6::SerialPort
)

# Windows 下不显示控制台
set_target_properties(main
    PROPERTIES
        WIN32_EXECUTABLE  FALSE #TRUE
) 

# 递归编译子目录
add_subdirectory(method)         # 方法
add_subdirectory(MainWindow)     # 图形界面


