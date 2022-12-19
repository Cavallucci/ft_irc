/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:54:58 by llalba            #+#    #+#             */
/*   Updated: 2022/12/19 10:05:22 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"

class Channel
{
	public:
		typedef void	(Channel::*ChannelFnPtr)( std::string, User *, bool );
		Channel();
		Channel(std::string name, std::string password);
		Channel(const Channel & src);
		~Channel();
		void			broadcast(std::string msg);
		void			rpl_whoreply(User *user, std::string srv);
		void			rpl_chan_mode(User *user, std::string srv);
		void			rpl_ban_list(User *user, std::string srv);
		void			rpl_names(User *user, std::string srv, bool send_end);
		void			msg(std::string srv, User *user, std::string content, bool silently);
		void			clearAll(void);
		// Getters & accessors
		usr_map			getUsers(void) const;
		User			*getUser(std::string nick) const;
		std::string		getName(void) const;
		std::string		getTopic(void) const;
		size_t			getNbUsers(bool with_invisible) const;
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
		std::string		getMode(void) const;
		// Setters & mutators
		void			addUser(User *user);
		void			delUser(User *user);
		void			addOp(User *user);
		void			delOp(User *user);
		void			ban(User *user);
		void			unban(User *user);
		void			addMod(User *user);
		void			delMod(User *user);
		void			setMaxUsers(size_t maxUsers);
		void			invite(User *user);
		void			rmInvite(User *user);
		void			setPassword(std::string password);
		void			setTopic(User *user, std::string topic);
		void			updateMode(std::string srv, User *user, bool adding, char letter);
		void			addMode(char new_mode);
		void			rmMode(char old_mode);
	private:
		std::string								_name;
		std::string								_topic;
		usr_map									_users;
		usr_map									_ops;
		usr_map									_banned;
		usr_map									_moderators;
		usr_map									_invited;
		size_t									_maxUsers;
		std::string								_password;
		std::string								_topicCtxt; // topic setter & timestamp
		/*
		CHANNELS MODES
		'o' = operator status
		'p' = private, topic won't be fully displayed in the LIST, NAMES and WHO output
		's' = secret, won't be displayed in the LIST, NAMES and WHO output
		'i' = invite-only channel
		'b' = channel ban mask
		'm' = moderated channel, only moderators & operators may speak
		'l' = maximum number of users in the channel
		't' = topic protection, ability to modify the topic
		'n' = no external messages, users not on the channel cannot send messages to it
		*/
		std::string								_mode;
		// Handlers to update channel modes
		std::map<char, ChannelFnPtr>			_modeHandlers;
		void									_initModeHandlers(void);
		void									_updateModeO(std::string srv, User *user, bool adding);
		void									_updateModeP(std::string srv, User *user, bool adding);
		void									_updateModeS(std::string srv, User *user, bool adding);
		void									_updateModeI(std::string srv, User *user, bool adding);
		void									_updateModeB(std::string srv, User *user, bool adding);
		void									_updateModeM(std::string srv, User *user, bool adding);
		void									_updateModeL(std::string srv, User *user, bool adding);
		void									_updateModeT(std::string srv, User *user, bool adding);
		void									_updateModeN(std::string srv, User *user, bool adding);
};


#endif
