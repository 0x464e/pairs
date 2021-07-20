#include "player.hh"
#include <iostream>

Player::Player(const std::string& name) :
	name_(name), collected_cards_(0)
{
}

std::string Player::get_name() const
{
	return name_;
}

unsigned int Player::number_of_pairs() const
{
	//cards can only be collected in batches of two
	//so the division is guaranteed to 
	return collected_cards_ / 2;
}

void Player::add_card(Card* card)
{
	//don't operate on an already removed card
	if (card->get_visibility() == EMPTY)
	{
		return;
	}

	card->remove_from_game_board();
	collected_cards_++;
}

void Player::print() const
{
	std::cout << "*** " << name_ << " has "
		<< collected_cards_ / 2 << " pair(s)." << std::endl;
}
