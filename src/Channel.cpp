/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:05 by llalba            #+#    #+#             */
/*   Updated: 2022/11/10 10:55:06 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

//-------------------------------- CONSTRUCTORS -------------------------------

Channel::Channel()
{

}

Channel::Channel(Channel const & src)
{
	(void)src;
}

//-------------------------------- DESTRUCTORS --------------------------------

Channel::~Channel()
{

}

//--------------------------------- OVERLOAD ----------------------------------

Channel &				Channel::operator=(Channel const & rhs)
{
	(void)rhs;
	//if (this != &rhs)
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<(std::ostream & o, Channel const & e)
{
	(void)e;
	//o << "Value = " << e.getValue();
	return o;
}

//---------------------------------- METHODS ----------------------------------



//---------------------------- ACCESSORS & MUTATORS ---------------------------
