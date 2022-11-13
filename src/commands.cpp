/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 13:06:04 by llalba            #+#    #+#             */
/*   Updated: 2022/11/13 17:12:55 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::_initCommands(void)
{
	// by alphabetical order
	_commands["invite"] = &Server::inviteCmd;
	_commands["join"] = &Server::joinCmd;
	_commands["kick"] = &Server::kickCmd;
	_commands["list"] = &Server::listCmd;
	_commands["mode"] = &Server::modeCmd;
	_commands["msg"] = &Server::msgCmd;
	_commands["names"] = &Server::namesCmd;
	_commands["nick"] = &Server::nickCmd;
	_commands["notice"] = &Server::noticeCmd;
	_commands["part"] = &Server::partCmd;
	_commands["pass"] = &Server::passCmd;
	_commands["ping"] = &Server::pingCmd;
	_commands["quit"] = &Server::quitCmd;
	_commands["topic"] = &Server::topicCmd;
	_commands["user"] = &Server::userCmd;
	_commands["who"] = &Server::whoCmd;
}

//------------------- SERVER COMMANDS BY ALPHABETICAL ORDER -------------------

void	Server::inviteCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

void	Server::joinCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

void	Server::kickCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

void	Server::listCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

void	Server::modeCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

void	Server::msgCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

void	Server::namesCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

void	Server::nickCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

void	Server::noticeCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

void	Server::partCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

void	Server::passCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

void	Server::pingCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

void	Server::quitCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

void	Server::topicCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

void	Server::userCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

void	Server::whoCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

/*------------------------------- IRSSI commands ------------------------------
COLOR CODE		MEANING
✅				done
🔴				mandatory
🔵				done by other students

accept			-> specifies who you want to receive private messages
action			-> sends an action emote to a nickname or a channel
admin			-> displays the administrative details of a server
alias			-> creates or updates an alias & the command to execute
away			-> marks yourself as ‘away’
ban				-> adds one or more bans to a channel
beep			-> outputs the bell-character
bind			-> adds or removes a binding (name & command to perform)
cat				-> displays the contents of the specified file
cd				-> changes the current active directory
channel			-> adds, removes or displays the configuration of channels
clear
completion
connect
ctcp
cycle
dcc
dehilight
deop
devoice
die
disconnect
echo
eval
exec
flushbuffer
foreach
format
hash
help
hilight
ignore
info
invite			-> 🔵
ircnet
ison
join			-> 🔴🔵
kick			-> 🔵
kickban
kill
knock
knockout
lastlog
layout
links
list			-> 🔵
load
log
lusers
map
me
mircdcc
mode			-> 🔵
motd
msg				-> 🔴🔵
names			-> 🔵
nctcp
netsplit
network
nick			-> 🔴🔵
notice			-> 🔵
notify
op
oper
part			-> 🔵
pass			-> 🔴🔵 NON IRSSI COMMAND!
ping			-> 🔵 Sends a CTCP PING request to a nickname or a channel
query
quit			-> 🔵
quote
rawlog
recode
reconnect
redraw
rehash
reload
resize
restart
rmreconns
rmrejoins
save
sconnect
script
scrollback
server
servlist
set
sethost
silence
squery
squit
stats
statusbar
time
toggle
topic			-> 🔵
trace
ts
unalias
unban
unignore
unload
unnotify
unquery
unsilence
upgrade
uptime
user			-> 🔴🔵 NON IRSSI COMMAND!
userhost
ver
version
voice
wait
wall
wallops
who				-> 🔵 displays information about users in the specified channel
whois
whowas
window
*/
