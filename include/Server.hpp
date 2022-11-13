/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:35:04 by llalba            #+#    #+#             */
/*   Updated: 2022/11/13 23:02:28 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"

class Server
{
	public:
		typedef std::vector<pollfd>::iterator	pfds_it;
		typedef void							(Server::*ServerFnPtr)( User * );
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
		std::vector<struct pollfd>			_pfds;
		std::vector<>						_users;
		// Server set up methods
		void								_serverSetUp(void);
		void								_serverConnect(void);
		bool								_parseInput(User *user);
		// Methods related to commands management
		char								_ascii_to_lower(char in);
		void								_initCommands(void);
		void								msgCmd(User *user);
};

std::ostream &	operator<<(std::ostream & o, Server const & e);

#endif
