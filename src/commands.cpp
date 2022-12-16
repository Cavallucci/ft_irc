/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 13:06:04 by llalba            #+#    #+#             */
/*   Updated: 2022/12/16 13:57:21 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::_initHandlers(void)
{
	// by alphabetical order
	_commands["cap"] = &Server::_capHandler;
	_commands["print"] = &Server::_printHandler; //TODO FIXME a enlever apres tests
	_commands["invite"] = &Server::_inviteHandler;
	_commands["join"] = &Server::_joinHandler;
	_commands["kick"] = &Server::_kickHandler;
	_commands["list"] = &Server::_listHandler;
	_commands["mode"] = &Server::_modeHandler;
	_commands["privmsg"] = &Server::_privMsgHandler;
	_commands["names"] = &Server::_namesHandler;
	_commands["nick"] = &Server::_nickHandler;
	_commands["notice"] = &Server::_noticeHandler;
	_commands["part"] = &Server::_partHandler;
	_commands["pass"] = &Server::_passHandler;
	_commands["ping"] = &Server::_pingHandler;
	_commands["quit"] = &Server::_quitHandler;
	_commands["topic"] = &Server::_topicHandler;
	_commands["user"] = &Server::_userHandler;
	_commands["who"] = &Server::_whoHandler;
}

//------------------- SERVER COMMANDS BY ALPHABETICAL ORDER -------------------


void	Server::_printHandler(User *user) // TODO FIXME
{
	(void)user;
	std::cout << MAG << "❓ INFORMATION ABOUT SERVER " << _name << END << std::endl;
	std::cout << std::endl;
	std::cout << GRN << "🚹 Users : " END;
	for (user_it it = _users.begin(); it != _users.end(); it++)
		std::cout << getUser(it->first)->getNick() << std::endl;
	std::cout << std::endl;
	std::cout << GRN << "📡 Channels : " << END << std::endl;
	std::cout << "NAME	||	TOPIC	||	USERS	||	OPS	||	BANNED	||	MOD	||" << std::endl;
	for (chan_it chan = _channels.begin(); chan != _channels.end(); chan++)
	{
		Channel	*channel = getChannel(chan->first);
		std::cout << channel->getName() << "	||";
		std::cout << channel->getTopic() << "	||";
		usr_map	usr = channel->getUsers();
		for (user_it ops = usr.begin(); ops != usr.end(); ops++)
		{
			if (channel->isOp(ops->first))
				std::cout << getUser(ops->first)->getNick() << ", ";
		}
		std::cout << "	||";
		for (user_it ban = usr.begin(); ban != usr.end(); ban++)
		{
			if (channel->isBanned(ban->first))
				std::cout << getUser(ban->first)->getNick() << ", ";
		}
		std::cout << "	||";
		for (user_it mod = usr.begin(); mod != usr.end(); mod++)
		{
			if (channel->isMod(mod->first))
				std::cout << getUser(mod->first)->getNick() << ", ";
		}
		std::cout << "	||";
		std::cout << std::endl;
	}
}


/*
CAP command, not mandatory, nor defined in RFC1459
It's empty just for the server to stay quiet when this command is sent by IRSSI
*/
void	Server::_capHandler(User *user)
{
	if (user->getArgs().size() >= 2 && user->getArgs()[1] == "LS")
		return (user->reply(ERR_UNKNOWNCOMMAND(getSrv(), user->getNick(), "CAP")));
}



/*
INVITE command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.7
*/
void	Server::_inviteHandler(User *user)
{
	if (!user->hasBeenWelcomed())
		return ;
	if (user->getArgs().size() < 2)
		user->reply(ERR_NEEDMOREPARAMS(getSrv(), user->getNick(), "INVITE"));
	// extra arguments are simply ignored
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
	}
	// the protocol does not mention any error message to return when the channel is invalid
}

