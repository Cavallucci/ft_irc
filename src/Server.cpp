/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:35:09 by llalba            #+#    #+#             */
/*   Updated: 2022/11/04 17:35:10 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

//-------------------------------- CONSTRUCTORS -------------------------------

Server::Server()
{

}

Server::Server(Server const & src)
{
	(void)src;
}

//-------------------------------- DESTRUCTORS --------------------------------

Server::~Server()
{

}

//--------------------------------- OVERLOAD ----------------------------------

Server &				Server::operator=(Server const & rhs)
{
	(void)rhs;
	//if (this != &rhs)
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<(std::ostream & o, Server const & e)
{
	(void)e;
	//o << "Value = " << e.getValue();
	return o;
}

//---------------------------------- METHODS ----------------------------------



//---------------------------- ACCESSORS & MUTATORS ---------------------------
