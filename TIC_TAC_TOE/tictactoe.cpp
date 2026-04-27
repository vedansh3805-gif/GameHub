#include <SFML/Graphics.hpp> // to use graphics
#include <SFML/Audio.hpp>// for sound
#include <iostream>

// ---------------- WINDOW ----------------
constexpr int WIDTH = 600;
constexpr int HEIGHT = 850;//not 600 so that button can come along
constexpr float CELL = 200.f;//each cell (200 , 3*3)

// ---------------- COLORS ----------------
const sf::Color BG(15, 15, 20); // very dark bluish-black background const sf::Color BG(18, 18, 28);        // dark navy background
const sf::Color GRID(80, 80, 100);// muted gray-blue grid lines             100, 100, 120);   // soft gray grid
const sf::Color X_COLOR(220, 80, 80);// soft red for X                      255, 99, 132); // pinkish red
const sf::Color O_COLOR(80, 160, 220);// light sky blue for O               54, 162, 235); // bright blue
const sf::Color WIN_COLOR(255, 80, 80);// bright red highlight for winner   255, 206, 86); // golden yellow highlight

// ---------------- SOUND ----------------
sf::SoundBuffer createClick() {
    sf::SoundBuffer buffer;
    buffer.loadFromFile("/System/Library/Sounds/Glass.aiff"); // load sound
    return buffer;
}

// ---------------- WIN CHECK ----------------
int checkWin(int b[3][3], int &type, int &idx) {
    for (int i = 0; i < 3; i++) {
        if (b[i][0] && b[i][0] == b[i][1] && b[i][1] == b[i][2]) { // ROW
            type = 0; idx = i; return b[i][0];
        }
        if (b[0][i] && b[0][i] == b[1][i] && b[1][i] == b[2][i]) { // Column 
            type = 1; idx = i; return b[0][i];
        }
    }
    if (b[0][0] && b[0][0] == b[1][1] && b[1][1] == b[2][2]) { // diagonal left to right
        type = 2; return b[0][0];
    }
    if (b[0][2] && b[0][2] == b[1][1] && b[1][1] == b[2][0]) { // diagonal right to left 
        type = 3; return b[0][2];
    }
    return 0;
}

