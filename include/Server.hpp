/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:35:04 by llalba            #+#    #+#             */
/*   Updated: 2022/11/10 14:03:47 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "irc.hpp"

class Server
{
	public:
		Server();
		Server(int port, std::string pwd);
		Server(const Server & src);
		~Server();
		Server &	operator=(Server const & rhs);
	private:
		int		_port;
		std::string		_pwd;
		std::string		_host;
		std::string		_pwd;
		int				_listener;

		void			_setUp(void);
		int				_printIP(std::string host);
};

std::ostream &	operator<<(std::ostream & o, Server const & e);

typedef int		(Server::*ServerFnPtr)( void );

typedef int		(Server::*ServerCstFnPtr)( void ) const;

#endif
