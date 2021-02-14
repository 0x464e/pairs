/* Pairs
*
* Desc:
* Pairs memory game. This program generates a memory game, where a desired
* amount of players flip over a desired amount randomized cards. The
* card placements are randomized based off a user input seed number.
*
* Each round each player gives gameboard coordinates to flip over two cards.
* The cards are flipped over for everyone to see, and they are a pair, they
* are removed from the gameboard and the player scores one pair and gets to
* continue his turn. If the cards weren't a pair, they’re turned backside up
* again, on the same spot and players should try to remember which cards were
* where. The turn passes over to the next player.
*
* The visible side of cards is represented as letters starting from 'A', and
* the backside of the cards is represented by a '#'. Removed cards are
* represented as a '.'
*
* The games ends when the gameboard is empty of cards. The program prints
* out the winner and their score (amount of card pairs collected), or in case
* of a tie, it prints out how many players were tied between each other and
* with what score. A player can also forfeit the game, in which case the game
* ends immediately.
*
*
* COMP.CS.110 K2021
*
* Program author
* Name: 
* Student number: 
* UserID: 
* E-Mail: 
*
* Notes about the program and it's implementation:
* Tried to practice good usage of constants and references.
* In parameters references are used on types with sizeof() > sizeof(ptr).
* Sudden usage of pointers for Card objects may seem like a weird design
* choice, but I wanted to get some practice with pointers as well.
* 
* run_game function may have grown a bit long, but breaking it up to even
* more functions would've made it less clear in my opinion. Anyway, its
* length mostly comes from spacing and mandatory braces.
* */

#include "player.hh"
#include "card.hh"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

const std::string INPUT_AMOUNT_OF_CARDS = "Enter the amount of cards (an even number): ";
const std::string INPUT_SEED = "Enter a seed value: ";
const std::string INPUT_AMOUNT_OF_PLAYERS = "Enter the amount of players (one or more): ";
const std::string INVALID_AMOUNT_OF_PLAYERS = "Invalid amount of players.";
const std::string INPUT_CARDS = "Enter two cards (x1, y1, x2, y2), or q to quit: ";
const std::string INVALID_CARD = "Invalid card.";
const std::string FOUND = "Pairs found.";
const std::string NOT_FOUND = "Pairs not found.";
const std::string GIVING_UP = "Why on earth you are giving up the game?";
const std::string GAME_OVER = "Game over!";

using Game_row_type = std::vector<Card>;
using Game_board_type = std::vector<std::vector<Card>>;

// Converts the given numeric string to the corresponding integer
// (by calling stoi).
// If the given string is not numeric, returns 0
// (which leads to an invalid card later).
unsigned int stoi_with_check(const std::string& str)
{
	//fix crashing on empty input
	if (str.empty())
	{
		return 0;
	}

	auto is_numeric = true;
	for (auto i : str)
	{
		if (!isdigit(i))
		{
			is_numeric = false;
			break;
		}
	}
	if (is_numeric)
	{
		return stoi(str);
	}
	return 0;
}

// Fills the game board, the size of which is rows * columns, with empty cards.
void init_with_empties(Game_board_type& g_board, const unsigned int rows,
	const unsigned int columns)
{
	g_board.clear();
	Game_row_type row;
	for (auto i = 0u; i < columns; ++i)
	{
		const Card card;
		row.push_back(card);
	}
	for (auto i = 0u; i < rows; ++i)
	{
		g_board.push_back(row);
	}
}

// Finds the next free position in the game board (g_board), starting from the
// given position start and continuing from the beginning if needed.
// (Called only by the function init_with_cards.)
unsigned int next_free(Game_board_type& g_board, const unsigned int start)
{
	// Finding out the number of rows and columns of the game board
	const auto rows = g_board.size();
	const auto columns = g_board.at(0).size();

	// Starting from the given value
	for (auto i = start; i < rows * columns; ++i)
	{
		// vaihdettu
		if (g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY)
		{
			return i;
		}
	}

	// Continuing from the beginning
	for (auto i = 0u; i < start; ++i)
	{
		if (g_board.at(i / columns).at(i % columns).get_visibility() == EMPTY)
		{
			return i;
		}
	}

	// You should never reach this
	std::cout << "No more empty spaces" << std::endl;
	return rows * columns - 1;
}

