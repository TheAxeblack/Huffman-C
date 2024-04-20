#include <stdio.h>
#include <stdlib.h>
#include "huffman_tree.h"
#include "compression.h"

void usage(char *nom_prog) {
    fprintf(stderr, "Usage: %s <fichier>\n", nom_prog);
    exit(EXIT_FAILURE);
}

void occurence(FILE *fic, int tab[NB_CARACTERES]) {
    int c;
    while ((c = fgetc(fic)) != EOF)
        tab[c]++;
}

void chercher_deux_plus_petits(noeud_s *tab[], int taille, int *index1, int *index2) {
    int min1 = INT_MAX, min2 = INT_MAX;

    *index1 = -1;
    *index2 = -1;

    if (taille < 2) {
        fprintf(stderr, "Erreur : taille inferieure a 2\n");
        return;
    }

    for (int i = 0; i < NB_CARACTERES; i++) {
        if (tab[i] != NULL) {
            if (tab[i]->occurrence < min1) {
                min2 = min1;
                min1 = tab[i]->occurrence;
                *index2 = *index1;
                *index1 = i;
            } else if (tab[i]->occurrence < min2) {
                min2 = tab[i]->occurrence;
                *index2 = i;
            }
        }
    }
}

void affichage_code(int nbr_bits, int codage) {
    for (int i = nbr_bits - 1; i >= 0; i--) // Pour chaque bit du codage
    {
        int bit = (codage >> i) & 1; // On récupère le bit de poids fort
        fprintf(stdout, "%d", bit);  // On affiche le bit de poids fort
    }
    fprintf(stdout, "\n"); // On saute une ligne
}


int main(int argc, char **argv) {
    FILE *fp;                             // Fichier à compresser
    int occurrences[NB_CARACTERES] = {0}; // Initialisation du tableau d'occurences à 0
    int nb_elements = 0;                  // Nombre d'éléments dans l'arbre
    noeud_s *arbre_huffman[NB_CARACTERES];// Tableau des noeuds de l'arbre de Huffman
    noeud_s *alphabet[NB_CARACTERES];     // Tableau des noeuds de l'alphabet
    FILE *fic_compresse;                  // Fichier compressé

    if (argc < 2)
        usage(argv[0]);

    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    occurence(fp, occurrences);

    fclose(fp);

    initialiser_arbre_huffman(arbre_huffman);

    for (int i = 0; i < NB_CARACTERES; i++) {
        if (occurrences[i] > 0) {
            nb_elements++;
            arbre_huffman[i] = creer_feuille(occurrences, i);
        }
    }

    for (int i = 0; i < NB_CARACTERES; i++) {
        if (arbre_huffman[i] != NULL) {
            if (arbre_huffman[i]->caractere == ' ')
                fprintf(stdout, "Caractere : ESPACE (%d), Occurrence : %d\n", arbre_huffman[i]->caractere,
                        arbre_huffman[i]->occurrence);
            else
                fprintf(stdout, "Caractere : %c (%d), Occurrence : %d\n", arbre_huffman[i]->caractere,
                        arbre_huffman[i]->caractere, arbre_huffman[i]->occurrence);
        }
    }

    while (nb_elements > 1) {
        creer_noeud(arbre_huffman, nb_elements);
        nb_elements--;
    }

    fprintf(stdout, "\nAffichage de l'arbre de Huffman (apres reduction):\n");

    for (int i = 0; i < NB_CARACTERES; i++) {
        if (arbre_huffman[i] != NULL) {
            if (arbre_huffman[i]->caractere == ' ')
                fprintf(stdout, "Caractere : ESPACE (%d), Occurrence : %d\n", arbre_huffman[i]->caractere,
                        arbre_huffman[i]->occurrence);
            else
                fprintf(stdout, "Caractere : %c (%d), Occurrence : %d\n", arbre_huffman[i]->caractere,
                        arbre_huffman[i]->caractere, arbre_huffman[i]->occurrence);
        }
    }

    // On initialise le tableau des noeuds de l'alphabet
    for (int i = 0; i < NB_CARACTERES; i++)
        alphabet[i] = NULL;

    // On crée le codage de chaque caractère
    for (int i = 0; i < NB_CARACTERES; i++) {
        if (arbre_huffman[i] != NULL) {
            fprintf(stdout, "\nCodage de %c (%d) : ", arbre_huffman[i]->caractere, arbre_huffman[i]->caractere);
            creer_code(arbre_huffman[i], 0, 0, alphabet);
        }
    }

    // On appelle la fonction pour écrire l'entête dans le fichier compressé
    fic_compresse = fopen("fichier_compresse.bin", "wb"); // On ouvre le fichier compressé en mode binaire pour écriture
    if (fic_compresse == NULL) {
        fprintf(stderr, "Erreur lors de la création du fichier compressé.\n");
        exit(EXIT_FAILURE);
    }
    ecrire_entete(fic_compresse, alphabet, argv[1]); // On écrit l'entête dans le fichier compressé
    fclose(fic_compresse);                           // On ferme le fichier compressé

    // On appelle la fonction pour écrire les codes des caractères dans le fichier compressé
    // On ouvre le fichier compressé en mode binaire pour ajout (écriture à la fin)
    fic_compresse = fopen("fichier_compresse.bin", "ab");

    // On vérifie si l'ouverture du fichier compressé a réussi
    if (fic_compresse == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier compressé pour l'écriture des codes des caractères.\n");
        exit(EXIT_FAILURE);
    }
    // On écrit les codes dans le fichier compressé
    ecrire_codes_caracteres(fic_compresse, argv[1], alphabet);

    // On ferme le fichier compressé
    fclose(fic_compresse);

    exit(EXIT_SUCCESS); // On quitte le programme
}