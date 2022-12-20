/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:35:09 by llalba            #+#    #+#             */
/*   Updated: 2022/12/18 13:45:17 by llalba           ###   ########.fr       */
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
	if (DEBUG)
	{
		std::cout << CYN DEBUG_IS_ON END << std::endl << std::endl;
	}
	_serverSetUp();
	_initHandlers();
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
		freeaddrinfo(servinfo);
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
	int				ret;
	struct pollfd	fd_server =
	{
		//the socket descriptor :
		_listener,
		//events we're interest in :
		POLLIN, //---> Alert when data is ready to recv
		//poll returns :
		0
	};
	_pfds.push_back(fd_server);

	std::cout << GRN RUNNING END << std::endl;
	while (running)
	{
		ret = poll(_pfds.data(), _pfds.size(), -1);
		if (ret == -1 && running == true)
			throw std::runtime_error(ERR_POLL_FAILED); 
		for (pfds_it iterator = _pfds.begin(); iterator != _pfds.end(); iterator++)
		{
			if (iterator->revents & POLLHUP) //revents for returns && POLLHUP means the socket is no longer connected
			{
				_deleteUser(iterator->fd);
				std::cout << "User deleted" << std::endl;
			}
			if (iterator->revents & POLLIN) //returns && data is ready
			{
				if (iterator->fd == _listener)
				{
					_addUser();
					break ;
				}
				User		*user = getUser(iterator->fd);
				if (_parseInput(user) == false)
					_deleteUser(iterator->fd);
			}
			if (iterator == _pfds.end())
				break ;
		}
	}
	_closeAll();
}

void	Server::_addUser(void)
{
	int						new_fd;
	struct sockaddr_storage	their_addr;
	socklen_t				addr_size;
	struct pollfd			pfd;
	char					host[INET6_ADDRSTRLEN];
	char					serv[INET6_ADDRSTRLEN];

	addr_size = sizeof(their_addr);
	new_fd = accept(_listener, (struct sockaddr *)&their_addr, &addr_size);
	if (new_fd == -1)
		throw std::runtime_error(ERR_USER_FD);
	else
	{
		pfd.fd = new_fd;
		pfd.events = POLLOUT;
		pfd.revents = 0;

		_pfds.push_back(pfd);
		_users.insert(std::make_pair(new_fd, new User(new_fd, &their_addr)));

		getnameinfo((struct sockaddr *)&their_addr, addr_size, host, INET6_ADDRSTRLEN,\
			serv, INET6_ADDRSTRLEN, 0); // Look up the host name and service name information for a given struct sockaddr
		std::cout << GRN "🙋 New connexion from " << _host << ":" << serv;
		std::cout << " on socket " << new_fd << END << std::endl;
	}
}


void	Server::_deleteUser(int fd)
{
	User	*user = getUser(fd);
	if (fd > 0)
	{
		close(fd);
		if (_users.count(fd))
			_users.erase(fd);
	}
	size_t	index = 0;
	while (index < _pfds.size())
	{
		if (_pfds[index].fd == fd)
			break ;
		index++;
	}
	_pfds.erase(_pfds.begin() + index);
	if (user != NULL)
	{
		chan_map	channels = user->getChannels();
		for (chan_it it = channels.begin(); it != channels.end(); ++it)
		{
			it->second->delUser(user); // gets the user out of every channel
			if (it->second->getNbUsers(true) == 0)
				delChannel(it->second); // empty channels are deleted as well
		}
		user->clearAll();
		delete user;
	}
}


bool	Server::_parseInput(User *user)
{
	if (!user->setInput()) // the user has disconnected or an error occurred
		return (false); // we're going to remove the user iterator
	str_vec						commands = user->getCommands(user->getInput());
	for (str_vec::iterator it = commands.begin(); it != commands.end(); ++it)
	{
		std::string::size_type	pos = (*it).find(' '); // might be string::npos
		std::string				cmd_str = (*it).substr(0, pos);
		if (DEBUG)
		{
			std::cout << WHT "(3) Command: [" END;
			std::cout << cmd_str << WHT "]" END << std::endl;
		} else {
			std::cout << WHT "Command: [" END << cmd_str << WHT "]" END << std::endl;
		}
		if ((*it).length() != cmd_str.length()) // there are arguments but maybe just spaces
		{
			user->setRawArgs((*it).substr(pos + 1));
			user->setArgs(split_str(user->getRawArgs(0), " ", false));
		}
		try {
			for (size_t i = 0; i < cmd_str.size(); i++) // CMD to lower case
				cmd_str[i] = ascii_to_lower(cmd_str[i]);
			if (!user->isLoggedIn() && cmd_str != "pass" && cmd_str != "cap")
				return (true); // unknown user: the command is silently ignored
			CALL_MEMBER_FN(this, _commands.at(cmd_str))(user);
		} catch (const std::out_of_range &e) {
			user->reply(ERR_UNKNOWNCOMMAND(getSrv(), user->getNick(), cmd_str));
		}
	}
	return (true);
}

void	Server::_closeAll(void)
{
	for (chan_it iterator = _channels.begin(); iterator != _channels.end(); iterator++)
		delete iterator->second;
	for (pfds_it iterator = _pfds.begin(); iterator != _pfds.end(); iterator++)
		close(iterator->fd);
	for (user_it iterator = _users.begin(); iterator != _users.end(); iterator++)
		delete iterator->second;
}

//---------------------------- ACCESSORS / GETTERS ----------------------------

std::string		Server::getSrv(void) const { return _name; }
usr_map			Server::getUsers() const { return _users; }
chan_map		Server::getChannels() const { return _channels; }
std::string		Server::getPwd(void) const { return _pwd; };

User			*Server::getUser(int fd) const
{
	for (usr_map::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->first == fd)
			return it->second;
	}
	if (DEBUG)
		std::cerr << YEL ERR_USER_FD_NOT_FOUND << fd << END << std::endl;
	return NULL;
}

User			*Server::getUser(std::string nick) const
{
	for (usr_map::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second->getNick() == nick)
			return it->second;
	}
	if (DEBUG)
		std::cerr << YEL ERR_USER_NICK_NOT_FOUND << nick << END << std::endl;
	return NULL;
}

Channel			*Server::getChannel(std::string chan_name) const
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

Channel			*Server::newChan(User *user, std::string name, size_t index)
{
	str_vec			passwords;
	std::string		pw;

	if (user->getArgs().size() == 2)
	{
		passwords = split_str(user->getArgs()[1], ",", true);
		// empty passwords will be ignored, the channel won't have a password
		if ((passwords.size() >= index + 1) && !passwords[index].empty())
			pw = passwords[index];
	}
	Channel			*chan = new Channel(name, pw);
	_channels[name] = chan;
	chan->addMode('n');
	chan->addMode('t');
	std::cout << GRN "✅ Channel " << name << " created." END << std::endl;
	chan->addUser(user);
	chan->addOp(user);
	user->registerChannel(chan);
	return (chan);
}

void			Server::delChannel(Channel *chan)
{
	chan->clearAll();
	_channels.erase(chan->getName());
	delete chan;
}
