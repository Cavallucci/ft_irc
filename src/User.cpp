/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:13 by llalba            #+#    #+#             */
/*   Updated: 2022/11/21 17:47:17 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

//-------------------------------- CONSTRUCTORS -------------------------------

User::User() : _nickname(""), _hostname(""), _username(""), _realname(""),
_input(""), _welcomed(false), _fd(-1), _args(str_vec()), _channels(chan_map()), _addr(NULL)
{
}

User::User(int fd, struct sockaddr_storage	*addr) : _nickname(""), _hostname("localhost"), _username(""), _realname(""),
_input(""), _welcomed(false), _fd(fd), _args(str_vec()), _channels(chan_map()), _addr(addr)
{
}

User::User(User const & src)
{
	*this = src;
}

//-------------------------------- DESTRUCTORS --------------------------------

User::~User()
{
	_args.clear();
	clearChannels();
}

//--------------------------------- OVERLOAD ----------------------------------

User &				User::operator=(User const & rhs)
{
	if (this != &rhs)
	{
		this->setNick(rhs.getNick());
		this->setHost(rhs.getHost());
		this->setUser(rhs.getUser());
		this->setReal(rhs.getReal());
		this->setServer(rhs.getServer());
		if (rhs.hasBeenWelcomed())
			this->welcome(true);
		this->setFd(rhs.getFd());
		this->clearChannels();
		chan_map	chan = rhs.getChannels();
		for (chan_map::const_iterator it = chan.begin();
		it != chan.end();
		it++)
			this->addChannel(it->first);
	}
	return *this;
}

std::ostream &			operator<<(std::ostream & o, User const & e)
{
	o << YEL "User(" << &e << ") 🔶" END << std::endl;
	o << "🔸Nickname:			" << e.getNick() << std::endl;
	o << "🔸Hostname:			" << e.getHost() << std::endl;
	o << "🔸Username:			" << e.getUser() << std::endl;
	o << "🔸Realname:			" << e.getReal() << std::endl;
	o << "🔸Server:				" << e.getServer() << std::endl;
	o << "🔸Has been welcomed:	" << e.hasBeenWelcomed() << std::endl;
	o << "🔸File descriptor:	" << e.getFd() << std::endl;
	o << "🔸Command parameters:	";
	str_vec		args = e.getArgs();
	for (str_vec::const_iterator it = args.begin(); it != args.end(); it++)
		o << "[" << *it << "] ";
	o << std::endl;
	o << "🔸Channels:			";
	chan_map	chan = e.getChannels();
	for (chan_map::const_iterator it = chan.begin(); it != chan.end(); it++)
		o << "[" << it->first << "] ";
	o << std::endl;
	return o;
}

//---------------------------------- METHODS ----------------------------------

void				User::reply(std::string msg)
{
	msg.append("\r\n"); // TODO
}

void				User::welcome(bool silently)
{
	if (!silently)
	{
		reply(RPL_WELCOME(getServer(), getNick(), getUser(), \
			getHost()));
	}
	_welcomed = true;
}

//---------------------------- ACCESSORS / GETTERS ----------------------------

std::string			User::getNick(void) const { return _nickname; }
std::string			User::getHost(void) const { return _hostname; }
std::string			User::getUser(void) const { return _username; }
std::string			User::getReal(void) const { return _realname; }
std::string			User::getServer(void) const { return _server; }
std::string			User::getInput(void) const { return _input; }
bool				User::hasBeenWelcomed(void) const { return _welcomed; }
int					User::getFd(void) const { return _fd; }
str_vec const &		User::getArgs(void) const { return _args; }
chan_map const		User::getChannels() const { return _channels; }
bool				User::isLoggedIn(void) const { return _loggedIn; }

bool				User::hasMode(char c) const
{
	if (_mode.find(c) != std::string::npos)
		return (true);
	return (false);
}

std::string			User::getRawArgs(size_t skipped) const
{
	std::string		output = _rawArgs;
	while (skipped > 0)
	{
		std::string::size_type	pos = output.find(' ');
		if (pos == std::string::npos)
		{
			output = "";
			break ;
		}
		output = output.substr(pos + 1);
		skipped--;
	}
	return output;
}

Channel *			User::getChannel(std::string chan_name) const
{
	chan_map	chan = getChannels();
	for (chan_map::const_iterator it = chan.begin(); it != chan.end(); it++)
	{
		if (it->first == chan_name)
			return it->second;
	}
	std::cerr << RED ERR_CHANNEL_NOT_FOUND << chan_name << END << std::endl;
	return NULL;
}

//----------------------------- MUTATORS / SETTERS ----------------------------

void				User::setNick(std::string name) { _nickname = name; }
void				User::setHost(std::string name) { _hostname = name; }
void				User::setUser(std::string name) { _username = name; }
void				User::setReal(std::string name) { _realname = name; }
void				User::setServer(std::string name) { _server = name; }
void				User::setFd(int fd) { _fd = fd; }
void				User::setAddr(struct sockaddr_storage *addr) { _addr = addr; }
void				User::logIn(void) { _loggedIn = true; }
void				User::setRawArgs(std::string content) { _rawArgs = content; }

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
				std::cout << getNick() << " on socket " << getFd() << " has hang up" << std::endl;
			else if (bytes < 0)
				std::cerr << RED ERR_RECV << getFd() << END << std::endl;
			return (false); // an error occurred, somehow
		}
		_input.append(buf);
		if (bytes > 512)
		{
			std::cerr << RED ERR_TOO_LONG << getFd() << END << std::endl;
			_input.erase(510, std::string::npos);
			break ; // IRC messages are limited to 512 char including "\r\n"
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

void				User::resetInput(void)
{
	_input.clear();
	_args.clear();
}

bool				User::setArgs(str_vec args)
{
	if (args.size() > 15)
		return (false);
	_args = args;
	return (true);
}

bool				User::addChannel(std::string chan_name)
{
	if (_channels.find(chan_name) == _channels.end()) {
		Channel		*channel_ptr = NULL;
		// TODO
		_channels[chan_name] = channel_ptr;
		return (true);
	} else { // already joined
		return (false);
	}
}

bool				User::rmChannel(std::string chan_name)
{
	if (_channels.find(chan_name) == _channels.end()) {
		return (false);
	} else {
		_channels.erase(chan_name);
		return (true);
	}
}

void				User::clearChannels()
{
	_channels.clear();
}
