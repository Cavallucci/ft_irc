/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:54:58 by llalba            #+#    #+#             */
/*   Updated: 2022/11/16 15:46:18 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "irc.hpp"

class Channel
{
	public:
		Channel();
		Channel(const Channel & src);
		~Channel();
		Channel &	operator=(Channel const & rhs);
		// Getters & accessors
		std::string		getName(void) const;
		std::string		getTopic(void) const;
		bool			isIn(int fd) const;
		bool			isOp(int fd) const;
		bool			isBanned(int fd) const;
		bool			isMod(int fd) const;
		// Setters & mutators
		void			addUser(User *user);
		void			delUser(User *user);
		void			addOp(User *user);
		void			delOp(User *user);
		void			ban(User *user);
		void			unban(User *user);
		void			addMod(User *user);
		void			delMod(User *user);

	private:
		std::string		_name;
		std::string		_topic;
		usr_map			_users;
		usr_map			_ops;
		usr_map			_banned;
		usr_map			_moderators;
};

std::ostream &	operator<<(std::ostream & o, Channel const & e);

#endif
