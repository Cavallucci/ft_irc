/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 17:19:36 by llalba            #+#    #+#             */
/*   Updated: 2022/11/21 15:10:18 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

char		ascii_to_lower(char in)
{
	if (in <= 'Z' && in >= 'A')
		return in - ('Z' - 'z');
	return (in);
}

str_vec		split_str(std::string initial, char delimiter)
{
	(void)delimiter; //TODO a enlever?
	std::stringstream	arg_stream(initial);
	str_vec				output;
	std::string			tmp;
	while (getline(arg_stream, tmp, delimiter))
		output.push_back(tmp);
	return (output);
}

std::string		join_vec(str_vec vec, std::string glue)
{
	std::string		output;
	size_t			i = vec.size() - 1;
	for (str_vec::iterator it = vec.begin(); it != vec.end(); ++it, --i)
	{
		output.append(*it);
		if (i)
			output.append(glue);
	}
	return (output);
}

bool		is_valid_channel_name(std::string chan, User *user, std::string srv)
{
	// channels names must begin with a '&' or '#' character
	if (chan[0] != '&' && chan[0] != '#')
	{
		user->reply(ERR_BADCHANMASK(srv, chan));
		return (false);
	}
	// channels names are strings of length up to 200 characters
	if (chan.size() > 200)
	{
		user->reply("TODO tester sur un vrai serveur IRC");
		return (false);
	}
	// channels names cannot contain a control G (ASCII 7) character
	if (chan.find(char(7)) != std::string::npos)
	{
		user->reply("TODO tester sur un vrai serveur IRC");
		return (false);
	}
	return (true);
}
