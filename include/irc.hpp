/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:29:39 by llalba            #+#    #+#             */
/*   Updated: 2022/11/14 21:41:22 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <cstdlib>			// EXIT_SUCCESS & EXIT_FAILURE
# include <iostream>		// std::cout
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <cstdlib>
# include <sstream>
# include <poll.h>
# include <cstdio>
# include <cstring>
# include <unistd.h>
# include <map>
# include <vector>

class User;
class Server;
class Channel;

# define CALL_MEMBER_FN(ptrToObject,ptrToMember)	((*ptrToObject).*(ptrToMember))

typedef std::vector<std::string>			str_vec;
typedef std::map<std::string, Channel *>	chan_map;
typedef std::map<int, User *>				usr_map;

# include "User.hpp"
# include "Server.hpp"
# include "Channel.hpp"

# define RED									"\033[1;31m"
# define GRN									"\033[1;32m"
# define YEL									"\033[1;33m"
# define BLU									"\033[1;34m"
# define MAG									"\033[1;35m"
# define CYN									"\033[1;36m"
# define WHT									"\033[1;37m"
# define END									"\033[0m"


# define ERR_NB_ARG								"❌ Invalid command, usage: ./ircserv <port> <password>"
# define ERR_PORT_NB							"❌ Invalid port number, an integer is expected"
# define ERR_PORT_RANGE							"❌ Invalid port, it should be in the range of 0 to 65535"
# define ERR_EMPTY_PWD							"❌ Error: an empty password has been provided"
# define ERR_GETADDR_INFO						"❌ Error get address informations"
# define ERR_OPEN_SOCKET						"❌ Error while opening socket"
# define ERR_RECV								"❌ Error while trying to read (recv) socket "
# define ERR_BIND_SOCKET						"❌ Error while binding socket"
# define ERR_LIST_SOCKET						"❌ Error while listening socket"
# define ERR_TOO_LONG							"❌ Message truncated to 512 bytes on socket "
# define ERR_CHANNEL_NOT_FOUND					"❌ There isn't any channel with this name!"
# define ERR_USER_NOT_FOUND						"❌ There isn't any user with this nickname!"
# define ERR_TOO_MANY_PARAM						"❌ More than 15 command parameters found on socket "
# define RUNNING								"✅ The server is up and running"
# define RECV_ZERO								"🤔 Command not ending with '\\r\\n' found on socket "
# define BUFFER_SIZE							2048

// list of numeric replies sent by the server
// cf. https://datatracker.ietf.org/doc/html/rfc2812#section-5
# define ERR_MORE_15_PARAM						"There cannot be more than 15 parameters in your command"
# define ERR_CMD_NOT_FOUND(srv, nick, cmd)		":" + srv + " 421 :" + nick + " " + cmd + " :Unknown command"
# define ERR_NO_NICK(srv)						":" + srv + " 431 :No nickname given"
# define ERR_NICK_USED(srv, nick)				":" + srv + " 436 :" + nick + " :Nickname collision KILL"

# define RPL_WELCOME(srv, nick, user, host)		":" + srv + " 001 " + nick + " :Welcome to the " + srv + " network, " + nick + "[" + user + "@" + host + "]"

#endif
