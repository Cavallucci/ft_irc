/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:35:09 by llalba            #+#    #+#             */
/*   Updated: 2022/11/14 15:43:47 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool running;

//-------------------------------- CONSTRUCTORS -------------------------------

Server::Server() : _port(""), _host(""), _pwd(""), _name("IRC")
{
}

Server::Server(std::string port, std::string pwd) :
_port(port), _host("localhost"), _pwd(pwd), _name("IRC")
{
	_serverSetUp();
	_initCommands();
	_serverConnect();
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

//--------------------------- SERVER SET UP METHODS ---------------------------

void	Server::_serverSetUp(void)
{
	int					fd;
	struct addrinfo		hints;
	struct addrinfo		*servinfo; // will point to the results

	memset(&hints, 0, sizeof(hints)); // make sure the struct is empty

	hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
	hints.ai_flags = AI_PASSIVE; // fill in my IP for me

	if (getaddrinfo(NULL, _port.c_str(), &hints, &servinfo))
		throw std::runtime_error(ERR_GETADDR_INFO);

	fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
	if (fd < 0)
		throw std::runtime_error(ERR_OPEN_SOCKET);

	if (bind(fd, servinfo->ai_addr, servinfo->ai_addrlen) < 0)
	{
		close(fd);
		throw std::runtime_error(ERR_BIND_SOCKET);
	}
	freeaddrinfo(servinfo); // free the linked-list
	if (listen(fd, 1000) < 0)
	{
		close(fd);
		throw std::runtime_error(ERR_LIST_SOCKET);
	}
	_listener = fd;
}

void	Server::_serverConnect(void)
{
	struct pollfd fd_server =
	{
		//the socket descriptor :
		_listener,
		//events we're interest in :
		POLLIN, //---> Alert when data is ready to recv
		//poll returns :
		0
	};
	_pfds.push_back(fd_server);

	std::cout << "Waiting for clients..." << std::endl;

	while (running)
	{
		poll(_pfds.data(), _pfds.size(), -1);
		for (pfds_it iterator = _pfds.begin(); iterator != _pfds.end(); iterator++)
		{
			if (iterator->revents & POLLHUP) //revents for returns && POLLHUP means the socket is no longer connected
			{
				std::cout << "PULLHUP\n";
				if (User *user = _users.at(iterator->fd))
				{
					_users.erase(iterator->fd);
					close(i);
				}
				_pfds.erase(iterator);
				while (user->isInChan())
					_partCmd()

				//delUser :
					//erase
					//close
					//_partCmd :
						//
			}
			if (iterator->revents & POLLIN) //returns && data is ready
			{
				std::cout << "PULLIN\n";
				if (iterator == _listener)
				{
					//client connect :
					//accepted()
					//adduser :
						//push_back()
						//insert
					//getnameinfo
				}

				//client msg :
					//getmessage :
						//clearmsg
						//rfind
						//recv
						//append
						//setmessage
					//deleteuser :
						//erase
						//partcmd : ?
							//
					//handlecmd : ?
						//
			}
		}
	}
	//_closeAll();
	std::cout << "OK" << std::endl;
}

char	Server::_ascii_to_lower(char in) {
	if (in <= 'Z' && in >= 'A')
		return in - ('Z' - 'z');
	return in;
}

bool	Server::_parseInput(User *user)
{
	if (!user->setInput()) // the user has disconnected or an error occurred
		return false; // we're going to remove the user iterator
	std::string::size_type	pos = user->getInput().find(' '); // might be string::npos
	std::string				cmd_str = user->getInput().substr(0, pos);
	if (user->getInput().length() != cmd_str.length())
	{
		std::string			arg_str = user->getInput().substr(pos + 1);
		std::stringstream	arg_stream(arg_str);
		str_vec				args;
		std::string			tmp;
		while (getline(arg_stream, tmp, ' '))
			args.push_back(tmp);
		if (!user->setArgs(args))
		{
			std::cout << ERR_TOO_MANY_PARAM << user->getFd() << std::endl;
			user->reply(IRC_TOO_MANY_PARAM);
			return true;
		}
	}
	try {
		for (size_t i = 0; i < cmd_str.size(); i++) // to lower case
			cmd_str[i] = _ascii_to_lower(cmd_str[i]);
		CALL_MEMBER_FN(this, _commands.at(cmd_str))(user);
	} catch (const std::out_of_range &e) {
		user->reply(IRC_CMD_NOT_FOUND(user->getServer(), user->getNick(), cmd_str));
	}
	return true;
}

//---------------------------- ACCESSORS / GETTERS ----------------------------

std::string		Server::_getName(void) const { return _name; }
usr_map			Server::_getUsers() const { return _users; }
chan_map		Server::_getChannels() const { return _channels; }

User *			Server::_getUser(int fd) const
{
	for (usr_map::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->first == fd)
			return it->second;
	}
	throw std::out_of_range(ERR_USER_NOT_FOUND);
}

User *			Server::_getUser(std::string nick) const
{
	for (usr_map::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second->getNick() == nick)
			return it->second;
	}
	throw std::out_of_range(ERR_USER_NOT_FOUND);
}


//----------------------------- MUTATORS / SETTERS ----------------------------

void			Server::_addChannel(Channel *chan)
{
	(void)chan; // TODO
}

void			Server::_delChannel(Channel *chan)
{
	(void)chan; // TODO
}
