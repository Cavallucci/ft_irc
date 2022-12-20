/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:20:15 by llalba            #+#    #+#             */
/*   Updated: 2022/12/12 13:10:25 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

extern bool running;

void	signHandler(int signum)
{
	(void)signum;
	running = false;
}

int main(int argc, char **argv)
{
	try
	{
		if (argc != 3)
			throw std::runtime_error(ERR_NB_ARG);
		std::string		port_str = argv[1];
		if (port_str.empty() || \
			port_str.find_first_not_of("0123456789") != std::string::npos)
			throw std::runtime_error(ERR_PORT_NB);
		int				port = str_to_size_t(argv[1]);
		if (port < 1024 || port > 65535)
			throw std::runtime_error(ERR_PORT_RANGE);
		// the password cannot be blank
		if (!argv[2][0])
			throw std::runtime_error(ERR_EMPTY_PWD);
		std::string		password = argv[2];
		// the password cannot contain spaces
		if (password.find(' ') != std::string::npos)
			throw std::runtime_error(ERR_INVALID_PWD);
		running = true;
		signal(SIGINT, signHandler);
		Server	server(argv[1], password);
		return (EXIT_SUCCESS);
	}
	catch (const std::exception &err)
	{
		std::cerr << RED << err.what() << END << std::endl;
		return (EXIT_FAILURE);
	}
}
