/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:01 by llalba            #+#    #+#             */
/*   Updated: 2022/12/20 12:00:04 by llalba           ###   ########.fr       */
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
		// Methods
		void								reply(std::string msg);
		void								welcome(std::string srv, bool silently);
		// Getters & accessors
		static bool							isValidMode(char mode); // class method
		std::string							getInput(void) const;
		str_vec								getCommands(std::string input) const;
		std::string							getRawArgs(size_t skipped) const;
		str_vec const &						getArgs(void) const;
		std::string							getNick(void) const;
		std::string							getHost(void) const;
		std::string							getUser(void) const;
		std::string							getReal(void) const;
		std::string							getMode(void) const;
		bool								hasMode(char c) const;
		bool								hasBeenWelcomed(void) const;
		bool								isLoggedIn(void) const;
		int									getFd(void) const;
		chan_map const						getChannels(void) const;
		Channel *							getChannel(std::string) const;
		str_vec								getReplies(void) const;
		// Setters & mutators
		void								clearReplies(void);
		bool								setInput(void);
		void								setRawArgs(std::string content);
		void								setArgs(str_vec args);
		void								setNick(std::string name);
		void								setHost(std::string name);
		void								setUser(std::string name);
		void								setReal(std::string name);
		void								addMode(char new_mode);
		void								rmMode(char new_mode);
		void								logIn(void);
		void								resetInput(void);
		void								setFd(int fd);
		void								registerChannel(Channel *chan);
		bool								rmChannel(std::string);
		void								clearAll(void);
		void								setAddr(struct sockaddr_storage *addr);
	private:
		static const std::string			_valid_modes;
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
		bool								_welcomed;
		bool								_loggedIn;
		int									_fd;
		chan_map							_channels;
		struct sockaddr_storage				*_addr;
		/*
		USERS MODES
		'i' = invisible, will be hidden to other users in NAMES and WHO output
		*/
		std::string							_mode;
		str_vec								_replies;
};

#endif
