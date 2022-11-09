#include "server.hpp"

server::server() : _port(""), _host(""), _pwd("")
{}

server::server(std::string port, std::string pwd): _port(port), _pwd(pwd), _host("localhost")
{
    _setUp();
    _printIP(_host);
}

server::~server()
{}

void    server::_setUp(void)
{
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo; // will point to the results

    memset(&hints, 0, sizeof(hints)); // make sure the struct is empty

    hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_PASSIVE; // fill in my IP for me

    if ((status = getaddrinfo(NULL, "3490", &hints, &servinfo)) !=0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n",
        gai_strerror(status));
        exit(1);
    }
    // servinfo now points to a linked list of 1 or more struct
    addrinfos
    // ... do everything until you don't need servinfo anymore
    freeaddrinfo(servinfo); // free the linked-list
}

int    server::printIP(std::string host)
{
    struct addrinfo hints, *res, *p;
	int status;
	char ipstr[INET6_ADDRSTRLEN];

	if (!host) 
    {
	    fprintf(stderr,"usage: showip hostname\n");
	    return 1;
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo(host, NULL, &hints, &res)) != 0) 
    {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 2;
	}

	printf("IP addresses for %s:\n\n", host);

	for(p = res;p != NULL; p = p->ai_next) 
    {
		void *addr;
		char *ipver;

		// get the pointer to the address itself,
		// different fields in IPv4 and IPv6:
		if (p->ai_family == AF_INET) { // IPv4
			struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		} else { // IPv6
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}

		// convert the IP to a string and print it:
		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		printf("  %s: %s\n", ipver, ipstr);
	}

	freeaddrinfo(res); // free the linked list

	return 0;
}