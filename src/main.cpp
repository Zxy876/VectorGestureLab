#include <iostream>
#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>
#include "MyVector.h"

void testCopyMove() {
    std::cout << "\n==== Test Copy / Move ====\n";

    MyVector<int> a;
    for (int i = 1; i <= 3; ++i) a.push_back(i);
    std::cout << "Original a: "; a.print();

    MyVector<int> b = a; // 拷贝构造
    std::cout << "Copied b: "; b.print();

    MyVector<int> c;
    c = a; // 拷贝赋值
    std::cout << "Assigned c: "; c.print();

    MyVector<int> d = std::move(a); // 移动构造
    std::cout << "Moved d: "; d.print();

    MyVector<int> e;
    e = std::move(b); // 移动赋值
    std::cout << "Moved e: "; e.print();
}
void testIterator() {
    std::cout << "\n==== Test Iterator / Range-for ====\n";

    MyVector<int> v;
    for (int i = 1; i <= 5; ++i) v.push_back(i);

    std::cout << "Manual iteration: ";
    for (MyVector<int>::Iterator it = v.begin(); it != v.end(); ++it)
        std::cout << *it << " ";
    std::cout << "\n";

    std::cout << "Range-for iteration: ";
    for (auto& x : v)
        std::cout << x << " ";
    std::cout << "\n";

    // STL 算法兼容性测试
    std::cout << "std::sort descending...\n";
    std::sort(v.begin(), v.end(), std::greater<int>());
    for (auto& x : v)
        std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    std::cout << "✅ Vector Gesture Lab (SFML 3 + OpenCV, macOS safe loop)\n";

    // 1. 打开摄像头
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "❌ Cannot open camera!\n";
        return -1;
    }

    // 2. 创建 SFML 窗口（注意 SFML 3 的写法）
    sf::RenderWindow window(sf::VideoMode({400u, 300u}), "Vector Gesture Lab - SFML Window");

    cv::Mat frame;

    // 3. 同一条主循环里跑 SFML + OpenCV
    while (window.isOpen()) {
        // --- 3.1 SFML event ---
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // --- 3.2 读取摄像头并显示 ---
        cap >> frame;
        if (!frame.empty()) {
            cv::imshow("Camera", frame);
            // 这行必须要，有了它 OpenCV 的窗口才能处理事件
            // 1ms 足够，不会卡 SFML
            if (cv::waitKey(1) == 27) { // ESC 退出摄像头+程序
                window.close();
            }
        }

        // --- 3.3 画 SFML 窗口 ---
        window.clear(sf::Color::Black);
        window.display();
    }
  testCopyMove();
   testIterator();
    return 0;
}