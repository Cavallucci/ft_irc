/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:35:04 by llalba            #+#    #+#             */
/*   Updated: 2022/11/11 14:22:28 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"

class Server
{
	public:
		typedef void						(Server::*ServerFnPtr)( User * );
		Server();
		Server(std::string port, std::string pwd);
		Server(const Server & src);
		~Server();
		Server &	operator=(Server const & rhs);

	private:
		// Class variables
		std::string							_port;
		std::string							_host;
		std::string							_pwd;
		int									_listener;
		std::map<std::string, ServerFnPtr>	_commands;
		// Server set up methods
		void								_setUp(void);
		int									_printIP(std::string host);
		bool								_parseInput(User *user);
		// Methods related to commands management
		void								_initCommands(void);
		void								msgCmd(User *user);
};

std::ostream &	operator<<(std::ostream & o, Server const & e);

#endif
