#ifndef HUFFMAN_C_HUFFMAN_TREE_H
#define HUFFMAN_C_HUFFMAN_TREE_H

#include "huffman.h"
typedef struct noeud
{
    char caractere;
    int occurrence;
    char *codage;
    int nb_bits_codage;
    struct noeud *fils_gauche;
    struct noeud *fils_droit;
} noeud;

/* Prototypage des fonctions*/
void initialiser_arbre_huffman(noeud *arbre_huffman[NB_CARACTERES]);

noeud *creer_feuille(int *tab, int index);

#endif
