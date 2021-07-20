# Pairs Memory Game

The first C++ project for my university's C++ programming course.

This game is played only in the command line and isn't too interesting. The GUI version of this game was done as the final project of this C++ programming course. You can find the GUI version [here](https://github.com/0x464e/pairs_gui).

---

This program generates a memory game, where a desired amount of players flip over a desired amount randomized cards. The card placements are randomized based off a user input seed number.

Each round each player gives gameboard coordinates to flip over two cards. The cards are flipped over for everyone to see, and they are a pair, they are removed from the gameboard and the player scores one pair and gets to continue his turn.  
If the cards weren't a pair, they’re turned backside up again, on the same spot and players should try to remember which cards were where. The turn passes over to the next player.

The visible side of cards is represented as letters starting from '**A**', and the backside of the cards is represented by a '**#**'. Removed cards are represented as a '**.**'

The games ends when the gameboard is empty of cards. The program prints out the winner and their score (amount of card pairs collected), or in case of a tie, it prints out how many players were tied between each other and with what score.  
A player can also forfeit the game, in which case the game ends immediately.

![image](https://i.imgur.com/Sx63N0d.png)

## Notes about the program and it's implementation
Tried to practice good usage of constants and references. In parameters references are used on types with `sizeof() > sizeof(ptr)`.  
Sudden usage of pointers for Card objects may seem like a weird design choice, but I wanted to get some practice with pointers as well.

## Compiling & Running
No special libraries or other dependencies are required, so you can just easily compile it in your favorite IDE or in the Linux command line with e.g these commands:
```
g++ -c -pipe -O2 -std=gnu++11 -o card.o card.cpp
g++ -c -pipe -O2 -std=gnu++11 -o main.o main.cpp
g++ -c -pipe -O2 -std=gnu++11 -o player.o player.cpp
g++ -Wl,-O1 -o pairs card.o main.o player.o
```

And then run it with  
```
./pairs
```

