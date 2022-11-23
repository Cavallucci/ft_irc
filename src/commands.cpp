/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 13:06:04 by llalba            #+#    #+#             */
/*   Updated: 2022/11/23 18:22:13 by llalba           ###   ########.fr       */
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
	_commands["privmsg"] = &Server::_privMsgCmd;
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

// TODO pour toutes les cmd: vérifier quel est le comportement attendu quand il y a trop d'arguments

/*
INVITE command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.7
*/
void	Server::_inviteCmd(User *user)
{
	if (!user->hasBeenWelcomed())
		return ;
	// TODO tester comportement attendu si plus de 2 arguments sont fournis
	if (user->getArgs().size() < 2)
		user->reply(ERR_NEEDMOREPARAMS(getSrv(), user->getNick(), "INVITE"));
	std::string		guest_nick = user->getArgs()[0];
	std::string		target_chan = user->getArgs()[1];
	User			*guest = getUser(guest_nick);
	Channel			*channel = getChannel(target_chan);
	if (guest == NULL)
		return (user->reply(ERR_NOSUCHNICK(getSrv(), user->getNick())));
	if (channel)
	{
		if (!channel->isIn(user->getFd()))
			return (user->reply(ERR_NOTONCHANNEL(getSrv(), target_chan)));
		if (channel->isIn(guest->getFd()))
			return (user->reply(ERR_USERONCHANNEL(getSrv(), guest->getUser(), target_chan)));
		if (!channel->isOp(user->getFd()))
			return (user->reply(ERR_CHANOPRIVSNEEDED(getSrv(), target_chan)));
		user->reply(RPL_INVITING(getSrv(), target_chan, guest_nick));
		guest->reply(RPL_INVITE(guest_nick, user->getNick(), target_chan));
		channel->addUser(guest);
	}
	// RPL_AWAY
}

/*
JOIN command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.1
*/
void	Server::_joinCmd(User *user)
{
	if (!user->hasBeenWelcomed())
		return ;
	if (user->getArgs().size() < 1)
		return user->reply(ERR_NEEDMOREPARAMS(getSrv(), user->getNick(), "JOIN"));
	str_vec		chans = split_str(user->getArgs()[0], ",", true);
	size_t		nth = 0;
	for (str_vec::iterator name = chans.begin(); name != chans.end(); ++name, ++nth)
	{
		if (!is_valid_channel_name(*name, user, getSrv()))
			continue ; // TODO vérifier que pas de message d'erreur, simplement ignoré
		Channel		*chan = getChannel(*name);
		if (chan == NULL) { // channel has to be created
			chan = newChan(user, *name, nth);
			chan->broadcast(RPL_JOIN(user->getNick(), *name));
			// TODO chan->rpl_namreply(user, true);
		} else if (chan->canJoin(getSrv(), user, nth)) { // channel already exists
			if (chan->isInvited(user->getFd()))
				chan->rmInvite(user);
			chan->addUser(user);
			chan->broadcast(RPL_JOIN(user->getNick(), *name));
			if (chan->getTopic() != "")
			{
				user->reply(RPL_TOPIC(getSrv(), *name, chan->getTopic()));
				user->reply(RPL_TOPICWHOTIME(getSrv(), *name, chan->getTopicCtxt()));
			}
			// TODO channel->rpl_namreply(user, true);
		}
	}
	// ERR_NOSUCHCHANNEL
	// ERR_TOOMANYCHANNELS
}