// ---------------- MAIN ----------------
int main() {

    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Tic Tac Toe"); // Window 
    window.setFramerateLimit(60); //Fps to 60 only 

    sf::Font font;
    font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf"); // load font

    sf::SoundBuffer clickBuf = createClick(); // load sound (18)
    sf::Sound click(clickBuf);

    int board[3][3] = {};
    bool xTurn = true; //whose turn
    bool gameOver = false; // game over 
    int winner = 0, type = -1, idx = -1;

    int scoreX = 0, scoreO = 0; // store score

    // -------- BUTTONS --------
    sf::RectangleShape button({200, 50}); //play again button 
    button.setPosition({120.f, 720.f});
    button.setFillColor(sf::Color(60, 60, 80));

    sf::RectangleShape backBtn({200, 50});//Back button 
    backBtn.setPosition({340.f, 720.f});
    backBtn.setFillColor(sf::Color(100, 100, 120));

    // -------- TEXT --------
    sf::Text btnText(font, "Play Again", 24);
    sf::Text backText(font, "Back", 24);

    // CENTER TEXT (IMPORTANT FIX)
    auto b1 = btnText.getLocalBounds(); // for center alingment of text to be in center 
    btnText.setOrigin({
    b1.position.x + b1.size.x/2, //finds origin 
    b1.position.y + b1.size.y/2
    });
    btnText.setPosition({
    button.getPosition().x + 100, // alingment
    button.getPosition().y + 25
    });

    auto b2 = backText.getLocalBounds();// for center alingment of text to be in center 
    backText.setOrigin({
    b2.position.x + b2.size.x/2, // finds origin 
    b2.position.y + b2.size.y/2
    });
    backText.setPosition({
    backBtn.getPosition().x + 100, // alingment
    backBtn.getPosition().y + 25
    });
    btnText.setFillColor(sf::Color::White);
    backText.setFillColor(sf::Color::White);

    sf::Text status(font, "", 28);
    sf::Text score(font, "", 22);

    // -------- LOOP --------
    while (window.isOpen()) {// runs throughout to keep game active 

        while (auto ev = window.pollEvent()) { // event handling 

            if (ev->is<sf::Event::Closed>())
                window.close();

            if (auto m = ev->getIf<sf::Event::MouseButtonPressed>()) { // mouse click check 

                float x = m->position.x;
                float y = m->position.y;

                // BACK
                if (backBtn.getGlobalBounds().contains({x,y})) {
                    window.close(); // returns to main menu
                }

                // RESTART
                else if (button.getGlobalBounds().contains({x,y})) {
                    for (auto &r : board)
                        for (auto &c : r) c = 0;

                    gameOver = false;
                    winner = 0;
                    xTurn = true;
                }

                // GRID
                else if (!gameOver && y < 600) {

                    int r = y / 200;
                    int c = x / 200;

                    if (board[r][c] == 0) { //check cell is empty
                        board[r][c] = xTurn ? 1 : 2; // place move 
                        xTurn = !xTurn; // switch turns

                        click.play(); // play sound

                        winner = checkWin(board, type, idx); //check win condition 

                        if (winner) {
                            gameOver = true;
                            if (winner == 1) scoreX++; // update score 
                            else scoreO++; // update score 
                        }
                    }
                }
            }
        }

        window.clear(BG);

        // GRID (line and colour)
        for (int i = 1; i < 3; i++) {
            sf::RectangleShape h({600, 4});
            h.setPosition({0.f, i * CELL});
            h.setFillColor(GRID);

            sf::RectangleShape v({4, 600});
            v.setPosition({i * CELL, 0.f});
            v.setFillColor(GRID);

            window.draw(h);
            window.draw(v);
        }

        // DRAW X/O
        for (int i = 0; i < 3; i++) {// row
            for (int j = 0; j < 3; j++) { // column

                float x = j * CELL;
                float y = i * CELL; 
                  //X
                if (board[i][j] == 1) { 
                    sf::RectangleShape l1({120, 6}); //strip 1  shape size
                    l1.setPosition({x + 40, y + 40});
                    l1.setRotation(sf::degrees(45));
                    l1.setFillColor(X_COLOR);

                    sf::RectangleShape l2({120, 6});//strpi 2
                    l2.setPosition({x + 40, y + 160});
                    l2.setRotation(sf::degrees(-45));
                    l2.setFillColor(X_COLOR);

                    window.draw(l1);
                    window.draw(l2);
                }
                    //O
                else if (board[i][j] == 2) {
                    sf::CircleShape o(70); // radius 
                    o.setOrigin({70,70});
                    o.setPosition({x + 100, y + 100});
                    o.setFillColor(sf::Color::Transparent);
                    o.setOutlineThickness(6);
                    o.setOutlineColor(O_COLOR);
                    window.draw(o);
                }
            }
        }

        // WIN LINE
        if (winner) {//Runs only when someone has won
            sf::RectangleShape line;
            line.setFillColor(WIN_COLOR);

            if (type == 0) {//Row win
                line.setSize({600, 6});
                line.setPosition({0, idx * CELL + 100});
            }
            else if (type == 1) {//Column win
                line.setSize({600, 6});
                line.setPosition({idx * CELL + 100, 0});
                line.setRotation(sf::degrees(90));
            }
            else if (type == 2) {//Diagonal ↘
                line.setSize({850, 6});
                line.setPosition({-100, -100});
                line.setRotation(sf::degrees(45));
            }
            else if (type == 3) {//Diagonal ↙
                line.setSize({850, 6});
                line.setPosition({700, -100});
                line.setRotation(sf::degrees(135));
            }

            window.draw(line);
        }

        // TEXT
        status.setString(!gameOver ?
            (xTurn ? "Player X Turn" : "Player O Turn") : // game is running 
            (winner == 1 ? "Player X Wins!" : // game is over
             winner == 2 ? "Player O Wins!" : "Draw!")); // game is over 

        status.setPosition({120, 610});

        score.setString("X: " + std::to_string(scoreX) + // score display 
                        "   O: " + std::to_string(scoreO));  // X: 2   O: 1
        score.setPosition({180, 650}); // position of score

        window.draw(status);
        window.draw(score);
        window.draw(button); // play again (restart button)
        window.draw(backBtn);
        window.draw(btnText);//play again (text)
        window.draw(backText);

        window.display();
    }
}