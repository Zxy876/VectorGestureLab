#!/bin/bash
# ===========================================
# 🎬 Vector Gesture Lab App 启动脚本
# 启动 Python 手势识别 + C++ 可视化
# ===========================================

set -e
export PATH="/usr/local/bin:/opt/homebrew/bin:/usr/bin:/bin:/usr/sbin:/sbin"
export DISPLAY=:0

# ✅ 项目根目录（请勿改）
BASE_DIR="/Users/zxydediannao/VectorGestureLab"
BUILD_DIR="$BASE_DIR/build"
VENV_DIR="$BASE_DIR/venv311"

echo "🚀 启动 Vector Gesture Lab..."
cd "$BASE_DIR"

# Step 1️⃣ 激活 Python 虚拟环境
if [ ! -d "$VENV_DIR" ]; then
    echo "⚙️ 创建虚拟环境..."
    python3 -m venv "$VENV_DIR"
    source "$VENV_DIR/bin/activate"
    pip install --upgrade pip
    pip install opencv-python mediapipe
else
    source "$VENV_DIR/bin/activate"
fi

# Step 2️⃣ 启动 Python 手势识别
echo "🎥 启动手势识别..."
python3 "$BASE_DIR/gesture_server.py" &
PY_PID=$!
sleep 2  # 等摄像头初始化

# Step 3️⃣ 启动 C++ 可视化程序（绝对路径）
echo "🎨 启动 C++ 可视化..."
cd "$BASE_DIR" && "$BUILD_DIR/VectorGestureLab"
# Step 4️⃣ 清理
echo "🧹 关闭 Python 手势识别..."
kill $PY_PID 2>/dev/null || true
deactivate
echo "✅ Vector Gesture Lab 结束。"
