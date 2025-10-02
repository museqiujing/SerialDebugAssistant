串口调试助手 (SerialDebugAssistant)
一个基于 Qt6 + CMake 的跨平台串口调试工具，支持热插拔、文本/HEX 双模式、GBK/UTF-8 编码.

| 技术       | 实现要点                                    |
| -------- | --------------------------------------- |
| **线程模型** | `QThread + moveToThread` 零锁队列           |
| **比较算法** | `std::equal + 排序` 不漏检同数量换口              |
| **编码支持** | `QString::fromLocal8Bit / fromUtf8` 双编码 |

