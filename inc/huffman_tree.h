
#ifndef HUFFMAN_C_HUFFMAN_TREE_H
#define HUFFMAN_C_HUFFMAN_TREE_H

typedef struct noeud{
    char caractere;
    int occurence;
    char *codage;
    int nb_bits_codage;
    struct noeud *fils_gauche;
    struct noeud *fils_droit;
} noeud;


#endif
