# DECODECODE-

DECODECODE is an interactive 2D shape puzzle game (OpenGL is used for graphical user interface) where players decode sequences by selecting the correct shape, aiming to achieve the highest score while progressing through levels.
![image](https://github.com/Mamoonalatif/DECODECODE-/assets/151760913/32d4e2d1-1fe5-41a8-bd6c-186fc82f413e)
![image](https://github.com/Mamoonalatif/DECODECODE-/assets/151760913/fadb9842-073c-4461-8365-9bd9b897d13d)
![image](https://github.com/Mamoonalatif/DECODECODE-/assets/151760913/31e2d40c-c4f2-4f80-b1b6-c1c70b3ab9ac)
![image](https://github.com/Mamoonalatif/DECODECODE-/assets/151760913/61dc6e95-3902-4b89-8298-22d9ff0d4086)

with timer funciton of 8s implemented in game1.cpp which resets after every question and minus 50 if user doesnot answer qs within time
![image](https://github.com/user-attachments/assets/6f3b762f-e5d6-43f7-b922-082982d473af)

## Table of Contents

- [About the Game](#about-the-game)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Game Files](#game-files)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## About the Game

DECODECODE challenges players with sequences of shapes. The goal is to interpret ("decode") these sequences and select the correct shape from several options. Players must think quickly and strategically as the game offers multiple levels of increasing difficulty. Your progress and high scores are tracked in the game.

## Features

- Interactive 2D gameplay with shape puzzles
- OpenGL-powered graphical interface
- Progressive levels with increasing complexity
- Score and progress tracking
- Engaging, logic-based challenges

## Installation

1. **Prerequisites**:
    - C++ compiler (e.g., g++, clang++)
    - OpenGL and GLUT development libraries

2. **Clone the Repository**:
    ```bash
    git clone https://github.com/Mamoonalatif/DECODECODE-.git
    cd DECODECODE-
    ```

3. **Build the Game**:
    ```bash
    g++ Game.cpp -o decodecode -lGL -lGLU -lglut
    ```
    *Alternatively, you can use `game1.cpp` as another entry point:*
    ```bash
    g++ game1.cpp -o decodecode -lGL -lGLU -lglut
    ```

4. **Run the Game**:
    ```bash
    ./decodecode
    ```

## Usage

- Launch the game and follow the on-screen instructions.
- Decode sequences by selecting the correct shape.
- Earn points and progress through levels.
- Your high scores and progress are saved in `highscore.txt` and `gameprogress.txt`.

## Game Files

- `Game.cpp` & `game1.cpp` — Main game source files.
- `highscore.txt` — Stores player high scores.
- `gameprogress.txt` — Tracks player progress.
- `PPt_Timingwheel.pptx` — Project-related presentation (not required for gameplay).

## Contributing

Contributions are welcome! To contribute:

1. Fork this repository
2. Create a new branch for your feature or bugfix
3. Make your changes and commit them
4. Open a pull request with a description of your changes

## License

This project does not currently specify a license. Please contact the repository owner for more information if you wish to use or distribute this code.

## Contact

Repository Owner: [Mamoonalatif](https://github.com/Mamoonalatif)

For questions or feedback, please open an issue in the repository.

---
Enjoy decoding!


