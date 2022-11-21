/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:05 by llalba            #+#    #+#             */
/*   Updated: 2022/11/21 09:48:06 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

//-------------------------------- CONSTRUCTORS -------------------------------

Channel::Channel()
{
}

Channel::Channel(std::string name, std::string password) :
_name(name), _password(password)
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

void					Channel::broadcast(std::string msg)
{
	for (usr_map::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		it->second->reply(msg);
	}
}

//---------------------------- ACCESSORS / GETTERS ----------------------------

std::string			Channel::getName(void) const { return _name; }
std::string			Channel::getTopic(void) const { return _topic; }
size_t				Channel::getMaxUsers(void) const { return _maxUsers; }
bool				Channel::hasPassword() const { return _password != ""; }
std::string			Channel::getMode(void) const { return _mode; }
std::string			Channel::getTopicCtxt(void) const { return _topicCtxt; };

bool				Channel::pwMatches(User *user, size_t nth) const
{
	str_vec		passwords;
	if (user->getArgs().size() == 1)
		return (false);
	passwords = split_str(user->getArgs()[1], ',');
	if (passwords.size() < nth)
		return (false);
	// TODO checker s'il est possible de définir un password ""
	return passwords[nth] == _password;
}

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

bool				Channel::isInvited(int fd) const
{
	if (_ops.count(fd))
		return (true);
	return (false);
}

bool				Channel::canJoin(User *user, size_t nth) const
{
	bool	isInviteOnly = (getMode().find('i') == std::string::npos);
	if (isInviteOnly && !isInvited(user->getFd()))
		user->reply(ERR_INVITEONLYCHAN(user->getServer(), getName()));
	else if (_users.size() + 1 > getMaxUsers())
		user->reply(ERR_CHANNELISFULL(user->getServer(), getName()));
	else if (isBanned(user->getFd()))
		user->reply(ERR_BANNEDFROMCHAN(user->getServer(), getName()));
	else if (hasPassword() && !pwMatches(user, nth))
		user->reply(ERR_BADCHANNELKEY(user->getServer(), getName()));
	else
		return (true);
	return (false);
}

//----------------------------- MUTATORS / SETTERS ----------------------------

void				Channel::setMode(std::string mode)
{
	_mode = mode;
}

void				Channel::setPassword(std::string password)
{
	_password = password;
}

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

void				Channel::invite(User *user)
{
	if (!isInvited(user->getFd()))
		_invited[user->getFd()] = user;
}

void				Channel::rmInvite(User *user)
{
	if (isInvited(user->getFd()))
		_invited.erase(user->getFd());
}
