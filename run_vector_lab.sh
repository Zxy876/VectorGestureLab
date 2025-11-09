#!/bin/bash
# ===========================================
# 🎬 Vector Gesture Lab 启动脚本（终端修复版）
# 同时启动：
#  - Python 手势识别（独立终端窗口）
#  - C++ Vector 可视化（当前终端）
# ===========================================

set -e

# 自动定位项目路径（兼容 .app 与普通执行）
if [[ "$0" == *".app/Contents/MacOS/"* ]]; then
    BASE_DIR="$(cd "$(dirname "$0")/../../.." && pwd)"
else
    BASE_DIR="$(cd "$(dirname "$0")" && pwd)"
fi

BUILD_DIR="$BASE_DIR/build"
VENV_DIR="$BASE_DIR/venv311"
GESTURE_SCRIPT="$BASE_DIR/gesture_server.py"
CPP_BINARY="$BUILD_DIR/VectorGestureLab"

echo "🚀 启动 Vector Gesture Lab"
echo "📂 项目路径: $BASE_DIR"
echo "------------------------------------------"

# Step 1️⃣ 检查虚拟环境
if [ ! -d "$VENV_DIR" ]; then
    echo "⚙️ 创建虚拟环境 venv311..."
    python3 -m venv "$VENV_DIR"
    source "$VENV_DIR/bin/activate"
    echo "📦 安装依赖 (opencv-python + mediapipe)..."
    pip install --upgrade pip
    pip install opencv-python mediapipe
else
    source "$VENV_DIR/bin/activate"
fi

# Step 2️⃣ 在新终端启动 Python 手势识别
if [ -f "$GESTURE_SCRIPT" ]; then
    echo "🎥 启动手势识别 (gesture_server.py)..."
    osascript -e "tell application \"Terminal\" \
        to do script \"cd $BASE_DIR; \
        source $VENV_DIR/bin/activate; \
        python3 $GESTURE_SCRIPT; \
        read -n 1 -s -r -p 'Press any key to close...'\""
else
    echo "❌ 未找到手势脚本: $GESTURE_SCRIPT"
fi

# Step 3️⃣ 构建 C++ 可视化
echo "🧱 构建 VectorGestureLab..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

if ! cmake .. > /dev/null; then
    echo "❌ CMake 配置失败"
    exit 1
fi

if ! make -j4; then
    echo "❌ 编译失败"
    exit 1
fi

# Step 4️⃣ 启动可视化界面
if [ -f "$CPP_BINARY" ]; then
    echo "🎨 启动 VectorGestureLab 界面..."
    "$CPP_BINARY"
else
    echo "❌ 未找到可执行文件: $CPP_BINARY"
fi

# Step 5️⃣ 退出清理
deactivate 2>/dev/null || true
echo "✅ Vector Gesture Lab 已结束。"