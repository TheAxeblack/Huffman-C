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
    feuille->codage = -1;
    feuille->nb_bits_codage = 0;
    feuille->fils_gauche = NULL;
    feuille->fils_droit = NULL;
    return feuille;
}


void creer_noeud(noeud_s *tab[], int taille) {
    int index1, index2;   // Indices des deux plus petits éléments
    noeud_s *nouveau_noeud; // Nouveau noeud_s

    chercher_deux_plus_petits(tab, taille, &index1, &index2); // On cherche les deux plus petits éléments

    // Si on a trouvé les deux plus petits éléments
    if (index1 != -1 && index2 != -1) {
        // On crée un nouveau noeud_s
        nouveau_noeud = (noeud_s *) malloc(sizeof(noeud_s));
        if (nouveau_noeud == NULL) {
            fprintf(stderr, "Erreur allocation memoire pour le nouveau noeud\n");
            exit(EXIT_FAILURE);
        }
        // On initialise le nouveau noeud_s
        nouveau_noeud->caractere = -1;
        nouveau_noeud->occurrence = tab[index1]->occurrence + tab[index2]->occurrence;
        nouveau_noeud->codage = -1;
        nouveau_noeud->nb_bits_codage = 0;
        nouveau_noeud->fils_gauche = tab[index1];
        nouveau_noeud->fils_droit = tab[index2];

        if (index1 < index2) {
            tab[index1] = nouveau_noeud;
            tab[index2] = NULL;

            for (int i = index2; i < taille - 1; i++) {
                tab[i] = tab[i + 1];
            }
        } else {
            tab[index2] = nouveau_noeud;
            tab[index1] = NULL;

            for (int i = index1; i < taille - 1; i++) {
                tab[i] = tab[i + 1];
            }
        }
        // On met à jour la taille du tableau
        tab[taille - 1] = NULL;
    }
}

void creer_code(noeud_s *element, int code, int profondeur, noeud_s *aplhabet[NB_CARACTERES]) {
    if (est_feuille(element)) {
        // On met à jour les informations du noeud_s
        element->codage = code;
        element->nb_bits_codage = profondeur;

        // On stocke le noeud_s dans le tableau des nœuds de l'alphabet
        aplhabet[(int) (element->caractere)] = element;
        fprintf(stderr, "(DEBUG) Element stocke dans l'alphabet : %c\n", element->caractere);

        fprintf(stdout, "Caractere : %c, Occurrence : %d, ", element->caractere, element->occurrence);
        affichage_code(element->nb_bits_codage, element->codage);
        return;
    } else {
        // On crée le code du fils gauche
        creer_code(element->fils_gauche, (code << 1) | 0, profondeur + 1, aplhabet);
        // On crée le code du fils droit
        creer_code(element->fils_droit, (code << 1) | 1, profondeur + 1, aplhabet);
    }
}

int est_feuille(noeud_s *element) {
    return element->fils_gauche == NULL && element->fils_droit == NULL;
}

void afficher_arbre(noeud_s *element, int profondeur) {
    if (element == NULL) {
        fprintf(stdout, "Arbre vide\n");
        return;
    }
    afficher_arbre(element->fils_gauche, profondeur + 1);
    for (int i = 0; i < profondeur; i++) {
        fprintf(stdout, "  ");
    }
    if (element->caractere == ' ') {
        fprintf(stdout, "Caractere : ESPACE (%d), Occurrence : %d\n", element->caractere, element->occurrence);
    } else {
        fprintf(stdout, "Caractere : %c (%d), Occurrence : %d\n", element->caractere, element->caractere,
                element->occurrence);
    }
    afficher_arbre(element->fils_droit, profondeur + 1);
}