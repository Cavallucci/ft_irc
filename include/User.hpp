/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:01 by llalba            #+#    #+#             */
/*   Updated: 2022/11/23 16:25:32 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "irc.hpp"

class User
{
	public:
		User();
		User(int fd, struct sockaddr_storage *addr);
		User(const User & src);
		~User();
		User &	operator=(User const & rhs);
		// Methods
		void								reply(std::string msg);
		void								welcome(std::string srv, bool silently);
		// Getters & accessors
		std::string							getInput(void) const;
		str_vec								getCommands(std::string input) const;
		std::string							getRawArgs(size_t skipped) const;
		str_vec const &						getArgs(void) const;
		std::string							getNick(void) const;
		std::string							getHost(void) const;
		std::string							getUser(void) const;
		std::string							getReal(void) const;
		bool								hasMode(char c) const;
		bool								hasBeenWelcomed(void) const;
		bool								isLoggedIn(void) const;
		int									getFd(void) const;
		chan_map const						getChannels() const;
		Channel *							getChannel(std::string) const;
		// Setters & mutators
		bool								setInput(void);
		str_vec								setCommands(std::string input);
		void								setRawArgs(std::string content);
		bool								setArgs(str_vec);
		void								setNick(std::string name);
		void								setHost(std::string name);
		void								setUser(std::string name);
		void								setReal(std::string name);
		void								logIn(void);
		void								resetInput(void);
		void								setFd(int fd);
		bool								addChannel(std::string);
		bool								rmChannel(std::string);
		void								clearChannels();
		void								setAddr(struct sockaddr_storage *addr);
	private:
		/*
		⏩ The parsing can be broken down into 4 steps:
		- STEP 1️⃣: raw user input -> _input
				"       NICK     nickname\r\nUSER username hostname server :realname"
		- STEP 2️⃣: input split by "\r\n" -> getCommands
				"NICK     nickname",
				"USER username hostname server :realname"
		- STEP 3️⃣ raw current command arguments -> _rawArgs, for each command:
				"nickname"
				"username hostname server :realname"
		- STEP 4️⃣ current command arguments split by " " -> _args, for each command:
				"nickname"
				"username", "hostname", "server", ":realname"
		*/
		std::string							_input;
		std::string							_rawArgs;
		str_vec								_args;
		std::string							_nickname;
		std::string							_hostname;
		std::string							_username;
		std::string							_realname;
		std::string							_mode;
		bool								_welcomed;
		bool								_loggedIn;
		int									_fd;
		chan_map							_channels;
		struct sockaddr_storage				*_addr;
};

std::ostream &	operator<<(std::ostream & o, User const & e);

#endif
