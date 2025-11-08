#!/usr/bin/env python3
import cv2
import mediapipe as mp
import os

GESTURE_FILE = "gesture.txt"

# 钢琴键数量（C++那边也要用同样的数）
NUM_KEYS = 8   # C D E F G A B C

mp_hands = mp.solutions.hands
hands = mp_hands.Hands(
    max_num_hands=1,
    min_detection_confidence=0.7,
    min_tracking_confidence=0.5
)
mp_draw = mp.solutions.drawing_utils

def write_key(key_str: str):
    # 覆盖写，C++每帧去读
    with open(GESTURE_FILE, "w") as f:
        f.write(key_str)

def main():
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("❌ cannot open camera")
        return

    # 启动时先写 none，避免C++一开始读到旧的
    write_key("none")

    print("✅ Air Piano gesture on!")
    print("👉 单手食指对准画面左右移动就能选键")
    print("👉 ESC 退出\n")

    last_key = "none"

    while True:
        ok, frame = cap.read()
        if not ok:
            break

        frame = cv2.flip(frame, 1)  # 镜像
        h, w, _ = frame.shape

        rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        result = hands.process(rgb)

        key_cmd = "none"

        if result.multi_hand_landmarks:
            # 只取第一只手
            hand = result.multi_hand_landmarks[0]
            mp_draw.draw_landmarks(frame, hand, mp_hands.HAND_CONNECTIONS)

            # 食指指尖id=8
            ix = hand.landmark[8].x  # 0~1
            # 根据x落在哪个区间，映射到键号
            key_id = int(ix * NUM_KEYS)
            if key_id < 0:
                key_id = 0
            if key_id >= NUM_KEYS:
                key_id = NUM_KEYS - 1

            key_cmd = f"key:{key_id}"

            # 画一下当前指尖
            cx, cy = int(hand.landmark[8].x * w), int(hand.landmark[8].y * h)
            cv2.circle(frame, (cx, cy), 8, (0, 255, 0), -1)

            # 显示当前键
            cv2.putText(frame, key_cmd, (10, 40),
                        cv2.FONT_HERSHEY_SIMPLEX, 1.1,
                        (0, 255, 0), 2)

        # 只有变化才写，避免文件疯狂写
        if key_cmd != last_key:
            write_key(key_cmd)
            last_key = key_cmd

        cv2.imshow("Air Piano - gesture", frame)
        if cv2.waitKey(1) & 0xFF == 27:
            break

    write_key("none")
    cap.release()
    cv2.destroyAllWindows()
    print("👋 bye")

if __name__ == "__main__":
    main()