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
    int index1, index2;   // Indices des deux plus petits éléments
    noeud *nouveau_noeud; // Nouveau noeud

    chercher_deux_plus_petits(tab, taille, &index1, &index2); // On cherche les deux plus petits éléments

    nouveau_noeud = (noeud *)malloc(sizeof(noeud)); // On alloue la mémoire pour le nouveau noeud
    if (nouveau_noeud == NULL)                      // Si l'allocation échoue
    {
        fprintf(stderr, "Erreur allocation memoire pour le noeud\n"); // On affiche un message d'erreur
        exit(EXIT_FAILURE);                                           // On quitte le programme
    }

    nouveau_noeud->caractere = '\0';                                               // On initialise le caractère à '\0'
    nouveau_noeud->occurrence = tab[index1]->occurrence + tab[index2]->occurrence; // On initialise l'occurrence à la somme des occurrences des deux plus petits éléments
    nouveau_noeud->codage = NULL;                                                  // On initialise le codage à NULL
    nouveau_noeud->nb_bits_codage = 0;                                             // On initialise le nombre de bits du codage à 0
    nouveau_noeud->fils_gauche = tab[index1];
    nouveau_noeud->fils_droit = tab[index2];

    tab[index1] = nouveau_noeud; // On remplace le premier plus petit élément par le nouveau noeud
    tab[index2] = NULL;
}

static void affichage_code(int nbr_bits, int codage)
{
    for (int i = nbr_bits - 1; i >= 0; i--) // Pour chaque bit du codage
    {
        int bit = (codage >> i) & 1; // On récupère le bit de poids fort
        fprintf(stdout, "%d", bit);  // On affiche le bit de poids fort
    }
    fprintf(stdout, "\n"); // On saute une ligne
}

void creer_code(noeud *element, int code, int profondeur, noeud *aplhabet[256])
{
    if (element == NULL) // Si l'élément est NULL
        return;          // On quitte la fonction

    if (element->fils_gauche == NULL && element->fils_droit == NULL) // Si on est sur une feuille
    {
        int masque;                                                  // Masque pour récupérer le bit de poids fort
        element->codage = (char *)malloc(sizeof(char) * profondeur); // On alloue la mémoire pour le codage
        if (element->codage == NULL)                                 // Si l'allocation échoue
        {
            fprintf(stderr, "Erreur allocation memoire pour le codage\n"); // On affiche un message d'erreur
            exit(EXIT_FAILURE);                                            // On quitte le programme
        }
        element->nb_bits_codage = profondeur; // On stocke le nombre de bits du codage

        masque = 1 << (profondeur - 1);      // On décale le bit de poids fort à la bonne position
        for (int i = 0; i < profondeur; i++) // Pour chaque bit du codage
        {
            element->codage[i] = ((code & masque) == 0) ? '0' : '1'; // On récupère le bit de poids fort
            masque >>= 1;                                            // On décale le masque d'un bit vers la droite
        }
        aplhabet[(int)(element->caractere)] = element; // On ajoute le caractère à l'alphabet
        fprintf(stdout, "%c : ", element->caractere);  // On affiche le caractère
        affichage_code(profondeur, code);              // On affiche le codage
    }
    else // Sinon
    {
        creer_code(element->fils_gauche, (code << 1) | 0, profondeur + 1, aplhabet); // Appel récursif pour le fils gauche
        creer_code(element->fils_droit, (code << 1) | 1, profondeur + 1, aplhabet);  // Appel récursif pour le fils droit
    }
}

void ecrire_entete(FILE *fic_compresse, noeud *alphabet[256], char *nom_ficher)
{
    int nb_feuilles = 0;      // Nombre de feuilles
    int longueur_nom_fichier; // Longueur du nom du fichier

    fprintf(stderr, "Ecriture de l'entete\n");

    for (int i = 0; i < NB_CARACTERES; i++) // Pour chaque caractère
    {
        if (alphabet[i] != NULL) // Si le caractère est présent dans le fichier
            nb_feuilles++;       // On incrémente le nombre de feuilles
    }

    fprintf(stderr, "Nombre de feuilles : %d\n", nb_feuilles);

    fwrite(&nb_feuilles, sizeof(int), 1, fic_compresse); // On écrit le nombre de feuilles dans le fichier

    fprintf(stderr, "Ecriture des feuilles\n");

    for (int i = 0; i < NB_CARACTERES; i++) // Pour chaque caractère
    {
        if (alphabet[i] != NULL) // Si le caractère est présent dans le fichier
        {
            fputc(alphabet[i]->caractere, fic_compresse);                                          // On écrit le caractère dans le fichier
            fputc(alphabet[i]->nb_bits_codage, fic_compresse);                                     // On écrit le nombre de bits du codage dans le fichier
            fwrite(alphabet[i]->codage, sizeof(char), alphabet[i]->nb_bits_codage, fic_compresse); // On écrit le codage dans le fichier
            fprintf(stderr, "Feuille %d ecrite\n", i);
        }
        else
            fprintf(stderr, "Feuille %d non ecrite\n", i);
    }

    fprintf(stderr, "Ecriture du nom du fichier\n");
    longueur_nom_fichier = strlen(nom_ficher) + 1;                         // On récupère la longueur du nom du fichier
    fwrite(nom_ficher, sizeof(char), longueur_nom_fichier, fic_compresse); // On écrit le nom du fichier dans le fichier
    fprintf(stderr, "Nom du fichier ecrit\n");
}

