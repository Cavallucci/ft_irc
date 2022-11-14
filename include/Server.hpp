/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:35:04 by llalba            #+#    #+#             */
/*   Updated: 2022/11/14 15:36:53 by llalba           ###   ########.fr       */
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
		std::string							_name;
		int									_listener;
		std::map<std::string, ServerFnPtr>	_commands;
		std::vector<struct pollfd>			_pfds;
		usr_map								_users;
		chan_map							_channels;
		// Getters & accessors
		std::string							_getName(void) const;
		usr_map								_getUsers() const;
		User *								_getUser(int fd) const;
		User *								_getUser(std::string nick) const;
		chan_map							_getChannels() const;
		// Setters & mutators
		void								_addChannel(Channel *chan);
		void								_delChannel(Channel *chan);
		// Server set up methods
		void								_serverSetUp(void);
		void								_serverConnect(void);
		bool								_parseInput(User *user);
		// Methods related to commands management
		char								_ascii_to_lower(char in);
		void								_initCommands(void);
		void								_inviteCmd(User* user);
		void								_joinCmd(User* user);
		void								_kickCmd(User* user);
		void								_listCmd(User* user);
		void								_modeCmd(User* user);
		void								_msgCmd(User* user);
		void								_namesCmd(User* user);
		void								_nickCmd(User* user);
		void								_noticeCmd(User* user);
		void								_partCmd(User* user);
		void								_passCmd(User* user);
		void								_pingCmd(User* user);
		void								_quitCmd(User* user);
		void								_topicCmd(User* user);
		void								_userCmd(User* user);
		void								_whoCmd(User* user);
};

std::ostream &	operator<<(std::ostream & o, Server const & e);

#endif
