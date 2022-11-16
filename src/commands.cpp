/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 13:06:04 by llalba            #+#    #+#             */
/*   Updated: 2022/11/16 16:13:27 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::_initCommands(void)
{
	// by alphabetical order
	_commands["invite"] = &Server::_inviteCmd;
	_commands["join"] = &Server::_joinCmd;
	_commands["kick"] = &Server::_kickCmd;
	_commands["list"] = &Server::_listCmd;
	_commands["mode"] = &Server::_modeCmd;
	_commands["msg"] = &Server::_msgCmd;
	_commands["names"] = &Server::_namesCmd;
	_commands["nick"] = &Server::_nickCmd;
	_commands["notice"] = &Server::_noticeCmd;
	_commands["part"] = &Server::_partCmd;
	_commands["pass"] = &Server::_passCmd;
	_commands["ping"] = &Server::_pingCmd;
	_commands["quit"] = &Server::_quitCmd;
	_commands["topic"] = &Server::_topicCmd;
	_commands["user"] = &Server::_userCmd;
	_commands["who"] = &Server::_whoCmd;
}

//------------------- SERVER COMMANDS BY ALPHABETICAL ORDER -------------------

/*
INVITE command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.7
*/
void	Server::_inviteCmd(User* user)
{
	(void)user->getArgs(); // TODO
	// ERR_NEEDMOREPARAMS
	// ERR_NOSUCHNICK
	// ERR_NOTONCHANNEL
	// ERR_USERONCHANNEL
	// ERR_CHANOPRIVSNEEDED
	// RPL_INVITING
	// RPL_AWAY
}

/*
JOIN command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.1
*/
void	Server::_joinCmd(User* user)
{
	// TODO verifier que l'utilisateur a deja ete accueilli
	if (user->getArgs().size() < 1)
		return user->reply(ERR_NEEDMOREPARAMS(user->getServer(), \
			user->getNick(), "JOIN"));
	// https://irssi.org/documentation/help/join/

	// ERR_NEEDMOREPARAMS
	// ERR_BANNEDFROMCHAN
	// ERR_INVITEONLYCHAN
	// ERR_BADCHANNELKEY
	// ERR_CHANNELISFULL
	// ERR_BADCHANMASK
	// ERR_NOSUCHCHANNEL
	// ERR_TOOMANYCHANNELS
	// RPL_TOPIC
}

/*
KICK command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.8
*/
void	Server::_kickCmd(User* user)
{
	(void)user->getArgs(); // TODO
}


/*
LIST command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.6
*/
void	Server::_listCmd(User* user)
{
	(void)user->getArgs(); // TODO
}


/*
MODE command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.3
*/
void	Server::_modeCmd(User* user)
{
	(void)user->getArgs(); // TODO
}


/*
PRIVMSG command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.4.1
*/
void	Server::_msgCmd(User* user)
{
	(void)user->getArgs(); // TODO
}


/*
NAMES command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.5
*/
void	Server::_namesCmd(User* user)
{
	(void)user->getArgs(); // TODO
}


/*
NICK command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.1.2
*/
void	Server::_nickCmd(User* user)
{
	if (user->getArgs().size() < 1)
		return user->reply(ERR_NO_NICK(user->getServer()));
	std::string		nick = user->getArgs()[0];
	if (fdAlreadyIn(user->getFd()))
		return user->reply(ERR_NICK_USED(getName(), user->getNick()));
	user->setNick(nick);
	// TODO to check: user logged in, username set
	if (!user->hasBeenWelcomed())
		user->welcome(false);
}


/*
NOTICE command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.4.2.
*/
void	Server::_noticeCmd(User* user)
{
	(void)user->getArgs(); // TODO
}


/*
PART command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.2
*/
void	Server::_partCmd(User* user)
{
	(void)user->getArgs(); // TODO
}


/*
PASS command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.1.1
*/
void	Server::_passCmd(User* user)
{
	(void)user->getArgs(); // TODO
}


/*
PING command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.6.2
*/
void	Server::_pingCmd(User* user)
{
	(void)user->getArgs(); // TODO
}


/*
QUIT command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.1.6
*/
void	Server::_quitCmd(User* user)
{
	(void)user->getArgs(); // TODO
}


/*
TOPIC command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.4
*/
void	Server::_topicCmd(User* user)
{
	(void)user->getArgs(); // TODO
}


/*
USER command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.1.3
*/
void	Server::_userCmd(User* user)
{
	(void)user->getArgs(); // TODO
}


/*
WHO command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.5.1
*/
void	Server::_whoCmd(User* user)
{
	(void)user->getArgs(); // TODO
}

