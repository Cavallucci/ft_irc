/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:29:39 by llalba            #+#    #+#             */
/*   Updated: 2022/11/04 17:57:22 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

# include <iostream>
# include <string>
# include "Server.hpp"

# define RED			"\033[1;31m"
# define GRN			"\033[1;32m"
# define YEL			"\033[1;33m"
# define BLU			"\033[1;34m"
# define MAG			"\033[1;35m"
# define CYN			"\033[1;36m"
# define WHT			"\033[1;37m"
# define END			"\033[0m"

# define ERR_NB_ARG		"❌ Invalid command, usage: ./ircserv <port> <password>"
# define ERR_PORT_NB	"❌ Invalid port number, an integer is expected"
# define ERR_PORT_RANGE	"❌ Invalid port, it should be in the range of 0 to 65535"
# define RUNNING		"✅ The server is up and running"

#endif
