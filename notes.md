# IRC

## Introduction

IRC est un **protocole de communication textuelle instantanée** sur Internet. Il peut aussi être utilisé pour faire du transfert de fichier. Conçu en **1988**, son utilisation diminue avec l'arrivée des messageries instantanées propriétaires grands publics comme **ICQ** (*freeware* originellement développé par des israéliens) puis **MSN Messenger**. L'IRC est un protocole utilisant **TCP** et de façon optionnelle **TLS**.

La plupart des serveurs IRC **ne nécessitent pas l'enregistrement par l'intermédiaire d'un compte**, l'utilisateur pouvant choisir son surnom librement tant qu'il est disponible. Le protocole m'a défini que des caractères de taille défini sur 8 bits, limitant les différents encodages possibles ainsi que le nombre de caractères. L'élément de base pour communiquer sur un réseau est **le canal**. Un canal est défini par une liste d'utilisateur connectés sur celui-ci. Pour entrer dans un canal ou pour créer un canal (s'il n'existe pas), l'utilisateur utilise la commande `join`. Les canaux peuvent être vus de tout le réseau, leur nom est alors préfixé par le caractère `#`. Ils peuvent aussi être des canaux locaux, portés par un serveur uniquement, leur nom est dans ce cas préfixé par le caractère `&`.

