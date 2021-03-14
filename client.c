#include "common.h"

int main(int argc, char* argv []){
  short operation;                              /* type d'opération*/
  int opt;                                      /* traitement arguments */
  int i = 0;                                    /* variable pour remplissage tableau */
  int Array_size = 0;                           /* taille du tableau */
  int port = 4000;                              /* port d'affectation socket d'écoute */
  int nb_octets;                                /* nombre octet retourné */
  int valeur = 0;                               /* valeur pour opération  */
  int puissance = 0;                            /* valeur de la puissance pour operation puissance */
  int* data;                                    /* tableau d'entier pour la statistique */
  long resultat_pf = -1;                        /* résultat de l'opération */
  char* nom_serveur = "localhost";              /* nom du serveur */
  char* buffer[TAILLEBUF];                      /* buffer envoie de donnée */
  char* buf;                                    /* buffer de lecture fichier txt */
  char* message;                                /* message envoyé */
  Statistique resultat_stat;                    /* resultat des Statistique */
  FILE* Fdata;

  /*traitement des arguments*/
  if(argc!=0){
    while((opt=getopt(argc, argv,"p:hvn:f:u:t:s:i"))!=-1)
    {
      switch(opt){
        /* reglage du port */
        case 'p':
                  if(sscanf(optarg, "%d", &port) == 0)
                    fprintf(stderr, "conversion of argument -p: %s failed\n", optarg);
                  break;
        /* reglage du nom du serveur */
        case 'n':
                  nom_serveur = optarg;
                  break;
        /* affichage des aides */
        case 'h':
                  print_Opt();
                  break;
        /* affichage de la version */
        case 'v':
                  printf("last version 20210306 by Olivier BACONNAIS\n");
                  break;
        /* choix de l'opération Factorielle */
        case 'f':
                  if(sscanf(optarg, "%d" , &valeur) == 0)
                    fprintf(stderr, "conversion of argument -f: %s failed\n", optarg);
                  operation = FACTORIELLE;
                  break;
        /* choix de l'opération puissancen format valeur/puissance à respecter */
        case 'u':
                  if(sscanf(optarg, "%d/%d" , &valeur, &puissance) == 0)
                    fprintf(stderr, "conversion of argument -u: %s failed\n", optarg);
                  operation = PUISSANCE;
                  break;
        /* réglage de la taille du tableau */
        case 's': if(sscanf(optarg,"%d",&Array_size)<=0)
                    fprintf(stderr, "conversion of argument -s: %s failed\n", optarg);
                  break;
        /* remplissage du tableau à partir d'un fichier texte */
        case 't':
                  if((Fdata = fopen(optarg, "r+")) == NULL){
                    perror("coudn't open Fdata\n");
                    exit(1);
                  }
                  buf = (char*)malloc(TAILLEBUF);
                  data = malloc(sizeof(int)*Array_size);
                  while(fgets(buf,TAILLEBUF,Fdata) != NULL){
                    sscanf(buf,"%d",&data[i]);
                    i++;
                  }
                  operation = STATS;
                  free(buf);
                  break;
        case 'i':
                  operation = FIN;
                  break;
      }
    }
  }

  /* connexion socket client/serveur */
  if(Connexion_Socket(port, nom_serveur) == -1){
    perror("client can't connect to server\n");
    exit(1);
  }

  /* en fonction de l'opération choisi, on effectue l'une des opérations.*/
  switch(operation){
    case FACTORIELLE :
                      if((resultat_pf=Factorielle(valeur)) == -1){
                        perror("operation Factorielle failed\n");
                        exit(1);
                      }
                      printf("%ld\n", resultat_pf);
                      break;
    case PUISSANCE  :
                      if((resultat_pf = Puissance(valeur, puissance))==-1){
                          perror("operation Puissance failed\n");
                          exit(1);
                      }
                      printf("%ld\n", resultat_pf);
                      break;
    case STATS      :
                      resultat_stat = Check_data(data,Array_size);
                      free(data);
                      Print_stat(resultat_stat);
                      break;
    case FIN        :
                      Fin();
                      break;
  };

  close(Socket);
  return 0;
}
