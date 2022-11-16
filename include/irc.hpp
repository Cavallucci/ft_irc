/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:29:39 by llalba            #+#    #+#             */
/*   Updated: 2022/11/16 16:54:05 by llalba           ###   ########.fr       */
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


# define BUFFER_SIZE							2048
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
# define ERR_USER_FD                            "❌ Error while accepting user fd"
# define RUNNING								"✅ The server is up and running"

// list of numeric replies sent by the server by alphabetical order
// cf. https://datatracker.ietf.org/doc/html/rfc2812#section-5
# define ERR_15_PARAM							"There cannot be more than 15 parameters in your command"
# define ERR_ALREADYREGISTRED					"462 :You may not reregister"
# define ERR_BADCHANMASK						"" // no longer in use?
# define ERR_BADCHANNELKEY						"475 :<channel> :Cannot join channel (+k)"
# define ERR_BANNEDFROMCHAN						"474 :<channel> :Cannot join channel (+b)"
# define ERR_CANNOTSENDTOCHAN					"404 :<channel name> :Cannot send to channel"
# define ERR_CHANNELISFULL						"471 :<channel> :Cannot join channel (+l)"
# define ERR_CHANOPRIVSNEEDED					"482 :<channel> :You're not channel operator"
# define ERR_ERRONEUSNICKNAME					"432 :<nick> :Erroneus nickname"
# define ERR_INVITEONLYCHAN						"473 :<channel> :Cannot join channel (+i)"
# define ERR_KEYSET								"467 :<channel> :Channel key already set"
# define ERR_NEEDMOREPARAMS(srv, nick, cmd)		":" + srv + " 461 :" + nick + " " + cmd + " :Not enough parameters"
# define ERR_NICKCOLLISION(srv, nick)			":" + srv + " 436 :" + nick + " :Nickname collision KILL"
# define ERR_NICKNAMEINUSE						"433 :<nick> :Nickname is already in use"
# define ERR_NONICKNAMEGIVEN(srv)				":" + srv + " 431 :No nickname given"
# define ERR_NOORIGIN							"409 :No origin specified"
# define ERR_NORECIPIENT						"411 :No recipient given (<command>)"
# define ERR_NOSUCHCHANNEL						"403 :<channel name> :No such channel"
# define ERR_NOSUCHNICK							"401 :<nickname> :No such nick/channel"
# define ERR_NOSUCHSERVER						"402 :<server name> :No such server"
# define ERR_NOTEXTTOSEND						"412 :No text to send"
# define ERR_NOTONCHANNEL						"442 :<channel> :You're not on that channel"
# define ERR_NOTOPLEVEL							"413 :<mask> :No toplevel domain specified"
# define ERR_TOOMANYCHANNELS					"405 :<channel name> :You have joined too many channels"
# define ERR_TOOMANYTARGETS						"407 :<target> :Duplicate recipients. No message delivered"
# define ERR_UMODEUNKNOWNFLAG					"501 :Unknown MODE flag"
# define ERR_UNKNOWNCOMMAND(srv, nick, cmd)		":" + srv + " 421 :" + nick + " " + cmd + " :Unknown command"
# define ERR_UNKNOWNMODE						"472 :<char> :is unknown mode char to me"
# define ERR_USERONCHANNEL						"443 :<user> <channel> :is already on channel"
# define ERR_USERSDONTMATCH						"502 :Cant change mode for other users"
# define ERR_WILDTOPLEVEL						"414 :<mask> :No toplevel domain specified"
# define RPL_AWAY								"301 :<nick> :<away message>"
# define RPL_BANLIST							"367 :<channel> <banid>"
# define RPL_CHANNELMODEIS						"324 :<channel> <mode> <mode params>"
# define RPL_ENDOFBANLIST						"368 :<channel> :End of channel ban list"
# define RPL_ENDOFNAMES							"366 :<channel> :End of /NAMES list"
# define RPL_ENDOFWHO							"315 :<name> :End of /WHO list"
# define RPL_INVITING							"341 :<channel> <nick>"
# define RPL_LIST								"322 :<channel> <# visible> :<topic>"
# define RPL_LISTEND							"323 :End of /LIST"
# define RPL_LISTSTART							"321 :Channel :Users  Name"
# define RPL_NAMREPLY							"353 :<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
# define RPL_NOTOPIC							"331 :<channel> :No topic is set"
# define RPL_TOPIC								"332 :<channel> :<topic>"
# define RPL_UMODEIS							"221 :<user mode string>"
# define RPL_WELCOME(srv, nick, user, host)		":" + srv + " 001 " + nick + " :Welcome to the " + srv + " network, " + nick + "[" + user + "@" + host + "]"
# define RPL_WHOREPLY							"352 :<channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>"

#endif
