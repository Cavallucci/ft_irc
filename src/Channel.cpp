/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:05 by llalba            #+#    #+#             */
/*   Updated: 2022/12/15 19:36:37 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

//-------------------------------- CONSTRUCTORS -------------------------------

Channel::Channel()
{
	_initModeHandlers();
}


Channel::Channel(std::string name, std::string password) :
_name(name), _password(password)
{
	_initModeHandlers();
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

//--------------------------------- OVERLOAD ----------------------------------

Channel &				Channel::operator=(Channel const & rhs)
{
	// TODO completer cette fonction meme si a priori elle ne sert pas cf. User.cpp OVERLOAD
	(void)rhs;
	if (this != &rhs)
	{
		this->_name = rhs.getName();
		this->_maxUsers = rhs.getMaxUsers();
		this->setPassword(rhs._password);
		this->_topic = rhs._topic;
		this->_topicCtxt = rhs._topicCtxt;

		//maps
		this->_users = rhs._users;
		this->_ops = rhs._ops;
		this->_banned = rhs._banned;
		this->_moderators = rhs._moderators;
		this->_invited = rhs._invited;
	}
	return *this;
}


std::ostream &			operator<<(std::ostream & o, Channel const & e)
{
	// TODO completer cette fonction meme si a priori elle ne sert pas cf. User.cpp OVERLOAD
	o << YEL "Channel(" << &e << ") 🔶" END << std::endl;
	o << "🔸Name:			" << e.getName() << std::endl;
	o << "🔸Topic:			" << e.getTopic() << std::endl;
	o << "🔸Users:			";
	usr_map	usr = e.getUsers();
	for (usr_map::const_iterator it = usr.begin(); it != usr.end(); it++)
		o << "[" << it->first << "] ";
	o << std::endl;
	o << "🔸Ops:			";
	usr_map	ops = e.getUsers();
	for (usr_map::const_iterator it = ops.begin(); it != ops.end() && e.isOp(it->first); it++)
		o << "[" << it->first << "] ";
	o << std::endl;
	o << "🔸Banned:			";
	usr_map	ban = e.getUsers();
	for (usr_map::const_iterator it = ban.begin(); it != ban.end() && e.isBanned(it->first); it++)
		o << "[" << it->first << "] ";
	o << std::endl;
	o << "🔸Moderators:			";
	usr_map	mod = e.getUsers();
	for (usr_map::const_iterator it = mod.begin(); it != mod.end() && e.isMod(it->first); it++)
		o << "[" << it->first << "] ";
	o << std::endl;
	o << "🔸Invited:			";
	usr_map	inv = e.getUsers();
	for (usr_map::const_iterator it = inv.begin(); it != inv.end() && e.isInvited(it->first); it++)
		o << "[" << it->first << "] ";
	o << std::endl;
	return o;
}

//------------------------------- REPLY METHODS -------------------------------

void			Channel::broadcast(std::string msg, int ignore_fd)
{
	for (user_it it = _users.begin(); it != _users.end(); ++it) {
		if (it->second->getFd() != ignore_fd)
			it->second->reply(msg);
	}
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
				getName(), // channel
				it->second->getUser(),
				it->second->getHost(),
				it->second->getNick(),
				((isOp(it->second->getFd())) ? "@" : ""), // is operator or not ?
				it->second->getReal()
			));
		}
	}
}


void			Channel::rpl_ban_list(User *user, std::string srv)
{
	for (user_it it = _banned.begin(); it != _banned.end(); ++it)
		user->reply(RPL_BANLIST(srv, getName(), it->second->getNick()));
	user->reply(RPL_ENDOFBANLIST(srv, getName()));
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
	user->reply(RPL_NAMREPLY(srv, mode_char, getName(), output));
	if (send_end)
		user->reply(RPL_ENDOFNAMES(srv, getName()));
}


void			Channel::rpl_chan_mode(User *user, std::string srv)
{
	std::string			modes;
	std::string			parameters;

	for (size_t i = 0; i < _mode.size(); ++i)
	{
		if (i > 0)
			modes.append(" ");
		modes.append("+");
		modes += _mode[i];
		if (_mode[i] == 'l')
		{
			parameters = size_t_to_str(getMaxUsers()) ;
			modes += " " + parameters;
		}
	}
	user->reply(RPL_CHANNELMODEIS(srv, getName(), modes, parameters));
}


void			Channel::msg(std::string srv, User *user, std::string txt, bool silently)
{
	if (hasMode('m') && !(isOp(user->getFd()) || isMod(user->getFd())))
	{
		if (!silently)
			user->reply(ERR_CANNOTSENDTOCHAN(srv, getName()));
		return ;
	}
	for (user_it it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second != user) // the user doesn't want to write to himself
			it->second->reply(RPL_MSG(user->getNick(), getName(), txt));
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
	_modeHandlers.clear();
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
	if (_ops.count(fd))
		return (true);
	return (false);
}


bool				Channel::canJoin(std::string srv, User *user, size_t index) const
{
	bool	isInviteOnly = hasMode('i');
	if (isInviteOnly && !isInvited(user->getFd()))
		user->reply(ERR_INVITEONLYCHAN(srv, getName()));
	else if (getNbUsers(true) + 1 > getMaxUsers())
		user->reply(ERR_CHANNELISFULL(srv, getName()));
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

void				Channel::_initModeHandlers(void)
{
	_modeHandlers['o'] = &Channel::_updateModeO;
	_modeHandlers['p'] = &Channel::_updateModeP;
	_modeHandlers['s'] = &Channel::_updateModeS;
	_modeHandlers['i'] = &Channel::_updateModeI;
	_modeHandlers['b'] = &Channel::_updateModeB;
	_modeHandlers['m'] = &Channel::_updateModeM;
	_modeHandlers['l'] = &Channel::_updateModeL;
	_modeHandlers['t'] = &Channel::_updateModeT;
	_modeHandlers['n'] = &Channel::_updateModeN;
}


void				Channel::updateMode(std::string srv, User *user,
bool adding, char letter)
{
	try {
		CALL_MEMBER_FN(this, _modeHandlers.at(letter))(srv, user, adding);
	} catch (const std::out_of_range &e) {
		user->reply(ERR_UNKNOWNMODE(srv, letter));
	}
}


void				Channel::_updateModeO(std::string srv, User *user, bool adding)
{
	std::string		cmd = "MODE -o";
	if (adding)
		cmd = "MODE +o";
	if (user->getArgs().size() < 3)
		return (user->reply(ERR_NEEDMOREPARAMS(srv, user->getNick(), cmd)));
	std::string		target_nick = user->getArgs()[2];
	User			*target = getUser(target_nick);
	if (target == NULL)
		return (user->reply(ERR_NOSUCHNICK(srv, target_nick)));
	if (adding && !isOp(target->getFd())) {
		addOp(target);
		broadcast(RPL_CHANNELMODEIS(srv, getName(), "+o", target_nick), NON_FD);
	} else if (!adding && isOp(target->getFd())) {
		delOp(target);
		broadcast(RPL_CHANNELMODEIS(srv, getName(), "-o", target_nick), NON_FD);
	}
}


void				Channel::_updateModeP(std::string srv, User *user, bool adding)
{
	if (adding)
		addMode('p');
	else
		rmMode('p');
	(void)srv;
	(void)user;
}


void				Channel::_updateModeS(std::string srv, User *user, bool adding)
{
	if (adding)
		addMode('s');
	else
		rmMode('s');
	(void)srv;
	(void)user;
}


void				Channel::_updateModeI(std::string srv, User *user, bool adding)
{
	if (adding)
		addMode('i');
	else
		rmMode('i');
	(void)srv;
	(void)user;
}


void				Channel::_updateModeB(std::string srv, User *user, bool adding)
{
	if (user->getArgs().size() < 3) { // the list of banned users has been requested
		for (user_it it = _banned.begin(); it != _banned.end(); ++it)
			user->reply(RPL_BANLIST(srv, getName(), it->second->getNick()));
		return (user->reply(RPL_ENDOFBANLIST(srv, getName())));
	}
	// the user wants to ban or unban someone
	std::string		target_nick = user->getArgs()[2];
	User			*target = getUser(target_nick);
	if (target == NULL)
		return (user->reply(ERR_NOSUCHNICK(srv, target_nick)));
	if (adding && !isOp(target->getFd())) {
		ban(target);
		if (isIn(target->getFd())) {
			broadcast(RPL_KICK(user->getNick(), target_nick, getName()), NON_FD);
			target->rmChannel(getName());
		}
	} else if (!adding) {
		unban(target);
	}
}


void				Channel::_updateModeM(std::string srv, User *user, bool adding)
{
	if (adding)
		addMode('m');
	else
		rmMode('m');
	(void)srv;
	(void)user;
}


void				Channel::_updateModeL(std::string srv, User *user, bool adding)
{
	if (adding)
	{
		if (user->getArgs().size() < 3)
			return (user->reply(ERR_NEEDMOREPARAMS(srv, user->getNick(), "MODE +l")));
		size_t		new_limit = str_to_size_t(user->getArgs()[2]);
		setMaxUsers(new_limit);
		addMode('l');
	} else {
		size_t max_size = (size_t) - 1;
		setMaxUsers(max_size);
		rmMode('l');
	}
}


void				Channel::_updateModeT(std::string srv, User *user, bool adding)
{
	if (adding)
		addMode('t');
	else
		rmMode('t');
	(void)srv;
	(void)user;
}


void				Channel::_updateModeN(std::string srv, User *user, bool adding)
{
	if (adding)
		addMode('n');
	else
		rmMode('n');
	(void)srv;
	(void)user;
}
