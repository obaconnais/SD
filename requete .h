#include <stdio.h>
#include "calculs.h"

int Socket;

enum requete_t{
  FIN = 0,
  FACTORIELLE,
  PUISSANCE,
  STATS
};

typedef struct requete{
  enum requete_t type_requete;
  int taille_requete;
}Segment;

void Print_Seg(Segment seg){
  printf("le type de requete est : ");
  switch(seg.type_requete){
    case 0: printf("FIN");
            break;
    case 1: printf("FACTORIELLE");
            break;
    case 2: printf("PUISSANCE");
            break;
    case 3: printf("STATS");
            break;
  }
  printf("\nle contenu est: %d", seg.taille_requete);
}

long Factorielle(int nombre){
  Segment req;        /* requete envoyée */
  char* message;      /* message à envoyer */
  int taille_msg;     /* taille du message */
  long resultat;      /* résultat de l'opération */

  /* intialisation de la requete */
  req.type_requete = FACTORIELLE;
  req.taille_requete = sizeof(int);

  /* l'envoi sera de taille Segment + int */
  taille_msg = sizeof(Segment) + sizeof(int);
  message = (char*) malloc(taille_msg);
  memcpy(message,&req,sizeof(Segment));
  memcpy(message+sizeof(Segment),&nombre, sizeof(int));


  if(write(Socket, message, taille_msg)<=0){
    perror("error, from FACTORIELLE can't write message\n");
    free(message);
    return -1;
  }

  if(read(Socket,&resultat, sizeof(long))<=0){
    perror("error, from FACTORIELLE, can't read answer\n");
    free(message);
    return -1;
  }

  free(message);
  return resultat;
}

long Puissance(int nombre, int puissance){
  Segment req;
  char* message;
  long resultat;
  int taille_msg;

  /* initialisation de la requête*/
  req.type_requete = PUISSANCE;
  req.taille_requete = sizeof(int);

  taille_msg = sizeof(Segment) + sizeof(int) + sizeof(int);
  message = (char*) malloc(taille_msg);

  /*envoie de la valeur*/
  memcpy(message,&req,sizeof(Segment));
  memcpy(message+sizeof(Segment),&nombre,sizeof(int));
  memcpy(message+sizeof(Segment)+sizeof(int),&puissance, sizeof(int));

  if(write(Socket,message,taille_msg)<=0){
    perror("write opération Puissance failed\n");
    exit(1);
  }

  if(read(Socket,&resultat,sizeof(long))<=0){
    perror("read operation Puissance failed\n");
    exit(1);
  }
  free(message);
  return resultat;
}

Statistique Check_data(int data[],int Array_size){
  Segment req;
  Statistique resul;
  int taille_msg;
  char* message;

  /* initialisation de la requête */
  req.type_requete = STATS;
  req.taille_requete = sizeof(Statistique);

  /* prération du message pour envoi au serveur*/
  taille_msg = sizeof(Segment) + sizeof(int)+sizeof(int)*Array_size;
  message = (char*)malloc(taille_msg);
  memcpy(message,&req,sizeof(Segment));
  memcpy(message+sizeof(Segment), &Array_size, sizeof(int));
  memcpy(message+sizeof(Segment)+sizeof(int),data,sizeof(int)*Array_size);


  /*envoi des données au serveur*/
  if(write(Socket, message,taille_msg)<=0){
    perror("write opération Statistique failed\n");
    exit(1);
  }

  if(read(Socket,&resul,sizeof(Statistique))<=0){
    perror("read opération Statistique failed\n");
    exit(1);
  }
  return resul;
}

void Fin(){
  Segment req;
  char* message;
  int taille_msg;

  /* init requete */
  req.type_requete = FIN;
  req.taille_requete = 0;

  /* définition de la taille du message*/
  taille_msg = sizeof(Segment);
  message = malloc(taille_msg);
  memcpy(message,&req,sizeof(Segment));
  write(Socket, message,taille_msg);
  free(message);
}
