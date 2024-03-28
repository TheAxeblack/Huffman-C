#include "huffman_tree.h"

void initialiser_arbre_huffman(noeud_s *arbre_huffman[NB_CARACTERES]) {
    for (int i = 0; i < NB_CARACTERES; i++) {
        arbre_huffman[i] = NULL;
    }
}

noeud_s *creer_feuille(int *tab, int index) {
    noeud_s *feuille = (noeud_s *) malloc(sizeof(noeud_s));
    if (feuille == NULL) {
        fprintf(stderr, "Erreur allocation memoire pour la feuille\n");
        exit(EXIT_FAILURE);
    }
    feuille->caractere = index;
    feuille->occurrence = tab[index];
    feuille->codage = NULL;
    feuille->nb_bits_codage = 0;
    feuille->fils_gauche = NULL;
    feuille->fils_droit = NULL;
    return feuille;
}

void creer_noeud(noeud_s *tab[], int taille) {
    int index1, index2;   // Indices des deux plus petits éléments
    noeud_s *nouveau_noeud; // Nouveau noeud_s

    chercher_deux_plus_petits(tab, taille, &index1, &index2); // On cherche les deux plus petits éléments

    // On alloue la mémoire pour le nouveau noeud_s
    nouveau_noeud = (noeud_s *) malloc(sizeof(noeud_s));
    // Si l'allocation échoue
    if (nouveau_noeud == NULL) {
        fprintf(stderr, "Erreur allocation memoire pour le noeud_s\n");
        exit(EXIT_FAILURE);
    }

    // On initialise le caractère à '\0'
    nouveau_noeud->caractere = '\0';
    // On initialise l'occurrence à la somme des occurrences des deux plus petits élément
    nouveau_noeud->occurrence = tab[index1]->occurrence + tab[index2]->occurrence;
    // On initialise le codage à NULL
    nouveau_noeud->codage = NULL;
    // On initialise le nombre de bits du codage à 0
    nouveau_noeud->nb_bits_codage = 0;
    nouveau_noeud->fils_gauche = tab[index1];
    nouveau_noeud->fils_droit = tab[index2];

    // On remplace le premier plus petit élément par le nouveau noeud_s
    tab[index1] = nouveau_noeud;
    // On décale les éléments suivants
    for (int i = index2; i < taille - 1; i++) {
        tab[i] = tab[i + 1];
    }
}

void creer_code(noeud_s *element, int code, int profondeur, noeud_s *aplhabet[NB_CARACTERES]) {
    if (element == NULL)
        return;

    // Si on est sur une feuille
    if (element->fils_gauche == NULL && element->fils_droit == NULL) {
        // Masque pour récupérer le bit de poids fort
        int masque;
        // On alloue la mémoire pour le codage
        element->codage = (char *) malloc(sizeof(char) * profondeur);
        // Si l'allocation échoue
        if (element->codage == NULL) {
            fprintf(stderr, "Erreur allocation memoire pour le codage\n");
            exit(EXIT_FAILURE);
        }
        // On stocke le nombre de bits du codage
        element->nb_bits_codage = profondeur;

        // On décale le bit de poids fort à la bonne position
        masque = 1 << (profondeur - 1);
        // Pour chaque bit du codage
        for (int i = 0; i < profondeur; i++) {
            // On récupère le bit de poids fort
            element->codage[i] = ((code & masque) == 0) ? '0' : '1';
            // On décale le masque d'un bit vers la droite
            masque >>= 1;
        }

        // On ajoute le caractère à l'alphabet
        aplhabet[(int) (element->caractere)] = element;
        // On affiche le caractère et le codage
        printf("Caractere : %c, Codage : ", element->caractere);
        for (int i = 0; i < profondeur; i++) {
            printf("%c", element->codage[i]);
        }
        printf("\n");
    } else {
        // Appel récursif pour le fils gauche
        creer_code(element->fils_gauche, (code << 1) | 0, profondeur + 1, aplhabet);
        // Appel récursif pour le fils droit
        creer_code(element->fils_droit, (code << 1) | 1, profondeur + 1, aplhabet);
    }
}