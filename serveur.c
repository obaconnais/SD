#include "common.h"

int main(int argc, char* argv[]){
  int opt;                                      /* traitement arguments */
  int port = 4000;                              /* port d'affectation socket d'écoute */
  int socket_ecoute, socket_service;            /* descripteur de socket ecoute et service */
  int addr_length = sizeof(struct sockaddr_in); /* longueur d'adresse */
  char* message;                                /* message reçu par le serveur */
  static struct sockaddr_in addr_client;        /* structure adresse client */
  static struct sockaddr_in addr_serveur;       /* structure adresse serveur */


  /*traitement de arguments*/
  if(argc!=0){
    while((opt=getopt(argc, argv,"p:hv"))!=-1)
    {
      switch(opt){
        case 'p':
                  if(sscanf(optarg, "%d", &port) == 0)
                    fprintf(stderr, "conversion of argument %s failed\n", optarg);
                  break;
        case 'h':
                  print_Opt();
                  break;
        case 'v':
                  printf("last version 20210306 by Olivier BACONNAIS\n");
                  break;
      }
    }
  }

  /* création de la socket d'écoute */
  if((socket_ecoute = CreerSocketTCP(port)) == -1){
    perror("listen socket failed\n");
    exit(1);
  }

  /* configuration du nombre d'utilisateur en attente */
  if(listen(socket_ecoute,5)==-1){
    perror("configuration of number of users failed\n");
    exit(1);
  }

  /* ignore la disparation des fils */
  signal(SIGCHLD,SIG_IGN);

  while(1){
      /* attente de connexion d'un client */
      if((socket_service=accept(socket_ecoute,(struct sockaddr *)&addr_client,(socklen_t *)&addr_length))==-1){
        perror("the operation accept failed\n");
        exit(1);
      }
      if(fork() == 0){
        /* on est dans le fils, on ferme la socket d'ecoute */
        close(socket_ecoute);
        /* on gère le client */
        gererClient(socket_service, sortie_boucle);
        /* fermeture fils */
        close(socket_service);
        exit(0);
      }
      close(socket_service);
  }
  
  close(socket_ecoute);
  return 0;
}
