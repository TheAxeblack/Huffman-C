#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman_tree.h"

int question1(void) {
    FILE *fp;
    char buffer[1000];

    fp = fopen("nom_du_fichier.txt", "r");

    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1; /* envoyer une erreur */
    }

    fread(buffer, sizeof(char), 1000, fp);  /* lecture des 1000 premiers caractères du fichier */

    fprintf(stdout, "%s", buffer);  /* affichage du contenu du fichier */

    fclose(fp);  /* fermeture du fichier */

    return 0;
}

int question2(void) {
    FILE *fp;
    int c;

    fp = fopen("nom_du_fichier.txt", "r");

    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    while ((c = fgetc(fp)) != EOF) {
        putchar(c);
    }
    fclose(fp);

    return 0;
}

void occurence(FILE *fic, int tab[256]) {
    int c;

    while ((c = fgetc(fic)) != EOF) {  // lecture caractère par caractère jusqu'à la fin de fichier
        tab[c]++;  // incrémentation du compteur d'occurrences correspondant au caractère lu
    }


}

noeud *creer_feuille(int *tab, int index) {
    noeud *n = (noeud *) malloc(sizeof(noeud));
    if (n == NULL) {
        printf("Erreur lors de l'allocation mémoire.\n");
        return NULL;
    }
    n->caractere = index;
    n->occurence = tab[index];
    n->codage = NULL;
    n->nb_bits_codage = 0;
    n->fils_gauche = NULL;
    n->fils_droit = NULL;
    return n;
}

int main() {
    FILE *fp;
    int occurences[256] = {0};
    int i;
    question1();
    question2();

    fp = fopen("nom_du_fichier.txt", "r");

    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return 1;
    }

    occurence(fp, occurences);

    // affichage des résultats
    printf("Occurrences des caractères :\n");
    for (i = 0; i < 256; i++) {
        if (occurences[i] > 0) {
            printf("Caractère %c : %d\n", i, occurences[i]);
        }
    }

    noeud *arbre_huffman[256];

/*    for (i = 0; i < 256; i++) {
        if (occurences[i] > 0) {
            noeud *n = malloc(sizeof(noeud));

            if (n == NULL) {
                printf("Erreur lors de l'allocation mémoire.\n");
                return 1;
            }

            n->caractere = i;
            n->occurence = occurences[i];
            n->codage = NULL;
            n->nb_bits_codage = 0;
            n->fils_gauche = NULL;
            n->fils_droit = NULL;
            arbe_huffman[i] = n;
            fprintf(stderr, "Caractère %c : %d\n", i, occurences[i]);
        }
    }*/

    for (i = 0; i < 256; i++) {
        if (occurences[i] > 0) {
            noeud *n = creer_feuille(occurences, i);
            arbre_huffman[i] = n;
            fprintf(stderr, "Caractère %c : %d\n", n->caractere, n->occurence);
        } else {
            arbre_huffman[i] = NULL;
        }
    }

    fclose(fp);

    return 0;
}
