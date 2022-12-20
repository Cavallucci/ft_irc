/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:13 by llalba            #+#    #+#             */
/*   Updated: 2022/12/20 11:58:25 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

const std::string	User::_valid_modes = "i";

//-------------------------------- CONSTRUCTORS -------------------------------

User::User() : _input(""), _rawArgs(""), _nickname(""), _hostname(""),
_username(""), _realname(""), _welcomed(false), _loggedIn(false),
_fd(-1), _addr(NULL), _mode("")
{
}


User::User(int fd, struct sockaddr_storage *addr) : _input(""), _rawArgs(""),
_nickname(""), _hostname("localhost"), _username(""), _realname(""),
_welcomed(false), _loggedIn(false), _fd(fd), _addr(addr), _mode("")
{
}


User::User(User const & src)
{
	*this = src;
}

//-------------------------------- DESTRUCTORS --------------------------------

User::~User()
{
}

//---------------------------------- METHODS ----------------------------------

void				User::reply(std::string msg)
{
	msg.append("\r\n");
	_replies.push_back(msg);
}


void				User::welcome(std::string srv, bool silently)
{
	if (!silently)
		reply(RPL_WELCOME(srv, getNick(), getUser(), getHost()));
	_welcomed = true;
}

//---------------------------- ACCESSORS / GETTERS ----------------------------


std::string			User::getInput(void) const { return _input; }
str_vec const &		User::getArgs(void) const { return _args; }
std::string			User::getNick(void) const { return _nickname; }
std::string			User::getHost(void) const { return _hostname; }
std::string			User::getUser(void) const { return _username; }
std::string			User::getReal(void) const { return _realname; }
std::string			User::getMode(void) const { return _mode; }
bool				User::hasBeenWelcomed(void) const { return _welcomed; }
bool				User::isLoggedIn(void) const { return _loggedIn; }
int					User::getFd(void) const { return _fd; }
chan_map const		User::getChannels() const { return _channels; }
str_vec				User::getReplies() const { return _replies; }

str_vec				User::getCommands(std::string input) const
{
	str_vec		commands = split_str(input, IRC_DELIMITER, false);
	if (DEBUG)
		std::cout << YEL "(2) Commands: " END;
	for (str_vec::iterator it = commands.begin(); it != commands.end(); ++it)
	{
		(*it).erase(0, (*it).find_first_not_of(' ')); // removes prefixing spaces
		if (DEBUG)
			std::cout << YEL "[" END << *it << YEL "]" END << std::endl;
	}
	return commands;
}

bool				User::isValidMode(char mode)
{
	if (_valid_modes.find(mode) != std::string::npos)
		return (true);
	return (false);
}


bool				User::hasMode(char c) const
{
	if (_mode.find(c) != std::string::npos)
		return (true);
	return (false);
}


/*
getRawArgs skips as many words as required, for instance with
_rawArgs = "hello   world,    how are  you?"
skipped = 2
"how are  you?" will be returned.
*/
std::string			User::getRawArgs(size_t skipped) const
{
	std::string		output = _rawArgs;
	output.erase(0, output.find_first_not_of(' ')); // removes prefixing spaces
	while (skipped > 0)
	{
		std::string::size_type	pos = output.find(' ');
		if (pos == std::string::npos)
		{
			output = "";
			break ;
		}
		output.erase(0, pos); // removes a word
		output.erase(0, output.find_first_not_of(' '));
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
	if (DEBUG)
		std::cerr << YEL ERR_CHANNEL_NOT_FOUND << chan_name << END << std::endl;
	return NULL;
}

//----------------------------- MUTATORS / SETTERS ----------------------------

void				User::clearReplies(void) { _replies.clear(); }
void				User::setNick(std::string name) { _nickname = name; }
void				User::setHost(std::string name) { _hostname = name; }
void				User::setUser(std::string name) { _username = name; }
void				User::setReal(std::string name) { _realname = name; }
void				User::setFd(int fd) { _fd = fd; }
void				User::setAddr(struct sockaddr_storage *addr) { _addr = addr; }
void				User::logIn(void) { _loggedIn = true; }


void				User::addMode(char new_mode)
{
	if (_mode.find(new_mode) == std::string::npos)
		_mode.append(1, new_mode);
}


void				User::rmMode(char new_mode)
{
	if (_mode.find(new_mode) != std::string::npos)
		_mode.erase(_mode.find(new_mode), 1);
}



void				User::setRawArgs(std::string content) {
	_rawArgs = content;
	if (DEBUG)
	{
		std::cout << WHT "(4) Raw arguments: [" END;
		std::cout << content << WHT "]" END << std::endl;
	}
}


bool				User::setInput(void)
{
	int				bytes;
	int				fd = getFd();
	char			buf[BUFFER_SIZE];
	resetInput();
	while (_input.length() < 2 || _input.rfind(IRC_DELIMITER) != _input.length() - 2)
	{
		memset(buf, 0, BUFFER_SIZE);
		bytes = recv(fd, buf, BUFFER_SIZE, 0);
		if (bytes <= 0)
		{
			if (!bytes)
				std::cout << RED RECV_ZERO << fd << END << std::endl;
			else if (bytes < 0)
				std::cerr << RED ERR_RECV << fd << END << std::endl;
			return (false); // an error occurred, somehow
		}
		_input.append(buf);
	}
	// we'll accept very long input not ending with "\r\n" since it doesn't matter
	if (bytes > 512) // IRC messages are limited to 512 char including "\r\n"
	{
		std::cerr << RED ERR_TOO_LONG << fd << END << std::endl;
		_input.erase(510, std::string::npos);
		_input.append(IRC_DELIMITER);
	}
	_input.erase(_input.length() - 2); // removes the final "\r\n"
	if (DEBUG)
		std::cout << YEL "(1) Input : [" END << _input << YEL "]" << END << std::endl;
	return (true);
}


void				User::resetInput(void)
{
	_input.clear();
	_args.clear();
}


void				User::setArgs(str_vec args)
{
	_args = args;
	if (DEBUG)
		std::cout << WHT "(5) Arguments:" END;
	for (str_vec::iterator it = _args.begin(); it != _args.end(); ++it)
	{
		(*it).erase(0, (*it).find_first_not_of(' ')); // removes prefixing spaces
		if (DEBUG)
			std::cout << WHT " [" END << *it << WHT "]" END;
	}
	if (DEBUG)
		std::cout << std::endl;
}


void				User::registerChannel(Channel *chan)
{
	if (_channels.find(chan->getName()) == _channels.end())
		_channels[chan->getName()] = chan;
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


void				User::clearAll()
{
	_args.clear();
	_channels.clear();
}
