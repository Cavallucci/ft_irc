/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:54:58 by llalba            #+#    #+#             */
/*   Updated: 2022/11/23 18:03:56 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"

class Channel
{
	public:
		Channel();
		Channel(std::string name, std::string password);
		Channel(const Channel & src);
		~Channel();
		Channel &	operator=(Channel const & rhs);
		void			broadcast(std::string msg);
		void			rpl_whoreply(User *user, std::string srv);
		void			msg(std::string srv, User *user, std::string content, bool silently);
		// Getters & accessors
		std::string		getName(void) const;
		std::string		getTopic(void) const;
		size_t			getNbUsers(void) const;
		bool			isIn(int fd) const;
		bool			isOp(int fd) const;
		bool			isBanned(int fd) const;
		bool			isMod(int fd) const;
		size_t			getMaxUsers(void) const;
		bool			canJoin(std::string srv, User *user, size_t nth) const;
		bool			isInvited(int fd) const;
		bool			hasPassword() const;
		bool			pwMatches(User *user, size_t nth) const;
		std::string		getTopicCtxt(void) const;
		bool			hasMode(char c) const;
		// Setters & mutators
		void			addUser(User *user);
		void			delUser(User *user);
		void			addOp(User *user);
		void			delOp(User *user);
		void			ban(User *user);
		void			unban(User *user);
		void			addMod(User *user);
		void			delMod(User *user);
		void			setMode(std::string mode);
		void			invite(User *user);
		void			rmInvite(User *user);
		void			setPassword(std::string password);
		void			setTopic(std::string topic, std::string nick);
	private:
		std::string		_name;
		std::string		_topic;
		usr_map			_users;
		usr_map			_ops;
		usr_map			_banned;
		usr_map			_moderators;
		usr_map			_invited;
		size_t			_maxUsers;
		std::string		_password;
		std::string		_topicCtxt; // topic setter & timestamp
		/*
		CHANNELS MODES
		'o' = operator status
		'm' = moderated channel, only moderators & operators may speak
		'p' = private, channel not displayed in your WHOIS output
		's' = secret, channel will not be displayed in the LIST output
		'i' = invite-only channel
		't' = ability to modify the topic
		'n' = users not on the channel cannot send messages to it
		'b' = channel ban mask
		'v' = voice status
		*/
		std::string		_mode;
};

std::ostream &	operator<<(std::ostream & o, Channel const & e);

#endif