/*
KICK command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.8
*/
void	Server::_kickCmd(User *user)
{
	if (!user->hasBeenWelcomed())
		return ;
	if (user->getArgs().size() < 2)
		return (user->reply(ERR_NEEDMOREPARAMS(getSrv(), user->getNick(), "KICK")));
	std::string		channel_name = user->getArgs()[0];
	std::string		target_user = user->getArgs()[1];
	Channel			*channel = getChannel(channel_name);
	User			*target = getUser(target_user);
	// TODO verifier l'ordre de priorité des erreurs
	// TODO on peut aussi gérer [KICK channel1,channel2 user1,user2]
	if (channel == NULL)
		return (user->reply(ERR_NOSUCHCHANNEL(getSrv(), channel_name)));
	// TODO tester qu'on renvoie bien ERR_NOTONCHANNEL si l'utilisateur n'existe pas du tout
	if (target == NULL || !channel->isIn(user->getFd()) || !channel->isIn(target->getFd()))
		return (user->reply(ERR_NOTONCHANNEL(getSrv(), channel_name)));
	if (!channel->isOp(user->getFd()))
		return (user->reply(ERR_CHANOPRIVSNEEDED(getSrv(), channel_name)));
	// TODO verifier que target recoit bien le message de broadcast concernant son propre KICK
	channel->broadcast(RPL_KICK(user->getNick(), target_user, channel_name));
	target->rmChannel(channel_name);
	// TODO et si target est moderator ? operator ? deja ban ? invite ? on le retire de toutes les listes ?
	channel->delUser(target);
	if (!channel->getNbUsers())
		delChannel(channel);
	// TODO possibilite d'ajouter un commentaire au KICK
	// ERR_BADCHANMASK
}


/*
LIST command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.6
*/
void	Server::_listCmd(User *user)
{

	(void)user->getArgs(); // FIXME
	// ERR_NOSUCHSERVER
	// RPL_LISTSTART
	// RPL_LIST
	// RPL_LISTEND
}


/*
MODE command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.3
*/
void	Server::_modeCmd(User *user)
{
	if (!user->hasBeenWelcomed())
		return ;
	if (user->getArgs().size() < 2)
		return (user->reply(ERR_NEEDMOREPARAMS(getSrv(), user->getNick(), "MODE")));
	std::string		name = user->getArgs()[0];
	std::string		action = user->getArgs()[1];
	if (name[0] == '#' || name[0] == '&') // channel modes
	{
		Channel		*channel = getChannel(name);
		if (channel == NULL)
			return (user->reply(ERR_NOSUCHCHANNEL(getSrv(), name)));
		// MARQUE-PAGE
	}
	else // user modes
	{
		// MARQUE-PAGE
	}
	// ERR_NEEDMOREPARAMS
	// RPL_CHANNELMODEIS
	// ERR_CHANOPRIVSNEEDED
	// ERR_NOSUCHNICK
	// ERR_NOTONCHANNEL
	// ERR_KEYSET
	// RPL_BANLIST
	// RPL_ENDOFBANLIST
	// ERR_UNKNOWNMODE
	// ERR_NOSUCHCHANNEL
	// ERR_USERSDONTMATCH
	// RPL_UMODEIS
	// ERR_UMODEUNKNOWNFLAG
}

/*
The following function may be called by 2 commands: PRIVMSG and NOTICE.
Contrary to PRIVMSG, with NOTICE, there aren't any automatic replies sent.
*/
void	Server::_msgCmd(User *user, bool silently)
{
	if (!user->hasBeenWelcomed())
		return ;
	// TODO verifier que les espaces sont indispensable, est-ce que [PRIVMSG nick:test] marche ?
	// TODO verifier l'ordre de priorité des erreurs
	if (user->getArgs().size() < 2 && silently)
		return ;
	else if (user->getArgs().size() < 2)
		return (user->reply(ERR_NOTEXTTOSEND(getSrv())));
	// TODO est-ce que les : sont indispensables ?
	if (user->getArgs()[0][0] != ':' && silently)
		return ;
	else if (user->getArgs()[0][0] != ':')
		return (user->reply(ERR_NORECIPIENT(getSrv(), "PRIVMSG")));
	str_vec			targets = split_str(user->getArgs()[0], ",", true);
	std::string		message = user->getRawArgs(1);
	for (str_vec::iterator it = targets.begin(); it != targets.end(); ++it)
	{
		if ((*it)[0] == '#' || (*it)[0] == '&') { // the target is a channel
			Channel		*channel = getChannel(*it);
			if (channel == NULL && !silently)
				user->reply(ERR_NOSUCHCHANNEL(getSrv(), *it));
			else if (channel != NULL) {
				if (!channel->isIn(user->getFd()) && channel->hasMode('n'))
				{
					if (!silently)
						user->reply(ERR_CANNOTSENDTOCHAN(getSrv(), *it));
					continue ;
				}
				// TODO bot ?
				channel->msg(getSrv(), user, message, silently);
			}
		} else { // the target is a user or a group of users
			User		*target = getUser(*it);
			// TODO bot ?
			if (target == NULL && !silently)
				user->reply(ERR_NOSUCHNICK(getSrv(), *it));
			// TODO verifier le fonctionnement attendu ici!! Pourquoi faudrait-il boucler sur les channels ?
			if (target != NULL)
				target->reply(RPL_MSG(user->getNick(), *it, message));
		}
	}
	// ERR_NORECIPIENT
	// ERR_NOTEXTTOSEND

	// ERR_NOTOPLEVEL
	// ERR_WILDTOPLEVEL
	// ERR_TOOMANYTARGETS
	// RPL_AWAY
}