// Initializes the given game board (g_board) with randomly generated cards,
// based on the given seed value.
void init_with_cards(Game_board_type& g_board, const int seed)
{
	// Finding out the number of rows and columns of the game board
	const auto rows = g_board.size();
	const auto columns = g_board.at(0).size();

	// Drawing a cell to be filled
	std::default_random_engine randomEng(seed);
	std::uniform_int_distribution<int> distr(0, rows * columns - 1);

	// Wiping out the first random number 
	// (that is always the lower bound of the distribution)
	distr(randomEng);

	// If the drawn cell is already filled with a card, next empty cell will be used.
	// (The next empty cell is searched for circularly, see function next_free.)
	for (unsigned int i = 0, c = 'A'; i < rows * columns - 1; i += 2, ++c)
	{
		// Adding two identical cards (pairs) in the game board
		for (auto j = 0u; j < 2; ++j)
		{
			auto cell = distr(randomEng);
			cell = next_free(g_board, cell);
			g_board.at(cell / columns).at(cell % columns).set_letter(c);
			g_board.at(cell / columns).at(cell % columns).set_visibility(HIDDEN);
		}
	}
}

// Prints a line consisting of the given character c.
// The length of the line is given in the parameter line_length.
// (Called only by the function print.)
void print_line_with_char(const char c, const unsigned int line_length)
{
	for (auto i = 0u; i < line_length * 2 + 7; ++i)
	{
		std::cout << c;
	}
	std::cout << std::endl;
}

// Prints a variable-length game board with borders.
void print(const Game_board_type& g_board)
{
	// Finding out the number of rows and columns of the game board
	const auto rows = g_board.size();
	const auto columns = g_board.at(0).size();

	print_line_with_char('=', columns);
	std::cout << "|   | ";
	for (auto i = 0u; i < columns; ++i)
	{
		std::cout << i + 1 << " ";
	}
	std::cout << "|" << std::endl;
	print_line_with_char('-', columns);
	for (auto i = 0u; i < rows; ++i)
	{
		std::cout << "| " << i + 1 << " | ";
		for (auto j = 0u; j < columns; ++j)
		{
			g_board.at(i).at(j).print();
			std::cout << " ";
		}
		std::cout << "|" << std::endl;
	}
	print_line_with_char('=', columns);
}

// Asks the desired product from the user, and calculates the factors of
// the product such that the factor as near to each other as possible.
void ask_product_and_calculate_factors(unsigned int& smaller_factor,
	unsigned int& bigger_factor)
{
	auto product = 0u;
	while (!(product > 0 && product % 2 == 0))
	{
		std::cout << INPUT_AMOUNT_OF_CARDS;
		std::string product_str = "";
		std::getline(std::cin, product_str);
		product = stoi_with_check(product_str);
	}

	for (auto i = 1u; i * i <= product; ++i)
	{
		if (product % i == 0)
		{
			smaller_factor = i;
		}
	}
	bigger_factor = product / smaller_factor;
}


//Asks the user to input a valid number of players (an integer larger than 0).
//Returns a valid player count as an uint.
unsigned int ask_number_of_players()
{
	auto number_of_players = 0u;

	//loop until an integer larger than zero was input
	for (;;)
	{
		std::string input = "";
		std::cout << INPUT_AMOUNT_OF_PLAYERS;
		getline(std::cin, input);
		number_of_players = stoi_with_check(input); //returns 0 for fail

		if (number_of_players) //if non-zero
		{
			return number_of_players;
		}
	}
}

//Splits text by the specified separator, optionally includes empty parts.
//Returns a vector of the split parts.
std::vector<std::string> split(const std::string& text, const char separator,
	const bool include_empty = false)
{
	if (text.empty())
	{
		return std::vector<std::string> {""};
	}

	//vector to hold the separated parts
	std::vector<std::string> parts = {};
	//temp variable to hold the current part
	std::string part = "";

	//loop through each character looking for occurrences of
	//the separator character
	for (auto c : text)
	{
		if (c == separator)
		{
			//include empty parts only if asked for
			if (!part.empty())
			{
				parts.push_back(part);
			}
			else if (include_empty)
			{
				parts.push_back(part);
			}
			part = "";
			continue;
		}
		part += c;
	}

	//also add in whatever was after the last separator
	if (!part.empty())
	{
		parts.push_back(part);
	}
	else if (include_empty)
	{
		parts.push_back(part);
	}
	return parts;
}

//Reads the specified amount of player names.
//Returns a vector of player names as strings.
std::vector<std::string> read_player_names(const unsigned int count)
{
	//loop until the requested amount of names is given
	for(;;)
	{
		std::string input = "";
		std::cout << "List " << count << " players: ";
		std::getline(std::cin, input);

		//assume player names are separated by spaces
		auto names = split(input, ' ');

		//check that the correct amount of names was given
		if(names.size() != count)
		{
			std::cout << INVALID_AMOUNT_OF_PLAYERS << std::endl;
		}
		else
		{
			return names;
		}
	}
}


