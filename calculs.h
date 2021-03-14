#include <stdio.h>
#include <stdlib.h>

typedef struct res_analyse_donnees {
  float moy;
  int max;
  int min;
}Statistique;

long factoriel(int nb);

void analyser_donnees(int donnees[], int taille, struct res_analyse_donnees *res);

long puissances(int nb, int puiss);
