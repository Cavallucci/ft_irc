/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llalba <llalba@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 17:19:36 by llalba            #+#    #+#             */
/*   Updated: 2022/12/12 13:08:47 by llalba           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

char		ascii_to_lower(char in)
{
	if (in <= 'Z' && in >= 'A')
		return in - ('Z' - 'z');
	return (in);
}


str_vec		split_str(std::string initial, std::string separator, bool with_empty)
{
	str_vec			output;
	size_t			pos = 0;
	std::string		tmp;
	while ((pos = initial.find(separator)) != std::string::npos) {
		tmp = initial.substr(0, pos);
		if (!tmp.empty() || with_empty)
			output.push_back(tmp);
		initial.erase(0, pos + separator.length());
	}
	if (!initial.empty() || with_empty)
		output.push_back(initial);
	return output;
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


std::string		size_t_to_str(size_t	nb)
{
	std::stringstream	ss;
	ss << nb;
	return (ss.str());
}


size_t			str_to_size_t(std::string my_str)
{
	std::stringstream	sstream(my_str);
	size_t				result;
	// result = 0 when the argument isn't a number
	sstream >> result;
	return (result);
}