//Creates and returns player objects for the inputted names
std::vector<Player> initialize_players(
	const std::vector<std::string>&player_names)
{
	std::vector<Player> players = {};
	for (const auto& name : player_names)
	{
		//create new player object by name
		players.emplace_back(name);
	}

	return players;
}

//Converts each element of a string vector to uint and returns
//a new uint vector.
std::vector<unsigned int> str_vect_to_uint_vect(
	const std::vector<std::string>& str_vector)
{
	std::vector<unsigned int> uint_vector = {};

	for (const auto& str : str_vector)
	{
		//If an element can't be converted, it's set to zero.
		uint_vector.push_back(stoi_with_check(str));
	}

	return uint_vector;
}

//Checks if the input uint vector consists of two valid 
//game board coordinate pairs.
//Returns true for valid coordinates and false for invalid.
bool are_valid_coordinates(const std::vector<unsigned int>& coordinates,
	const Game_board_type& g_board)
{
	//two coordinate pairs can only form from four numbers
	//and disallow the two coordinate pairs being the same
	//coordinates are in 'x1 y1 x2 y2' format
	if (coordinates.size() != 4 ||
		(coordinates.at(0) == coordinates.at(2) && coordinates.at(1) == coordinates.at(3)))
	{
		return false;
	}

	const auto rows = g_board.size();
	const auto columns = g_board.at(0).size();

	//toggle variable for whether the x coordinate is
	//currently being checked or not
	auto checking_x_coordinate = true;

	//loop through each coordinate
	for (const auto& coord : coordinates)
	{
		//zero indicates an invalid coordinate
		if (!coord)
		{
			return false;
		}

		//if x outside of game field
		if (checking_x_coordinate && coord > columns)
		{
			return false;
		}

		//if y outside of game field
		if (!checking_x_coordinate && coord > rows)
		{
			return false;
		}

		//coordinates are in 'x y x y' format so can simply toggle between x and y
		checking_x_coordinate = !checking_x_coordinate;
	}

	//if we got this far, all coordinates are fine
	return true;
}

//Gets pointers to two card objects at the two specified coordinate pairs.
//Returns a vector of card object pointers.
std::vector<Card*> coords_to_card_ptrs(
	const std::vector<unsigned int>& coords, Game_board_type& g_board)
{
	//vector of Card object pointers
	std::vector<Card*> cards = {};

	//coordinates are in 'x1 y1 x2 y2' format
	//i starts at index 0 (where x1 is)
	//j starts at index 1 (where y1 is)
	//both indexes increase by 2 to get to x2 and y2
	for (auto i = 0, j = 1; i < 4; i += 2, j += 2)
	{
		//subtract one because vector indexing is zero based
		const auto x = coords.at(i) - 1;
		const auto y = coords.at(j) - 1;

		//append pointer of card object at coords
		cards.push_back(&g_board.at(y).at(x));
	}

	return cards;
}


//Attempts to turn over the specified card pair.
//Returns true upon success and false upon failure.
bool try_turn_cards(const std::vector<Card*>& cards,
	const Game_board_type& g_board)
{
	for (auto card : cards)
	{
		const auto visibility = card->get_visibility();
		//if the card isn't hidden, it cant be turned over
		//immediately return fail
		if (visibility != HIDDEN)
		{
			return false;
		}
	}

	//only turn cards if both can be turned
	//only two cards will ever be turned at once in this game
	//so we can safely index them with 0 and 1
	cards.at(0)->turn();
	cards.at(1)->turn();
	//print the board with the two cards open
	print(g_board);
	//hide the cards again
	cards.at(0)->turn();
	cards.at(1)->turn();
	return true;
}

//Checks if two specified cards are a pair and adds them to the player's
//collection if so
//Returns true if they're pairs, false if not.
bool is_pair(const std::vector<Card*>& cards, Player& player)
{
	const auto card1 = cards.at(0);
	const auto card2 = cards.at(1);

	if (card1->get_letter() == card2->get_letter())
	{
		player.add_card(card1);
		player.add_card(card2);
		return true;
	}
	return false;
}

//Compares players by their number of card pairs.
//Used as a custom comparator for std::sort.
bool compare_players_by_score(const Player& player1, const Player& player2)
{
	return player1.number_of_pairs() > player2.number_of_pairs();
}

