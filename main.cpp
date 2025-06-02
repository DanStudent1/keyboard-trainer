#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cctype>
#include <map>
#include <ctime>
#include <cstdlib>

struct KeyButton {
    sf::RectangleShape shape;
    sf::Text label;
    char character;
};

std::string generateRandomText(int length) {
    const std::string charset = "abcdefghijklmnopqrstuvwxyz ";
    std::string result;
    for (int i = 0; i < length; ++i) {
        result += charset[rand() % charset.size()];
    }
    return result;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Keyboard Trainer");
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/consola.ttf")) {
        return 1;
    }

    srand(static_cast<unsigned>(time(nullptr)));

    std::string targetText = generateRandomText(40);
    std::string typedText = "";
    int errorCount = 0;

    sf::Text targetDisplay(targetText, font, 30);
    targetDisplay.setPosition(50, 20);
    targetDisplay.setFillColor(sf::Color::White);

    sf::Text typedDisplay("", font, 30);
    typedDisplay.setPosition(50, 60);
    typedDisplay.setFillColor(sf::Color::Green);

    sf::Text errorDisplay("Errors: 0", font, 30);
    errorDisplay.setPosition(50, 100);
    errorDisplay.setFillColor(sf::Color::Red);

    std::vector<std::string> rows = {
        "QWERTYUIOP",
        "ASDFGHJKL",
        "ZXCVBNM"
    };

    std::vector<KeyButton> keyButtons;
    std::map<char, std::size_t> keyMap;
    float startX = 50, startY = 200, keySize = 50, spacing = 10;

    for (std::size_t row = 0; row < rows.size(); ++row) {
        float y = startY + row * (keySize + spacing);
        float xOffset = row == 1 ? 25 : (row == 2 ? 60 : 0);
        for (std::size_t col = 0; col < rows[row].size(); ++col) {
            char c = rows[row][col];
            float x = startX + col * (keySize + spacing) + xOffset;

            KeyButton key;
            key.character = c;
            key.shape.setSize({keySize, keySize});
            key.shape.setPosition(x, y);
            key.shape.setFillColor(sf::Color::White);
            key.shape.setOutlineColor(sf::Color::Black);
            key.shape.setOutlineThickness(2);

            key.label.setFont(font);
            key.label.setString(std::string(1, c));
            key.label.setCharacterSize(20);
            key.label.setFillColor(sf::Color::Black);
            key.label.setPosition(x + 15, y + 10);

            keyMap[c] = keyButtons.size();
            keyButtons.push_back(key);
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char c = static_cast<char>(std::toupper(static_cast<char>(event.text.unicode)));
                    if (std::isalpha(c) || c == ' ') {
                        if (typedText.length() < targetText.length()) {
                            if (c == std::toupper(targetText[typedText.length()])) {
                                typedText += targetText[typedText.length()];
                            } else {
                                ++errorCount;
                            }
                        }
                    }
                }
            }
        }

        // Reset key colors
        for (auto& key : keyButtons) {
            key.shape.setFillColor(sf::Color::White);
        }

        // Highlight pressed keys
        for (char c = 'A'; c <= 'Z'; ++c) {
            if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(c - 'A'))) {
                auto it = keyMap.find(c);
                if (it != keyMap.end()) {
                    keyButtons[it->second].shape.setFillColor(sf::Color::Green);
                }
            }
        }

        typedDisplay.setString(typedText);
        errorDisplay.setString("Errors: " + std::to_string(errorCount));

        window.clear(sf::Color(100, 100, 100));
        window.draw(targetDisplay);
        window.draw(typedDisplay);
        window.draw(errorDisplay);
        for (const auto& key : keyButtons) {
            window.draw(key.shape);
            window.draw(key.label);
        }
        window.display();
    }

    return 0;
}
