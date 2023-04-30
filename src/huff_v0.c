# include "Huffman.h"

void usage(char *nom_prog) {
    fprintf(stderr, "Usage: %s <fichier>\n", nom_prog);
    exit(EXIT_FAILURE);
}

void occurence(FILE *fic, int tab[NB_CARACTERES]) {
    int c;
    while ((c = fgetc(fic)) != EOF) {
        tab[c]++;
    }
}

int main(int argc, char **argv) {
    FILE *fp;
    //char buffer[TAILLE_MAX];
    //int c;
    int occurrences[NB_CARACTERES] = {0};


    if (argc < 2) {
        usage(argv[0]);
    }

    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    /* Question 1 (avec la fonction fread()) */
    /*fread(buffer, sizeof(char), TAILLE_MAX, fp);
    fprintf(stdout, "%s", buffer);*/

    /* Question 2 */
    /*while ((c = fgetc(fp)) != EOF) {
        fprintf(stdout, "%c", c);
    }
    fprintf(stdout, "\n");*/

    /* Question 3 */
    occurence(fp, occurrences);

    for (int i = 0; i < NB_CARACTERES; i++) {
        if (occurrences[i] > 0) {
            fprintf(stdout, "%c : %d\n", i, occurrences[i]);
        }
    }

    fclose(fp);

    exit(EXIT_SUCCESS);
}
