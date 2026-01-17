# [Project Name: e.g., SFML Pong]

![Gameplay Screenshot](path/to/screenshot.png)
## 📝 Description
A classic recreation of the arcade game **Pong**, built from scratch using **C++** and the **SFML** (Simple and Fast Multimedia Library). This project demonstrates the fundamentals of 2D game development, including collision detection, game loops, and basic physics.

## ✨ Features
* **Classic Gameplay:** Two paddles, one ball, and a scoring system.
* **Single Player / Two Player:** Single Player mode has an AI as Enemy, Multiplayer lets you play with another one of your friend
* **Smooth Rendering:** Runs at 60 FPS using SFML's graphics module.

## 🎮 Controls
* **Player 1 (Left):** `W` (Up) / `S` (Down)
* **Player 2 (Right):** `Up Arrow` / `Down Arrow`
* **Select Option:** `Enter`

## 🛠️ Tech Stack
* **Language:** C++
* **Library:** SFML 2.6.1
* **IDE:** Visual Studio 2022

## What I Learned
* Implementing the Game Loop pattern.
* Handling AABB (Axis-Aligned Bounding Box) collision detection.
* Managing state (Menu vs. Gameplay vs. Game Over).
* Memory management in C++.

## 🚀 How to Build and Run

### Prerequisites
* C++ Compiler (g++, clang, or MSVC)
* SFML Library installed on your machine.

### Building with Visual Studio

**Option 1: Visual Studio**
1.  Open `MyGameCPlusPlus.sln`.
2.  Ensure the SFML include and library paths are configured in Project Properties.
3.  Build and Run in Release mode.

**Option 2: Command Line (g++)**
```bash
   g++ main.cpp -o Pong -I<path-to-sfml-include> -L<path-to-sfml-lib> -lsfml-graphics -lsfml-window -lsfml-system
   # On Windows
   ./Pong.exe
   # On Linux/Mac
   ./Pong
```

