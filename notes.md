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

## Testing

Install `irssi` and `miniircd` (for instance) with the following commands:

```bash
sudo apt-get install irssi
git clone https://github.com/jrosdahl/miniircd.git
cd miniircd
```

Split your terminal and start the executables with the command `iirc` on one hand and `./miniircd --verbose --ports PORT_NUMBER` on the other hand.
On the IRSSI side, enter:

```irc
/network add NET_NAME
/server add -network NET_NAME localhost PORT_NUMBER
/connect NET_NAME
```

https://en.wikipedia.org/wiki/List_of_Internet_Relay_Chat_commands#PING

http://chi.cs.uchicago.edu/chirc/irc.html

https://beej.us/guide/bgnet/html/#client-server-background

https://github.com/jmartini89/42_irc

https://github.com/barimehdi77/ft_irc

https://ircgod.com

https://wiki.ircnow.org/index.php?n=Netcat.Irc

https://phil.lavin.me.uk/2012/02/how-to-use-netcat-a-netcat-tutorial/

https://gist.github.com/vivien/3b1c2efb80f3f3dde001

https://dd.ircdocs.horse/refs/commands/

https://matrix-org.github.io/matrix-appservice-irc/latest/irc_modes.html

http://abcdrfc.free.fr/rfc-vf/rfc1459.html

https://mathieu-lemoine.developpez.com/tutoriels/irc/protocole

http://riivo.planc.ee/irc/rfc/index.php?page=reply.php&rnr=436



_input.erase(_input.find_last_not_of(' ') + 1); // removes suffixing spaces
while (_input.rfind("  ") != std::string::npos) // removes duplicate spaces
	_input.replace(_input.find("  "), 2, " ");



