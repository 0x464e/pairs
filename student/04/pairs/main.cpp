/* Pairs
 *
 * Desc:
 *  This program generates a pairs (memory) game. The game has a variant
 * number of cards and players. At the beginning, the program also asks for a
 * seed value, since the cards will be set randomly in the game board.
 *  On each round, the player in turn gives the coordinates of two cards
 * (totally four numbers). After that the given cards will be turned as
 * visible and told if they are pairs or not. If they are pairs, they are
 * removed from the game board, the score of the player is increased, and
 * a new turn is given for the player. If the cards are not pairs, they
 * will be  turned hidden again, and the next player will be in turn.
 *  The program checks if the user-given coordinates are legal. The cards
 * determined by the coordinates must be found in the game board.
 *  After each change, the game board is printed again. The cards are
 * described as letters, starting from A and lasting so far as there are
 * cards. In printing the game board, a visible card is shown as its letter,
 * a hidden one as the number sign (#), and a removed one as a dot.
 *  Game will end when all pairs have been found, and the game board is
 * empty. The program tells who has/have won, i.e. collected most pairs.
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
 * TBD
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
	if(str.empty())
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
	for (unsigned int i = 0; i < columns; ++i)
	{
		const Card card;
		row.push_back(card);
	}
	for (unsigned int i = 0; i < rows; ++i)
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
	for (unsigned int i = 0; i < start; ++i)
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

	// Wiping out the first random number (that is always the lower bound of the distribution)
	distr(randomEng);

	// If the drawn cell is already filled with a card, next empty cell will be used.
	// (The next empty cell is searched for circularly, see function next_free.)
	for (unsigned int i = 0, c = 'A'; i < rows * columns - 1; i += 2, ++c)
	{
		// Adding two identical cards (pairs) in the game board
		for (unsigned int j = 0; j < 2; ++j)
		{
			unsigned int cell = distr(randomEng);
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
	for (unsigned int i = 0; i < line_length * 2 + 7; ++i)
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
	for (unsigned int i = 0; i < columns; ++i)
	{
		std::cout << i + 1 << " ";
	}
	std::cout << "|" << std::endl;
	print_line_with_char('-', columns);
	for (unsigned int i = 0; i < rows; ++i)
	{
		std::cout << "| " << i + 1 << " | ";
		for (unsigned int j = 0; j < columns; ++j)
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
	unsigned int product = 0;
	while (!(product > 0 && product % 2 == 0))
	{
		std::cout << INPUT_AMOUNT_OF_CARDS;
		std::string product_str = "";
		std::getline(std::cin, product_str);
		product = stoi_with_check(product_str);
	}

	for (unsigned int i = 1; i * i <= product; ++i)
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
	unsigned int number_of_players = 0;

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

	std::vector<std::string> parts;
	std::string part = "";

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
//Returns a vector player names as strings.
std::vector<std::string> read_player_names(const unsigned int count)
{
	std::string input = "";
	std::cout << "List " << count << " players: ";
	std::getline(std::cin, input);

	//assume player names are separated by spaces and
	//that the correct amount of names was given
	return split(input, ' ');
}


//Creates and returns player objects for the inputted names
std::vector<Player> create_player_objects(const std::vector<std::string>& 
	player_names)
{
	std::vector<Player> players;
	for(const auto name : player_names)
	{
		players.push_back(Player(name));
	}

	return players;
}

//Converts each element of a string vector to uint and returns
//a new uint vector.
//If an element can't be converted, it's set to zero.
std::vector<unsigned int> str_vect_to_uint_vect(
	const std::vector<std::string>& str_vector)
{
	std::vector<unsigned int> int_vector;

	for(const auto str : str_vector)
	{
		int_vector.push_back(stoi_with_check(str));
	}

	return int_vector;
}

//Checks if the input string consists of two valid game board coordinates.
//Returns true for valid coordinates and false for invalid.
bool are_valid_coordinates(const std::vector<unsigned int>& coordinates,
	const Game_board_type& g_board)
{
	//two coordinate pairs can only form from four numbers
	//and disallow the two coordinate pairs being the same
	if(coordinates.size() != 4 || 
		(coordinates.at(0) == coordinates.at(2) && coordinates.at(1) == coordinates.at(3)))
	{
		return false;
	}

	const auto rows = g_board.size();
	const auto columns = g_board.at(0).size();
	auto checking_x_coordinate = true;

	for(const auto& coord : coordinates)
	{
		//zero indicates an invalid coordinate
		if(!coord)
		{
			return false;
		}

		//if x outside of game field
		if(checking_x_coordinate && coord > columns)
		{
			return false;
		}

		//if y outside of game field
		if(!checking_x_coordinate && coord > rows)
		{
			return false;
		}

		//coordinates are in form 'x y x y' so can simply toggle between x and y
		checking_x_coordinate = !checking_x_coordinate;
	}

	//if we got this far, all coordinates are fine
	return true;
}

//Gets pointers to card objects at the two specified koordinate pairs
//Returns a vector of card object pointers.
std::vector<Card*> coords_to_card_ptrs(
	const std::vector<unsigned int>& coords, Game_board_type& g_board)
{
	//vector of Card object pointers
	std::vector<Card*> cards;
	for (unsigned int i = 0, j = 1; i < 4; i += 2, j += 2)
	{
		const auto x = coords.at(i) - 1;
		const auto y = coords.at(j) - 1;

		//append pointer of card object
		cards.push_back(&g_board.at(y).at(x));
	}

	return cards;
}


//Attempts to turn over cards at the specified game field coordinates.
//Returns true upon success and false upon failure.
bool try_turn_cards(const std::vector<Card*>& cards,
	Game_board_type& g_board)
{
	for(auto card : cards)
	{
		const auto visibility = card->get_visibility();
		if (visibility == EMPTY || visibility == OPEN)
		{
			std::cout << INVALID_CARD << std::endl;
			return false;
		}
	}

	//only turn cards if both can be turned
	cards.at(0)->turn();
	cards.at(1)->turn();
	//print the board with the two cards open
	print(g_board);
	//hide the cards again
	cards.at(0)->turn();
	cards.at(1)->turn();
	return true;
}

//Checks if two specified cards are a pair.
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
bool compare_players(const Player& player1, const Player& player2)
{
	return player1.number_of_pairs() > player2.number_of_pairs();
}

//Determines the winner of the game.
//If it's a tie between two or more people, 
//return all tie players in a vector.
std::vector<Player> get_winner(std::vector<Player> players)
{
	//sort by descending
	std::sort(players.begin(), players.end(), compare_players);

	std::vector<Player> winners;

	unsigned int previous_score = 0;

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

void print_winner(const std::vector<Player>& players)
{
	const auto winners = get_winner(players);

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
			<< winners.at(0).get_name() << " pairs.";
	}
	
}

//Checks whether the game has ended (if all game field cards are empty)
//
bool is_game_over(const Game_board_type& g_board)
{
	//loop through all cards, if even one is non-empty, the game isn't over
	for (const auto& row : g_board)
	{
		for (const auto& card : row)
		{
			if (card.get_visibility() != EMPTY)
			{
				return false;
			}
		}
	}
	return true;
}

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

//Runs the actual game
void run_game(const std::vector<std::string>& player_names, 
	Game_board_type& g_board)
{
	auto players = create_player_objects(player_names);
	auto found_pair = false;

	print(g_board);

	//loop all players until game ends
	for(;;)
	{
		//go through each player
		for (auto& player : players)
		{
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

				const auto coordinates = str_vect_to_uint_vect(inputs);

				if (are_valid_coordinates(coordinates, g_board))
				{
					const auto cards = coords_to_card_ptrs(coordinates, g_board);
					if(try_turn_cards(cards, g_board))
					{
						found_pair = is_pair(cards, player);
						const auto game_has_ended = is_game_over(g_board);
						print_outs(g_board, players, found_pair, game_has_ended);

						//exit after printouts if game has ended
						if(game_has_ended)
						{
							return;
						}
						//jump to the next player only if a pair wasn't found
						if(!found_pair)
						{
							break;
						}
					}
				}
			}
		}
	}
}

int main()
{
	Game_board_type game_board;

	unsigned int factor1 = 1;
	unsigned int factor2 = 1;
	ask_product_and_calculate_factors(factor1, factor2);
	init_with_empties(game_board, factor1, factor2);

	std::string seed_str = "";
	std::cout << INPUT_SEED;
	std::getline(std::cin, seed_str);
	const int seed = stoi_with_check(seed_str);
	init_with_cards(game_board, seed);

	const auto number_of_players = ask_number_of_players();
	const auto player_names = read_player_names(number_of_players);

	run_game(player_names, game_board);

	return EXIT_SUCCESS;
}