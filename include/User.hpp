/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:55:01 by llalba            #+#    #+#             */
/*   Updated: 2022/11/14 14:55:00 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

# include "irc.hpp"

class User
{
	public:
		User();
		User(int fd, struct sockaddr_storage	*addr);
		User(const User & src);
		~User();
		User &	operator=(User const & rhs);
		// Methods
		void								reply(std::string msg);
		void								welcome(bool silently);
		// Getters & accessors
		std::string							getNick(void) const;
		std::string							getHost(void) const;
		std::string							getUser(void) const;
		std::string							getReal(void) const;
		std::string							getServer(void) const;
		std::string							getInput(void) const;
		bool								hasBeenWelcomed(void) const;
		int									getFd(void) const;
		str_vec const &						getArgs(void) const;
		chan_map const						getChannels() const;
		Channel *							getChannel(std::string) const;
		// Setters & mutators
		void								setNick(std::string name);
		void								setHost(std::string name);
		void								setUser(std::string name);
		void								setReal(std::string name);
		void								setServer(std::string name);
		bool								setInput(void);
		void								resetInput(void);
		void								setFd(int fd);
		bool								setArgs(str_vec);
		bool								addChannel(std::string);
		bool								rmChannel(std::string);
		void								clearChannels();
	private:
		std::string							_nickname;
		std::string							_hostname;
		std::string							_username;
		std::string							_realname;
		std::string							_server;
		std::string							_input;
		// std::string							_mode; // TODO
		bool								_welcomed;
		int									_fd;
		str_vec								_args;
		chan_map							_channels;
		struct sockaddr_storage				*_addr;
};

std::ostream &	operator<<(std::ostream & o, User const & e);

#endif
