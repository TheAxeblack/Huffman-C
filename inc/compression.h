#ifndef HUFFMAN_C_COMPRESSION_H
#define HUFFMAN_C_COMPRESSION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman_tree.h"

/* Prototypage des fonctions */
// Ecrire l'entête du fichier compressé
void ecrire_entete(FILE *fic_compresse, noeud_s *alphabet[NB_CARACTERES], char *nom_ficher);

// Ecrire les codes des caractères dans le fichier compressé
void ecrire_codes_caracteres(FILE *fic_compresse, char *nom_fichier, noeud_s *alphabet[NB_CARACTERES]);

#endif //HUFFMAN_C_COMPRESSION_H
