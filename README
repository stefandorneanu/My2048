# Minimal Memory Allocator (libosmem)

**Student:** Dorneanu Stefan Cristian, 311 CC

This project is a complete implementation of the classic 2048 game in C, featuring all core requirements and several bonus functionalities for enhanced interactivity and user experience.

---

## Implemented Requirements

### 1. Functional Main Menu
The menu provides smooth navigation with the following options:
* **New Game**
* **Resume**: Becomes active only after a game has been started, allowing the continuation of the saved state.
* **Quit**

**In-Game Controls:**
* Pressing the **`q`** key during gameplay saves the current board state and returns to the menu.
* The **Resume** option loads the previously saved game.

### 2. Complete Game Board Interface
The board is initialized with **two random values (2 or 4)** in random positions.
The interface includes a side panel displaying:
* Current Date and Time
* The running **Score** (starts at 0)
* A **Legend** showing the valid movement commands.

### 3. Core Gameplay Mechanics
* **Cell Movement and Merging**: Full logic for cell sliding and combining.
* **New Cell Generation**: A new cell (2 or 4) is generated automatically **only after a valid move**.
* **Invalid Moves**: If a move causes no change on the board, the board remains unchanged, and the system waits for another input.
* **Score Update**: The score is updated after every valid move.

### Bonus: Visual Enhancement
Every cell on the board has **distinct background and font colors** corresponding to its value. This visual aid makes identifying cells for merging easier. Colors can be easily adjusted within the `culori` function.

### 4. Automatic Cell Movement (AI)
The game includes an automatic move feature, triggered after a set time interval.
* **Strategy**: The move performed is the one that **frees up the most cells** (maximizing potential for merges and space creation).

### Bonus: Adjustable Auto-Move Time
The time interval for the automatic move can be easily modified in the source code.

### 5. Game End Conditions
When the game concludes, one of two messages is displayed on the screen:
* **"Ai castigat" (You Won)**: If one cell reaches the value **2048**.
* **"Ai pierdut" (You Lost)**: If all cells are occupied, and **no further valid moves can be made**.

---

## How It Works

The main execution flow starts with the `menu` function call inside `main`.

1.  **Menu Loop**: The menu can be navigated until an option is chosen (`choice` receives 1, 2, or 3).
2.  **Game Start**: If `choice` is 1 or 2, the game board and detail panel are displayed.
3.  **Movement Loop**: The `mutare` function enters an *infinite loop* that processes valid key inputs to change the board state. The score, time, and date are updated after every valid move.
4.  **Exiting**: The game screen can be exited by:
    * Pressing **`q`** (saves state and returns to the menu).
    * The **Game Ending** (displays the final message).

---

> **Note:** It is suggested to run the application in **full screen** mode for correct interface display.
