# Connect 4 with Minimax AI

This project implements a Connect 4 game with a computer-controlled AI opponent using the Minimax algorithm. The AI evaluates possible moves and makes optimal decisions to compete against the player.

## Features
- **Minimax Algorithm with Alpha-Beta Pruning**: The AI evaluates the game board, making optimal decisions by minimizing the opponent’s score and maximizing its own.
- **C++ and SFML**: The project is built using C++ with the Simple and Fast Multimedia Library (SFML) for rendering.
- **Human vs AI Gameplay**: Play Connect 4 against an AI that makes strategic moves based on the game state.

## Project Structure

- `Board.h`: Defines the Connect 4 board structure, handles move logic, and checks for win conditions.
- `AI.h`: Contains the Minimax algorithm with alpha-beta pruning, enabling the AI to calculate the best possible move.
- `main.cpp`: Main game file that runs the game loop, handles user inputs, and renders the board using SFML.

## Prerequisites

- **SFML**: You need SFML installed on your system to run the game. To install it, use:
  ```bash
  sudo apt-get install libsfml-dev

# Usage
1. Clone the repository:
```bash
git clone https://github.com/hupeyy/Connect_4.git
```

2. Build the project:
```bash
cd Connect_4
mkdir build && cd build
cmake ..
make
```

3. Run the executable:
```bash
./Connect_4
```
