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

}

void Player::print() const
{

}