/*
--------------------------- IRSSI official commands ---------------------------
COLOR CODE		MEANING
✅				done
🔴				mandatory
🔵				done by other students

accept			-> specifies who you want to receive private messages
action			-> sends an action emote to a nickname or a channel
admin			-> displays the administrative details of a server
alias			-> creates or updates an alias & the command to execute
away			-> marks yourself as ‘away’
ban				-> adds one or more bans to a channel
beep			-> outputs the bell-character
bind			-> adds or removes a binding (name & command to perform)
cat				-> displays the contents of the specified file
cd				-> changes the current active directory
channel			-> adds, removes or displays the configuration of channels
clear			-> scrolls up the text in the window and fills it with blank lines
completion		-> replaces or completes words or letters
connect			-> opens a new connexion to the specified network/server
ctcp			-> sends a CTCP request towards the given target nickname or channel
cycle			-> leaves and rejoins a channel
dcc				-> initiates direct CTC chat connections and file transfers
dehilight		-> removes the specified highlight from configuration
deop			-> removes the channel operator privileges from the given nicknames
devoice			-> removes the channel voice privilege from the given nicknames
die				-> terminates the IRC server (reserved for IRC operators)
disconnect		-> disconnect from an IRC server
echo			-> displays the given text
eval			-> evaluates the given commands and executes them
exec			-> executes the specified command in the background
flushbuffer		-> forces an immediate flush of the buffer
format			-> reconfigures the way messages are displayed
hash			-> (deprecated)
help			-> displays the documentation for the given command
hilight			-> highlight the keyword or pattern provided
ignore			-> ignores nicknames or text that matches a pattern
info			-> displays information about the IRC server software
invite			-> 🔵 invites the specified nick to a channel
ircnet			-> similar to `network`
ison			-> displays whether the specified nicknames are online
join			-> 🔴🔵 joins the given channels
kick			-> 🔵 removes the given nicknames from the specified channel
kickban			-> removes & bans the given nicknames from the specified channel
kill			-> terminates a nickname’s connection from the network
knock			-> sends an invitation request to the channel ops of the target
knockout		-> removes & then bans the given nicknames from the channel,
				the ban will be automatically lifted after the specified time.
lastlog			-> searches the active window for a pattern
layout			-> saves the layout of your window configuration
links			-> displays the links between an IRC server and its connections
list			-> 🔵 displays the channel names that match your request
load			-> loads a plugin
log				-> opens a log file and stores the targets messages into it
lusers			-> displays the user statistics of the active or remote server
map				-> displays the network map of an IRC network
me				-> sends an action emote to the active nickname or channel
mircdcc			-> enables mIRC compatibility mode
mode			-> 🔵 modifies the user or channel modes
motd			-> displays the welcome message of an IRC server
msg				-> 🔴🔵 sends a message to a nickname or channel
names			-> 🔵 displays the users who are in a channel
nctcp			-> sends a CTCP reply to a nickname or channel
netsplit		-> displays some information about users who are currently lost
				in one or more net splits
network			-> displays/adds/modifies/removes the configuration of networks
nick			-> 🔴🔵 changes your nickname on the active server
notice			-> 🔵 sends a notice to the target nickname or channel, often
				used in automated bots or scripts
notify			-> notifies you when a nickname/host comes online or offline
op				-> grants the channel operator privileges to the given nicknames
oper			-> grants you the IRC operator status
part			-> 🔵 leaves the given channels
ping			-> 🔵 sends a CTCP PING request to a nickname or a channel
query			-> starts a private conversation with a nickname
quit			-> 🔵 terminates the application and advertises the given
				message on all the networks you are connected to.
quote			-> sends raw data to the server without any parsing
rawlog			-> saves all the raw data that is received from and transmitted
				to the active server into a log file
recode			-> recodes the data transmitted to and received from nicknames
				and channels into a specific charset.
reconnect		-> disconnect and reconnect from a network
rehash			-> reloads the configuration of the IRC server
reload			-> reloads the Irssi configuration file
restart			-> restarts the active IRC server (restricted to admin)
rmreconns		-> removes all active and pending reconnections.
rmrejoins		-> removes all active and pending join requests
save			-> saves the configuration file
sconnect		-> connects a server to the IRC network (restricted to IRC ops)
script			-> interact with the Perl engine to execute scripts
scrollback		-> manipulates the text to go to a to the given line number
server			-> displays/adds/modifies/removes the network configuration
servlist		-> list the network services currently present on the network
set				-> modifies the value of a setting
silence			-> manages the server side ignore list; users or hostnames that
				match an entry on the list are not able to send you any
				messages or invites
squery			-> sends a query to the specified service
squit			-> disconnects a server from the IRC network (restricted to ops)
stats			-> displays statistics from the IRC server
statusbar		-> allows adjustment of the attributes and items of a statusbar
time			-> displays the local time of the server
toggle			-> modifies a setting to its counter value
topic			-> 🔵 displays or modifies the topic of a channel
trace			-> displays the list of servers that connects you to a server
ts				-> displays a list of the channels you are on and their topics
unalias			-> removes an alias
unban			-> a channel & the nicknames, hostnames to unban
unignore		-> removes an entry from the ignore list
unload			-> removes a module from the memory
unnotify		-> removes an entry from the notify list
unquery			-> closes a query window for a nickname
unsilence		-> removes an entry from the silence list
upgrade			-> upgrades to a new version
uptime			-> displays how long Irssi has been running
userhost		-> displays the user@host for the given nicknames
ver				-> queries a remote client about the version of the client they
				are using
version			-> displays the version and compatibility parameters of the
				given server,
voice			-> grants the channel voice privileges to the given nicknames
wait			-> waits the specified amount of ms before sending the next command
wall			-> sends a message to all channel operators
wallops			-> sends a network wide message to all the users in the 'w' usermod
who				-> 🔵 displays information about users in the specified channel
whois			-> displays information about users in the specified channels
whowas			-> displays historical user information
window			-> manipulates the window layout and positioning attributes

----------------------------- IRC server commands -----------------------------

PASS			-> 🔴🔵
USER			-> 🔴🔵
*/


https://stackoverflow.com/questions/12886573/implementing-irc-rfc-how-to-respond-to-mode





TODO
- tester rpl_chan_mode
- strinstream faut-il les clear ?
- supprimer tmp.cpp
- ::cout, fix-me et to-do
