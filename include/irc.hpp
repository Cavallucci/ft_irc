/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:29:39 by llalba            #+#    #+#             */
/*   Updated: 2022/12/20 13:23:20 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <cstdlib>
# include <iostream>
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
# include <csignal>

class User;
class Server;
class Channel;

# define CALL_MEMBER_FN(ptrToObject,ptrToMember)	((*ptrToObject).*(ptrToMember))

typedef std::vector<std::string>			str_vec;
typedef std::vector<User *>					usr_vec;
typedef std::map<std::string, Channel *>	chan_map;
typedef std::map<int, User *>				usr_map;
typedef std::vector<pollfd>::iterator		pfds_it;
typedef chan_map::iterator					chan_it;
typedef usr_map::const_iterator				user_it;

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

# define DEBUG									1
# define NON_FD									-1
# define IRC_DELIMITER							"\r\n"

// custom error messages
# define BUFFER_SIZE							512
# define DEBUG_IS_ON							"🔍 DEBUG mode is ON: have a look at the class User.hpp to better understand how the parsing works"
# define ALLOWED_CHAR_IN_NICK					"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_[]{}\\`|"
# define ERR_NB_ARG								"❌ Invalid command, usage: ./ircserv <port> <password>"
# define ERR_PORT_NB							"❌ Invalid port number, an integer is expected"
# define ERR_PORT_RANGE							"❌ Invalid port, it should be in the range of 1024 to 65535"
# define ERR_EMPTY_PWD							"❌ Error: an empty password has been provided"
# define ERR_INVALID_PWD						"❌ Error: invalid characters in your password (it cannot contain spaces)"
# define ERR_GETADDR_INFO						"❌ Error get address informations"
# define ERR_OPEN_SOCKET						"❌ Error while opening socket"
# define ERR_BIND_SOCKET						"❌ Error while binding socket: this port seems to be already taken!"
# define ERR_LIST_SOCKET						"❌ Error while listening socket"
# define ERR_USER_FD							"❌ Error while accepting user fd"
# define ERR_SEND_FD							"❌ Error while sending user fd"
# define ERR_POLL_FAILED						"❌ Error while calling poll()"
# define RUNNING								"✅ The server is running, waiting for clients..."
# define ERR_LENGTH_OR_RN						"📜 The message is longer than 512 bytes or doesn't end with '\\r\\n' on socket "
# define BYE									"👋 Someone left, socket "
# define ERR_CHANNEL_NOT_FOUND					"🤔 There isn't any channel with this name: "
# define ERR_USER_FD_NOT_FOUND					"🤔 There isn't any user with this fd: "
# define ERR_USER_NICK_NOT_FOUND				"🤔 There isn't any user with this nickname: "
# define ERR_BAN_OP								"😱 This user tried to ban a channel operator: "
# define WARNING_MAX_CHANNEL					"🔶 Warning, the maximum number of users is lower than the current number of users for this channel: "
# define ERR_RECV								"❌ Error while trying to read (recv) socket "
# define RECV_ZERO								"🚫 A socket has hang up, socket"
# define PRV_TOPIC								"Prv"
# define CHANNEL_MODE							"🔧 Channel mode "
# define USER_MODE								"🔧 User mode "

// list of official numeric replies
// cf. https://datatracker.ietf.org/doc/html/rfc2812#section-5
# define ERR_ALREADYREGISTRED(srv)				":" + srv + " 462 :You may not reregister"
# define ERR_BADCHANMASK(srv, chan)				":" + srv + " 476 :" + chan + " :Bad Channel Mask"
# define ERR_BADCHANNELKEY(srv, chan)			":" + srv + " 475 :" + chan + " :Cannot join channel (+k)"
# define ERR_BANNEDFROMCHAN(srv, chan)			":" + srv + " 474 :" + chan + " :Cannot join channel (+b)"
# define ERR_CANNOTSENDTOCHAN(srv, nick, chan)	":" + srv + " 404 " + nick + " " + chan + " :Cannot send to channel"
# define ERR_CHANNELISFULL(srv, nick, chan)		":" + srv + " 471 " + nick + " " + chan + " :Cannot join channel (+l)"
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
# define ERR_NOSUCHNICK_CHAN(srv, nick, chan)	":" + srv + " 401 " + nick + " " + chan + " :No such nick"
# define ERR_NOSUCHSERVER(srv, target)			":" + srv + " 402 :" + target + " :No such server"
# define ERR_NOTEXTTOSEND(srv)					":" + srv + " 412 :No text to send"
# define ERR_NOTONCHANNEL_41(srv, nick, chan)	":" + srv + " 441 " + nick + " " + chan + " :They aren't on that channel"
# define ERR_NOTONCHANNEL_42(srv, chan)			":" + srv + " 442 :" + chan + " :You're not on that channel"
# define ERR_PASSWDMISMATCH(srv)				":" + srv + " 464 :Password incorrect"
# define ERR_TOOMANYCHANNELS(srv, chan)			":" + srv + " 405 :" + chan + " :You have joined too many channels"
# define ERR_TOOMANYTARGETS(srv, target)		":" + srv + " 407 :" + target + " :Duplicate recipients. No message delivered"
# define ERR_UMODEUNKNOWNFLAG(srv)				":" + srv + " 501 :Unknown MODE flag"
# define ERR_UNKNOWNCOMMAND(srv, nick, cmd)		":" + srv + " 421 :" + nick + " " + cmd + " :Unknown command"
# define ERR_UNKNOWNMODE(srv, c)				":" + srv + " 472 :" + c + " :is unknown mode char to me"
# define ERR_USERONCHANNEL(srv, user, chan)		":" + srv + " 443 :" + user + " " + chan + " :is already on channel"
# define ERR_USERSDONTMATCH(srv)				":" + srv + " 502 :Cant change mode for other users"
# define ERR_WILDTOPLEVEL(srv, mask)			":" + srv + " 414 :" + mask + " :No toplevel domain specified"
# define RPL_AWAY(srv, nick, msg)				":" + srv + " 301 :" + nick + " :" + msg
# define RPL_BANLIST(srv, nick, chan, banned)	":" + srv + " 367 " + nick + " " + chan + " " + banned
# define RPL_CHANNELMODEIS(srv, nick, ch, mo)	":" + srv + " 324 " + nick + " " + ch + " " + mo
# define RPL_ENDOFBANLIST(srv, nick, chan)		":" + srv + " 368 " + nick + " " + chan + " :End of channel ban list"
# define RPL_ENDOFNAMES(srv, nick, chan)		":" + srv + " 366 " + nick + " " + chan + " :End of NAMES list"
# define RPL_ENDOFWHO(srv, nick, channel)		":" + srv + " 315 "+ nick + " " + channel + " :End of WHO list"
# define RPL_INVITING(srv, nick, invited, chan)	":" + srv + " 341 " + nick + " " + invited + " " + chan
# define RPL_LIST(srv, nick, chan, nb, topic)	":" + srv + " 322 " + nick + " " + chan + " " + nb + " :" + topic
# define RPL_LISTEND(srv, nick)					":" + srv + " 323 " + nick + " :End of LIST"
# define RPL_LISTSTART(srv)						":" + srv + " 321 Channel :Users  Name"
# define RPL_NAMREPLY(srv, nick, m, c, nicks)	":" + srv + " 353 " + nick + " " + m + " " + c + " :" + nicks
# define RPL_NOTOPIC(srv, nick, chan)			":" + srv + " 331 :" + nick + " " + chan + " :No topic is set"
# define RPL_TOPIC(srv, nick, chan, topic)		":" + srv + " 332 " + nick + " " + chan + " :" + topic
# define RPL_TOPICWHOTIME(srv, nick, chan, ctx)	":" + srv + " 333 " + nick + " " + chan + " " + ctx
# define RPL_UMODEIS(srv, mode)					":" + srv + " 221 :" + mode
# define RPL_WELCOME(s, n, u, h)				":"+s+" 001 "+n+" :Welcome to the "+s+" network, "+n+"["+u+"@"+h+"]"
# define RPL_WHOREPLY(s, mon_nick, channel, u, h, n, op, r)		":"+s+" 352 "+mon_nick+" "+channel+" "+u+" "+h+" "+s+" "+n+" H"+op+" :0 "+r

# define ERR_MSG_TOO_LONG(srv)					":" + srv + " 412 :Message too long (max 512 bytes)"
# define ERR_TOOMANYMODES(srv, nick, cmd)		":" + srv + " 461 :" + nick + " " + cmd + " :Cannot update the mode with both +l and +b/+m/+o in one command"
# define ERR_INVALID_MAX(srv, nick, cmd)		":" + srv + " 461 :" + nick + " " + cmd + " :The maximum number of guests must be positive"
# define RPL_JOIN(nick, channel)				":" + nick + "@IRC JOIN " + channel
# define RPL_MODE(nick, target, mode)			":" + nick + "@IRC MODE " + target + " " + mode
# define RPL_TOPIC_SET(nick, channel, topic)	":" + nick + "@IRC TOPIC " + channel + " :" + topic
# define RPL_INVITE(nick, by, channel)			":" + nick + "@IRC INVITE " + by + " " + channel
# define RPL_KICK(by, channel, kicked)			":" + by + "@IRC KICK " + channel + " " + kicked + " " + by
# define RPL_QUIT(nick, msg)					":" + nick + "@IRC QUIT " + msg
# define RPL_PART(nick, channel)				":" + nick + "@IRC PART " + channel
# define RPL_NICK(nick)							":" + nick + "@IRC NICK " + nick
# define RPL_NOTICE(nick, target, txt)			":" + nick + "@IRC NOTICE " + target + " :" + txt
# define RPL_MSG(nick, target, txt)				":" + nick + "@IRC PRIVMSG " + target + " :" + txt
# define RPL_PING(nick, server)					":" + nick + "@IRC PONG " + server

// utils.cpp
char			ascii_to_lower(char in);
str_vec			split_str(std::string initial, std::string separator, bool with_empty);
bool			is_valid_channel_name(std::string chan, User *user, std::string srv);
std::string		size_t_to_str(size_t	nb);
size_t			str_to_size_t(std::string my_str);

#endif
