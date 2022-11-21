/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:35:04 by llalba            #+#    #+#             */
/*   Updated: 2022/11/21 14:53:40 by llalba           ###   ########.fr       */
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
		// Getters & accessors
		std::string							getSrv(void) const;
		std::string							getPwd(void) const;
		usr_map								getUsers() const;
		User *								getUser(int fd) const;
		bool								fdAlreadyIn(int fd) const;
		User *								getUser(std::string nick) const;
		chan_map							getChannels() const;
		Channel *							getChannel(std::string chan_name) const;
		// Setters & mutators
		void								addChannel(Channel *chan);
		Channel								*newChan(User *user, std::string name, size_t nth);
		void								delChannel(Channel *chan);
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
		// Server set up methods
		void								_serverSetUp(void);
		void								_serverConnect(void);
		void								_addUser(void);
		void								_deleteUser(pfds_it &it);
		//void								_clientHandle(pfds_it &iterator);
		bool								_parseInput(User *user);
		// Methods related to commands management
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
