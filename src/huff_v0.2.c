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
        tab[c]++;
}

void initialiser_arbre_huffman(noeud *arbre_huffman[NB_CARACTERES])
{
    for (int i = 0; i < NB_CARACTERES; i++)
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

static void chercher_deux_plus_petits(noeud *tab[], int taille, int *index1, int *index2)
{
    int min1 = INT_MAX, min2 = INT_MAX;

    *index1 = -1;
    *index2 = -1;

    if (taille < 2)
    {
        fprintf(stderr, "Erreur : taille inferieure a 2\n");
        return;
    }

    for (int i = 0; i < NB_CARACTERES; i++)
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

void creer_noeud(noeud *tab[], int taille)
{
    int index1, index2;
    noeud *nouveau_noeud;

    chercher_deux_plus_petits(tab, taille, &index1, &index2);

    nouveau_noeud = (noeud *)malloc(sizeof(noeud));
    if (nouveau_noeud == NULL)
    {
        fprintf(stderr, "Erreur allocation memoire pour le noeud\n");
        exit(EXIT_FAILURE);
    }

    nouveau_noeud->caractere = '\0';
    nouveau_noeud->occurrence = tab[index1]->occurrence + tab[index2]->occurrence;
    nouveau_noeud->codage = NULL;
    nouveau_noeud->nb_bits_codage = 0;
    nouveau_noeud->fils_gauche = tab[index1];
    nouveau_noeud->fils_droit = tab[index2];

    tab[index1] = nouveau_noeud;
    tab[index2] = NULL;
}

void affichage_code(int nbr_bits, int codage)
{
    for (int i = nbr_bits - 1; i >= 0; i--)
    {
        fprintf(stdout, "%d", (codage >> i) & 1);
    }
    fprintf(stdout, "\n");
}

void creer_code(noeud *element, int code, int profondeur, noeud *aplhabet[NB_CARACTERES])
{
    int masque;

    if (element->fils_gauche == NULL && element->fils_droit == NULL)
    {
        element->codage = (char *)malloc(sizeof(char) * profondeur);
        if (element->codage == NULL)
        {
            fprintf(stderr, "Erreur allocation memoire pour le codage\n");
            exit(EXIT_FAILURE);
        }
        element->nb_bits_codage = profondeur;

        masque = 1 << (profondeur - 1);
        for (int i = 0; i < profondeur; i++)
        {
            element->codage[i] = ((code & masque) == 0) ? '0' : '1';
            masque >>= 1;
        }

        aplhabet[(int)(element->caractere)] = element;

        affichage_code(profondeur, code);
    }
    else
    {
        creer_code(element->fils_gauche, (code << 1) | 0, profondeur + 1, aplhabet);
        creer_code(element->fils_droit, (code << 1) | 1, profondeur + 1, aplhabet);
    }
}

int main(int argc, char **argv)
{
    FILE *fp;                             // Fichier à compresser
    int occurrences[NB_CARACTERES] = {0}; // Initialisation du tableau d'occurences à 0
    int nb_elements = 0;                  // Nombre d'éléments dans l'arbre
    noeud *arbre_huffman[NB_CARACTERES];

    if (argc < 2)
        usage(argv[0]);

    fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    occurence(fp, occurrences);

    fclose(fp);

    initialiser_arbre_huffman(arbre_huffman);

    for (int i = 0; i < NB_CARACTERES; i++)
    {
        if (occurrences[i] > 0)
        {
            nb_elements++;
            arbre_huffman[i] = creer_feuille(occurrences, i);
        }
    }

    for (int i = 0; i < NB_CARACTERES; i++)
    {
        if (arbre_huffman[i] != NULL)
        {
            if (arbre_huffman[i]->caractere == ' ')
                fprintf(stdout, "Caractere : ESPACE (%d), Occurrence : %d\n", arbre_huffman[i]->caractere, arbre_huffman[i]->occurrence);
            else
                fprintf(stdout, "Caractere : %c (%d), Occurrence : %d\n", arbre_huffman[i]->caractere, arbre_huffman[i]->caractere, arbre_huffman[i]->occurrence);
        }
    }

    while (nb_elements > 1)
    {
        creer_noeud(arbre_huffman, nb_elements);
        nb_elements--;
    }

    fprintf(stdout, "\nAffichage de l'arbre de Huffman (apres reduction):\n");

    for (int i = 0; i < NB_CARACTERES; i++)
    {
        if (arbre_huffman[i] != NULL)
        {
            if (arbre_huffman[i]->caractere == ' ')
                fprintf(stdout, "Caractere : ESPACE (%d), Occurrence : %d\n", arbre_huffman[i]->caractere, arbre_huffman[i]->occurrence);
            else
                fprintf(stdout, "Caractere : %c (%d), Occurrence : %d\n", arbre_huffman[i]->caractere, arbre_huffman[i]->caractere, arbre_huffman[i]->occurrence);
        }
    }

    for (int i = 0; i < NB_CARACTERES; i++)
    {
        if (arbre_huffman[i] != NULL)
        {
            fprintf(stdout, "\nAffichage du codage de %c (%d) :\n", arbre_huffman[i]->caractere, arbre_huffman[i]->caractere);
            creer_code(arbre_huffman[i], 0, 0, arbre_huffman);
        }
    }

    exit(EXIT_SUCCESS);
}
