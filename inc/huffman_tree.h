#ifndef HUFFMAN_C_HUFFMAN_TREE_H
#define HUFFMAN_C_HUFFMAN_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define NB_CARACTERES 256

typedef struct noeud {
    char caractere;
    int occurrence;
    int codage;
    int nb_bits_codage;
    struct noeud *fils_gauche;
    struct noeud *fils_droit;
} noeud_s;


/* Prototypage des fonctions*/
// Afficher l'usage
void usage(char *nom_prog);

// Initialiser le tableau des occurrences à zéro
void initialiser_occurrences(int tab[NB_CARACTERES]);

// Compter le nombre d'occurrences de chaque caractère
void occurrence(FILE *fic, int tab[NB_CARACTERES]);

// Initialiser l'arbre de Huffman
void initialiser_arbre_huffman(noeud_s *arbre_huffman[NB_CARACTERES]);

// Créer une feuille
noeud_s *creer_feuille(int *tab, int index);

// Chercher les deux plus petits éléments
void chercher_deux_plus_petits(noeud_s *tab[], int taille, int *index1, int *index2);

// Créer un nœud
void creer_noeud(noeud_s *tab[], int taille);

// Afficher le code
void affichage_code(int nbr_bits, int codage);

// Créer le code de chaque caractère
void creer_code(noeud_s *element, int code, int profondeur, noeud_s *aplhabet[NB_CARACTERES]);

// Est-ce que le nœud est une feuille ?
int est_feuille(noeud_s *element);

// Afficher l'arbre
void afficher_arbre(noeud_s *element, int profondeur);

#endif // HUFFMAN_C_HUFFMAN_TREE_H
