/*
 * COMP.CS.110 K2021
 * 
 * Program author
 * Name: 
 * Student number: 
 * UserID: 
 * E-Mail: 
 */

#include "card.hh"
#include <iostream>

Card::Card():
    letter_(EMPTY_CHAR), visibility_(EMPTY)
{

}

Card::Card(const char c):
    letter_(c), visibility_(HIDDEN)
{

}

void Card::set_letter(const char c)
{
    letter_ = c;
}

void Card::set_visibility(const Visibility_type visibility)
{
    visibility_ = visibility;
}

char Card::get_letter() const
{
    return letter_;
}

Visibility_type Card::get_visibility() const
{
    return visibility_;
}

void Card::turn()
{
    if(visibility_ == HIDDEN)
    {
        visibility_ = OPEN;
    }
    else if(visibility_ == OPEN)
    {
        visibility_ = HIDDEN;
    }
    else // if(visibility_ == EMPTY)
    {
        std::cout << "Cannot turn an empty place." << std::endl;
    }
}

void Card::print() const
{
	switch (visibility_)
	{
		case OPEN:
			std::cout << letter_;
			break;
		case HIDDEN:
			std::cout << HIDDEN_CHAR;
			break;
		case EMPTY:
			std::cout << EMPTY_CHAR;
			break;
	}
}

void Card::remove_from_game_board()
{
    visibility_ = EMPTY;
}
