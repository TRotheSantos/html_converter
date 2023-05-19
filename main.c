#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FILE *source, *targettxt, *targethtml = NULL;
char c;

// FIN -> NOUV_PARA -> REST (pas de conflict)
enum TokenType {MOTCLE_T,//0
        MOTCLE_A,
        SECTION,
        SSECTION,
        NOUV_PARA,  //4
        MOT,                // last to check
        FIN} token;         // first to check, 6
char tokenValue[50];

int is_char(char c) {
    return 'a' <= c && c <= 'z'
    || 'A' <= c && c <= 'Z'
    || '0' <= c && c <= '9'
    || c == '.' || c == ';' || c == '!' || c == '?' || c == ','
    || c == ':' || c == '-' || c == '#' ||  c == '/' || c == '\'';
}


int scanner() {
    goto i;
    i:
    if (c == '\t' || c == ' ') {
        c = fgetc(source);
        goto i;
    }
    if (c == '\n') {
        goto iNP1;
    }
    if (c == '>') {
        c = fgetc(source);
        goto MC1MC2;
    }
    if (c == '=') {
        c = fgetc(source);
        goto S1SS1;
    }
    if (is_char(c)) { //no next char since we need the value
        goto M1;
    }
    if (c == EOF) {
        goto FIN;
    }
    goto ERROR;

    MC1MC2:
    if (c == 'T') {
        c = fgetc(source);
        if (c == 'i') {
            c = fgetc(source);
            if (c == 't') {
                c = fgetc(source);
                if (c == 'r') {
                    c = fgetc(source);
                    if (c == 'e') {
                        c = fgetc(source);
                        goto MC1b;
                    }
                }
            }
        }
    }
    if (c == 'A') {
        c = fgetc(source);
        if (c == 'u') {
            c = fgetc(source);
            if (c == 't') {
                c = fgetc(source);
                if (c == 'e') {
                    c = fgetc(source);
                    if (c == 'u') {
                        c = fgetc(source);
                        if (c == 'r') {
                            c = fgetc(source);
                            goto MC2b;
                        }
                    }
                }
            }
        }
    }
    goto ERROR;

    S1SS1:
    if (c == '\t' || c == ' ' || c == '\n' || c == EOF) {
        c = fgetc(source);
        goto SECTION;
    }
    if (c == '=') {
        c = fgetc(source);
        goto SS2;
    }
    goto ERROR;

    iNP1:
    if (c == '>') {
        c = fgetc(source);
        goto MC1MC2;
    }
    if (c == '=') {
        c = fgetc(source);
        goto S1SS1;
    }
    if (c == '\n') {
        c = fgetc(source);
        goto iNP1NP2;
    }
    if (c == ' ' || c == '\t') {
        c = fgetc(source);
        goto iNP1;
    }
    if (is_char(c)) {
        c = fgetc(source);
        goto M1;
    }
    if (c == EOF) {
        goto FIN;
    }
    goto ERROR;


    M1:
    if (c == '\n' || c == '\t' || c == ' ' || c == EOF) {
        c = fgetc(source);
        goto MOT;
    }
    if (is_char(c)) {
        int index = 0;
        while (is_char(c)) {
            tokenValue[index] = c;
            index++;
            c = fgetc(source);
        }
        goto M1;
    }
    goto ERROR;

    MC1b:
    if (c == '\n' || c == '\t' || c == ' ' || c == EOF) {
        goto TITRE;
    }
    goto ERROR;

    MC2b:
    if (c == '\n' || c == '\t' || c == ' ' || c == EOF) {
        goto AUTEUR;
    }
    goto ERROR;

    SS2:
    if (c == '\n' || c == '\t' || c == ' ' || c == EOF) {
        c = fgetc(source);
        goto SSECTION;
    }
    goto ERROR;

    SECTION:
    token = SECTION;
    return 1;

    iNP1NP2:
    if (c == '>') {
        c = fgetc(source);
        goto MC1MC2;
    }
    if (c == '=') {
        c = fgetc(source);
        goto S1SS1;
    }
    if (c == '\n' || c == '\t' || c == ' ') {
        c = fgetc(source);
        goto iNP1NP2;
    }
    if (is_char(c)) {
        goto M1NOUVPARA;
    }
    if (c == EOF) {
        goto FIN;
    }
    goto ERROR;

    TITRE:
    token = MOTCLE_T;
    return 1;

    AUTEUR:
    token = MOTCLE_A;
    return 1;

    SSECTION:
    token = SSECTION;
    return 1;

    M1NOUVPARA:
    token = NOUV_PARA;
    return 1;

    FIN:
    token = FIN;
    return 1;

    MOT:
    token = MOT;
    return 1;

    ERROR:
    return 0;

}

