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
void drawVector(sf::RenderWindow& window, const MyVector<T>& vec, int startX, int startY, bool flash = false) {
    float boxW = 40.f, boxH = 40.f, spacing = 10.f;
    if (boxAnims.size() < vec.capacity()) boxAnims.resize(vec.capacity());

    for (size_t i = 0; i < vec.capacity(); ++i) {
        sf::RectangleShape box({boxW, boxH});
        box.setPosition({static_cast<float>(startX + i * (boxW + spacing)),
                         static_cast<float>(startY + boxAnims[i].yOffset)});
        sf::Color color = (i < vec.size()) ? sf::Color(100, 200, 255) : sf::Color(40, 40, 40);
        if (flash) color = sf::Color(180, 230, 255);

        // ✅ 改：更慢的入栈动画（下滑入）
        if (boxAnims[i].isNew) {
            float t = boxAnims[i].timer.getElapsedTime().asSeconds();
            boxAnims[i].yOffset = std::max(0.f, 60.f - t * 60.f); // 慢慢滑上来
            if (t > 1.0f) boxAnims[i].isNew = false; // 延长到1秒
        }

        // ✅ 改：更平滑的出栈动画（淡出下坠）
        if (boxAnims[i].isRemoved) {
            float t = boxAnims[i].timer.getElapsedTime().asSeconds();
            boxAnims[i].alpha = std::max(0.f, 255.f - t * 200.f);
            boxAnims[i].yOffset = t * 30.f; // 轻微下坠
            if (t > 0.8f) boxAnims[i].isRemoved = false;
        }

        color.a = static_cast<std::uint8_t>(boxAnims[i].alpha);
        box.setFillColor(color);
        window.draw(box);
    }

    // ==================== 文字 ====================
    static sf::Font font;
    static bool loaded = false;
    if (!loaded) {
        if (!font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
            std::cerr << "⚠️ 字体加载失败\n";
        }
        loaded = true;
    }

    sf::Text text("size=" + std::to_string(vec.size()) +
                  "  cap=" + std::to_string(vec.capacity()), font, 20);
    text.setPosition({static_cast<float>(startX), static_cast<float>(startY + 60)});
    window.draw(text);
}

// ==================== 手势文件读取 ====================
std::string readGestureFile() {
std::ifstream file("/Users/zxydediannao/VectorGestureLab/gesture.txt");
    std::string gesture;
    if (file.is_open()) std::getline(file, gesture);
    return gesture;
}

// ==================== 主程序 ====================
int main() {
    std::cout << "✅ Vector Gesture Lab - 慢动作动画模式\n";
    std::cout << "请确保 gesture_server.py 正在运行...\n";

    sf::RenderWindow window(sf::VideoMode(800, 600), "Vector Gesture Lab");

    MyVector<int> vec;
    int counter = 1;
    bool autoMode = false;
    sf::Clock autoClock;
    sf::Clock gestureCooldown;
    float cooldown = 0.8f; // 手势冷却 0.8 秒

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        std::string gesture = readGestureFile();

        // ✅ 冷却保护：避免过快切换动作
        if (gestureCooldown.getElapsedTime().asSeconds() < cooldown)
            gesture = "none";

        if (gesture == "push") {
            vec.push_back(counter++);
            boxAnims.resize(vec.capacity());
            boxAnims[vec.size() - 1].isNew = true;
            boxAnims[vec.size() - 1].timer.restart();
            std::cout << "✊ push_back()\n";
            gestureCooldown.restart();
        } 
        else if (gesture == "pop") {
            if (vec.size() > 0) {
                boxAnims[vec.size() - 1].isRemoved = true;
                boxAnims[vec.size() - 1].timer.restart();
                vec.pop_back();
                std::cout << "🤚 pop_back()\n";
                gestureCooldown.restart();
            }
        } 
        else if (gesture == "auto") {
            autoMode = !autoMode;
            std::cout << (autoMode ? "🌀 auto ON\n" : "🛑 auto OFF\n");
            gestureCooldown.restart();
        } 
        else if (gesture == "clear") {
            vec.clear();
            boxAnims.clear();
            counter = 1;
            std::cout << "🧹 clear()\n";
            gestureCooldown.restart();
        }

        // ✅ 自动模式节奏不变
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