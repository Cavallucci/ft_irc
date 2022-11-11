/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:35:04 by llalba            #+#    #+#             */
/*   Updated: 2022/11/11 12:44:18 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"
# include "User.hpp"

class Server
{
	public:
		Server();
		Server(std::string port, std::string pwd);
		Server(const Server & src);
		~Server();
		Server &	operator=(Server const & rhs);

	private:
		std::string							_port;
		std::string							_host;
		std::string							_pwd;
		int									_listener;
		std::map<std::string, UserFnPtr>	_commands;
		void								_setUp(void);
		int									_printIP(std::string host);
};

std::ostream &	operator<<(std::ostream & o, Server const & e);

#endif