void Analyse_TEXT() {
    if(token == MOT) {
        if (token == MOT) {
            fprintf(targethtml, "%s ", tokenValue);
            memset(tokenValue, 0, sizeof(tokenValue)); //Back to default after USAGE
            scanner();
        } else {
            printf("Error! MOT attendu\n");
            exit(-1);
        }
        Analyse_TEXT();
    } else if (!(token == MOTCLE_A || token == FIN ||token == NOUV_PARA || token == SECTION || token == SSECTION)){
        printf("%s", token);
        printf("ERROR TEXT\n");
        exit(-1);
    }
}

void Analyse_H1() {
    if (token == SECTION) {
        fprintf(targethtml,"\n%s\n", "<h1>");
        scanner();
    } else {
        printf("Error! SECTION attendu\n");
        exit(-1);
    }
    Analyse_TEXT();
    fprintf(targethtml,"\n%s\n", "</h1>");
}

void Analyse_H2() {
    if (token == SSECTION) {
        fprintf(targethtml,"\n%s\n", "<h2>");
        scanner();
    } else {
        printf("Error! SOUSSECTION attendu\n");
        exit(-1);
    }
    Analyse_TEXT();
    fprintf(targethtml,"\n%s\n", "</h2>");
}

void Analyse_P() {
    if(token == NOUV_PARA) {
        if (token == NOUV_PARA) {
            scanner();
        } else {
            printf("Error! NOUVEAU PARAGRAPH attendu\n");
            exit(-1);
        }
        Analyse_TEXT();
        fprintf(targethtml,"<br><br>");
        printf("\n lel \n");
        Analyse_P();
    } else if (!(token == SECTION || token == SSECTION ||token == FIN)){
        printf("ERROR P\n");
        exit(-1);
    }
}

void Analyse_S2() {
    if(token == SSECTION){
        Analyse_H2();
        Analyse_P();
        Analyse_S2();
    } else if (!(token == SECTION || token == FIN)){
        printf("token %d", token);
        printf("ERROR S2");
        exit(-1);
    }
}

void Analyse_S1() {
    if(token == SECTION){
        Analyse_H1();
        Analyse_P();
        Analyse_S2();
        Analyse_S1();

    } else if (!(token == FIN)){
        printf("ERROR S1");
        exit(-1);
    }
}


void Analyse_HEAD() {
    if(token == MOTCLE_T){
        fprintf(targethtml,"\n%s\n", "<h1 align=center>");
        scanner();
        Analyse_TEXT();
        fprintf(targethtml,"\n%s\n", "</h1>");
        if(token == MOTCLE_A){
            fprintf(targethtml,"\n%s\n", "<h3 align=center>");
            scanner();
            Analyse_TEXT();
            fprintf(targethtml,"\n%s\n", "</h3>");
        } else{ printf("Error, missing Auteur\n"); }
    } else { printf("Error, missing Titre"); }
}

void Analyse_BODY() {
    if(token == NOUV_PARA){
        Analyse_P();
        Analyse_S1();
    } else { printf("Error, in Body\n");}
}


void Analyse_AXIOME() {
    scanner();
    Analyse_HEAD();
    Analyse_BODY();
}







int main(int argc, char const *argv[]) {

	// Ouvre le fichier test.txt en lecture seulement (le fichier doit exister) :
	source = fopen("lftext.txt", "r");

	// Cree et ouvre un fichier tmp.html en lecture/ecriture,
	// avec suppression du contenu au prealable :
	targettxt = fopen("target.txt", "w");

    targethtml = fopen("target.html", "w");

    if (targethtml == NULL) {
        printf("Error opening the targettxt.html file\n");
        return 1;
    }

    if (source == NULL) {
        perror("Error");
        return -1;
    }

    if (targettxt == NULL) {
        perror("Error");
        return -1;
    }

    // Beggining html
    fprintf(targethtml, "<!DOCTYPE html>\n");
    fprintf(targethtml, "<html>\n");
    fprintf(targethtml, "<head>\n");
    fprintf(targethtml, "<title>Text file converted to HTML</title>\n");
    fprintf(targethtml, "</head>\n");
    fprintf(targethtml, "<body>\n");

	c = fgetc(source); // lecture du caractere suivant du fichier source

    Analyse_AXIOME();
/*    while (token != 6) { //FIN
        if(scanner()){
            printf("token number %d:", token);
            printf(" %s\n", tokenValue);
            memset(tokenValue, 0, sizeof(tokenValue)); //Back to default after USAGE
        } else {
            printf("\nNo Token found\n");
        }
    }*/

    // END OF HTML
    fprintf(targethtml, "</body>\n");
    fprintf(targethtml, "</html>\n");


    if(targethtml != NULL) fclose(targethtml);
	if (source != NULL) fclose(source); // fermeture du fichier source
	if (targettxt != NULL) fclose(targettxt); // fermeture du fichier targettxt

	return 0;
}





