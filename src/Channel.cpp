/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:05 by llalba            #+#    #+#             */
/*   Updated: 2022/12/19 22:16:16 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

//-------------------------------- CONSTRUCTORS -------------------------------

Channel::Channel()
{
}


Channel::Channel(std::string name, std::string password) :
_name(name), _password(password)
{
	size_t	max_size = (size_t) - 1;
	setMaxUsers(max_size);
}


Channel::Channel(Channel const & src)
{
	(void)src;
}

//-------------------------------- DESTRUCTORS --------------------------------

Channel::~Channel()
{

}

//------------------------------- REPLY METHODS -------------------------------

void			Channel::broadcast(std::string msg)
{
	for (user_it it = _users.begin(); it != _users.end(); ++it)
		it->second->reply(msg);
}


void			Channel::rpl_whoreply(User *user, std::string srv)
{
	// if the channel is private or secret, it will be silently ignored
	if ((hasMode('p') || hasMode('s')) && !isIn(user->getFd()))
		return ;
	for (user_it it = _users.begin(); it != _users.end(); ++it)
	{
		if (!it->second->hasMode('i')) {
			user->reply(RPL_WHOREPLY(
				srv,
				user->getNick(),
				getName(), // channel
				it->second->getUser(),
				it->second->getHost(),
				it->second->getNick(),
				((isOp(it->second->getFd())) ? "@" : ""), // operator or not ?
				it->second->getReal()
			));
		}
	}
	user->reply(RPL_ENDOFWHO(srv, user->getNick(), getName()));
}


void			Channel::rpl_ban_list(User *user, std::string srv)
{
	for (user_it it = _banned.begin(); it != _banned.end(); ++it)
		user->reply(RPL_BANLIST(srv, user->getNick(), getName(), it->second->getNick()));
	user->reply(RPL_ENDOFBANLIST(srv, user->getNick(), getName()));
}


void			Channel::rpl_names(User *user, std::string srv, bool send_end)
{
	std::string		output;
	std::string		mode_char;

	for (user_it it = _users.begin(); it != _users.end(); ++it)
	{
		if (!it->second->hasMode('i'))
		{
			if (!output.empty())
				output.append(" ");
			if (isOp(it->second->getFd()))
				output += "@";
			if (isMod(it->second->getFd()))
				output += "+";
			output.append(it->second->getNick());
		}
	}
	mode_char = "=";
	if (hasMode('s'))
		mode_char = "@";
	else if (hasMode('p'))
		mode_char = "*";
	user->reply(RPL_NAMREPLY(srv, user->getNick(), mode_char, getName(), output));
	if (send_end)
		user->reply(RPL_ENDOFNAMES(srv, user->getNick(), getName()));
}


void			Channel::rpl_chan_mode(User *user, std::string srv)
{
	std::string			mode_str = "+" + _mode;

	if (hasMode('l'))
		mode_str += " " + size_t_to_str(getMaxUsers());
	user->reply(RPL_CHANNELMODEIS(srv, user->getNick(), getName(), mode_str));
}


void			Channel::msg(std::string srv, User *user, std::string txt, bool silently)
{
	if (hasMode('m') && !(isOp(user->getFd()) || isMod(user->getFd())))
	{
		if (!silently)
			user->reply(ERR_CANNOTSENDTOCHAN(srv, user->getNick(), getName()) " (+m)");
		return ;
	}
	for (user_it it = _users.begin(); it != _users.end(); ++it)
	{
		// the user doesn't want to write to himself
		if (it->second != user && !silently)
			it->second->reply(RPL_MSG(user->getNick(), getName(), txt));
		else if (it->second != user && silently)
			it->second->reply(RPL_NOTICE(user->getNick(), getName(), txt));
	}
}

void			Channel::clearAll(void)
{
	for (usr_map::iterator it = _users.begin(); it != _users.end(); it++)
		it->second->clearAll();
	_users.clear();
	_ops.clear();
	_banned.clear();
	_moderators.clear();
	_invited.clear();
}

//---------------------------- ACCESSORS / GETTERS ----------------------------

usr_map				Channel::getUsers() const { return _users; }
std::string			Channel::getName(void) const { return _name; }
std::string			Channel::getTopic(void) const { return _topic; }
size_t				Channel::getMaxUsers(void) const { return _maxUsers; }
bool				Channel::hasPassword() const { return _password != ""; }
std::string			Channel::getTopicCtxt(void) const { return _topicCtxt; }
std::string			Channel::getMode(void) const { return _mode; }


User				*Channel::getUser(std::string nick) const
{
	for (usr_map::const_iterator it = _users.begin(); it != _users.end(); it++)
	{
		if (it->second->getNick() == nick)
			return it->second;
	}
	if (DEBUG)
		std::cerr << YEL ERR_USER_NICK_NOT_FOUND << nick << END << std::endl;
	return NULL;
}


size_t				Channel::getNbUsers(bool with_invisible) const {
	if (with_invisible)
		return _users.size();
	usr_map		users = getUsers();
	size_t	nb = 0;
	for (user_it it = users.begin(); it != users.end(); ++it)
	{
		if (!it->second->hasMode('i'))
			nb++;
	}
	return (nb);
}


bool				Channel::hasMode(char c) const
{
	if (_mode.find(c) != std::string::npos)
		return (true);
	return (false);
}


bool				Channel::pwMatches(User *user, size_t index) const
{
	str_vec		passwords;
	if (user->getArgs().size() == 1)
		return (false);
	passwords = split_str(user->getArgs()[1], ",", true);
	if (passwords.size() < index + 1)
		return (false);
	return (passwords[index] == _password);
}


bool				Channel::isIn(int fd) const
{
	if (_users.count(fd))
		return (true);
	return (false);
}


bool				Channel::isOp(int fd) const
{
	if (_ops.count(fd))
		return (true);
	return (false);
}


bool				Channel::isBanned(int fd) const
{
	if (_banned.count(fd))
		return (true);
	return (false);
}


bool				Channel::isMod(int fd) const
{
	if (_moderators.count(fd))
		return (true);
	return (false);
}


bool				Channel::isInvited(int fd) const
{
	if (_invited.count(fd))
		return (true);
	return (false);
}


bool				Channel::canJoin(std::string srv, User *user, size_t index) const
{
	bool	isInviteOnly = hasMode('i');
	if (isInviteOnly && !isInvited(user->getFd()))
		user->reply(ERR_INVITEONLYCHAN(srv, getName()));
	else if (getNbUsers(true) + 1 > getMaxUsers())
		user->reply(ERR_CHANNELISFULL(srv, user->getNick(), getName()));
	else if (isBanned(user->getFd()))
		user->reply(ERR_BANNEDFROMCHAN(srv, getName()));
	else if (hasPassword() && !pwMatches(user, index))
		user->reply(ERR_BADCHANNELKEY(srv, getName()));
	else
		return (true);
	return (false);
}

//----------------------------- MUTATORS / SETTERS ----------------------------

void				Channel::setPassword(std::string password)
{
	_password = password;
}


void				Channel::setMaxUsers(size_t maxUsers)
{
	if (maxUsers < getNbUsers(true))
		std::cout << YEL << WARNING_MAX_CHANNEL << getName() << END << std::endl;
	_maxUsers = maxUsers;
}


void				Channel::addUser(User *user)
{
	if (!isIn(user->getFd()))
		_users[user->getFd()] = user;
}


void				Channel::delUser(User *user)
{
	if (isIn(user->getFd()))
		_users.erase(user->getFd());
}


void				Channel::addOp(User *user)
{
	if (!isOp(user->getFd()))
		_ops[user->getFd()] = user;
}


void				Channel::delOp(User *user)
{
	if (isOp(user->getFd()))
		_ops.erase(user->getFd());
}


void				Channel::ban(User *user)
{
	if (!isBanned(user->getFd()))
		_banned[user->getFd()] = user;
}


void				Channel::unban(User *user)
{
	if (isBanned(user->getFd()))
		_banned.erase(user->getFd());
}


void				Channel::addMod(User *user)
{
	if (!isMod(user->getFd()))
		_moderators[user->getFd()] = user;
}


void				Channel::delMod(User *user)
{
	if (isMod(user->getFd()))
		_moderators.erase(user->getFd());
}


void				Channel::invite(User *user)
{
	if (!isInvited(user->getFd()))
		_invited[user->getFd()] = user;
}


void				Channel::rmInvite(User *user)
{
	if (isInvited(user->getFd()))
		_invited.erase(user->getFd());
}


void				Channel::setTopic(User *user, std::string topic)
{
	std::stringstream	tmp;
	tmp << time(NULL);
	_topic = topic;
	_topicCtxt = user->getNick() + " " + tmp.str();
}


void				Channel::addMode(char new_mode)
{
	if (_mode.find(new_mode) == std::string::npos)
		_mode.append(1, new_mode);
}


void				Channel::rmMode(char old_mode)
{
	if (_mode.find(old_mode) != std::string::npos)
		_mode.erase(_mode.find(old_mode), 1);
}


//----------------------------------- MODES -----------------------------------


void				Channel::updateMode(Server *srv, User *user, bool adding, char letter)
{
	std::string		mode_str = "";
	std::string		basic_modes = "psitnm";

	if (basic_modes.find(letter) != std::string::npos) {
		if (adding) {
			mode_str = "+";
			addMode(letter);
		} else {
			mode_str = "-";
			rmMode(letter);
		}
		mode_str += letter;
		broadcast(RPL_MODE(user->getNick(), getName(), mode_str));
	} else if (letter == 'b')
		_updateModeB(srv, user, adding);
	else if (letter == 'v')
		_updateModeO(srv->getSrv(), user, adding);
	else if (letter == 'o')
		_updateModeO(srv->getSrv(), user, adding);
	else if (letter == 'l')
		_updateModeL(srv->getSrv(), user, adding);
	else
		user->reply(ERR_UNKNOWNMODE(srv->getSrv(), letter));
}


void				Channel::_updateModeB(Server *srv, User *user, bool adding)
{
	std::string		my_str = adding ? "MODE +b" : "MODE -b";
	if (user->getArgs().size() < 3)
		return (user->reply(ERR_NEEDMOREPARAMS(srv->getSrv(), user->getNick(), my_str)));
	std::string		target_nick = user->getArgs()[2];
	User			*target = srv->getUser(target_nick);
	if (target == NULL)
		return (user->reply(ERR_NOSUCHNICK(srv->getSrv(), target_nick)));
	if (adding && !isOp(target->getFd())) {
		ban(target);
		broadcast(RPL_MODE(user->getNick(), getName(), "+b " + target_nick));
		if (isIn(target->getFd())) {
			broadcast(RPL_KICK(user->getNick(), getName(), target_nick));
			target->rmChannel(getName());
			delUser(target);
		}
	} else if (adding && isOp(target->getFd())) {
		std::cout << YEL << ERR_BAN_OP << user->getNick() << END << std::endl;
	} else if (!adding && isBanned(target->getFd())) {
		unban(target);
		broadcast(RPL_MODE(user->getNick(), getName(), "-b " + target_nick));
	}
}


void				Channel::_updateModeO(std::string srv, User *user, bool adding)
{
	std::string		my_str = adding ? "MODE +o" : "MODE -o";
	if (user->getArgs().size() < 3)
		return (user->reply(ERR_NEEDMOREPARAMS(srv, user->getNick(), my_str)));
	std::string		target_nick = user->getArgs()[2];
	User			*target = getUser(target_nick);
	if (target == NULL)
		return (user->reply(ERR_NOSUCHNICK(srv, target_nick)));
	if (adding && !isOp(target->getFd())) {
		my_str = "+o " + target_nick;
		addOp(target);
		broadcast(RPL_MODE(user->getNick(), getName(), my_str));
	} else if (!adding && isOp(target->getFd())) {
		my_str = "-o " + target_nick;
		delOp(target);
		broadcast(RPL_MODE(user->getNick(), getName(), my_str));
	}
}


void				Channel::_updateModeV(std::string srv, User *user, bool adding)
{
	std::string		my_str = adding ? "MODE +v" : "MODE -v";
	if (user->getArgs().size() < 3)
		return (user->reply(ERR_NEEDMOREPARAMS(srv, user->getNick(), my_str)));
	std::string		target_nick = user->getArgs()[2];
	User			*target = getUser(target_nick);
	if (target == NULL)
		return (user->reply(ERR_NOSUCHNICK(srv, target_nick)));
	if (adding && !isMod(target->getFd())) {
		my_str = "+v " + target_nick;
		addMod(target);
		broadcast(RPL_MODE(user->getNick(), getName(), my_str));
	} else if (!adding && isMod(target->getFd())) {
		my_str = "-v " + target_nick;
		delMod(target);
		broadcast(RPL_MODE(user->getNick(), getName(), my_str));
	}
}


void				Channel::_updateModeL(std::string srv, User *user, bool adding)
{
	std::string		mode_str = "-l";

	if (adding)
	{
		if (user->getArgs()[1].find_first_of("bmo") != std::string::npos)
			return (user->reply(ERR_TOOMANYMODES(srv, user->getNick(), "MODE +l")));
		if (user->getArgs().size() < 3)
			return (user->reply(ERR_NEEDMOREPARAMS(srv, user->getNick(), "MODE +l")));
		if (user->getArgs()[2][0] == '-')
			return (user->reply(ERR_INVALID_MAX(srv, user->getNick(), "MODE +l")));
		size_t		new_limit = str_to_size_t(user->getArgs()[2]);
		if (new_limit == 0)
			return (user->reply(ERR_INVALID_MAX(srv, user->getNick(), "MODE +l")));
		setMaxUsers(new_limit);
		addMode('l');
		mode_str = "+l " + size_t_to_str(new_limit);
	} else {
		size_t max_size = (size_t) - 1;
		setMaxUsers(max_size);
		rmMode('l');
	}
	broadcast(RPL_MODE(user->getNick(), getName(), mode_str));
}