/*
JOIN command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.1
*/
void	Server::_joinHandler(User *user)
{
	if (!user->hasBeenWelcomed())
		return ;
	if (user->getArgs().size() < 1)
		return user->reply(ERR_NEEDMOREPARAMS(getSrv(), user->getNick(), "JOIN"));
	// extra arguments are simply ignored
	str_vec			chans = split_str(user->getArgs()[0], ",", true);
	size_t			index = 0;
	for (str_vec::iterator name = chans.begin(); name != chans.end(); ++name, ++index)
	{
		if (!is_valid_channel_name(*name, user, getSrv()))
			continue ; // invalid channel names are silently ignored
		Channel		*chan = getChannel(*name);
		if (chan == NULL) { // channel has to be created
			chan = newChan(user, *name, index); // chan.addUser will be called here
			user->registerChannel(chan);
			chan->broadcast(RPL_JOIN(user->getNick(), *name), NON_FD);
			chan->rpl_names(user, getSrv(), true);
		} else if (chan->canJoin(getSrv(), user, index)) { // channel already exists
			if (chan->isInvited(user->getFd()))
				chan->rmInvite(user);
			if (chan->isIn(user->getFd()))
				continue ;
			chan->addUser(user);
			user->registerChannel(chan);
			chan->broadcast(RPL_JOIN(user->getNick(), *name), NON_FD);
			if (chan->getTopic() != "")
			{
				user->reply(RPL_TOPIC(getSrv(), *name, chan->getTopic()));
				user->reply(RPL_TOPICWHOTIME(getSrv(), *name, chan->getTopicCtxt()));
			}
			chan->rpl_names(user, getSrv(), true);
		}
	}
}

/*
KICK command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.8
*/
void	Server::_kickHandler(User *user)
{
	if (!user->hasBeenWelcomed())
		return ;
	if (user->getArgs().size() < 2)
		return (user->reply(ERR_NEEDMOREPARAMS(getSrv(), user->getNick(), "KICK")));
	std::string		channel_name = user->getArgs()[0];
	std::string		target_user = user->getArgs()[1];
	Channel			*channel = getChannel(channel_name);
	User			*target = getUser(target_user);
	if (channel == NULL)
		return (user->reply(ERR_NOSUCHCHANNEL(getSrv(), channel_name)));
	if (target == NULL || !channel->isIn(user->getFd()) || !channel->isIn(target->getFd()))
		return (user->reply(ERR_NOTONCHANNEL(getSrv(), channel_name)));
	if (!channel->isOp(user->getFd()))
		return (user->reply(ERR_CHANOPRIVSNEEDED(getSrv(), channel_name)));
	channel->broadcast(RPL_KICK(user->getNick(), channel_name, target_user), NON_FD);
	target->rmChannel(channel_name);
	channel->delUser(target);
	if (!channel->getNbUsers(true))
		delChannel(channel);
}


/*
LIST command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.6
*/
void	Server::_listHandler(User *user)
{
	if (!user->hasBeenWelcomed())
		return;
	Channel			*chan;
	std::string		nb_visible;
	user->reply(RPL_LISTSTART(getSrv()));
	if (user->getArgs().size()) { // ⏩ lists specified channels and their topics
		str_vec		chans = split_str(user->getArgs()[0], ",", true);
		for (str_vec::iterator name = chans.begin(); name != chans.end(); ++name) {
			chan = getChannel(*name);
			if (chan == NULL || (chan->hasMode('s') && !(chan->isIn(user->getFd()))))
				continue ; // if the channel cannot be found, it will be skipped
			// invisible users are not counted
			nb_visible = size_t_to_str(chan->getNbUsers(false));
			if (chan->hasMode('p') && !(chan->isIn(user->getFd())))
				user->reply(RPL_LIST(getSrv(), *name, nb_visible, PRV_TOPIC));
			else
				user->reply(RPL_LIST(getSrv(), *name, nb_visible, chan->getTopic()));
		}
	} else { // ⏩ lists all visible channels and their topics
		for (chan_it it = _channels.begin(); it != _channels.end(); ++it) {
			chan = it->second;
			if (chan->hasMode('s') && !(chan->isIn(user->getFd())))
				continue ; // if the channel cannot be found, it will be skipped
			// invisible users are not counted
			nb_visible = size_t_to_str(chan->getNbUsers(false));
			if (chan->hasMode('p') && !(chan->isIn(user->getFd())))
				user->reply(RPL_LIST(getSrv(), chan->getName(), nb_visible, PRV_TOPIC));
			else
				user->reply(RPL_LIST(getSrv(), chan->getName(), nb_visible, chan->getTopic()));
		}
	}
	user->reply(RPL_LISTEND(getSrv(), user->getNick()));
}


