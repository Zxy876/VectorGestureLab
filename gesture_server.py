#!/usr/bin/env python3
import cv2
import mediapipe as mp
import time

GESTURE_FILE = "gesture.txt"

# 初始化 MediaPipe
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(max_num_hands=1, min_detection_confidence=0.7)
mp_draw = mp.solutions.drawing_utils

import os

def write_gesture(g):
    with open(GESTURE_FILE, "w") as f:
        f.write(g)
        f.flush()
        os.fsync(f.fileno())

def get_finger_count(hand_landmarks):
    # 根据关键点判断伸出的手指数量
    tips = [4, 8, 12, 16, 20]
    fingers = []

    # 拇指
    if hand_landmarks.landmark[tips[0]].x < hand_landmarks.landmark[tips[0]-1].x:
        fingers.append(1)
    else:
        fingers.append(0)

    # 其他4指
    for tip in tips[1:]:
        if hand_landmarks.landmark[tip].y < hand_landmarks.landmark[tip - 2].y:
            fingers.append(1)
        else:
            fingers.append(0)

    return sum(fingers)

def main():
    write_gesture("none")
    cap = cv2.VideoCapture(0)
    prev_time = 0
    gesture_last = "none"

    print("✅ 手势识别启动：")
    print("✊ <=1指: push   ✌️ 2指: auto   🤚 3~4指: pop   🖐 >=5指: clear")
    print("ESC 退出程序\n")

    while True:
        success, img = cap.read()
        if not success:
            break

        img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        results = hands.process(img_rgb)

        gesture = "none"

        if results.multi_hand_landmarks:
            for handLms in results.multi_hand_landmarks:
                mp_draw.draw_landmarks(img, handLms, mp_hands.HAND_CONNECTIONS)
                count = get_finger_count(handLms)
                if count <= 1:
                    gesture = "push"
                elif count == 2:
                    gesture = "auto"
                elif 3 <= count <= 4:
                    gesture = "pop"
                elif count >= 5:
                    gesture = "clear"

        # 降低写入频率
        if gesture != gesture_last:
            write_gesture(gesture)
            gesture_last = gesture

        cv2.putText(img, f"Gesture: {gesture}", (10, 50),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        cv2.imshow("Gesture Recognition", img)

        if cv2.waitKey(1) & 0xFF == 27:
            break

    cap.release()
    cv2.destroyAllWindows()
    write_gesture("none")
    print("👋 手势识别结束")

if __name__ == "__main__":
    main()