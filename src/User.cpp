/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:13 by llalba            #+#    #+#             */
/*   Updated: 2022/11/11 18:10:16 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

//-------------------------------- CONSTRUCTORS -------------------------------

User::User()
{

}

User::User(User const & src)
{
	(void)src;
}

//-------------------------------- DESTRUCTORS --------------------------------

User::~User()
{

}

//--------------------------------- OVERLOAD ----------------------------------

User &				User::operator=(User const & rhs)
{
	(void)rhs;
	//if (this != &rhs)
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<(std::ostream & o, User const & e)
{
	(void)e;
	//o << "Value = " << e.getValue();
	return o;
}

//---------------------------------- METHODS ----------------------------------

void	User::reply(std::string msg)
{
	msg.append("\r\n");
}

//---------------------------- ACCESSORS & MUTATORS ---------------------------

std::string			User::getInput(void)
{
	return _input;
}

int					User::getFd(void)
{
	return _fd;
}

std::string			User::getNickname(void)
{
	return _nickname;
}

bool				User::setInput(void)
{
	int				bytes;
	char			buf[BUFFER_SIZE];
	std::string		input = "";

	resetInput();
	while (input.length() < 2 || input.rfind("\r\n") != input.length() - 2 )
	{
		memset(buf, 0, BUFFER_SIZE);
		bytes = recv(getFd(), buf, BUFFER_SIZE, 0);
		if (bytes <= 0)
		{
			if (!bytes)
				std::cout << RECV_ZERO << getFd() << std::endl;
			else
				std::cerr << RED ERR_RECV << getFd() << END << std::endl;
			return (false);
		}
		input.append(buf);
	}
	return (true);
}

void				User::resetInput(void)
{
	_input.clear();
}