/*
MODE command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.3
*/
void	Server::_modeHandler(User *user)
{
	if (!user->hasBeenWelcomed())
		return ;
	if (user->getArgs().size() < 1)
		return (user->reply(ERR_NEEDMOREPARAMS(getSrv(), user->getNick(), "MODE")));
	std::string		name = user->getArgs()[0];
	std::string		action;
	if (user->getArgs().size() >= 2)
		action = user->getArgs()[1];
	if (name[0] == '#' || name[0] == '&') { // channel modes
		Channel		*channel = getChannel(name);
		if (channel == NULL)
			return (user->reply(ERR_NOSUCHCHANNEL(getSrv(), name)));
		if (!channel->isOp(user->getFd()))
			return (user->reply(ERR_CHANOPRIVSNEEDED(getSrv(), name)));
		if (!channel->isIn(user->getFd()))
			return (user->reply(ERR_NOTONCHANNEL(getSrv(), name)));
		if (action.empty()) // the user just wants to get the channel modes
			return (channel->rpl_chan_mode(user, getSrv())); // RPL_CHANNELMODEIS
		if (action[0] == 'b')
			return (channel->rpl_ban_list(user, getSrv())); // RPL_BANLIST, RPL_ENDOFBANLIST
		if (!(action[0] == '+' || action[0] == '-'))
			return (user->reply(ERR_UMODEUNKNOWNFLAG(getSrv())));
		for (std::string::size_type i = 1; i < action.size(); i++)
			channel->updateMode(getSrv(), user, action[0] == '+', action[i]);
		if (!channel->getNbUsers(true))
			delChannel(channel);
		std::cout << GRN << CHANNEL_MODE << "[" END;
		std::cout << channel->getMode() << GRN "]" END << std::endl;
	} else { // user modes
		User		*target = getUser(name);
		if (target == NULL)
			return (user->reply(ERR_NOSUCHNICK(getSrv(), name)));
		if (target != user)
			return (user->reply(ERR_USERSDONTMATCH(getSrv())));
		if (action.empty())
			return (user->reply(RPL_UMODEIS(getSrv(), user->getMode())));
		if (action[0] != '+' && action[0] != '-')
			return (user->reply(ERR_UMODEUNKNOWNFLAG(getSrv())));
		for (size_t i = 1; i < action.size(); i++) { // ignores the starting '+' or '-'
			if (!user->isValidMode(action[i]))
				user->reply(ERR_UNKNOWNMODE(getSrv(), action[i]));
			else if (action[0] == '+' && action[i] != 'o') // prevents [MODE myself +o]
				user->addMode(action[i]);
			else if (action[0] == '-')
				user->rmMode(action[i]);
		}
		std::cout << GRN << USER_MODE << user->getNick() << " [" END;
		std::cout << user->getMode() << GRN "]" END << std::endl;
	}
}

