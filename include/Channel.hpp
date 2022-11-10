/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:54:58 by llalba            #+#    #+#             */
/*   Updated: 2022/11/10 10:54:59 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# define RED									"\033[1;31m"
# define GRN									"\033[1;32m"
# define YEL									"\033[1;33m"
# define BLU									"\033[1;34m"
# define MAG									"\033[1;35m"
# define CYN									"\033[1;36m"
# define WHT									"\033[1;37m"
# define END									"\033[0m"
# define CALL_MEMBER_FN(object,ptrToMember)		((object).*(ptrToMember))

class Channel
{
	public:
		Channel();
		Channel(const Channel & src);
		~Channel();
		Channel &	operator=(Channel const & rhs);
	private:

};

std::ostream &	operator<<(std::ostream & o, Channel const & e);

typedef int		(Channel::*ChannelFnPtr)( void );

typedef int		(Channel::*ChannelCstFnPtr)( void ) const;

#endif
