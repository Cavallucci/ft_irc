/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:01 by llalba            #+#    #+#             */
/*   Updated: 2022/11/11 16:28:56 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "irc.hpp"

class User
{
	public:
		User();
		User(const User & src);
		~User();
		User &	operator=(User const & rhs);
		std::string			getInput(void);
		int					getFd(void);
		bool				setInput(void);
		void				resetInput(void);
	private:
		std::string			_nickname;
		std::string			_input;
		int					_fd;
};

std::ostream &	operator<<(std::ostream & o, User const & e);

#endif
