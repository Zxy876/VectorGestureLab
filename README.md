# Vector Gesture Lab

一个通过手势控制交互的 C++ Vector 数据结构可视化学习工具。结合计算机视觉技术，让你用简单的手势动作直观地理解 Vector 的 push、pop、clear 等操作。

## 截图

<div align="center">
  <img src="demo.gif" alt="演示动画" width="800"/>
</div>

## 功能特性

### 👆 手势控制
通过摄像头捕捉手势，直观控制 Vector 操作：
- **1 指** → `push_back()` - 添加元素（下滑入动画）
- **2 指** → `auto` - 开启/关闭自动模式
- **3-4 指** → `pop_back()` - 删除元素（淡出下坠动画）
- **5 指** → `clear()` - 清空所有元素

### 🎨 可视化动画
- **入栈动画**：新元素从上方平滑滑入
- **出栈动画**：元素淡出并轻微下坠
- **实时统计**：显示当前 size 和 capacity
- **优雅 UI**：深色主题，现代化界面

### 📚 教学价值
- 直观展示 Vector 的动态扩容机制
- 完整的 MyVector 类实现（包含拷贝/移动语义）
- 迭代器实现示例
- 适合 C++ 数据结构学习

## 技术栈

### C++ 端
- **C++17** - 现代化 C++ 特性
- **SFML 2.6** - 图形渲染和窗口管理
- **CMake** - 跨平台构建系统

### Python 端
- **OpenCV** - 摄像头图像处理
- **MediaPipe** - 手势识别

## 系统要求

- macOS / Linux
- C++17 编译器 (Clang / GCC)
- Python 3.8+
- 摄像头设备

## 安装运行

### 快速启动（推荐）

项目提供了一键启动脚本：

```bash
chmod +x run_vector_lab.sh
./run_vector_lab.sh
```

脚本会自动：
1. 创建 Python 虚拟环境
2. 安装所需依赖
3. 在新终端启动手势识别
4. 编译并运行 C++ 可视化程序

### 手动安装

#### 1. 安装 Python 依赖

```bash
python3 -m venv venv311
source venv311/bin/activate
pip install opencv-python mediapipe
```

#### 2. 安装 C++ 依赖

**macOS:**
```bash
brew install sfml opencv
```

**Ubuntu:**
```bash
sudo apt-get install libsfml-dev libopencv-dev
```

#### 3. 编译项目

```bash
mkdir build && cd build
cmake ..
make -j4
```

#### 4. 运行

**终端 1 - 启动手势识别:**
```bash
python3 gesture_server.py
```

**终端 2 - 启动可视化:**
```bash
./build/VectorGestureLab
```

## 手势说明

| 手势 | 手指数 | 操作 | 说明 |
|------|--------|------|------|
| ✊ | 0-1 | push | 添加元素到尾部 |
| ✌️ | 2 | auto | 切换自动演示模式 |
| 🤟 | 3-4 | pop | 删除尾部元素 |
| 🖐 | 5 | clear | 清空所有元素 |

## 项目结构

```
VectorGestureLab/
├── include/
│   └── MyVector.h          # 自定义 Vector 实现
├── src/
│   └── main.cpp            # SFML 可视化程序
├── gesture_server.py        # Python 手势识别服务
├── run_vector_lab.sh       # 一键启动脚本
├── CMakeLists.txt          # CMake 构建配置
└── sounds/                 # 音效文件（可选）
```

## MyVector 类特点

本项目实现了一个完整的 MyVector 类，包含：

- ✅ 基本操作：`push_back()`, `pop_back()`, `clear()`, `reserve()`
- ✅ 拷贝语义：拷贝构造函数、拷贝赋值运算符
- ✅ 移动语义：移动构造函数、移动赋值运算符
- ✅ 迭代器：完整的随机访问迭代器实现
- ✅ 自动扩容：容量翻倍策略

## 开发计划

- [ ] 添加更多 Vector 操作（insert、erase等）
- [ ] 支持多种数据类型可视化
- [ ] 添加音效反馈
- [ ] 录制回放功能
- [ ] Windows 支持

## License

MIT

## Author

[Zxy876](https://github.com/Zxy876)
