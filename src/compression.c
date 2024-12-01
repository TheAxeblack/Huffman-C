#include "compression.h"

void ecrire_entete(FILE *fic_compresse, noeud_s *alphabet[NB_CARACTERES], char *nom_ficher) {
    int nb_feuilles = 0;      // Nombre de feuilles
    int longueur_nom_fichier; // Longueur du nom du fichier

    fprintf(stderr, "Ecriture de l'entete\n");

    // Pour chaque caractère
    for (int i = 0; i < NB_CARACTERES; i++) {
        // Si le caractère est présent dans le fichier, on incrémente le nombre de feuilles
        if (alphabet[i] != NULL)
            nb_feuilles++;
    }

    fprintf(stderr, "Nombre de feuilles : %d\n", nb_feuilles);

    fwrite(&nb_feuilles, sizeof(int), 1, fic_compresse); // On écrit le nombre de feuilles dans le fichier

    fprintf(stderr, "Ecriture des feuilles\n");

    for (int i = 0; i < NB_CARACTERES; i++) // Pour chaque caractère
    {
        // Si le caractère est présent dans le fichier
        if (alphabet[i] != NULL) {
            // On écrit le caractère, le nombre de bits du codage et le codage dans le fichier
            fprintf(stderr, "Ecriture de la feuille %c\n", i);
            fwrite(&alphabet[i]->caractere, sizeof(char), 1, fic_compresse);
            fwrite(&alphabet[i]->nb_bits_codage, sizeof(int), 1, fic_compresse);
            fwrite(&alphabet[i]->codage, sizeof(int), 1, fic_compresse);
            fprintf(stderr, "Feuille %d ecrite\n", i);
        }
    }

    fprintf(stderr, "Ecriture du nom du fichier\n");
    // On écrit la longueur du nom du fichier
    longueur_nom_fichier = strlen(nom_ficher);
    fwrite(&longueur_nom_fichier, sizeof(int), 1, fic_compresse);
    // On écrit le nom du fichier dans le fichier
    fwrite(nom_ficher, sizeof(char), longueur_nom_fichier, fic_compresse);
    fprintf(stderr, "Nom du fichier ecrit\n");
}


void ecrire_codes_caracteres(FILE *fic_compresse, char *nom_fichier, noeud_s *alphabet[256]) {
    FILE *fic_original;
    int bit_buffer = 0; // Buffer pour stocker temporairement les bits à écrire dans le fichier
    int bit_count = 0;  // Nombre de bits dans le buffer
    int c;              // Caractère lu dans le fichier original

    fic_original = fopen(nom_fichier, "r"); // On ouvre le fichier en lecture seule
    if (fic_original == NULL)               // Si l'ouverture du fichier échoue
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s pour la compression\n",
                nom_fichier); // On affiche un message d'erreur
        exit(EXIT_FAILURE);                                                                             // On quitte le programme
    }

    while ((c = fgetc(fic_original)) != EOF) {
        noeud_s *caractere = alphabet[c]; // On récupère le noeud_s du caractère lu

        for (int i = 0; i < caractere->nb_bits_codage; i++) {
            // On décale le buffer d'un bit vers la gauche et on ajoute le bit du codage
            bit_buffer = bit_buffer << 1;
            bit_buffer = bit_buffer | ((caractere->codage >> (caractere->nb_bits_codage - i - 1)) & 1);
            // On incrémente le nombre de bits dans le buffer
            bit_count++;

            // Ecrire le buffer dans le fichier s'il est plein
            if (bit_count == 8) {
                fputc(bit_buffer, fic_compresse); // On écrit le buffer dans le fichier
                bit_buffer = 0;                    // On réinitialise le buffer
                bit_count = 0;                     // On réinitialise le nombre de bits dans le buffer
            }
        }
    }

    // Si le buffer n'est pas vide, on écrit les bits restants dans le fichier
    if (bit_count > 0) {
        bit_buffer = bit_buffer << (8 - bit_count); // On décale le buffer pour qu'il soit aligné à gauche
        fputc(bit_buffer, fic_compresse);           // On écrit le buffer dans le fichier
    }

    fclose(fic_original); // On ferme le fichier
}
