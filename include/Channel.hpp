/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:54:58 by llalba            #+#    #+#             */
/*   Updated: 2022/11/14 15:58:47 by llalba           ###   ########.fr       */
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
		// // Getters & accessors
		// std::string		getName(void);
		// std::string		getTopic(void);
		// // Setters & mutators
		// void			addUser(User *);
		// void			delUser(User *);
		// void			addOp(User *);
		// void			delOp(User *);
		// void			ban(User *);
		// void			unban(User *);
		// void			addMod(User *);
		// void			delMod(User *);
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