/*
The following function may be called by 2 commands: PRIVMSG and NOTICE.
Contrary to PRIVMSG, with NOTICE, there aren't any automatic replies sent.
*/
void	Server::_msgHandler(User *user, bool silently)
{
	if (!user->hasBeenWelcomed())
		return ;
	if (user->getArgs().size() < 2 && silently)
		return ;
	else if (user->getArgs().size() < 2)
		return (user->reply(ERR_NOTEXTTOSEND(getSrv())));
	if (user->getArgs()[0][0] == ':' && silently)
		return ;
	else if (user->getArgs()[0][0] == ':')
		return (user->reply(ERR_NORECIPIENT(getSrv(), "PRIVMSG")));
	str_vec				targets = split_str(user->getArgs()[0], ",", true);
	std::string			message;
	if (user->getArgs()[1][0] == ':')
		message = user->getRawArgs(1).substr(1);
	else  // no colon -> we'll only keep the 1st word
		message = user->getArgs()[1];
	for (str_vec::iterator it = targets.begin(); it != targets.end(); ++it)
	{
		if ((*it)[0] == '#' || (*it)[0] == '&') { // the target is a channel
			Channel		*channel = getChannel(*it);
			if (channel == NULL && !silently)
				user->reply(ERR_NOSUCHCHANNEL(getSrv(), *it));
			else if (channel != NULL) {
				if (!channel->isIn(user->getFd()) && channel->hasMode('n')) {
					if (!silently)
						user->reply(ERR_CANNOTSENDTOCHAN(getSrv(), *it));
					continue ;
				}
				channel->msg(getSrv(), user, message, silently);
			}
		} else { // the target is a user
			User		*receiver = getUser(*it);
			if (receiver == NULL && !silently)
				user->reply(ERR_NOSUCHNICK(getSrv(), *it));
			if (receiver != NULL && silently)
				receiver->reply(RPL_NOTICE(user->getNick(), *it, message));
			else if (receiver != NULL && !silently)
				receiver->reply(RPL_MSG(user->getNick(), *it, message));
		}
	}
}


/*
NAMES command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.5
*/
void	Server::_namesHandler(User *user)
{
	if (!user->hasBeenWelcomed())
		return ;
	Channel		*chan = NULL;
	if (user->getArgs().size()) // ⏩ lists visible users on the specified channels
	{
		// only the first argument will be taken into account
		str_vec		chans = split_str(user->getArgs()[0], ",", true);
		for (str_vec::iterator name = chans.begin(); name != chans.end(); ++name) {
			chan = getChannel(*name);
			// secret and invalid channels will simply be skipped
			if (chan != NULL && (chan->isIn(user->getFd()) || \
			(!chan->hasMode('p') && !chan->hasMode('s'))))
				chan->rpl_names(user, getSrv(), true);
		}
	} else { // ⏩ lists absolutely all visible channels and users
		for (chan_it it = _channels.begin(); it != _channels.end(); ++it) {
			chan = it->second;
			if (chan->isIn(user->getFd()) || \
			(!chan->hasMode('p') && !chan->hasMode('s')))
				chan->rpl_names(user, getSrv(), true);
		}
		std::string		list;
		bool			listed;
		for (user_it it = _users.begin(); it != _users.end(); ++it) {
			if (!it->second->hasMode('i')) // checks that the target is visible
				chan_map const		its_channels = it->second->getChannels();
			{
				listed = false; // checks that the target hasn't already been listed
				for (chan_it it = _channels.begin(); it != _channels.end() && !listed; ++it)
				{ // if he has been listed, he must be in at least 1 visible channel
					if (!it->second->hasMode('p') && !it->second->hasMode('s'))
						listed = true;
				}
				if (!list.empty())
					list.append(" ");
				if (!listed)
					list.append(it->second->getNick());
			}
		}
		user->reply(RPL_NAMREPLY(getSrv(), user->getNick(), "=", "*", list));
		user->reply(RPL_ENDOFNAMES(getSrv(), user->getNick(), "*"));
	}
}


