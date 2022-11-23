/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:05 by llalba            #+#    #+#             */
/*   Updated: 2022/11/23 18:04:23 by llalba           ###   ########.fr       */
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
	// TODO completer cette fonction meme si a priori elle ne sert pas cf. User.cpp OVERLOAD
	(void)rhs;
	//if (this != &rhs)
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}


std::ostream &			operator<<(std::ostream & o, Channel const & e)
{
	// TODO completer cette fonction meme si a priori elle ne sert pas cf. User.cpp OVERLOAD
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


void					Channel::rpl_whoreply(User *user, std::string srv)
{
	if (hasMode('p') && !isIn(user->getFd()))
		return ;
	// TODO checker si le mode 's' doit changer quelque chose ou pas ici
	for (usr_map::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (!it->second->hasMode('i')) {
			user->reply(RPL_WHOREPLY(
				srv,
				getName(), // channel
				it->second->getUser(),
				it->second->getHost(),
				it->second->getNick(),
				it->second->getReal()
			));
		}
	}
}


void	Channel::msg(std::string srv, User *user, std::string txt, bool silently)
{
	if (hasMode('m') && !(isOp(user->getFd()) || isMod(user->getFd())))
	{
		if (!silently)
			user->reply(ERR_CANNOTSENDTOCHAN(srv, getName()));
		return ;
	}
	for (usr_map::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second != user) // the user doesn't want to write to himself
			it->second->reply(RPL_MSG(user->getNick(), getName(), txt));
	}
}

//---------------------------- ACCESSORS / GETTERS ----------------------------

std::string			Channel::getName(void) const { return _name; }
std::string			Channel::getTopic(void) const { return _topic; }
size_t				Channel::getMaxUsers(void) const { return _maxUsers; }
bool				Channel::hasPassword() const { return _password != ""; }
std::string			Channel::getTopicCtxt(void) const { return _topicCtxt; }
size_t				Channel::getNbUsers(void) const { return _users.size(); }


bool				Channel::hasMode(char c) const
{
	if (_mode.find(c) != std::string::npos)
		return (true);
	return (false);
}


bool				Channel::pwMatches(User *user, size_t nth) const
{
	str_vec		passwords;
	if (user->getArgs().size() == 1)
		return (false);
	passwords = split_str(user->getArgs()[1], ",", true);
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


bool				Channel::canJoin(std::string srv, User *user, size_t nth) const
{
	bool	isInviteOnly = hasMode('i');
	if (isInviteOnly && !isInvited(user->getFd()))
		user->reply(ERR_INVITEONLYCHAN(srv, getName()));
	else if (getNbUsers() + 1 > getMaxUsers())
		user->reply(ERR_CHANNELISFULL(srv, getName()));
	else if (isBanned(user->getFd()))
		user->reply(ERR_BANNEDFROMCHAN(srv, getName()));
	else if (hasPassword() && !pwMatches(user, nth))
		user->reply(ERR_BADCHANNELKEY(srv, getName()));
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


void				Channel::setTopic(std::string topic, std::string nick)
{
	std::stringstream	tmp;
	tmp << time(NULL);
	_topic = topic;
	_topicCtxt = nick + " " + tmp.str();
}
