#include <SFML/Graphics.hpp> // for graphics 
#include <SFML/Audio.hpp> // audio 
#include <vector> // store cards 
#include <algorithm> // shuffle
#include <random> //randomness
#include <iostream> 


// ---------- CARD ----------
class Card {
public:
   int value; // numbers 1-4
   bool revealed = false;//current visibility 
   bool matched = false;//permanently solved


   sf::RectangleShape rect; // card shape 
   sf::Text text; // number assign 


   Card(int v, sf::Font& font, float x, float y)
       : value(v), text(font, std::to_string(v), 30)
   {
       rect.setSize({100, 100});//size
       rect.setFillColor(sf::Color(140, 140, 180));//color
       rect.setPosition({x, y});//position 


       text.setFillColor(sf::Color::White);//text colour


       auto b = text.getLocalBounds();
       text.setOrigin({
           b.position.x + b.size.x / 2.f,// starting point + half width for perfect center
           b.position.y + b.size.y / 2.f
       });


       text.setPosition({x + 50, y + 50}); // center
   }


   void draw(sf::RenderWindow& window) {//matching 
       window.draw(rect);
       if (revealed || matched)
           window.draw(text);
   }


   bool contains(float x, float y) {// mouse clik check
       return rect.getGlobalBounds().contains({x, y});
   }
};


int main() {
   sf::RenderWindow window(sf::VideoMode({800, 600}), "Memory Card Game");//window size setup 


   // ---------- FONT ----------
   sf::Font font;
   if (!font.openFromFile("/System/Library/Fonts/Supplemental/Arial.ttf")) {
       std::cout << "Font load failed\n";
   }


   // ---------- SOUND ----------
   sf::SoundBuffer buffer;
   if (!buffer.loadFromFile("/System/Library/Sounds/Glass.aiff")) {
       std::cout << "Sound load failed\n";
   }
   sf::Sound click(buffer);


   // ---------- VALUES ----------
   std::vector<int> values = {1,1,2,2,3,3,4,4};// total 4 pairs 
   std::shuffle(values.begin(), values.end(), std::mt19937(std::random_device()()));//shuffle make it random 


   // ---------- CARDS ----------
   std::vector<Card> cards;
   int idx = 0;
   for (int i = 0; i < 2; i++)//row 
       for (int j = 0; j < 4; j++) //column
           cards.emplace_back(values[idx++], font, 100 + j * 150, 150 + i * 150);// position of card


   Card* first = nullptr; // card selection 
   Card* second = nullptr; // card selection 


   bool gameWon = false; //game still running


   sf::Clock timer;//clock timer built in function 
   float finalTime = 0;


   // ---------- TIMER ----------
   sf::Text timerText(font, "", 24);
   timerText.setPosition({20, 20});


   // ---------- WIN TEXT ----------
   sf::Text winText(font, "YOU WIN!", 40);
   winText.setFillColor(sf::Color::Green);


   auto b = winText.getLocalBounds();
   winText.setOrigin({
       b.position.x + b.size.x / 2.f,
       b.position.y + b.size.y / 2.f
   });


   // ---------- BUTTON ----------
   sf::RectangleShape button({200, 50});
   button.setFillColor(sf::Color(100, 100, 120));


   sf::Text btnText(font, "Restart", 24);
   btnText.setFillColor(sf::Color::Blue);


   auto bb = btnText.getLocalBounds();
   btnText.setOrigin({
       bb.position.x + bb.size.x / 2.f,
       bb.position.y + bb.size.y / 2.f
   });
   // ---------- BACK BUTTON ----------
    sf::RectangleShape backBtn({200, 50});
    backBtn.setPosition({550, 20});
     backBtn.setFillColor(sf::Color(120,120,150));

    sf::Text backText(font, "Back", 22);
    backText.setFillColor(sf::Color::White);

     // center text properly
    auto bt = backText.getLocalBounds();
    backText.setOrigin({
        bt.position.x + bt.size.x / 2.f,
        bt.position.y + bt.size.y / 2.f
    });
    backText.setPosition({650, 45});


   // ---------- LOOP ----------
   while (window.isOpen()) {
       while (auto event = window.pollEvent()) {//event handling
           if (event->is<sf::Event::Closed>())
               window.close();


           if (event->is<sf::Event::MouseButtonPressed>()) {//mouse clicks 
               auto pos = sf::Mouse::getPosition(window);
               float x = pos.x;
               float y = pos.y;
               // BACK FIRST
               if (backBtn.getGlobalBounds().contains({x, y})) {//going back
               window.close();
               }


               // restart
               else if (gameWon && button.getGlobalBounds().contains({x, y})) {
                   std::shuffle(values.begin(), values.end(), std::mt19937(std::random_device()()));
                   cards.clear();


                   int i2 = 0;
                   for (int i = 0; i < 2; i++)
                       for (int j = 0; j < 4; j++)
                           cards.emplace_back(values[i2++], font, 100 + j * 150, 150 + i * 150);//recreate grid 


                   gameWon = false;
                   timer.restart();
                   first = second = nullptr;    // as game restart everything reset
               }


               // card clicks
               if (!gameWon) {
                   for (auto& c : cards) {
                       if (c.contains(x, y) && !c.revealed && !c.matched) {//card pairing matching 
                           click.play();


                           c.revealed = true;//reveled card


                           if (!first) first = &c;//first card seletion 
                           else if (!second) {
                               second = &c;


                               if (first->value == second->value) {//match check 
                                   first->matched = second->matched = true;
                               } else {
                                   sf::sleep(sf::milliseconds(400));//if wrong match 
                                   first->revealed = second->revealed = false;
                               }


                               first = second = nullptr;// reset selection 
                           }
                           break;
                       }
                   }
               }
           }
       }


       // ---------- CHECK WIN ----------
       if (!gameWon) {
           gameWon = true; // assume player won 
           for (auto& c : cards)
               if (!c.matched) gameWon = false;// if even one card is not match then game goes on 


           if (gameWon)
               finalTime = timer.getElapsedTime().asSeconds();// save the final time pause it 
       }


       // ---------- DRAW ----------
       window.clear(sf::Color(20, 20, 40));//clear screen to prevent overlapping


       if (!gameWon)// if game is going on timer goes on 
           timerText.setString("Time: " + std::to_string((int)timer.getElapsedTime().asSeconds()) + "s");
       else // game won time 
           timerText.setString("Time: " + std::to_string((int)finalTime) + "s");


       window.draw(timerText);//timer text 


       for (auto& c : cards)
           c.draw(window);
       window.draw(backBtn); // display back button 
       window.draw(backText);


       // ---------- FIXED POSITIONS (BELOW CARDS) ----------
       if (gameWon) {//show after game is won 
           float centerX = window.getSize().x / 2.f;  //find center of screen 


           // move below grid
           winText.setPosition({centerX, 440.f}); // position you win 


           button.setPosition({centerX - 100.f, 500.f}); // position of restart button 
           btnText.setPosition({centerX, 525.f});


           window.draw(winText);//'you win '
           window.draw(button);//restart button 
           window.draw(btnText);//button text 
           
       }
       window.display(); // render everything on screen 
   }


   return 0;
}

