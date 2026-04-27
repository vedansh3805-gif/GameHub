#include <SFML/Graphics.hpp> //for graphics. (window, shapes, text, events) AND Simple and fast multimedia library
#include <cstdlib> //command execution, random number generation, memory management, program control

int main() {
//Display screen 
    sf::RenderWindow window(sf::VideoMode({600, 400}), "Game Hub");  // SFML constructor 

    sf::Font font;
    font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");
//tictactoe
    sf::RectangleShape tttBtn({300.f, 70.f});
    tttBtn.setPosition({150.f, 120.f});//SFML function (float only) 
    tttBtn.setFillColor(sf::Color(80,80,100));

    sf::Text tttText(font, "Tic Tac Toe", 28);//font size 
    tttText.setPosition({200.f, 135.f});
//Memory card 
    sf::RectangleShape memBtn({300.f, 70.f});
    memBtn.setPosition({150.f, 220.f});
    memBtn.setFillColor(sf::Color(80,80,100));

    sf::Text memText(font, "Memory Game", 28);
    memText.setPosition({190.f, 235.f});
//Game Loop
    while (window.isOpen()) {
        while (auto ev = window.pollEvent()) { 

            if (ev->is<sf::Event::Closed>()) // How to CLOSE
                window.close();

            if (auto m = ev->getIf<sf::Event::MouseButtonPressed>()) { // mouse clicks 
                float x = m->position.x;
                float y = m->position.y;

                // RUN GAMES IN BACKGROUND
                if (tttBtn.getGlobalBounds().contains({x,y})) { // detects click
                    system("./tictactoe &"); // Run tictactoe code 
                }

                if (memBtn.getGlobalBounds().contains({x,y})) { 
                    system("./memory &"); // Run memory card code 
                }
            }
        }

        window.clear(sf::Color(20,20,30)); // Background 
        window.draw(tttBtn);
        window.draw(memBtn);
        window.draw(tttText);
        window.draw(memText);
        window.display(); // Display 
    }
}