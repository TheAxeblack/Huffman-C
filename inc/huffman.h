#ifndef HUFFMAN_C_HUFFMAN_H
#define HUFFMAN_C_HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define NB_CARACTERES 256
#define TAILLE_MAX 1000

void usage(char *nom_prog);

void occurence(FILE *fic, int tab[NB_CARACTERES]);

#endif // HUFFMAN_C_HUFFMAN_H