//Determines the winner of the game based off the input players
//If it's a tie between two or more people, return all tied players in a vector.
//Otherwise just return one player in the vector
std::vector<Player> get_winners(std::vector<Player> players)
{
	//sort by descending
	std::sort(players.begin(), players.end(), compare_players_by_score);

	std::vector<Player> winners = {};

	auto previous_score = 0u;

	for (const auto& player : players)
	{
		const auto score = player.number_of_pairs();

		//scores are sorted by descending, so once the current score
		//is smaller than the previous score, we're done
		if (score >= previous_score)
		{
			previous_score = score;
			winners.push_back(player);
		}
		else
		{
			break;
		}
	}

	return winners;
}

//First prints the winner of the game based off the input players, 
//or if the game was tied between two or more people, print the tie.
void print_winner(const std::vector<Player>& players)
{
	const auto winners = get_winners(players);

	std::cout << GAME_OVER << std::endl;
	if (winners.size() == 1)
	{
		const auto& winner = winners.at(0);
		std::cout << winner.get_name() << " has won with "
			<< winner.number_of_pairs() << " pairs." << std::endl;
	}
	else
	{
		std::cout << "Tie of " << winners.size() << " players with "
			<< winners.at(0).number_of_pairs() << " pairs.";
	}
}

//Checks whether the game has ended (if all game field cards are empty)
//Returns true if game has ended, false if not.
bool is_game_over(const Game_board_type& g_board)
{
	//loop through all cards, if even one is non-empty, the game isn't over
	for (const auto& row : g_board)
	{
		for (const auto& card : row)
		{
			//if even one card isn't empty, the game isn't over
			if (card.get_visibility() != EMPTY)
			{
				return false;
			}
		}
	}
	//if we got this far, all cards are empty, so the game is over
	return true;
}

//Handles the printouts that should occur after coordinates are input
void print_outs(const Game_board_type& g_board,
	const std::vector<Player>& players,
	const bool found_pair, const bool game_has_ended)
{
	//print whether a pair was found or not
	std::cout << (found_pair ? FOUND : NOT_FOUND) << std::endl;
	//print each player's score
	for (const auto& player : players)
	{
		player.print();
	}
	//print the game board of hidden or empty cards
	print(g_board);

	if (game_has_ended)
	{
		print_winner(players);
	}
}

//Runs the actual game, needs the names of the players
//and the game board with initialized cards as arguments
void run_game(const std::vector<std::string>& player_names,
	Game_board_type& g_board)
{
	auto players = initialize_players(player_names);
	const auto player_count = players.size();

	print(g_board);

	//loop players until game ends
	for (auto i = 0u; true; i++)
	{
		//modulo player count to always go back to the index
		//of the first player 
		auto& player = players.at(i % player_count);
		//loop until valid coordinates, where a card can
		//successful be turned, are given
		for (;;)
		{
			std::cout << player.get_name() << ": " << INPUT_CARDS;
			std::string line = "";
			std::getline(std::cin, line);

			//assume inputs are split by a space
			auto inputs = split(line, ' ');

			//give up by entering q
			if (!inputs.empty() && inputs.at(0) == "q")
			{
				std::cout << GIVING_UP << std::endl;
				return;
			}

			//convert string inputs to coordinates
			const auto coordinates = str_vect_to_uint_vect(inputs);

			if (are_valid_coordinates(coordinates, g_board))
			{
				const auto cards = coords_to_card_ptrs(coordinates, g_board);
				if (try_turn_cards(cards, g_board))
				{
					const auto found_pair = is_pair(cards, player);
					const auto game_has_ended = is_game_over(g_board);
					print_outs(g_board, players, found_pair, game_has_ended);

					//exit after printouts if game has ended
					if (game_has_ended)
					{
						return;
					}
					//jump to the next player only if a pair wasn't found
					if (!found_pair)
					{
						break;
					}
				}
				else //turning card failed
				{
					std::cout << INVALID_CARD << std::endl;
				}
			}
			else //invalid coordinates
			{
				std::cout << INVALID_CARD << std::endl;
			}
		}
	}
}

//entry point
int main()
{
	Game_board_type game_board;

	auto factor1 = 1u;
	auto factor2 = 1u;
	ask_product_and_calculate_factors(factor1, factor2);
	init_with_empties(game_board, factor1, factor2);

	std::string seed_str = "";
	std::cout << INPUT_SEED;
	std::getline(std::cin, seed_str);
	const auto seed = stoi_with_check(seed_str);
	init_with_cards(game_board, seed);

	const auto number_of_players = ask_number_of_players();
	const auto player_names = read_player_names(number_of_players);

	run_game(player_names, game_board);

	return EXIT_SUCCESS;
}