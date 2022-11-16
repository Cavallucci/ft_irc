/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:05 by llalba            #+#    #+#             */
/*   Updated: 2022/11/16 15:46:12 by llalba           ###   ########.fr       */
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


//---------------------------- ACCESSORS / GETTERS ----------------------------

std::string			Channel::getName(void) const { return _name; }
std::string			Channel::getTopic(void) const { return _topic; }

bool				Channel::isIn(int fd) const
{
	if (_users.count(fd))
		return (true);
	return (false);
}

bool				Channel::isOp(int fd) const
{
	if (_ops.count(fd))
		return (true);
	return (false);
}

bool				Channel::isBanned(int fd) const
{
	if (_banned.count(fd))
		return (true);
	return (false);
}

bool				Channel::isMod(int fd) const
{
	if (_moderators.count(fd))
		return (true);
	return (false);
}

//----------------------------- MUTATORS / SETTERS ----------------------------

void				Channel::addUser(User *user)
{
	if (!isIn(user->getFd()))
		_users[user->getFd()] = user;
}

void				Channel::delUser(User *user)
{
	if (isIn(user->getFd()))
		_users.erase(user->getFd());
}

void				Channel::addOp(User *user)
{
	if (!isOp(user->getFd()))
		_ops[user->getFd()] = user;
}

void				Channel::delOp(User *user)
{
	if (isOp(user->getFd()))
		_ops.erase(user->getFd());
}

void				Channel::ban(User *user)
{
	if (!isBanned(user->getFd()))
		_banned[user->getFd()] = user;
}

void				Channel::unban(User *user)
{
	if (isBanned(user->getFd()))
		_banned.erase(user->getFd());
}

void				Channel::addMod(User *user)
{
	if (!isMod(user->getFd()))
		_moderators[user->getFd()] = user;
}

void				Channel::delMod(User *user)
{
	if (isMod(user->getFd()))
		_moderators.erase(user->getFd());
}
