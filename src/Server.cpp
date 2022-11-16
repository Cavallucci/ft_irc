/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:35:09 by llalba            #+#    #+#             */
/*   Updated: 2022/11/16 18:15:58 by llalba           ###   ########.fr       */
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
	/*
	while (running)
	{
		poll(_pfds.data(), _pfds.size(), -1);
		for (pfds_it iterator = _pfds.begin(); iterator != _pfds.end(); iterator++)
		{
			if (iterator->revents & POLLHUP) //revents for returns && POLLHUP means the socket is no longer connected
			{
				std::cout << "PULLHUP\n";
				_deleteUser(iterator);
			}
			if (iterator->revents & POLLIN) //returns && data is ready
			{
				if (iterator->fd == _listener)
				{
					_addUser();
					break;
				}
				User		*user = _users.at(iterator->fd);
				if (user->setInput() == false)
					_deleteUser(iterator);
			}
			if (iterator == _pfds.end())
				break;
		}
	}
	std::cout << "OK" << std::endl;
	//_closeAll(); //TODO
}

void	Server::_addUser(void)
{
	int						new_fd;
	struct sockaddr_storage their_addr;
	socklen_t				addr_size;
	struct pollfd			pfd;
	char					host[INET6_ADDRSTRLEN];
	char					serv[INET6_ADDRSTRLEN]; // TODO a voir si 1000

	addr_size = sizeof their_addr;
	new_fd = accept(_listener, (struct sockaddr *)&their_addr, &addr_size);
	if (new_fd == -1)
		throw std::runtime_error(ERR_USER_FD);
	else
	{
		pfd.fd = new_fd;
		pfd.events = POLLIN;
		pfd.revents = 0;

		_pfds.push_back(pfd);
		_users.insert(std::make_pair(new_fd, new User(new_fd, &their_addr)));

		getnameinfo((struct sockaddr *)&their_addr, addr_size, host, INET6_ADDRSTRLEN,\
		serv, INET6_ADDRSTRLEN, 0); // Look up the host name and service name information for a given struct sockaddr
		std::cout << "New connexion from " << _host << ":" << host << " on socket " << new_fd << std::endl;
	}
}

void	Server::_deleteUser(pfds_it &it)
{
	User	*user = _users.at(it->fd);

	if (it->fd > 0)
	{
		_users.erase(it->fd);
		close(it->fd);
	}
	_pfds.erase(it);
	// while (user->_channels.size() != 0)
		//TODO supprimer des channels ou le user est present
	delete user;
	*/
}

bool	Server::_parseInput(User *user)
{
	if (!user->setInput()) // the user has disconnected or an error occurred
		return false; // we're going to remove the user iterator
	std::string::size_type	pos = user->getInput().find(' '); // might be string::npos
	std::string				cmd_str = user->getInput().substr(0, pos);
	if (user->getInput().length() != cmd_str.length())
	{
		str_vec				args = split_str(user->getInput().substr(pos + 1), ' ');
		if (!user->setArgs(args))
		{
			std::cout << ERR_TOO_MANY_PARAM << user->getFd() << std::endl;
			user->reply(ERR_15_PARAM);
			return true;
		}
	}
	try {
		for (size_t i = 0; i < cmd_str.size(); i++) // to lower case
			cmd_str[i] = ascii_to_lower(cmd_str[i]);
		CALL_MEMBER_FN(this, _commands.at(cmd_str))(user);
	} catch (const std::out_of_range &e) {
		user->reply(ERR_UNKNOWNCOMMAND(user->getServer(), user->getNick(), cmd_str));
	}
	return true;
}

//---------------------------- ACCESSORS / GETTERS ----------------------------

std::string		Server::getName(void) const { return _name; }
usr_map			Server::getUsers() const { return _users; }
chan_map		Server::getChannels() const { return _channels; }

User *			Server::getUser(int fd) const
{
	for (usr_map::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->first == fd)
			return it->second;
	}
	std::cerr << RED ERR_USER_FD_NOT_FOUND << fd << END << std::endl;
	return NULL;
}

User *			Server::getUser(std::string nick) const
{
	for (usr_map::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second->getNick() == nick)
			return it->second;
	}
	std::cerr << RED ERR_USER_NICK_NOT_FOUND << nick << END << std::endl;
	return NULL;
}

bool			Server::fdAlreadyIn(int fd) const
{
	if (_users.count(fd))
		return (true);
	return (false);
}

Channel *		Server::getChannel(std::string chan_name) const
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

void			Server::addChannel(Channel *chan)
{
	(void)chan; // TODO
}

void			Server::delChannel(Channel *chan)
{
	(void)chan; // TODO
}