/*
NICK command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.1.2
*/
void	Server::_nickHandler(User *user)
{
	if (user->getArgs().size() < 1)
		return user->reply(ERR_NONICKNAMEGIVEN(getSrv()));
	// we'll consider the first argument ony and ignore the others
	std::string		nick = user->getArgs()[0];
	if (nick.length() > 9)
		return user->reply(ERR_ERRONEUSNICKNAME(getSrv(), nick));
	if (nick.find_first_not_of(ALLOWED_CHAR_IN_NICK) != std::string::npos)
		return user->reply(ERR_ERRONEUSNICKNAME(getSrv(), nick));
	for (user_it it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second->getNick() == nick)
			return user->reply(ERR_NICKNAMEINUSE(getSrv(), nick));
	}
	// ERR_NICKCOLLISION doesn't need to be implemented with 1 server only
	user->setNick(nick);
	if (!user->getUser().empty() && user->isLoggedIn() && !user->hasBeenWelcomed())
		user->welcome(getSrv(), false);
	else
		user->reply(RPL_NICK(nick)); // the user just wants to update its nick
}


/*
NOTICE command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.4.2
*/
void	Server::_noticeHandler(User *user) { _msgHandler(user, true); } // TODO


/*
PART command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.2
*/
void	Server::_partHandler(User *user)
{
	if (!user->hasBeenWelcomed())
		return ;
	if (!user->getArgs().size())
		return (user->reply(ERR_NEEDMOREPARAMS(getSrv(), user->getNick(), "PART")));
	// we'll consider the first argument only and ignore the others
	str_vec		names = split_str(user->getArgs()[0], ",", true);
	Channel		*chan;
	for (str_vec::iterator it = names.begin(); it != names.end(); ++it) {
		chan = getChannel(*it);
		if (chan == NULL) {
			user->reply(ERR_NOSUCHCHANNEL(getSrv(), *it));
		} else if (chan->isIn(user->getFd())) {
			chan->broadcast(RPL_PART(user->getNick(), *it), NON_FD);
			user->rmChannel(*it);
			chan->delUser(user);
			if (chan->getNbUsers(true) == 0)
				delChannel(chan);
		} else {
			user->reply(ERR_NOTONCHANNEL(getSrv(), *it));
		}
	}
}


/*
PASS command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.1.1
*/
void	Server::_passHandler(User *user)
{
	if (user->hasBeenWelcomed())
		return (user->reply(ERR_ALREADYREGISTRED(getSrv())));
	if (user->getArgs().size() < 1)
		return (user->reply(ERR_NEEDMOREPARAMS(getSrv(), user->getNick(), "PASS")));
	// we'll consider the first argument only and ignore the others...
	// but anyway, the password cannot contain spaces (cf. main.cpp)
	std::string		password = user->getArgs()[0];
	if (password == getPwd())
		user->logIn();
	else
		user->reply(ERR_PASSWDMISMATCH(getSrv()));
}


/*
PING command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.6.2
*/
void	Server::_pingHandler(User *user)
{
	if (!user->hasBeenWelcomed())
		return ;
	std::string		param = user->getRawArgs(0); // extra arguments are simply ignored
	if (param.empty())
		return (user->reply(ERR_NOORIGIN(getSrv())));
	if ((param != _host) && (param != getSrv()))
		return (user->reply(ERR_NOSUCHSERVER(getSrv(), param)));
	user->reply(RPL_PING(user->getNick(), getSrv()));
}

/*
PRIVMSG command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.4.1
*/
void	Server::_privMsgHandler(User *user) { _msgHandler(user, false); }


/*
QUIT command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.1.6
*/
void	Server::_quitHandler(User *user)
{
	if (!user->hasBeenWelcomed())
		return ;
	std::string		quit_msg = user->getRawArgs(0);
	if (quit_msg.empty()) // the default message is just the nick
		quit_msg = user->getNick();
	chan_map		channels = user->getChannels();
	int				fd = user->getFd();
	for (chan_it it = channels.begin(); it != channels.end(); ++it) {
		it->second->delUser(user);
		if (it->second->getNbUsers(true) > 0)
			it->second->broadcast(RPL_QUIT(user->getNick(), quit_msg), NON_FD);
	}
	_deleteUser(fd); // deletes empty channels as well
	std::cout << MAG BYE << fd << END << std::endl;
}


