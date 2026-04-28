#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>

int main() {

    // ---------------- WINDOW ----------------
    sf::RenderWindow window(sf::VideoMode({600, 400}), "Game Hub");

    // ---------------- FONT ----------------
    sf::Font font;
    if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
        std::cout << "Font failed to load\n";
    }

    // ---------------- TIC TAC TOE BUTTON ----------------
    sf::RectangleShape tttBtn({300.f, 70.f});
    tttBtn.setPosition({150.f, 120.f});
    tttBtn.setFillColor(sf::Color(80, 80, 100));

    sf::Text tttText(font, "Tic Tac Toe", 28);
    tttText.setPosition({190.f, 135.f});
    tttText.setFillColor(sf::Color::White);

    // ---------------- MEMORY GAME BUTTON ----------------
    sf::RectangleShape memBtn({300.f, 70.f});
    memBtn.setPosition({150.f, 220.f});
    memBtn.setFillColor(sf::Color(80, 80, 100));

    sf::Text memText(font, "Memory Game", 28);
    memText.setPosition({180.f, 235.f});
    memText.setFillColor(sf::Color::White);

    // ---------------- LOOP ----------------
    while (window.isOpen()) {

        while (auto ev = window.pollEvent()) {

            // CLOSE WINDOW
            if (ev->is<sf::Event::Closed>())
                window.close();

            // MOUSE CLICK
            if (auto m = ev->getIf<sf::Event::MouseButtonPressed>()) {

                float x = static_cast<float>(m->position.x);
                float y = static_cast<float>(m->position.y);

                // ---------- RUN GAMES ----------
                if (tttBtn.getGlobalBounds().contains(sf::Vector2f(x, y))) {
                    system("../TIC_TAC_TOE/tictactoe &");
                }

                if (memBtn.getGlobalBounds().contains(sf::Vector2f(x, y))) {
                    system("../Memory_Card/memory &");
                }
            }
        }

        // ---------------- DRAW ----------------
        window.clear(sf::Color(20, 20, 30));

        window.draw(tttBtn);
        window.draw(memBtn);
        window.draw(tttText);
        window.draw(memText);

        window.display();
    }

    return 0;
}