void ecrire_codes_caracteres(FILE *fic_compresse, char *nom_fichier, noeud *alphabet[256])
{
    FILE *fic_original;
    int bit_buffer = 0; // Buffer pour stocker temporairement les bits à écrire dans le fichier
    int bit_count = 0;  // Nombre de bits dans le buffer
    int c;              // Caractère lu dans le fichier original

    fic_original = fopen(nom_fichier, "r"); // On ouvre le fichier en lecture seule
    if (fic_original == NULL)               // Si l'ouverture du fichier échoue
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s pour la compression\n", nom_fichier); // On affiche un message d'erreur
        exit(EXIT_FAILURE);                                                                             // On quitte le programme
    }

    while ((c = fgetc(fic_original)) != EOF)
    {
        noeud *caractere = alphabet[c]; // On récupère le noeud du caractère lu

        for (int i = 0; i < caractere->nb_bits_codage; i++)
        {
            bit_buffer = (bit_buffer << 1) | (caractere->codage[i] - '0'); // On décale le buffer d'un bit vers la gauche et on ajoute le bit du codage
            bit_count++;                                                   // On incrémente le nombre de bits dans le buffer

            // Ecrire le buffer dans le fichier si il est plein
            if (bit_count >= 8)
            {
                fputc(bit_buffer, fic_compresse); // On écrit le buffer dans le fichier
                bit_buffer = 0;                   // On réinitialise le buffer
                bit_count = 0;                    // On réinitialise le nombre de bits dans le buffer
            }
        }
    }

    // Ecrire les derniers bits restants dans le buffer
    if (bit_count > 0)
    {
        bit_buffer <<= (8 - bit_count);   // On décale le buffer pour que le dernier bit soit le bit de poids fort
        fputc(bit_buffer, fic_compresse); // On écrit le buffer dans le fichier
    }

    fclose(fic_original); // On ferme le fichier
}

int main(int argc, char **argv)
{
    FILE *fp;                             // Fichier à compresser
    int occurrences[NB_CARACTERES] = {0}; // Initialisation du tableau d'occurences à 0
    int nb_elements = 0;                  // Nombre d'éléments dans l'arbre
    noeud *arbre_huffman[NB_CARACTERES];  // Tableau des noeuds de l'arbre de Huffman
    noeud *alphabet[NB_CARACTERES];       // Tableau des noeuds de l'alphabet
    FILE *fic_compresse;                  // Fichier compressé

    if (argc < 2)       // Si le nombre d'arguments est inférieur à 2
        usage(argv[0]); // On affiche l'usage du programme

    fp = fopen(argv[1], "r"); // On ouvre le fichier en lecture seule

    if (fp == NULL) // Si l'ouverture du fichier échoue
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n"); // On affiche un message d'erreur
        exit(EXIT_FAILURE);                                          // On quitte le programme
    }

    occurence(fp, occurrences); // On compte les occurences des caractères dans le fichier

    fclose(fp); // On ferme le fichier

    initialiser_arbre_huffman(arbre_huffman); // On initialise le tableau des noeuds de l'arbre de Huffman

    for (int i = 0; i < NB_CARACTERES; i++) // Pour chaque caractère
    {
        if (occurrences[i] > 0) // Si le caractère est présent dans le fichier
        {
            nb_elements++;                                    // On incrémente le nombre d'éléments dans l'arbre
            arbre_huffman[i] = creer_feuille(occurrences, i); // On crée une feuille pour le caractère
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

    while (nb_elements > 1) // Tant qu'il reste plus d'un élément dans l'arbre
    {
        creer_noeud(arbre_huffman, nb_elements); // On crée un noeud
        nb_elements--;                           // On décrémente le nombre d'éléments dans l'arbre
    }

    fprintf(stdout, "\nAffichage de l'arbre de Huffman (apres reduction):\n"); // On affiche l'arbre de Huffman

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

    for (int i = 0; i < NB_CARACTERES; i++) // Pour chaque caractère
    {
        if (arbre_huffman[i] != NULL)
        {
            fprintf(stdout, "\nAffichage du codage de %c (%d) :\n", arbre_huffman[i]->caractere, arbre_huffman[i]->caractere); // On affiche le codage du caractère
            creer_code(arbre_huffman[i], 0, 0, alphabet);                                                                      // On crée le codage du caractère
        }
    }

    // On appelle la fonction pour écrire l'entête dans le fichier compressé
    fic_compresse = fopen("fichier_compresse.bin", "wb"); // On ouvre le fichier compressé en mode binaire pour écriture
    if (fic_compresse == NULL)
    {
        fprintf(stderr, "Erreur lors de la création du fichier compressé.\n");
        exit(EXIT_FAILURE);
    }
    ecrire_entete(fic_compresse, alphabet, argv[1]); // On écrit l'entête dans le fichier compressé
    fclose(fic_compresse);                           // On ferme le fichier compressé

    // On appelle la fonction pour écrire les codes des caractères dans le fichier compressé
    fic_compresse = fopen("fichier_compresse.bin", "ab"); // On ouvre le fichier compressé en mode binaire pour ajout (écriture à la fin)
    if (fic_compresse == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier compressé pour l'écriture des codes des caractères.\n");
        exit(EXIT_FAILURE);
    }
    ecrire_codes_caracteres(fic_compresse, argv[1], alphabet); // On écrit les codes dans le fichier compressé
    fclose(fic_compresse);                                     // On ferme le fichier compressé

    exit(EXIT_SUCCESS); // On quitte le programme
}