/*
TOPIC command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.2.4
*/
void	Server::_topicHandler(User *user)
{
	if (!user->hasBeenWelcomed())
		return;
	if (user->getArgs().size() < 1)
		return (user->reply(ERR_NEEDMOREPARAMS(getSrv(), user->getNick(), "TOPIC")));
	std::string			name = user->getArgs()[0];
	Channel				*chan = getChannel(name);
	if (chan != NULL && chan->isIn(user->getFd())) { // channel does exist & the user can access it
		// the user simply wants to view the channel topic
		if (user->getArgs().size() == 1) {
			if (chan->getTopic().empty())
				return (user->reply(RPL_NOTOPIC(getSrv(), user->getNick(), name)));
			return (user->reply(RPL_TOPIC(getSrv(), name, chan->getTopic())));
		}
		// the topic for that channel will be changed if its modes permit it
		std::string		topic = user->getRawArgs(1);
		if (topic.find(':') == 0)
			topic = topic.substr(1);
		else // no colon -> we'll only keep the 1st word
			topic = user->getArgs()[1];
		if (chan->hasMode('t') && !chan->isOp(user->getFd()))
			return (user->reply(ERR_CHANOPRIVSNEEDED(getSrv(), name)));
		chan->broadcast(RPL_TOPIC_SET(user->getNick(), name, topic), NON_FD);
	} else { // channel not found
		user->reply(ERR_NOTONCHANNEL(getSrv(), name));
	}
}


/*
USER command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.1.3
*/
void	Server::_userHandler(User *user)
{
	if (user->hasBeenWelcomed())
		return user->reply(ERR_ALREADYREGISTRED(getSrv()));
	if (user->getArgs().size() < 4)
		return user->reply(ERR_NEEDMOREPARAMS(getSrv(), user->getNick(), "USER"));
	user->setUser(user->getArgs()[0]);
	user->setHost(user->getArgs()[1]);
	std::string		real = user->getRawArgs(3);
	if (real[0] != ':')
		real = user->getArgs()[3];
	else // no colon -> we'll only keep the 1st word
		real = real.substr(1);
	user->setReal(real);
	if (!user->getNick().empty() && user->isLoggedIn() && !user->hasBeenWelcomed())
		user->welcome(getSrv(), false);
}


/*
WHO command as described here:
https://www.rfc-editor.org/rfc/rfc1459.html#section-4.5.1
*/
void	Server::_whoHandler(User *user)
{
	if (!user->hasBeenWelcomed())
		return ;
	User				*target;
	if (user->getArgs().size()) {
		std::string		name = user->getArgs()[0]; // considers only the 1st argument
		Channel			*chan = getChannel(name);
		if (chan != NULL) { // channel found based on the name passed to WHO
			chan->rpl_whoreply(user, getSrv());
		} else { // channel not found, maybe that the user is looking for another user
			target = getUser(name); // <name> is matched against users' nicknames
			if (target && (!target->hasMode('i') || user == target)) {
				user->reply(RPL_WHOREPLY(getSrv(), user->getNick(), "*", target->getUser(), \
					target->getHost(), target->getNick(), "", target->getReal()));
			}
			user->reply(RPL_ENDOFWHO(getSrv(), user->getNick(), "*"));
		}
	} else { // in the absence of the <name> parameter, all visible users are listed
		bool	skip;
		for (user_it usr = _users.begin(); usr != _users.end(); ++usr) {
			target = usr->second;
			skip = false;
			if (target->hasMode('i') && user != target)
				skip = true;
			chan_map	channels = target->getChannels();
			for (chan_it chan = channels.begin(); \
			chan != channels.end() && (!skip); ++chan) {
				if (chan->second->isIn(user->getFd()))
					skip = true;
			}
			if (!skip) {
				user->reply(RPL_WHOREPLY(getSrv(), user->getNick(), "*", target->getUser(), \
					target->getHost(), target->getNick(), "", target->getReal()));
			}
		}
		user->reply(RPL_ENDOFWHO(getSrv(), user->getNick(), "*"));
	}
}