/*
NAMES command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.5
*/
void	Server::_namesCmd(User *user)
{
	(void)user->getArgs(); // FIXME
	// RPL_NAMREPLY
	// RPL_ENDOFNAMES
}


/*
NICK command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.1.2
*/
void	Server::_nickCmd(User *user)
{
	if (user->getArgs().size() < 1)
		return user->reply(ERR_NONICKNAMEGIVEN(getSrv()));
	std::string		nick = user->getArgs()[0];
	if (nick.length() > 9)
		return user->reply(ERR_ERRONEUSNICKNAME(getSrv(), nick));
	if (nick.find_first_not_of(ALLOWED_CHAR_IN_NICK) != std::string::npos)
		return user->reply(ERR_ERRONEUSNICKNAME(getSrv(), nick));
	for (usr_map::iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second->getNick() == nick)
			return user->reply(ERR_NICKNAMEINUSE(getSrv(), nick));
	}
	// ERR_NICKCOLLISION doesn't need to be implemented with 1 server only
	user->setNick(nick);
	if (!user->getUser().empty() && user->isLoggedIn() && !user->hasBeenWelcomed())
		user->welcome(getSrv(), false);
}


/*
NOTICE command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.4.2
*/
void	Server::_noticeCmd(User *user) { _msgCmd(user, true); }


/*
PART command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.2
*/
void	Server::_partCmd(User *user)
{
	(void)user->getArgs(); // FIXME
	// ERR_NEEDMOREPARAMS
	// ERR_NOSUCHCHANNEL
	// ERR_NOTONCHANNEL
}


/*
PASS command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.1.1
*/
void	Server::_passCmd(User *user)
{
	if (user->hasBeenWelcomed())
		return (user->reply(ERR_ALREADYREGISTRED(getSrv())));
	if (user->getArgs().size() < 1)
		return (user->reply(ERR_NEEDMOREPARAMS(getSrv(), user->getNick(), "PASS")));
	// TODO un mot de passe peut il contenir un espace ? peut il être une chaîne vide ?
	std::string		password = user->getArgs()[0];
	if (password == getPwd())
	{
		user->logIn();
		if (!user->getNick().empty() && !user->getUser().empty())
			user->welcome(getSrv(), false);
	} else {
		// TODO est-ce que "PASS mauvais_mdp" déconnecte l'utilisateur ?
		user->reply(ERR_PASSWDMISMATCH(getSrv()));
	}
}


/*
PING command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.6.2
*/
void	Server::_pingCmd(User *user)
{
	if (!user->hasBeenWelcomed())
		return ;
	std::string		param = user->getRawArgs(0);
	// TODO si plusieurs params fournis, seul le 1er est retenu ou est-ce que l'on compte les espaces ?
	if (param.empty())
		return (user->reply(ERR_NOORIGIN(getSrv())));
	if (param != _host && param != getSrv())
		return (user->reply(ERR_NOSUCHSERVER(getSrv(), param)));
	user->reply(RPL_PING(user->getNick(), getSrv()));
}

/*
PRIVMSG command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.4.1
*/
void	Server::_privMsgCmd(User *user) { _msgCmd(user, false); }

