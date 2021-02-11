/* Class: Player
 * -------------
 * Represents a single player in pairs (memory) game.
 *
 * COMP.CS.110 K2021
 *
 * Program author
 * Name: 
 * Student number: 
 * UserID: 
 * E-Mail: 
 * */

#ifndef PLAYER_HH
#define PLAYER_HH

#include "card.hh"
#include <string>

class Player
{
public:
    // Constructor: creates a player with the given name.
    Player(const std::string& name);

    // Returns the name of the player.
    std::string get_name() const;

    // Returns the number of pairs collected by the player so far.
    unsigned int number_of_pairs() const;

    // Moves the given card from the game board for the player,
    // i.e. inserts it to the collected cards of the player and
    // removes it from the game board.
    void add_card(Card& card);

    // Prints the game status of the player: name and collected pairs so far.
    void print() const;

private:
	std::string name_;
	unsigned int number_of_pairs_;
};

#endif // PLAYER_HH
