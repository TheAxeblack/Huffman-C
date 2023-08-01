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
void initialiser_arbre_huffman(noeud *arbre_huffman[NB_CARACTERES]); // Initialiser l'arbre de Huffman

noeud *creer_feuille(int *tab, int index); // Créer une feuille

void creer_noeud(noeud *tab[], int taille); // Créer un noeud

void creer_code(noeud *element, int code, int profondeur, noeud *aplhabet[NB_CARACTERES]); // Créer le code de chaque caractère

void ecrire_entete(FILE *fic_compresse, noeud *alphabet[256], char *nom_ficher); // Ecrire l'entête du fichier compressé

void ecrire_codes_caracteres(FILE *fic_compresse, char *nom_fichier, noeud *alphabet[256]); // Ecrire les codes des caractères dans le fichier compressé

#endif // HUFFMAN_C_HUFFMAN_TREE_H
