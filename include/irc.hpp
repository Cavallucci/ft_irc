/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:29:39 by llalba            #+#    #+#             */
/*   Updated: 2022/11/21 09:40:14 by llalba           ###   ########.fr       */
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

// list of official numeric replies sent by the server by alphabetical order
// cf. https://datatracker.ietf.org/doc/html/rfc2812#section-5
# define ERR_ALREADYREGISTRED(srv)				":" + srv + " 462 :You may not reregister"
# define ERR_BADCHANMASK(srv, chan)				":" + srv + " 476 :" + chan + " :Bad Channel Mask"
# define ERR_BADCHANNELKEY(srv, chan)			":" + srv + " 475 :" + chan + " :Cannot join channel (+k)"
# define ERR_BANNEDFROMCHAN(srv, chan)			":" + srv + " 474 :" + chan + " :Cannot join channel (+b)"
# define ERR_CANNOTSENDTOCHAN(srv, chan)		":" + srv + " 404 :" + chan + " :Cannot send to channel"
# define ERR_CHANNELISFULL(srv, chan)			":" + srv + " 471 :" + chan + " :Cannot join channel (+l)"
# define ERR_CHANOPRIVSNEEDED(srv, chan)		":" + srv + " 482 :" + chan + " :You're not channel operator"
# define ERR_ERRONEUSNICKNAME(srv, nick)		":" + srv + " 432 :" + nick + " :Erroneus nickname"
# define ERR_INVITEONLYCHAN(srv, chan)			":" + srv + " 473 :" + chan + " :Cannot join channel (+i)"
# define ERR_KEYSET(srv, chan)					":" + srv + " 467 :" + chan + " :Channel key already set"
# define ERR_NEEDMOREPARAMS(srv, nick, cmd)		":" + srv + " 461 :" + nick + " " + cmd + " :Not enough parameters"
# define ERR_NICKCOLLISION(srv, nick)			":" + srv + " 436 :" + nick + " :Nickname collision KILL"
# define ERR_NICKNAMEINUSE(srv, nick)			":" + srv + " 433 :" + nick + " :Nickname is already in use"
# define ERR_NONICKNAMEGIVEN(srv)				":" + srv + " 431 :No nickname given"
# define ERR_NOORIGIN(srv)						":" + srv + " 409 :No origin specified"
# define ERR_NORECIPIENT(srv, cmd)				":" + srv + " 411 :No recipient given (" + cmd + ")"
# define ERR_NOSUCHCHANNEL(srv, chan)			":" + srv + " 403 :" + chan + " :No such channel"
# define ERR_NOSUCHNICK(srv, nick)				":" + srv + " 401 :" + nick + " :No such nick/channel"
# define ERR_NOSUCHSERVER(srv, target)			":" + srv + " 402 :" + target + " :No such server"
# define ERR_NOTEXTTOSEND(srv)					":" + srv + " 412 :No text to send"
# define ERR_NOTONCHANNEL(srv, chan)			":" + srv + " 442 :" + chan + " :You're not on that channel"
# define ERR_NOTOPLEVEL(srv, mask)				":" + srv + " 413 :" + mask + " :No toplevel domain specified"
# define ERR_TOOMANYCHANNELS(srv, chan)			":" + srv + " 405 :" + chan + " :You have joined too many channels"
# define ERR_TOOMANYTARGETS(srv, target)		":" + srv + " 407 :" + target + " :Duplicate recipients. No message delivered"
# define ERR_UMODEUNKNOWNFLAG(srv)				":" + srv + " 501 :Unknown MODE flag"
# define ERR_UNKNOWNCOMMAND(srv, nick, cmd)		":" + srv + " 421 :" + nick + " " + cmd + " :Unknown command"
# define ERR_UNKNOWNMODE(srv, c)				":" + srv + " 472 :" + c + " :is unknown mode char to me"
# define ERR_USERONCHANNEL(srv, user, chan)		":" + srv + " 443 :" + user + " " + chan + " :is already on channel"
# define ERR_USERSDONTMATCH(srv)				":" + srv + " 502 :Cant change mode for other users"
# define ERR_WILDTOPLEVEL(srv, mask)			":" + srv + " 414 :" + mask + " :No toplevel domain specified"
# define RPL_AWAY(srv, nick, msg)				":" + srv + " 301 :" + nick + " :" + msg
# define RPL_BANLIST(srv, chan, ban_id)			":" + srv + " 367 :" + chan + " " + ban_id
# define RPL_CHANNELMODEIS(srv, chan, mod, par)	":" + srv + " 324 :" + chan + " " + mod + " " + par
# define RPL_ENDOFBANLIST(srv, chan)			":" + srv + " 368 :" + chan + " :End of channel ban list"
# define RPL_ENDOFNAMES(srv, chan)				":" + srv + " 366 :" + chan + " :End of /NAMES list"
# define RPL_ENDOFWHO(srv, name)				":" + srv + " 315 :" + name + " :End of /WHO list"
# define RPL_INVITING(srv, chan, nick)			":" + srv + " 341 :" + chan + " " + nick
# define RPL_LIST(srv, chan)					":" + srv + " 322 :" + chan + " <# visible> :<topic>"
# define RPL_LISTEND(srv)						":" + srv + " 323 :End of /LIST"
# define RPL_LISTSTART(srv)						":" + srv + " 321 :Channel :Users  Name"
# define RPL_NAMREPLY(srv, chan)				":" + srv + " 353 :" + chan + " :[[@|+]<nick> [[@|+]<nick> [...]]]"
# define RPL_NOTOPIC(srv, chan)					":" + srv + " 331 :" + chan + " :No topic is set"
# define RPL_TOPIC(srv, chan, topic)			":" + srv + " 332 :" + chan + " :" + topic
# define RPL_TOPICWHOTIME(srv, chan, context)	":" + srv + " 333 :" + chan + " " + context
# define RPL_UMODEIS(srv, mode)					":" + srv + " 221 :" + mode
# define RPL_WELCOME(s, n, u, h)				":"+s+" 001 "+n+" :Welcome to the "+s+" network, "+n+"["+u+"@"+h+"]"
# define RPL_WHOREPLY(s, c, u, h, n, ho, r)		":"+s+" 352 :"+c+" "+u+" "+h+" "+s+" "+n+" <H|G>[*][@|+] :"+ho+" "+r

// List of custom (non official) replies by alphabetical order
# define ERR_15_PARAM							"There cannot be more than 15 parameters in your command"
# define RPL_JOIN(nick, channel)				":" + nick + "@IRC JOIN :" + channel

// utils.cpp
char			ascii_to_lower(char in);
str_vec			split_str(std::string initial, char delimiter);
bool			is_valid_channel_name(std::string chan, User *user, std::string srv);

#endif
