# IRC

## Introduction

IRC est un **protocole de communication textuelle instantanée** sur Internet. Il peut aussi être utilisé pour faire du transfert de fichier. Conçu en **1988**, son utilisation diminue avec l'arrivée des messageries instantanées propriétaires grands publics comme **ICQ** (*freeware* originellement développé par des israéliens) puis **MSN Messenger**. L'IRC est un protocole utilisant **TCP** et de façon optionnelle **TLS**.

La plupart des serveurs IRC **ne nécessitent pas l'enregistrement par l'intermédiaire d'un compte**, l'utilisateur pouvant choisir son surnom librement tant qu'il est disponible. Le protocole m'a défini que des caractères de taille défini sur 8 bits, limitant les différents encodages possibles ainsi que le nombre de caractères. L'élément de base pour communiquer sur un réseau est **le canal**. Un canal est défini par une liste d'utilisateur connectés sur celui-ci. Pour entrer dans un canal ou pour créer un canal (s'il n'existe pas), l'utilisateur utilise la commande `join`. Les canaux peuvent être vus de tout le réseau, leur nom est alors préfixé par le caractère `#`. Ils peuvent aussi être des canaux locaux, portés par un serveur uniquement, leur nom est dans ce cas préfixé par le caractère `&`.

[WeeChat](https://weechat.org/) est un exemple de client IRC. Il est disponible sur Linux, Windows, mac os. [irssi](https://irssi.org/) est un aussi client IRC, de type CLI. [Kiwiirc](https://kiwiirc.com/) en est encore un autre.

## Commands

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

- Protocol : [RFC1459](https://tools.ietf.org/html/rfc1459)
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
