// Louis : fichier pour faire des tests
// c++ -Wall -Wextra -Werror -std=c++98 tmp.cpp && ./a.out "CMD mes arguments"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

typedef std::vector<std::string>			str_vec;

str_vec		split_str(std::string initial, std::string separator)
{
	str_vec			output;
	size_t			pos = 0;
	std::string		tmp;
	while ((pos = initial.find(separator)) != std::string::npos) {
		tmp = initial.substr(0, pos);
		output.push_back(tmp);
		initial.erase(0, pos + separator.length());
	}
	output.push_back(initial);
	return output;
}

std::string		ft_to_string(size_t	nb)
{
	std::stringstream	ss;
	ss << nb;
	return (ss.str());
}


size_t			ft_to_size_t(std::string my_str)
{
	std::stringstream	sstream(my_str);
	size_t				result;
	sstream >> result;
	return (result);
}



int main(int argc, char **argv)
{
	if (argc == 2)
	{
		std::string input = argv[1];
        // input.append("\r\n");
		std::string::size_type pos = input.find(' '); // might be string::npos
		std::string cmd_str = input.substr(0, pos);
		// std::string test = input.substr(pos + 1, std::string::npos);
		// size_t	result = ft_to_size_t(test);
		std::string arg;
		str_vec		args;
		if (input.length() != cmd_str.length())
		{
			arg = input.substr(pos + 1, std::string::npos);
			args = split_str(arg, " ");
		}
		std::cout << "command: [" << cmd_str << "]" << std::endl;
		std::cout << "args: [";
		size_t		nth = 0;
		for (str_vec::const_iterator i = args.begin(); i != args.end(); ++i, ++nth)
		{
			std::cout << "{" << *i << "} " << nth << std::endl;
		}
		std::cout << "]" << std::endl;
	}
	else
		std::cout << "Wrong number of arguments" << std::endl;
	return (0);
}






/*
==894697== Invalid read of size 4
==894697==    at 0x42AE0A: User::reply(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /mnt/nfs/homes/llalba/irc/ircserv)
==894697==    by 0x411E7E: Channel::broadcast(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /mnt/nfs/homes/llalba/irc/ircserv)
==894697==    by 0x42459A: Server::_quitHandler(User*) (in /mnt/nfs/homes/llalba/irc/ircserv)
==894697==    by 0x406929: Server::_parseInput(User*) (in /mnt/nfs/homes/llalba/irc/ircserv)
==894697==    by 0x404F37: Server::_serverConnect() (in /mnt/nfs/homes/llalba/irc/ircserv)
==894697==    by 0x40476F: Server::Server(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /mnt/nfs/homes/llalba/irc/ircserv)
==894697==    by 0x40411C: main (in /mnt/nfs/homes/llalba/irc/ircserv)
==894697==  Address 0x4ded7ec is 220 bytes inside a block of size 312 free'd
==894697==    at 0x484B8AF: operator delete(void*) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==894697==    by 0x405BFF: Server::_deleteUser(int) (in /mnt/nfs/homes/llalba/irc/ircserv)
==894697==    by 0x404F59: Server::_serverConnect() (in /mnt/nfs/homes/llalba/irc/ircserv)
==894697==    by 0x40476F: Server::Server(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (in /mnt/nfs/homes/llalba/irc/ircserv)
==894697==    by 0x40411C: main (in /mnt/nfs/homes/llalba/irc/ircserv)


*/