/*
QUIT command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.1.6
*/
void	Server::_quitCmd(User *user)
{
	if (!user->hasBeenWelcomed())
		return ;
	std::string		quit_msg = user->getRawArgs(0);
	// gets the user out of every channel
	chan_map	channels = user->getChannels();
	for (chan_map::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		// TODO checker quel est le quit_msg par défaut
		it->second->delUser(user);
		if (it->second->getNbUsers() > 0)
			it->second->broadcast(RPL_QUIT(user->getNick(), quit_msg));
		else
			delChannel(it->second);
	}
	// frees the chan_vec of the User instance
	user->clearChannels();
	// deletes the User instance itself
	// TODO remplacer les lignes ci-dessous par Server::_deleteUser ?
	int				fd = user->getFd();
	close(fd);
	_users.erase(fd);
	delete user;
	for (pfds_it it = _pfds.begin(); it != _pfds.end(); ++it)
	{
		if (it->fd == fd)
		{
			_pfds.erase(it);
			break;
		}
	}
	std::cout << MAG BYE << fd << END << std::endl;
}


/*
TOPIC command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.4
*/
void	Server::_topicCmd(User *user)
{
	if (!user->hasBeenWelcomed())
		return;
	if (user->getArgs().size() < 1)
		return (user->reply(ERR_NEEDMOREPARAMS(getSrv(), user->getNick(), "TOPIC")));
	std::string		name = user->getArgs()[0];
	Channel			*chan = getChannel(name);
	// TODO checker si on peut TOPIC sans être dedans : filtrer, chercher dans les channels de l'User ?
	if (chan != NULL) { // channel does exist & the user can access it
		// the user simply wants to view the channel topic
		if (user->getArgs()[1].find(':') == std::string::npos) {
			if (chan->getTopic() == "")
				return (user->reply(RPL_NOTOPIC(user->getNick(), name)));
			return (user->reply(RPL_TOPIC(user->getNick(), name, chan->getTopic())));
		}
		// the topic for that channel will be changed if its modes permit it
		std::string		topic = user->getRawArgs(1);
		topic = topic.substr(topic.find(':') + 1);
		// TODO tester comportement attendu si pas de ':' avant le topic
		// TODO vérifier l'ordre de priorité des messages d'erreur suivants
		if (chan->hasMode('t') && !chan->isOp(user->getFd()))
			return (user->reply(ERR_CHANOPRIVSNEEDED(getSrv(), name)));
		if (!chan->isIn(user->getFd()))
			return (user->reply(ERR_NOTONCHANNEL(getSrv(), name)));
		chan->setTopic(topic, user->getNick());
		chan->broadcast(RPL_TOPIC(user->getNick(), name, topic));
	} else { // channel not found
		user->reply(ERR_NOTONCHANNEL(getSrv(), name));
	}
}


/*
USER command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.1.3
*/
void	Server::_userCmd(User *user)
{
	if (user->hasBeenWelcomed())
		return user->reply(ERR_ALREADYREGISTRED(getSrv()));
	// TODO vérifier ce qu'il doit se passer si real_name ne commence pas avec ':'
	if (user->getArgs().size() < 4 || user->getArgs()[3][0] != ':')
		return user->reply(ERR_NEEDMOREPARAMS(getSrv(), user->getNick(), "USER"));
	// TODO vérifier ce qu'il doit se passer si Nick non encore défini
	user->setUser(user->getArgs()[0]);
	user->setHost(user->getArgs()[1]);
	user->setReal(user->getArgs()[3]);
	if (user->getNick().size() && user->isLoggedIn() && !user->hasBeenWelcomed())
		user->welcome(getSrv(), false);
}


/*
WHO command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.5.1
*/
void	Server::_whoCmd(User *user)
{
	if (user->hasBeenWelcomed())
		return ;
	// TODO tester comportement si pas d'arguments
	std::string		name = user->getArgs()[0];
	Channel			*chan = getChannel(name);
	if (chan != NULL) { // channel found based on the name passed to WHO
		chan->rpl_whoreply(user, getSrv());
	} else { // channel not found
		// TODO <name> is matched against users' host, server, realname & nickname
		User		*target = getUser(name);
		if (target && (!target->hasMode('i') || user == target))
		{
			user->reply(RPL_WHOREPLY(getSrv(), "*", target->getUser(), \
				target->getHost(), target->getNick(), target->getReal()));
		}
	}
	user->reply(RPL_ENDOFWHO(getSrv(), user->getNick()));
	// ERR_NOSUCHSERVER
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

----------------------------- IRC server commands -----------------------------

PASS			-> 🔴🔵
USER			-> 🔴🔵
*/
