# 🎮 Game Hub (SFML)

A multi-game launcher built using C++ and SFML.

## 🎯 Games Included
- Tic Tac Toe
- Memory Card Game

## 🧠 Concept
Main menu acts as a launcher to run different games.  
Structure is modular so new games can be added easily.

## 📁 Project Structure



## ⚙️ How to Run

### Compile

Main Menu:
g++ MAIN_MENU/main.cpp -o menu -lsfml-graphics -lsfml-window -lsfml-system

TicTacToe:
g++ TIC_TAC_TOE/tictactoe.cpp -o TIC_TAC_TOE/tictactoe -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

Memory Game:
g++ Memory_Card/memory.cpp -o Memory_Card/memory -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

### Run
./menu

## 🚀 Future Improvements
- Add more games (Snake, Hangman)
- Improve UI/UX
- Add scoring system across games
