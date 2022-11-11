/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:35:09 by llalba            #+#    #+#             */
/*   Updated: 2022/11/11 17:56:15 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool running;
//-------------------------------- CONSTRUCTORS -------------------------------

Server::Server() : _port(""), _host(""), _pwd("")
{
}

Server::Server(std::string port, std::string pwd) :
_port(port), _host("localhost"), _pwd(pwd)
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
		POLLIN,
		//poll returns :
		0
	};
	_pfds.push_back(fd_server);

	std::cout << "Waiting for clients..." << std::endl;

	while (running)
	{
		//poll
		//client connect :
			//accepted()
			//adduser :
				//push_back()
				//insert
			//getnameinfo
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
	std::cout << "OK" << std::endl;
}

bool	Server::_parseInput(User *user)
{
	if (!user->setInput()) // the user has disconnected or an error occurred
		return false; // remove user with iterator
	std::string::size_type	pos = user->getInput().find(' ');
	std::string				cmd_str = user->getInput().substr(0, pos);
	try {
		CALL_MEMBER_FN(this, _commands.at(cmd_str))(user);
	} catch (const std::out_of_range &e) {
		// user->reply(ERR_CMD_NOT_FOUND(user->getNickname(), cmd_str));
	}
	return true;
}

//---------------------------- ACCESSORS & MUTATORS ---------------------------
