/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:13 by llalba            #+#    #+#             */
/*   Updated: 2022/11/13 16:21:26 by llalba           ###   ########.fr       */
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

int					User::getFd(void) { return _fd; }
std::string			User::getInput(void) { return _input; }
std::string			User::getNickname(void) { return _nickname; }
str_vec const &		User::getArgs(void) const { return _args; }

bool				User::setInput(void)
{
	int				bytes;
	char			buf[BUFFER_SIZE];

	resetInput();
	while (_input.length() < 2 || _input.rfind("\r\n") != _input.length() - 2 )
	{
		memset(buf, 0, BUFFER_SIZE);
		bytes = recv(getFd(), buf, BUFFER_SIZE, 0);
		if (bytes <= 0)
		{
			if (!bytes)
				std::cout << RECV_ZERO << getFd() << std::endl;
			else if (bytes < 0)
				std::cerr << RED ERR_RECV << getFd() << END << std::endl;
			return (false);
		}
		_input.append(buf);
		if (bytes > 512) // IRC messages are limited to 512 characters
		{
			std::cerr << RED ERR_TOO_LONG << getFd() << END << std::endl;
			_input.erase(510, std::string::npos);
			break ;
		}
	}
	if (_input.rfind("\r\n") == _input.length() - 2) // removes the final "\r\n"
		_input.erase(_input.length() - 2);
	_input.erase(0, _input.find_first_not_of(' ')); // removes prefixing spaces
	_input.erase(_input.find_last_not_of(' ') + 1); // removes suffixing spaces
	while (_input.rfind("  ") != std::string::npos) // removes duplicate spaces
		_input.replace(_input.find("  "), 2, " ");
	return (true);
}

bool				User::setArgs(str_vec args)
{
	if (args.size() > 15)
	return (false);
	_args = args;
	return (true);
}

void				User::resetInput(void)
{
	_input.clear();
	_args.clear();
}