[WeeChat](https://weechat.org/) est un exemple de client IRC. Il est disponible sur Linux, Windows, mac os. [irssi](https://irssi.org/) est un aussi client IRC, de type CLI. [Kiwiirc](https://kiwiirc.com/) en est encore un autre.

## Commands to be implemented

| Commands	| Effect |
|:----------|:-------|
| `/help`	| Displays all available commands |
| `/join <channel>`	| Joins a channel |
| `/nick <nickname>`	| Changes your nickname |
| `/quit`	| Exits IRC all together |
| `/msg <nick> <message>`	| Sends a private message to the specified nickname |
| `/list`	| Gives all channels on IRC |
| `/oper <nick>`	| Authenticates a user as an IRC operator on that server |
| `/die`	| Instructs the server to shut down |
| `/user <username> <hostname> <:realname>`	| Used at the beginning of a connection to specify the username, hostname, real name of the connecting client |
| `/part <channels>`	| Causes a user to leave the channels in the comma-separated list `channels`.

The full list of commands is available [here](https://en.wikipedia.org/wiki/List_of_Internet_Relay_Chat_commands).

## Official documentation

- Original protocol : [RFC1459](https://tools.ietf.org/html/rfc1459)
- Architecture : [RFC2810](https://tools.ietf.org/html/rfc2810)
- Channel Management : [RFC2811](https://tools.ietf.org/html/rfc2811)
- Client : [RFC2812](https://tools.ietf.org/html/rfc2812)
- Server Protocol : [RFC2813](https://tools.ietf.org/html/rfc2813)
- Default Port for IRC via TLS/SSL : [RFC7194](https://tools.ietf.org/html/rfc7194)

## Useful repositories

- [marijnvanh](https://github.com/marijnvanh/ft_IRC)
- [lusehair](https://github.com/lusehair/ft_irc)
- [execrate0](https://github.com/execrate0/ft_irc)

## Other resources and tutorials

- [What is a socket?](https://beej.us/guide/bgnet/html/)
- [chirc, a tutorial of the University of Chicago](http://chi.cs.uchicago.edu/chirc/intro.html)

## Useful functions (from man)

- **`int socket(int domain, int type, int protocol);`** returns a file descriptor that refers to that endpoint (or -1 on error). The `domain` argument specifies the protocol family which will be used for communication. The `type` argument specifies the communication semantics. The `protocol` specifies a particular protocol to be used with the socket.
- **`ssize_t send(int sockfd, const void *buf, size_t len, int flags);`** is useful to transmit a message to another socket. The only difference between `send` and `write` is the presence of flags. On success, returns the number of bytes sent. On error. -1 is returned.
- **`ssize_t recv(int sockfd, void *buf, size_t len, int flags);`** useful to receive messages from a connected socket. The only difference between `recv` and `read` is the presence of flags. Returns the number of bytes received or -1 on error.
- **`getsockopt`** and **`setsockopt`** manipulate options for a socket.
- **`struct protoent *getprotobyname(const char *name);`** returns a `protoent` structure for the entry from the DB that matches the protocol `name`.
- **`getaddrinfo`** returns one or more `addrinfo` structures, each of which contains an Internet address that can be specified in a call to `bind` or `connect`.
- **`freeaddrinfo`** frees the memory that was allocated for the dynamically allocated linked list `res` which points to the socket address structures.
- **`int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);`** assigns a name to a socket. When a socket is created, it exists in a name space (address family) but has no address assigned to it. `bind()` assigns the address `addr` to the socket referred to by `sockfd`.
- **`int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);`** system call which initiates a connection on a socket referred to by `sockfd`. Returns 0 on success, -1 on error.
- **`int listen(int sockfd, int backlog);`** marks the socket as a passive socket, that is, as a socket that will be used to accept incoming connection requests using `accept`.
- **`int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);`** extracts the first connection request on the queue of pending connections for the listening socket, creates a new connected socket and returns a new `fd` referring to that socket (not in the listening state). The original socket is unaffected by this call.
- **`htons`**, **`htonl`**, **`ntohs`**, **`ntohl`** convert integers from network byte order (most significant byte first, as used on the Internet) to host byte order or vice versa.
- **`in_addr_t inet_addr(const char *cp);`** converts the Internet host address `cp` from IPv4 numbers-and-dots notation into binary data in network byte order.
- **`char *inet_ntoa(struct in_addr in);`** conversely, converts the Internet host address `in`, given in network byte order, to a string in IPv4 dotted-decimal notation.
- **`off_t lseek(int fd, off_t offset, int whence);`** repositions the file offset of the open fd associated with `fd` to the argument `offset`.
- **`fstat`** retrieves information about a file specified by a file descriptor.
- **`fcntl`** performs operations (e.g. duplications) on an open `fd`.
- **`int poll(struct pollfd *fds, nfds_t nfds, int timeout);`** waits for one of a set of fd (`fds`) to become ready to perform I/O.
- **`select`** (poll equivalent) monitors multiple fd, waiting until one or more of them become "ready" for some class of I/O operation (e.g., input possible).
- **`kqueue`** (poll equivalent)
- **`epoll`** (poll equivalent)

## Stream sockets vs. datagram sockets

There are mainly 2 types of Internet sockets: "*stream sockets*"  (`SOCK_STREAM`) and "*datagram sockets*" aka "*connectionless sockets*" (`SOCK_DGRAM`).
**Stream sockets** are reliable two-way connected communication streams. If you output 2 items into the socket in the order "*1, 2*" they will arrive in the order "*1, 2*". The telnet and the Hypertext Transfer Protocol use stream sockets. Stream sockets rely on the "*Transmission Control Protocol*", otherwise known as TCP. TCP makes sure your data arrives sequentially and error-free, it is responsible for data integrity.
**Datagram sockets** use the *User Datagram Protocol* (UDP) which is much faster. Sample applications: multiplayer games, streaming audio, video conferencing, etc. `tftp` and `dhcpcd` use UDP but with their own protocol on top of UDP. For each packet that gets sent, the recipient has to send back an *ACK packet*.

## Byte order

Endianness is the order or sequence of bytes of a word of digital data in computer memory. It is primarily expressed as **big-endian** or **little-endian**. A big-endian system stores the most significant byte of a word at the smallest memory address (left to right). Intel-compatible processor store the bytes reversed. The more-sane Big-Endian is also called **Network Byte Order**. Our computers store numbers in **Host Byte Order**. Depending on the machine, host byte order can be either big-endian or little-endian.



http://chi.cs.uchicago.edu/chirc/irc_examples.html

https://beej.us/guide/bgnet/html/#client-server-background
