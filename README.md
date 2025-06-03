# Hitori-Like

![Status](https://img.shields.io/badge/status-completed-brightgreen)  
![Language](https://img.shields.io/badge/language-C-blue)  

---

## ℹ️ About

Hitori-Like is a completed puzzle game inspired by the classic [*Hitori*](https://en.wikipedia.org/wiki/Hitori) game, developed in C as part of the Laboratórios de Informática II course (1st year, 2nd semester) at the University of Minho (2024/25).

Full project specification in the [Projeto.pdf](./Projeto.pdf) file.

---

## ✨ Description

Solve a board of letters by painting cells white (uppercase) or crossing them out (`#`) following uniqueness, adjacency, and connectivity rules.  

Includes an interactive command-line interface with loading, saving, solving, undo, and validation features.

---

## 📜 Game Rules

- Each cell initially contains a lowercase letter.
- In each row and column, only one painted white (uppercase) instance of each symbol is allowed.
- All other instances of the same symbol must be crossed out (`#`).
- If a cell is crossed out (`#`), all orthogonally adjacent cells must be painted white (uppercase).
- There must be an orthogonal path connecting all white cells on the board (connectivity).

---

## 🛠️ Features

- Load and save boards from files, supporting multiple difficulty levels.
- Visualize the board with coordinates.
- Select and modify cells by painting or crossing out.
- Validate the board state for rule compliance.
- Automatic inference to deduce cell states.
- Automatic puzzle solver.
- Undo functionality.
- Interactive command line interface (REPL).

---

## 🕹️ Available Commands

| Command           | Description                                                 |
|-------------------|-------------------------------------------------------------|
| `g <filename>`    | Save the current game state to a file.                      |
| `l <filename>`    | Load game state from a file.                                |
| `<coord>`         | Select a cell by coordinate (e.g. `a1`, `c3`).              |
| `b <coord>`       | Paint the selected cell white (uppercase).                  |
| `r <coord>`       | Cross out the selected cell (`#`).                           |
| `v`               | Validate the game state and report violated rules.          |
| `a`               | Apply inference rules to automatically deduce cell states.  |
| `A`               | Repeatedly apply inference until no changes occur.          |
| `R`               | Automatically solve the puzzle.                             |
| `d`               | Undo the last executed command.                              |
| `s`               | Exit the program.                                           |

---

## 🎲 Board File Format

The board file format is as follows:

```bash
L C
row_1
row_2
...
row_L
```

- `L` and `C` are integers representing the number of rows and columns.
- Each subsequent line contains exactly `C` lowercase letters.

Example:

```bash
5 5
ecadc
dcdec
bddce
cdeeb
accbb
```

---

## 👨‍💻 Compilation and Running

The project includes a Makefile with these targets:

- `make jogo` — Compiles the executable named `jogo`.
- `make testar` — Compiles and runs unit tests using CUnit.
- Compilation uses recommended flags for warnings and sanitizers.

To run the program:

```bash
./jogo
```

---

## 📄 Testing

- Uses CUnit testing framework.  
- Tests cover all rules and functionalities to ensure robustness.  
- Ensures full coverage of code and correct behavior, with over 90% code coverage measured using gcov.

---

## 🇵🇹 Language

The game interface and commands are in Portuguese, as this project was developed for a Portuguese university course.  

Command syntax and instructions follow Portuguese conventions.

---

## 👥 Authors

- [Simão Santos](https://github.com/simaosantoss)  
- [Afonso Coelho](https://github.com/ACoelho-07)  
- [João Falcão](https://github.com/Falcao06)
