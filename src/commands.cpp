/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 13:06:04 by llalba            #+#    #+#             */
/*   Updated: 2022/11/11 13:17:18 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::_initCommands(void)
{
	_commands["msg"] = &Server::msgCmd;
}

//------------------------------ SERVER COMMANDS ------------------------------

void	Server::msgCmd(User* user)
{
	(void) user;
	std::cout << "hello world" << std::endl;
}
