#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include "MyVector.h"

// ==================== 动画结构 ====================
struct BoxAnim {
    float yOffset = 0.f;
    float alpha = 255.f;
    bool isNew = false;
    bool isRemoved = false;
    sf::Clock timer;
};
std::vector<BoxAnim> boxAnims;

// ==================== 绘制 Vector 可视化 ====================
template <typename T>
void drawVector(sf::RenderWindow& window, const MyVector<T>& vec,
                int startX, int startY, bool flash = false) {
    float boxW = 40.f, boxH = 40.f, spacing = 10.f;
    if (boxAnims.size() < vec.capacity()) boxAnims.resize(vec.capacity());

    for (size_t i = 0; i < vec.capacity(); ++i) {
        sf::RectangleShape box({boxW, boxH});
        box.setPosition({static_cast<float>(startX + i * (boxW + spacing)),
                         static_cast<float>(startY + boxAnims[i].yOffset)});
        sf::Color color = (i < vec.size()) ? sf::Color(100, 200, 255)
                                           : sf::Color(40, 40, 40);
        if (flash) color = sf::Color(180, 230, 255);

        if (boxAnims[i].isNew) {
            float t = boxAnims[i].timer.getElapsedTime().asSeconds();
            boxAnims[i].yOffset = std::max(0.f, 30.f - t * 60.f);
            if (t > 0.5f) boxAnims[i].isNew = false;
        }
        if (boxAnims[i].isRemoved) {
            float t = boxAnims[i].timer.getElapsedTime().asSeconds();
            boxAnims[i].alpha = std::max(0.f, 255.f - t * 300.f);
            if (t > 0.4f) boxAnims[i].isRemoved = false;
        }

        color.a = static_cast<std::uint8_t>(boxAnims[i].alpha);
        box.setFillColor(color);
        window.draw(box);
    }

    static sf::Font font;
    static bool loaded = false;
    if (!loaded) {
        [[maybe_unused]] bool ok = font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");
        loaded = true;
    }

    // ✅ SFML 3 的构造方式：Text(font, string, size)
    sf::Text text(font,
                  "size=" + std::to_string(vec.size()) +
                      "  cap=" + std::to_string(vec.capacity()),
                  20);
    text.setPosition({static_cast<float>(startX), static_cast<float>(startY + 60)});
    window.draw(text);
}

// ==================== 读取 Python 端手势文件 ====================
std::string readGestureFile() {
    std::ifstream file("../gesture.txt"); // 读取上一级目录的文件
    std::string gesture;
    if (file.is_open()) std::getline(file, gesture);
    return gesture;
}

// ==================== 主函数 ====================
int main() {
    std::cout << "✅ Vector Gesture Lab - Python Link Mode (SFML 3)\n";
    std::cout << "请确保 gesture_server.py 正在运行...\n";

    // ✅ SFML 3 的构造方式：RenderWindow(VideoMode{width, height}, title, state)
    sf::RenderWindow window(sf::VideoMode({800u, 600u}),
                            "Vector Gesture Lab",
                            sf::State::Windowed);

    MyVector<int> vec;
    int counter = 1;
    bool autoMode = false;
    sf::Clock autoClock;

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        std::string gesture = readGestureFile();

        if (gesture == "push") {
            vec.push_back(counter++);
            boxAnims.resize(vec.capacity());
            boxAnims[vec.size() - 1].isNew = true;
            boxAnims[vec.size() - 1].timer.restart();
            std::cout << "✊ push_back()\n";
        } else if (gesture == "pop") {
            if (vec.size() > 0) {
                boxAnims[vec.size() - 1].isRemoved = true;
                boxAnims[vec.size() - 1].timer.restart();
                vec.pop_back();
                std::cout << "🤚 pop_back()\n";
            }
        } else if (gesture == "auto") {
            autoMode = !autoMode;
            std::cout << (autoMode ? "🌀 auto ON\n" : "🛑 auto OFF\n");
        } else if (gesture == "clear") {
            vec.clear();
            boxAnims.clear();
            counter = 1;
            std::cout << "🧹 clear()\n";
        }

        if (autoMode && autoClock.getElapsedTime().asSeconds() > 1.0f) {
            vec.push_back(counter++);
            boxAnims.resize(vec.capacity());
            boxAnims[vec.size() - 1].isNew = true;
            boxAnims[vec.size() - 1].timer.restart();
            autoClock.restart();
        }

        window.clear(sf::Color(20, 20, 30));
        drawVector(window, vec, 60, 250);
        window.display();
    }
    return 0;
}