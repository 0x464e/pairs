/*
 * COMP.CS.110 K2021
 *
 * Program author
 * Name: 
 * Student number: 
 * UserID: 
 * E-Mail: 
 */

#include "player.hh"
#include <iostream>

Player::Player(const std::string& name) :
	name_(name), number_of_pairs_(0)
{
}

std::string Player::get_name() const
{
	return name_;
}

unsigned int Player::number_of_pairs() const
{
	return number_of_pairs_;
}

void Player::add_card(Card& card)
{
	//don't operate on an already removed card
	if (card.get_visibility() == EMPTY)
	{
		return;
	}

	card.remove_from_game_board();
	number_of_pairs_++;
}

void Player::print() const
{
	std::cout << "*** " << name_ << " has "
		<< number_of_pairs_ << " pair(s)." << std::endl;
}
