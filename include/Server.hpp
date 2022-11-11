/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 17:35:04 by llalba            #+#    #+#             */
/*   Updated: 2022/11/11 12:26:12 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <iostream>
# include <string>
# include <cstring>
# include "User.hpp"
# define RED									"\033[1;31m"
# define GRN									"\033[1;32m"
# define YEL									"\033[1;33m"
# define BLU									"\033[1;34m"
# define MAG									"\033[1;35m"
# define CYN									"\033[1;36m"
# define WHT									"\033[1;37m"
# define END									"\033[0m"
# define CALL_MEMBER_FN(object,ptrToMember)		((object).*(ptrToMember))

class Server
{
	public:
		Server();
		Server(int port, std::string pwd);
		Server(const Server & src);
		~Server();
		Server &	operator=(Server const & rhs);
	private:
		int									_port;
		std::string							_pwd;
		std::string							_host;
		std::map<std::string, UserFnPtr>	_commands;
		void								_setUp(void);
		int									_printIP(std::string host);
};

std::ostream &	operator<<(std::ostream & o, Server const & e);

#endif
