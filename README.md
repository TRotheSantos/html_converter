# html_converter
Converts text files in simple html format



Polytech Marseille - Informatique Nicolas Baudru
5
Et le code source associé :
Polytech Marseille - Informatique Nicolas Baudru
6
Travail préliminaire
Le programme suivant copie un fichier source dans un fichier target.
Vous pouvez le télécharger sur la page Ametice du cours. Comprenez-le et testez-
le. Il servira de point de départ.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
FILE *source, *target = NULL;
char c;
int main(int argc, char const *argv[]) {
// Ouvre le fichier test.txt en lecture seulement
// (le fichier doit exister) :
source = fopen("test.txt", "r");
// Cree et ouvre un fichier tmp.html en lecture/ecriture,
// avec suppression du contenu au prealable :
target = fopen("target.html", "w+");
if (source == NULL) {
printf("Impossible d'ouvrir le fichier source\n");
return -1;
}
if (target == NULL) {
printf("Impossible d'ouvrir le fichier target\n");
return -1;
}
c = fgetc(source); // on lit un premier caractère
while(c!=EOF) { // tant que la fin du fichier n'est pas atteinte
fputc(c,target); // écrire c dans le fichier target
c = fgetc(source); // lecture du caractere suivant
}
if (source != NULL) fclose(source); // fermeture du fichier source
if (target != NULL) fclose(target); // fermeture du fichier target
return 0;
}
Polytech Marseille - Informatique Nicolas Baudru
7
D’autres modes d’ouverture d’un fichier sont possibles. Par exemple :
• “w” : ouverture en écriture seul. Le fichier est créé s’il n’existe pas.
• “r+” : lecture et écriture. Le fichier doit exister.
• “a” : mode d’ajout, i.e. écriture dans le fichier à partir de la fin. Le fichier
est créé s’il n’existe pas.
Les méthodes suivantes permettent de lire et écrire des strings dans un fichier
fichier. N’hésitez pas à consulter leur manuel d’utilisation.
• int fputs( const char * string, FILE * stream );
• char * fgets( char * string, int maxLength, FILE * stream );
• int fprintf( FILE * stream, const char *format, ... );
• int fscanf( FILE * stream, const char *format, ... );
Cette autre méthode permet de comparer deux chaines de caractère. Elle pourra
vous être utile.
• int strcmp( const char * first, const char * second );
Analyseur lexical
On supposera dans ce qui suit que « CP » est l’ensemble constitué :
• des lettres de l’alphabet minuscules ou majuscules,
• des symboles de ponctuation,
• du symbole #.
En C, on vérifiera qu’un caractère est dans CP grâce à la fonction suivante :
Int is_char(char c) {
return ‘a’ <= c && c <= ‘z’
|| ‘A’ <= c && c <= ‘Z’
|| ‘0’ <= c && c <= ‘9’
|| c == ‘.’ || c == ‘;’ || c == ‘!’ || c == ‘?’ || c == ‘,’
|| c == ‘:’ || c == ‘-‘|| c == ‘\’’ || c == ‘#’;
}
Polytech Marseille - Informatique Nicolas Baudru
8
Remarque : ‘\’’ est le caractère représentant la cote en C. De même, ‘\n’ et ‘\t’ sont
les caractères représentant le saut de ligne et la tabulation. De plus, un fichier
termine toujours par un symbole spécial, noté EOF en C.
Attention ! Il existe deux normes pour représenter un saut de ligne dans un texte.
Dans la norme CRLF le saut de ligne est composé de deux caractères successifs
dans le fichier source : \r\n dans la notation du langage C. Dans la norme LF, seul le
caractère \n est utilisé. Par défaut, Linux utilise la norme LF alors que Windows
utilise la norme CRLF. Cependant, certains IDE sous Windows permettent de choisir
le mode, comme VSCode ou Clion. Dans notre TP, nous considérons la norme LF.
En conséquence, si vous êtes sous Windows, faites bien attention à enregistrer vos
fichier texte dans le mode LF.
Description formelle des lexèmes (tokens)
Les lexèmes à reconnaître sont MOTCLE_T (mot-clé pour le titre), MOTCLE_A
(mot-clé pour l’auteur), SECTION, SSECTION (sous section), NOUV_PARA
(nouveau paragraphe), MOT et FIN décrits par les expressions régulières suivantes :
MOTCLE_T = (‘\t’ + ‘ ‘ + ‘\n’)* . > . “Titre”. (‘\t’ + ‘ ‘ + ‘\n’ + EOF)
MOTCLE_A = (‘\t’ + ‘ ‘ + ‘\n’)* . > . “Auteur” . (‘\t’ + ‘ ‘ + ‘\n’ + EOF)
SECTION = (‘\t’ + ‘ ‘ + ‘\n’)* .’=’ . (‘\t’ + ‘ ‘ + ‘\n’ + EOF)
SSECTION = (‘\t’ + ‘ ‘ + ‘\n’)* .’=’.’=’ . (‘\t’ + ‘ ‘ + ‘\n’ + EOF)
NOUV_PARA = (‘\t’ + ‘ ‘ + ‘\n’)* .‘\n’ . (‘\t’ + ‘ ‘ + ‘\n’)* . ‘\n’ . (‘\t’ + ‘ ‘ + ‘\n’)*. CP
MOT = (‘\t’ + ‘ ‘ + ‘\n’)* . CP+ . (\t’ + ‘ ‘ + ‘\n’ + EOF)
FIN = (‘\t’ + ‘ ‘ + ‘\n’)* . EOF
On peut voir que les lexèmes MOTCLE_T, MOTCLE_A, SECTION, SSECTION et
MOT sont précédés d’un nombre quelconque d’espaces, de tabulations ou de sauts
de ligne, et sont suivis ou d’un espace, ou d’une tabulation ou d’un saut de ligne. Le
lexème FIN termine seulement par EOF car il représente le dernier caractère du
fichier texte source. Enfin un NOUV_PARA doit contenir au moins deux sauts de
lignes (avec possiblement des espaces ou des tabulations au milieu)
Polytech Marseille - Informatique Nicolas Baudru
9
Travail à effectuer
1. Pour chaque expression rationnelle, déterminez un automate équivalent.
Remarquez que tous ces automates ont le même état initial.
2. Fusionnez les états initiaux pour obtenir un nouvel automate équivalent à l’union
des expressions rationnelles. Veillez à garder tous les états finaux. Ils vous
permettront de déterminer le token trouvé dans la suite du TP.
3. Donnez l’automate déterministe correspondant à l’automate de la question 2.
4. Il est intéressant de noter que le mot ‘\n’.’\n’.toto.’\n’ par exemple est reconnu de
deux façons : en une seul fois, par l’expression rationnelle MOT; ou en deux fois,
par l’expression rationnelle NOUV_PARA suivie de MOT. Le bon choix est
évidemment le second. Mais cet exemple illustre le fait que les expressions
rationnelles doivent être priorisées. Voici comment : FIN est prioritaire sur
NOUV_PARA qui est prioritaire sur le reste. Les expressions rationnelles autres
que FIN et NOUV_PARA peuvent être de même priorité car elles ne sont pas en
conflit. Noter qu’un tel mécanisme de priorité existe aussi dans les outils de
compilation tel que Lex/Flex.
Modifier votre automate pour tenir compte de ces priorités.
5. Ecrivez une fonction scanner() qui cherche le prochain lexème d’un fichier texte.
La fonction renvoie 1 si un lexème est trouvé, 0 sinon (ce qui correspond à une
erreur). Elle sera écrite à l’aide d’instructions goto. Chaque étiquette doit
correspondre à un état de votre automate. Ainsi le code du scanner “collera” à la
conception. Lorsqu’un nouveau lexème est trouvé, la fonction enregistre son type
et sa valeur dans deux variables globales token et tokenValue (ces variables
seront donc disponibles partout dans le programme). La variable token peut
valoir MOTCLE_T, MOTCLE_A, SECTION, SSECTION, NOUV_PARA, MOT ou
FIN. La variable tokenValue sera utilisée pour enregistrer le mot correspondant
au token MOT. La lecture d’un caractère dans le fichier en entrée se fera par
l’instruction c = fgetc(source) où la variable c est aussi globale. Nous avons donc
besoin des structures de données suivantes (ces variables sont globales) :
char c;
enum TokenType {MOTCLE_T, MOTCLE_A, SECTION, SSECTION,
NOUV_PARA, MOT, FIN} token;
char tokenValue[50];
Polytech Marseille - Informatique Nicolas Baudru
10
Si une partie de votre automate ressemble à
alors le morceau de code correspondant dans la fonction scanner() est celui de
l’encadré si dessous. Notez que dans ce code nous lisons le caractère suivant avant
un goto X uniquement si l’état X n’est pas final.
scanner() {
...
A : // si A est un label correspondant à un état non final
if (c == ’>’) {c = fgetc(source); goto B; }
If (c == EOF) goto F; // pas de fgetc() ici car F est final
goto Error;
...
F : // si F est un label correspondant à un état final
token = FIN;
return 1;
...
Erreur : // Label supplémentaire pour gérer les erreurs
printf(“aucun token trouve\n”);
exit(-1); // le programme s’arrête dès qu’une erreur est détectée.
}
6. Ecrivez un programme test qui
○ ouvre un fichier texte,
○ lit un premier caractère puis,
○ tant que le lexème FIN n’est pas trouvé, appelle la fonction scanner() et
affiche le type et la valeur du token trouvé dans le terminal.
○ lorsque le token FIN est trouvé, ferme le fichier texte.
Essayez de bien gérer les erreurs, i.e. quand scanner() renvoie 0.
A
F
B
>
EOF
Polytech Marseille - Informatique Nicolas Baudru
11
Analyseur syntaxique
Description formelle de la structure du fichier texte
Elle est donnée par la grammaire suivante où :
● AXIOME est le l’axiome de la grammaire
● les symboles terminaux sont les lexèmes :
MOTCLE_T, MOTCLE_A, SECTION, SSECTION, NOUV_PARA, MOT et FIN.
● les règles de production sont
AXIOME → HEAD BODY FIN
HEAD → MOTCLE_T TEXT MOTCLE_A TEXT
TEXT → MOT TEXT | epsilon
BODY → P S1
P → NOUV_PARA TEXT P | epsilon
S1 → H1 P S2 S1 | epsilon
H1 → SECTION TEXT
S2 → H2 P S2 | epsilon
H2 → SSECTION TEXT
Travail à effectuer
1. Calculez les premiers et les suivants de cette grammaire.
2. Vérifiez que cette grammaire est LL(1).
3. Implémentez l’analyseur syntaxique à l’aide de fonctions récursives. Les
différents tokens seront obtenus par un appel à la fonction scanner(). Par
exemple, à la règle
TEXT → MOT TEXT | epsilon
correspondra à la fonction Analyse_TEXT() suivante (la liste des règles
effectuées n’apparait pas dans ce code... ) :
Polytech Marseille - Informatique Nicolas Baudru
12
Analyse_TEXT() {
if (token ∈ possible(TEXT → MOT TEXT) )
{
if ( token == MOT )
{
scanner();
}
else
{
printf(”erreur dans l’analyse de TEXT : MOT attendu \n”);
exit(-1); // on s’arrête dès qu’une erreur est trouvée.
}
Analyse_TEXT();
}
else if ( token ∈ possible(TEXT → epsilon) ) { // alors il n’y a rien à faire.}
else
{
printf(”erreur dans l’analyse de TEXT \n”);
exit(-1); // on s’arrête dès qu’une erreur est trouvée.
}
}
Autre exemple pour la règle S2 → H2 P S2 | epsilon :
Analyse_S2() {
if (token ∈ possible(S2 → H2 P S2) )
{
Analyse_H2();
Analyse_P();
Analyse_S2() ;
}
else if ( token ∈ possible(S2 → epsilon) ) { // alors il n’y a rien à faire.}
else
{
printf(”erreur dans l’analyse de S2 \n”);
exit(-1); // on s’arrête dès qu’une erreur est trouvée.
}
}
Polytech Marseille - Informatique Nicolas Baudru
13
7. Ecrivez un programme test qui
○ ouvre un fichier texte,
○ lit un premier caractère puis
○ cherche le premier token
○ lance l’analyse syntaxique à partir de l’axiome.
○ ferme proprement le fichier texte.
Puis testez !
Génération de code HTML
Nous allons maintenant produire le fichier HTML à partir du fichier texte donnée en
entrée. L’idée principale est de générer le code HTML durant la phase d’analyse
syntaxique.
Travail à effectuer
1. Dans le programme principal, avant le lancement de l’analyseur syntaxique,
ajoutez le code nécessaire pour créer un fichier target.html et écrire dans ce
fichier le texte de l’exemple (code source html) allant du DOCTYPE jusqu’à la
balise ouvrante <body> incluse. Après l’analyse syntaxique, ajoutez le code
nécessaire pour écrire dans le fichier target.html les balises fermantes
</body></html>. N’oubliez pas de refermer votre fichier. Testez votre
programme et vérifiez dans un navigateur que le fichier target.html est bien
créé et contient ce qu’il faut.
2. Dans la fonction Analyse_TEXT() correspondant à l’analyse du non-terminal
TEXT de la grammaire, ajoutez les instructions permettant de recopier le texte
source dans un fichier target.html. Plus précisément, il s’agit ici d’écrire dans
le fichier target.html la variable globale tokenValue au bon moment. Testez
votre programme. Vérifiez dans un navigateur que le contenu de votre fichier
texte source est bien présent. Notez que pour l’instant aucun formatage
HTML n’est présent.
3. Dans la fonction Analyse_H1() correspondant à l’analyse du non-terminal
H1 de la grammaire, juste avant l’instruction Analyse_TEXT(), ajoutez le code
nécessaire pour écrire dans le fichier target.html une balise <h1>. De même,
juste après l’instruction Analyse_TEXT(), ajoutez le code nécessaire pour
écrire dans le fichier target.html une balise </h1>. Testez votre programme.
Normalement, si vous ouvrez votre fichier target.html dans un navigateur, les
titres de section seront formatés.
Polytech Marseille - Informatique Nicolas Baudru
14
4. En procédant de manière similaire, ajoutez dans les bonnes fonctions le code
nécessaire pour reproduire l’exemple donné au début du TP. Vous aurez
donc à formater le titre, les auteurs, les sous sections et les paragraphes.
Pour aller plus loin
Numéro de sections automatiques
Nous allons ici ajouter la numérotation automatique des sections et sous sections. Il
suffit pour cela de maintenir deux variables entières num_section et
num_ssousection au cours de l’analyse syntaxique et de les ajouter dans le fichier
target.html au moment de l’écriture des titres de section et sous section.
...
Génération de la table des matières
...
Génération de la table d’index
...
