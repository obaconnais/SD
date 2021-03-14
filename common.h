#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <dirent.h>
#include <netinet/in.h>
#include <errno.h>
#include "requete.h"


#define TAILLEBUF 100

/* création d'une socket de base */
int CreerSocket(int port, int type){
  int sock;                              /* descripteur de socket */
  static struct sockaddr_in addr_local;  /* adresse local         */
  sock = socket(AF_INET, type,0);

  /* liaison de la socket sur le port local 4000 */
  bzero(&addr_local, sizeof(struct sockaddr_in));
  addr_local.sin_family = AF_INET;
  addr_local.sin_port = htons(port);
  addr_local.sin_addr.s_addr=htonl(INADDR_ANY);

  if(bind(sock, (struct sockaddr*)&addr_local, sizeof(addr_local))== -1 ) {
      perror("erreur bind");
      exit(1);
  }

  return sock;
};

/* création d'une socket TCP */
int CreerSocketTCP(int port){
  return CreerSocket(port,SOCK_STREAM);
}

/* création d'une socket UDP */
int CreerSocketUDP(int port){
  return CreerSocket(port,SOCK_DGRAM);
}

/* connexion de socket partie client */
int Connexion_Socket(int port, char* nom_serveur){

  struct hostent* serveur_host;                 /* structure décrivant la machine */
  static  struct sockaddr_in addr_serveur;      /* structure décrivant l'adresse serveur */

  /* récupération des informations du serveur via DNS */
  if((serveur_host = gethostbyname(nom_serveur)) == NULL){
    perror("get information about serveur failed\n");
    return -1;
  }

  /* mise à zero de l'ensemble des champs de l'adresse */
  bzero(&addr_serveur, sizeof(addr_serveur));

  /* configuration de l'adresse à partir des informations récupéré */
  addr_serveur.sin_family = AF_INET;
  addr_serveur.sin_port = htons(port);
  memcpy(&addr_serveur.sin_addr.s_addr,serveur_host->h_addr, serveur_host->h_length);
  /* création de la socket client */

  if((Socket = socket(AF_INET,SOCK_STREAM,0)) == -1){
    perror("creation of socket client failed\n");
    return -1;
  }

  if(connect(Socket,(struct sockaddr *)&addr_serveur,sizeof(struct sockaddr_in)) == -1){
    perror("socket client can't connect with server\n");
    return -1;
  }

  return 0;
}

/*affichage option*/
void print_Opt(){
  printf("\toption h: display help of this programm\n\n");
  printf("\toption v: display version of this programm\n\n");
  printf("\toption p: set port of TCP socket\n\n");
  printf("\toption f: set value of function FACTORIELLE\n");
  printf("\toption u: set value and power of function PUISSANCE\n");
  printf("\t\tformat option is as follow: value/power\n");

}

void Print_stat(Statistique stat){
  printf("max: %d\n", stat.max);
  printf("min: %d\n", stat.min);
  printf("moy: %f\n", stat.moy);

}

void gererClient(int socket_service, int sortie_boucle){
  int nbr_octet;                                /* nombre d'octet reçus par le serveur */
  int nb;                                       /* valeur pour les calculs */
  int puissance;                                /* puissance pour les calculs */
  int taille_tableau = 0;                       /* taille du tableau reçu */
  int* tableau;                                 /* tableau a traiter */
  long rep;                                     /* valeut de retour des calculs */
  Segment req;                                  /* requête à analyser */
  Statistique rep_stat;                         /* réponse faite au client */


  if((nbr_octet = read(socket_service,&req, sizeof(req))) == -1){
    perror("can't read requete from client\n");
    exit(1);
  }
  switch(req.type_requete){
    case FACTORIELLE  :
                        nbr_octet = read(socket_service,&nb,sizeof(int));
                        rep=factoriel(nb);
                        nbr_octet = write(socket_service,&rep,sizeof(long));
                        break;
    case PUISSANCE    :
                        nbr_octet = read(socket_service,&nb,sizeof(int));
                        nbr_octet = read(socket_service,&puissance,sizeof(int));
                        rep = puissances(nb, puissance);
                        nbr_octet = write(socket_service,&rep,sizeof(long));
                        break;
    case STATS        :
                        nbr_octet = read(socket_service,&taille_tableau,sizeof(int));
                        tableau = malloc(sizeof(int)*taille_tableau);
                        nbr_octet = read(socket_service,tableau,sizeof(int)*taille_tableau);
                        analyser_donnees(tableau,taille_tableau,&rep_stat);
                        nbr_octet = write(socket_service,&rep_stat,sizeof(Statistique));
                        free(tableau);
                        break;
    case FIN          :
                        break;
  }
}
