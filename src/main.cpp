/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 14:20:15 by llalba            #+#    #+#             */
/*   Updated: 2022/11/04 17:57:01 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int main(int argc, char **argv)
{
	try
	{
		if (argc != 3) {
			throw std::runtime_error(ERR_NB_ARG);
		}
		int		port = std::atoi(argv[1]);
		if (port < 0 || port > 65535)
		{
			throw std::runtime_error(ERR_PORT_RANGE);
		}
		// Server	server(port, argv[2]);
		// server.start();
		return (EXIT_SUCCESS);
	}
	catch (const std::exception &err)
	{
		std::cerr << RED << err.what() << END << std::endl;
		return (EXIT_FAILURE);
	}
}
