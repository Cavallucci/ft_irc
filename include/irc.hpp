/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:29:39 by llalba            #+#    #+#             */
/*   Updated: 2022/11/11 17:52:52 by llalba           ###   ########.fr       */
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
# include <poll.h>
# include <cstdio>
# include <cstring>
# include <unistd.h>
# include <map>

class User;
class Server;
class Channel;

# define CALL_MEMBER_FN(ptrToObject,ptrToMember)	((*ptrToObject).*(ptrToMember))

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
# define RUNNING								"✅ The server is up and running"
# define RECV_ZERO								"🤔 Command not ending with '\\r\\n' found in socket "
# define BUFFER_SIZE							2048

// list of numeric replies sent by the server
// cf. https://datatracker.ietf.org/doc/html/rfc2812#section-5
# define ERR_CMD_NOT_FOUND(nick, cmd)			":IRC 421 :" nick " " cmd " :Unknown command"

#endif
