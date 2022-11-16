/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:29:39 by llalba            #+#    #+#             */
/*   Updated: 2022/11/16 18:17:57 by llalba           ###   ########.fr       */
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

// custom error messages
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
# define ERR_USER_FD							"❌ Error while accepting user fd"
# define ERR_TOO_MANY_PARAM						"❌ More than 15 command parameters found on socket "
# define RUNNING								"✅ The server is up and running"
# define ERR_TOO_LONG							"📜 Message truncated to 512 bytes on socket "
# define ERR_CHANNEL_NOT_FOUND					"❓ There isn't any channel with this name: "
# define ERR_USER_FD_NOT_FOUND					"❓ There isn't any user with this fd: "
# define ERR_USER_NICK_NOT_FOUND				"❓ There isn't any user with this nickname: "

// list of numeric replies sent by the server by alphabetical order
// cf. https://datatracker.ietf.org/doc/html/rfc2812#section-5
# define ERR_15_PARAM							"There cannot be more than 15 parameters in your command"
# define ERR_ALREADYREGISTRED(srv)				":" + srv + " 462 :You may not reregister"
# define ERR_BADCHANMASK(srv)					":" + srv + " 496 :<channel> :Bad Channel Mask"
# define ERR_BADCHANNELKEY(srv)					":" + srv + " 475 :<channel> :Cannot join channel (+k)"
# define ERR_BANNEDFROMCHAN(srv)				":" + srv + " 474 :<channel> :Cannot join channel (+b)"
# define ERR_CANNOTSENDTOCHAN(srv)				":" + srv + " 404 :<channel name> :Cannot send to channel"
# define ERR_CHANNELISFULL(srv)					":" + srv + " 471 :<channel> :Cannot join channel (+l)"
# define ERR_CHANOPRIVSNEEDED(srv)				":" + srv + " 482 :<channel> :You're not channel operator"
# define ERR_ERRONEUSNICKNAME(srv)				":" + srv + " 432 :<nick> :Erroneus nickname"
# define ERR_INVITEONLYCHAN(srv)				":" + srv + " 473 :<channel> :Cannot join channel (+i)"
# define ERR_KEYSET(srv)						":" + srv + " 467 :<channel> :Channel key already set"
# define ERR_NEEDMOREPARAMS(srv, nick, cmd)		":" + srv + " 461 :" + nick + " " + cmd + " :Not enough parameters"
# define ERR_NICKCOLLISION(srv, nick)			":" + srv + " 436 :" + nick + " :Nickname collision KILL"
# define ERR_NICKNAMEINUSE(srv)					":" + srv + " 433 :<nick> :Nickname is already in use"
# define ERR_NONICKNAMEGIVEN(srv)				":" + srv + " 431 :No nickname given"
# define ERR_NOORIGIN(srv)						":" + srv + " 409 :No origin specified"
# define ERR_NORECIPIENT(srv)					":" + srv + " 411 :No recipient given (<command>)"
# define ERR_NOSUCHCHANNEL(srv)					":" + srv + " 403 :<channel name> :No such channel"
# define ERR_NOSUCHNICK(srv)					":" + srv + " 401 :<nickname> :No such nick/channel"
# define ERR_NOSUCHSERVER(srv)					":" + srv + " 402 :<server name> :No such server"
# define ERR_NOTEXTTOSEND(srv)					":" + srv + " 412 :No text to send"
# define ERR_NOTONCHANNEL(srv)					":" + srv + " 442 :<channel> :You're not on that channel"
# define ERR_NOTOPLEVEL(srv)					":" + srv + " 413 :<mask> :No toplevel domain specified"
# define ERR_TOOMANYCHANNELS(srv)				":" + srv + " 405 :<channel name> :You have joined too many channels"
# define ERR_TOOMANYTARGETS(srv)				":" + srv + " 407 :<target> :Duplicate recipients. No message delivered"
# define ERR_UMODEUNKNOWNFLAG(srv)				":" + srv + " 501 :Unknown MODE flag"
# define ERR_UNKNOWNCOMMAND(srv, nick, cmd)		":" + srv + " 421 :" + nick + " " + cmd + " :Unknown command"
# define ERR_UNKNOWNMODE(srv)					":" + srv + " 472 :<char> :is unknown mode char to me"
# define ERR_USERONCHANNEL(srv)					":" + srv + " 443 :<user> <channel> :is already on channel"
# define ERR_USERSDONTMATCH(srv)				":" + srv + " 502 :Cant change mode for other users"
# define ERR_WILDTOPLEVEL(srv)					":" + srv + " 414 :<mask> :No toplevel domain specified"
# define RPL_AWAY(srv)							":" + srv + " 301 :<nick> :<away message>"
# define RPL_BANLIST(srv)						":" + srv + " 367 :<channel> <banid>"
# define RPL_CHANNELMODEIS(srv)					":" + srv + " 324 :<channel> <mode> <mode params>"
# define RPL_ENDOFBANLIST(srv)					":" + srv + " 368 :<channel> :End of channel ban list"
# define RPL_ENDOFNAMES(srv)					":" + srv + " 366 :<channel> :End of /NAMES list"
# define RPL_ENDOFWHO(srv)						":" + srv + " 315 :<name> :End of /WHO list"
# define RPL_INVITING(srv)						":" + srv + " 341 :<channel> <nick>"
# define RPL_LIST(srv)							":" + srv + " 322 :<channel> <# visible> :<topic>"
# define RPL_LISTEND(srv)						":" + srv + " 323 :End of /LIST"
# define RPL_LISTSTART(srv)						":" + srv + " 321 :Channel :Users  Name"
# define RPL_NAMREPLY(srv)						":" + srv + " 353 :<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
# define RPL_NOTOPIC(srv)						":" + srv + " 331 :<channel> :No topic is set"
# define RPL_TOPIC(srv)							":" + srv + " 332 :<channel> :<topic>"
# define RPL_UMODEIS(srv)						":" + srv + " 221 :<user mode string>"
# define RPL_WELCOME(srv, nick, user, host)		":" + srv + " 001 " + nick + " :Welcome to the " + srv + " network, " + nick + "[" + user + "@" + host + "]"
# define RPL_WHOREPLY(srv)						":" + srv + " 352 :<channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>"

// utils.cpp
char			ascii_to_lower(char in);
str_vec			split_str(std::string initial, char delimiter);
bool			is_valid_channel_name(std::string chan, User *user, std::string srv);

#endif
