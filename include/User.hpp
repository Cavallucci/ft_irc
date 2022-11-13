/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:01 by llalba            #+#    #+#             */
/*   Updated: 2022/11/13 15:45:10 by llalba           ###   ########.fr       */
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
		std::string			getNickname(void);
		str_vec const &		getArgs(void) const;
		bool				setInput(void);
		bool				setArgs(str_vec);
		void				resetInput(void);
		void				reply(std::string msg);
	private:
		std::string			_nickname;
		std::string			_input;
		int					_fd;
		str_vec				_args;
};

std::ostream &	operator<<(std::ostream & o, User const & e);

#endif
