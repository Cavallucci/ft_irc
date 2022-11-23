/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:20:15 by llalba            #+#    #+#             */
/*   Updated: 2022/11/23 11:45:29 by llalba           ###   ########.fr       */
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
		if (argc != 3) {
			throw std::runtime_error(ERR_NB_ARG);
		}
		std::string		port_str = argv[1];
		if (port_str.empty() || \
			port_str.find_first_not_of("0123456789") != std::string::npos)
		{
			throw std::runtime_error(ERR_PORT_NB);
		}
		int				port = std::atoi(argv[1]);
		if (port < 0 || port > 65535)
		{
			throw std::runtime_error(ERR_PORT_RANGE);
		}
		if (!argv[2][0])
		{
			throw std::runtime_error(ERR_EMPTY_PWD);
		}
		running = true;
		signal(SIGINT, signHandler); // TODO a supprimer ?
		std::string		password = argv[2];

		Server	server(argv[1], password);

		return (EXIT_SUCCESS);
	}
	catch (const std::exception &err)
	{
		std::cerr << RED << err.what() << END << std::endl;
		return (EXIT_FAILURE);
	}
}
