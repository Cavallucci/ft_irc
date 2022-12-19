/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:35:04 by llalba            #+#    #+#             */
/*   Updated: 2022/12/19 15:12:18 by llalba           ###   ########.fr       */
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
		// Getters & accessors
		std::string							getSrv(void) const;
		std::string							getPwd(void) const;
		usr_map								getUsers(void) const;
		User								*getUser(int fd) const;
		User								*getUser(std::string nick) const;
		chan_map							getChannels() const;
		Channel								*getChannel(std::string chan_name) const;
		// Setters & mutators
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
		void								_deleteUser(int fd);
		//void								_clientHandle(pfds_it &iterator);
		bool								_parseInput(User *user);
		void								_closeAll(void);
		// Methods related to commands management
		void								_printHandler(User* user);
		void								_initHandlers(void);
		void								_capHandler(User *user);
		void								_inviteHandler(User* user);
		void								_joinHandler(User* user);
		void								_kickHandler(User* user);
		void								_listHandler(User* user);
		void								_modeHandler(User* user);
		void								_msgHandler(User* user, bool silently);
		void								_namesHandler(User* user);
		void								_nickHandler(User* user);
		void								_noticeHandler(User* user);
		void								_partHandler(User* user);
		void								_passHandler(User* user);
		void								_pingHandler(User* user);
		void								_privMsgHandler(User *user);
		void								_quitHandler(User* user);
		void								_topicHandler(User* user);
		void								_userHandler(User* user);
		void								_whoHandler(User* user);
};

#endif
