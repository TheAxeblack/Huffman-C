#include "huffman_tree.h"

void usage(char *nom_prog)
{
    fprintf(stderr, "Usage: %s <fichier>\n", nom_prog);
    exit(EXIT_FAILURE);
}

void occurence(FILE *fic, int tab[NB_CARACTERES])
{
    int c;
    while ((c = fgetc(fic)) != EOF)
    {
        tab[c]++;
    }
}

void initialiser_arbre_huffman(noeud *arbre_huffman[NB_CARACTERES])
{
    int i;
    for (i = 0; i < NB_CARACTERES; i++)
    {
        arbre_huffman[i] = NULL;
    }
}

noeud *creer_feuille(int *tab, int index)
{
    noeud *feuille = (noeud *)malloc(sizeof(noeud));
    if (feuille == NULL)
    {
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

void chercher_deux_plus_petits(noeud *tab[], int *index1, int *index2)
{
    int min1 = INT_MAX, min2 = INT_MAX;
    int i;

    *index1 = -1;
    *index2 = -1;

    for (i = 0; i < NB_CARACTERES; i++)
    {
        if (tab[i] != NULL)
        {
            if (tab[i]->occurrence < min1)
            {
                min2 = min1;
                min1 = tab[i]->occurrence;
                *index2 = *index1;
                *index1 = i;
            }
            else if (tab[i]->occurrence < min2)
            {
                min2 = tab[i]->occurrence;
                *index2 = i;
            }
        }
    }
}

void creer_noeud(noeud *tab[NB_CARACTERES], int taille)
{
    int index1, index2;
    noeud *nouveau_noeud;

    chercher_deux_plus_petits(tab, &index1, &index2);

    if (index1 == -1 || index2 == -1)
    {
        fprintf(stderr, "Erreur : impossible de trouver les deux plus petits\n");
        return;
    }

    nouveau_noeud = (noeud *)malloc(sizeof(noeud));
    if (nouveau_noeud == NULL)
    {
        fprintf(stderr, "Erreur allocation memoire pour le noeud\n");
        exit(EXIT_FAILURE);
    }

    nouveau_noeud->occurrence = tab[index1]->occurrence + tab[index2]->occurrence;
    nouveau_noeud->nb_bits_codage = 0;
    nouveau_noeud->fils_gauche = tab[index1];
    nouveau_noeud->fils_droit = tab[index2];

    fprintf(stdout, "Occurrence : %d\n", nouveau_noeud->occurrence);

    tab[index1] = nouveau_noeud;
    tab[index2] = NULL;
}

void afficher_arbre_huffman(noeud *racine)
{
    if (racine == NULL)
    {
        return;
    }

    // Affichage de la valeur du noeud et son niveau
    if (racine->occurrence > 0)
        printf("Caractere : %c | Occurrence : %d\n", racine->caractere, racine->occurrence);

    // Parcours récursif des fils gauche et droit
    afficher_arbre_huffman(racine->fils_gauche);
    afficher_arbre_huffman(racine->fils_droit);
}

int main(int argc, char **argv)
{
    FILE *fp;                             // Fichier à compresser
    int occurrences[NB_CARACTERES] = {0}; // Initialisation du tableau d'occurences à 0
    int i;                                // Variable de boucle
    int nb_elements = 0;                  // Nombre d'éléments dans l'arbre
    noeud *arbre_huffman[NB_CARACTERES];

    if (argc < 2)
    {
        usage(argv[0]);
    }

    fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    occurence(fp, occurrences);

    fclose(fp);

    initialiser_arbre_huffman(arbre_huffman);

    for (i = 0; i < NB_CARACTERES; i++)
    {
        if (occurrences[i] > 0)
        {
            nb_elements++;
            arbre_huffman[i] = creer_feuille(occurrences, i);
            (arbre_huffman[i]->caractere == ' ') ? fprintf(stdout, "Caractere : ESPACE, Occurrence : %d\n", arbre_huffman[i]->occurrence) : fprintf(stdout, "Caractere : %c, Occurrence : %d\n", arbre_huffman[i]->caractere, arbre_huffman[i]->occurrence);
        }
    }

    while (nb_elements > 1)
    {
        creer_noeud(arbre_huffman, nb_elements);
        nb_elements--;
    }

    afficher_arbre_huffman(arbre_huffman);


    exit(EXIT_SUCCESS);
}