/*
--------------------------- IRSSI official commands ---------------------------
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
clear			-> scrolls up the text in the window and fills it with blank lines
completion		-> replaces or completes words or letters
connect			-> opens a new connexion to the specified network/server
ctcp			-> sends a CTCP request towards the given target nickname or channel
cycle			-> leaves and rejoins a channel
dcc				-> initiates direct CTC chat connections and file transfers
dehilight		-> removes the specified highlight from configuration
deop			-> removes the channel operator privileges from the given nicknames
devoice			-> removes the channel voice privilege from the given nicknames
die				-> terminates the IRC server (reserved for IRC operators)
disconnect		-> disconnect from an IRC server
echo			-> displays the given text
eval			-> evaluates the given commands and executes them
exec			-> executes the specified command in the background
flushbuffer		-> forces an immediate flush of the buffer
format			-> reconfigures the way messages are displayed
hash			-> (deprecated)
help			-> displays the documentation for the given command
hilight			-> highlight the keyword or pattern provided
ignore			-> ignores nicknames or text that matches a pattern
info			-> displays information about the IRC server software
invite			-> 🔵 invites the specified nick to a channel
ircnet			-> similar to `network`
ison			-> displays whether the specified nicknames are online
join			-> 🔴🔵 joins the given channels
kick			-> 🔵 removes the given nicknames from the specified channel
kickban			-> removes & bans the given nicknames from the specified channel
kill			-> terminates a nickname’s connection from the network
knock			-> sends an invitation request to the channel ops of the target
knockout		-> removes & then bans the given nicknames from the channel,
				the ban will be automatically lifted after the specified time.
lastlog			-> searches the active window for a pattern
layout			-> saves the layout of your window configuration
links			-> displays the links between an IRC server and its connections
list			-> 🔵 displays the channel names that match your request
load			-> loads a plugin
log				-> opens a log file and stores the targets messages into it
lusers			-> displays the user statistics of the active or remote server
map				-> displays the network map of an IRC network
me				-> sends an action emote to the active nickname or channel
mircdcc			-> enables mIRC compatibility mode
mode			-> 🔵 modifies the user or channel modes
motd			-> displays the welcome message of an IRC server
msg				-> 🔴🔵 sends a message to a nickname or channel
names			-> 🔵 displays the users who are in a channel
nctcp			-> sends a CTCP reply to a nickname or channel
netsplit		-> displays some information about users who are currently lost
				in one or more net splits
network			-> displays/adds/modifies/removes the configuration of networks
nick			-> 🔴🔵 changes your nickname on the active server
notice			-> 🔵 sends a notice to the target nickname or channel, often
				used in automated bots or scripts
notify			-> notifies you when a nickname/host comes online or offline
op				-> grants the channel operator privileges to the given nicknames
oper			-> grants you the IRC operator status
part			-> 🔵 leaves the given channels
ping			-> 🔵 sends a CTCP PING request to a nickname or a channel
query			-> starts a private conversation with a nickname
quit			-> 🔵 terminates the application and advertises the given
				message on all the networks you are connected to.
quote			-> sends raw data to the server without any parsing
rawlog			-> saves all the raw data that is received from and transmitted
				to the active server into a log file
recode			-> recodes the data transmitted to and received from nicknames
				and channels into a specific charset.
reconnect		-> disconnect and reconnect from a network
rehash			-> reloads the configuration of the IRC server
reload			-> reloads the Irssi configuration file
restart			-> restarts the active IRC server (restricted to admin)
rmreconns		-> removes all active and pending reconnections.
rmrejoins		-> removes all active and pending join requests
save			-> saves the configuration file
sconnect		-> connects a server to the IRC network (restricted to IRC ops)
script			-> interact with the Perl engine to execute scripts
scrollback		-> manipulates the text to go to a to the given line number
server			-> displays/adds/modifies/removes the network configuration
servlist		-> list the network services currently present on the network
set				-> modifies the value of a setting
silence			-> manages the server side ignore list; users or hostnames that
				match an entry on the list are not able to send you any
				messages or invites
squery			-> sends a query to the specified service
squit			-> disconnects a server from the IRC network (restricted to ops)
stats			-> displays statistics from the IRC server
statusbar		-> allows adjustment of the attributes and items of a statusbar
time			-> displays the local time of the server
toggle			-> modifies a setting to its counter value
topic			-> 🔵 displays or modifies the topic of a channel
trace			-> displays the list of servers that connects you to a server
ts				-> displays a list of the channels you are on and their topics
unalias			-> removes an alias
unban			-> a channel & the nicknames, hostnames to unban
unignore		-> removes an entry from the ignore list
unload			-> removes a module from the memory
unnotify		-> removes an entry from the notify list
unquery			-> closes a query window for a nickname
unsilence		-> removes an entry from the silence list
upgrade			-> upgrades to a new version
uptime			-> displays how long Irssi has been running
userhost		-> displays the user@host for the given nicknames
ver				-> queries a remote client about the version of the client they
				are using
version			-> displays the version and compatibility parameters of the
				given server,
voice			-> grants the channel voice privileges to the given nicknames
wait			-> waits the specified amount of ms before sending the next command
wall			-> sends a message to all channel operators
wallops			-> sends a network wide message to all the users in the 'w' usermod
who				-> 🔵 displays information about users in the specified channel
whois			-> displays information about users in the specified channels
whowas			-> displays historical user information
window			-> manipulates the window layout and positioning attributes

----------------------------- other IRC commands ------------------------------

pass			-> 🔴🔵
user			-> 🔴🔵
*/
