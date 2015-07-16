#define _XOPEN_SOURCE 700
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <netdb.h>
#include "../include/afficher_erreur.h"
 
int cree_socket_ecoute(int port){ 
  int sock;              /* socket */  
  struct sockaddr_in addr; /* adresse IPv4 d'ecoute */
  int one = 1;             /* utiliser avec setsockopt */
  
  /* cree une socket TCP */
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock==-1) afficher_erreur("echec de socket");
  
  /* evite le delai entre deux bind successifs sur le meme port */
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one))==-1)
    afficher_erreur("echec de setsockopt(SO_REUSEADDR)");

  /* remplit l'adresse addr */
  memset(&addr, 0, sizeof(addr));  /* initialisation Ã  0 */
  addr.sin_family = AF_INET;       /* adresse Internet */
  addr.sin_port   = htons(port);   /* port: 2 octets en ordre de reseau */
  /* on accepte les connections sur toutes les adresses IP de la machine */
  addr.sin_addr.s_addr = htonl(INADDR_ANY); 

  /* ancre listen_fd a  l'adresse addr */
  if (bind(sock, (const struct sockaddr*) &addr, sizeof(addr))==-1)
    afficher_erreur("echec de bind");

  /* transforme liste_fd en une socket d'ecoute */
  if (listen(sock, 10)==-1) 
    afficher_erreur("echec de listen");
  
//  printf("Serveur actif sur le port  %i\n", port);

  return sock;
}
char* get_addr() {
    struct hostent * host;
      host = gethostbyname("djabir-PC" );  

return  inet_ntoa(*(struct in_addr *)host->h_addr);
}
