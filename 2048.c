#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
// dimensiunile ferestrei meniu
#define MENU_WIDTH 30
#define MENU_HEIGHT 8
// nr. de alegeri din meniu
#define n_choices 3
// dimensiunile fiecarui dreptunghi din tabla de joc
#define WIN_WIDTH 9
#define WIN_HEIGHT 5
// dimensiunile panoului
#define PAN_HEIGHT 10
#define PAN_WIDTH 60
// q
#define q 113
#define Q 81
// nr de secunde / 10 care trebuie sa treaca pt a se face o miscare automata
#define TIMP 100
//  CULORI//
#define DARK_GREY 11
#define GREY 12
#define DARK_YELLOW 13
#define DARK_ORANGE 14
#define MEDIUM_ORANGE 15
#define MEDIUM_YELLOW 16
#define LIGHT_GREEN 17
#define CYAN 18
#define PURPLE 19
#define RED 20
#define WHITE 21
#define BLACK 22
// initializez culorile custom si perechile de culori
void culori();
// sterge ferestra precizata din memorie si de pe ecran
void sterge_ecran(WINDOW *win);
// afiseaza meniul in modul cum este el acum
void print_menu(WINDOW *menu_win, int highlight, char **choices);
// miscare prin meniu
void menu(WINDOW *menu_win, char **choices, WINDOW *M2048[][4],
          int matrice[][4], int *resume, int *score);
// initializez valorile matricei cu 0 la NEW GAME
void init_matrice(int matrice[][4]);
// initializez chenarul de inceput si sterg menu
// aici la rand_start si col_start schimb colt stanga sus al chenarului
void init_M2048(WINDOW *M2048[][4], WINDOW *menu_win,
                int matrice[][4], int *score);
// atribui valoare random de 2 sau 4 la matrice
void random_val_atr(int matrice[][4]);
// schimb valorile chenarului in functie de "matrice"
void M2048_refresh(WINDOW *M2048[][4], int matrice[][4]);
//
void mutare(WINDOW *menu_win, char **choices, WINDOW *M2048[][4],
            int matrice[][4], int *resume, int *score);
// mut toate elemntele lui matrice in directia respectiva
void mutare_sus(int matrice[][4]);
void mutare_jos(int matrice[][4]);
void mutare_stanga(int matrice[][4]);
void mutare_dreapta(int matrice[][4]);
// adun toate elementele care au aceeasi valoare si se unesc
// in directia precizata
void adunare_sus(int matrice[][4], int *score);
void adunare_jos(int matrice[][4], int *score);
void adunare_stanga(int matrice[][4], int *score);
void adunare_dreapta(int matrice[][4], int *score);
// verific daca doua matrice au aceleasi valori
int la_fel(int matrice[][4], int matrice_anterioara[][4]);
// verific daca am un element cu val. 2048 in matrice
int is_winner(int matrice[][4]);
// verific daca nu mai am mutari valide pe care pot sa le fac
int is_loser(int matrice[][4], int *score);
// afisez mesajul corespunzator in cazul in care s-a terminat jocul
void mesaj_final(WINDOW *menu_win, WINDOW *M2048[][4], int matrice[][4],
                 int *resume, char **choices, int *score);
//
char *timestr(struct tm t, char *time);
//
char *timestr2(struct tm t, char *time);
// afisez panoul cu instructiunile de miscare si care afiseaza data, timpul
//  si scorul
void afisare_panou(int *score);
// in cazul in care au trecut TIMP/10 secunde fac mutarea care goleste cele
// mai multe casute
void auto_move(int matrice[][4], int *score, WINDOW *M2048[][4]);
// copiez matricea "matrice" in matricea "copie"
void copiere_matrice(int matrice[][4], int copie[][4]);
//
void printCustomAsciiArt(int y, int x);
//
int main()
{
    //////// MENU //////////
    char *choices[] = {
        "New Game",
        "Resume",
        "Quit"};
    WINDOW *menu_win = NULL;
    //////// 2048 /////////
    int matrice[4][4];
    WINDOW *M2048[4][4];
    int resume = 0, score = 0;
    ///////////////////////
    initscr();
    keypad(stdscr, TRUE);
    culori();
    /* seed o singura data pentru intregul program */
    srand(time(NULL));
    curs_set(0);
    noecho();
    cbreak();
    // initializez meniul
    menu(menu_win, choices, M2048, matrice, &resume, &score);
    endwin();
}

void culori()
{
    start_color();

    init_color(DARK_GREY, 500, 500, 500);
    init_color(GREY, 750, 750, 750);
    init_color(DARK_YELLOW, 500, 500, 0);
    init_color(RED, 900, 0, 0);
    init_color(DARK_ORANGE, 597, 781, 0);
    init_color(MEDIUM_ORANGE, 255, 597, 0);
    init_color(MEDIUM_YELLOW, 255, 796, 0);
    init_color(LIGHT_GREEN, 781, 796, 0);
    init_color(CYAN, 781, 796, 796);
    init_color(PURPLE, 400, 0, 400);
    init_color(BLACK, 50, 50, 50);
    init_color(WHITE, 900, 900, 900);
    // pentru fiecare putere lui doi atribui o pereche du culori
    // specifica ei pt. casuta in care se afla,
    // iar 100 reprezinta casuta goala
    init_pair(100, WHITE, DARK_GREY);
    init_pair(1, WHITE, GREY);
    init_pair(2, WHITE, DARK_YELLOW);
    init_pair(3, WHITE, DARK_ORANGE);
    init_pair(4, WHITE, MEDIUM_ORANGE);
    init_pair(5, WHITE, MEDIUM_YELLOW);
    init_pair(6, WHITE, LIGHT_GREEN);
    init_pair(7, WHITE, COLOR_CYAN);
    init_pair(8, WHITE, PURPLE);
    init_pair(9, WHITE, RED);
    init_pair(10, PURPLE, CYAN);
    init_pair(11, RED, WHITE);
    // pt panou
    init_pair(20, COLOR_BLACK, COLOR_WHITE);
}

void print_menu(WINDOW *menu_win, int highlight, char **choices)
{

    int x, y, i;
    y = (MENU_HEIGHT - n_choices) / 2;
    box(menu_win, 0, 0);
    for (i = 0; i < n_choices; ++i)
    {
        x = (MENU_WIDTH - strlen(choices[i])) / 2 - n_choices / 2;
        // evidentiez optiunea aleasa
        if (highlight == i + 1)
        {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE);
        }
        else
            mvwprintw(menu_win, y, x, "%s", choices[i]);
        ++y;
    }
    wrefresh(menu_win);
}

void menu(WINDOW *menu_win, char **choices, WINDOW *M2048[][4],
          int matrice[][4], int *resume, int *score)
{
    int choice = 0, highlight = 1, c;
    int stdx, stdy;
    getmaxyx(stdscr, stdy, stdx);
    printCustomAsciiArt(5, (stdx - 46) / 2);
    int startx = (stdx - MENU_WIDTH) / 2;
    int starty = (stdy - MENU_HEIGHT) / 2;
    menu_win = newwin(MENU_HEIGHT, MENU_WIDTH, starty, startx);
    wbkgd(menu_win, COLOR_PAIR(20));
    keypad(menu_win, TRUE);
    print_menu(menu_win, highlight, choices);
    while (1)
    {
        c = wgetch(menu_win);
        // ma misc prin meniu pana cand apas enter sa imi aleg optiunea
        switch (c)
        {
        case KEY_UP:
            if (highlight == 1)
                highlight = n_choices;
            else
                --highlight;
            break;
        case KEY_DOWN:
            if (highlight == n_choices)
                highlight = 1;
            else
                ++highlight;
            break;
        case 10:
            choice = highlight;
            break;
        default:
            // mvwprintw(menu_win, 1, 0, "Charcter pressed is = %3d
            // Hopefully it can be printed as '%c'", c, c);
            // wrefresh(menu_win); /// atentie la asta
            break;
        }
        print_menu(menu_win, highlight, choices);
        if (choice != 0)
            break;
    }
    refresh();
    // in functie de ce optiune imi aleg fac urmatoarele chestiuni:
    // initializez tabla de joc si matricea care tine minte valorile celulelor
    if (choice == 1)
    {
        // fac posibila utilizarea optiunei resume pt. a continua jocul
        *resume = 1;
        // sterg scorul obtinut in jocul anterior
        *score = 0;
            // sterg ferestra meniu de pe ecran si din memorie sa nu ocupe spatiu
            sterge_ecran(menu_win);
            /* pointer-ul local devine dangling dupa stergere; setam NULL ca
               să nu fie folosit accidental și să evităm double-delwin */
            menu_win = NULL;
        init_matrice(matrice);
        init_M2048(M2048, menu_win, matrice, score);
        // afisez panoul cu informatii
        afisare_panou(score);
        // fac posibila mutarea pieselor pe tabla de joc
        mutare(menu_win, choices, M2048, matrice, resume, score);
    }
    // daca apas resume
    else if (choice == 2)
    {
        // daca exista deja un joc, ma intorc la el
        if (*resume == 1)
        {
            sterge_ecran(menu_win);
            afisare_panou(score);
            mutare(menu_win, choices, M2048, matrice, resume, score);
        }
        /*daca nu exista, instinteez jucatorul ca trebuie sa porneasca un joc
        nou */
        else if (*resume == 0)
        {
            sterge_ecran(menu_win);
            /* evităm folosirea pointer-ului șters */
            menu_win = NULL;
            mvwprintw(stdscr, 10, 10, "Mai intai trebuie sa creezi un joc nou");
            wrefresh(stdscr);
            getch();
            werase(stdscr);
            wrefresh(stdscr);
            menu(menu_win, choices, M2048, matrice, resume, score);
        }
    }
    /*daca apas quit dealoc memoria folosita si inchid executabilul*/
    else if (choice == 3)
    {
        if (menu_win != NULL)
            sterge_ecran(menu_win);
        int i, j;
        for (i = 0; i < 4; ++i)
            for (j = 0; j < 4; ++j)
                if (M2048[i][j] != NULL)
                    delwin(M2048[i][j]);
        endwin();
        exit(0);
    }
}

void init_matrice(int matrice[][4])
{
    int i, j;
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            matrice[i][j] = 0;
}

void init_M2048(WINDOW *M2048[][4], WINDOW *menu_win,
                int matrice[][4], int *score)
{
    (void)score; /* parametrul nu e folosit aici; evitam warning */
    (void)menu_win; /* menu_win este gestionat de apelant; evitam warning */
    // atribui valorile de inceput a tablei de joc
    random_val_atr(matrice);
    random_val_atr(matrice);
    int rand_start, col_start;
    int stdy, stdx;
    getmaxyx(stdscr, stdy, stdx);
    // aleg pozitia tablei de joc in fereastra
    rand_start = (stdy - WIN_HEIGHT * 4) / 2;
    col_start = (stdx - WIN_WIDTH * 4 - PAN_WIDTH) / 2;
    int i, j;
    // creez fieacre dreptunghi in parte intr-o matrice de ferestre
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            M2048[i][j] = newwin(WIN_HEIGHT, WIN_WIDTH, rand_start, col_start);
            // aici am facut sa fie spatiu intre patrate
            col_start += WIN_WIDTH + 1;
        }
        // daca schimbi nu uita si aici
        col_start = (stdx - WIN_WIDTH * 4 - PAN_WIDTH) / 2;
        rand_start += WIN_HEIGHT + 1;
    }
    M2048_refresh(M2048, matrice);
    // afisare_panou(score);
}

void random_val_atr(int matrice[][4])
{
    int rand_row, rand_col;
    int ok = 0, two_four[] = {2, 4}, val;
    while (!ok)
    {
        // aleg o coloana si o linie aleatorie din matrice
        rand_row = rand() % 4;
        rand_col = rand() % 4;
        // verifc daca spatiul ales nu e deja ocupat
        if (matrice[rand_row][rand_col] == 0)
            ok = 1;
    }
    // atribui spatiului ales o valoare dintre 2 si 4
    val = two_four[rand() % 2];
    matrice[rand_row][rand_col] = val;
}

void M2048_refresh(WINDOW *M2048[][4], int matrice[][4])
{
    int pow, cop;
    int i, j;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
        {
            // sterg valoarea care era initial in dreptunghi
            werase(M2048[i][j]);
            if (matrice[i][j] != 0)
            {
                cop = matrice[i][j];
                pow = 0;
                while (cop % 2 == 0)
                {
                    cop = cop / 2;
                    ++pow;
                }
                // schimb fundalul in functie de elementul din celula
                wbkgd(M2048[i][j], COLOR_PAIR(pow));
                mvwprintw(M2048[i][j], WIN_HEIGHT / 2,
                          WIN_WIDTH / 2, "%d", matrice[i][j]);
            }
            else
                wbkgd(M2048[i][j], COLOR_PAIR(100));
            wrefresh(M2048[i][j]);
        }
}

void mutare(WINDOW *menu_win, char **choices, WINDOW *M2048[][4],
            int matrice[][4], int *resume, int *score)
{
    keypad(stdscr, TRUE);
    int i, j;
    int matrice_anterioara[4][4];
    M2048_refresh(M2048, matrice);
    while (1)
    {
        for (i = 0; i < 4; ++i)
            for (j = 0; j < 4; ++j)
                matrice_anterioara[i][j] = matrice[i][j];
        int c;
        halfdelay(TIMP);
        c = getch();
        /*in functie de optiunea pe care am ales-o mut elementele din matrice
        fie sus, jos, stanga sau dreapta, daca apas q ma intorc la meniu,
        iar daca nu o fac o mutare in timoul precizat se va face mutarea care
        elibereaza cele mai multe celule*/
        switch (c)
        {
        case KEY_UP:
            adunare_sus(matrice, score);
            // reafisez tabla de joc in functie de noile valori
            M2048_refresh(M2048, matrice);
            // reafiszez panoiul cu noul scor
            afisare_panou(score);
            // aceste doua instructiuni le pun pentru a termina
            // functia halfdelay
            nocbreak();
            cbreak();
            break;
        case KEY_DOWN:
            adunare_jos(matrice, score);
            M2048_refresh(M2048, matrice);
            afisare_panou(score);
            nocbreak();
            cbreak();
            break;
        case KEY_LEFT:
            adunare_stanga(matrice, score);
            M2048_refresh(M2048, matrice);
            afisare_panou(score);
            nocbreak();
            cbreak();
            break;
        case KEY_RIGHT:
            adunare_dreapta(matrice, score);
            M2048_refresh(M2048, matrice);
            afisare_panou(score);
            nocbreak();
            cbreak();
            break;
        case q:
            // sterg de pe ecran tabla de joc
            for (i = 0; i < 4; ++i)
                for (j = 0; j < 4; ++j)
                    if (M2048[i][j] != NULL)
                    {
                        wbkgd(M2048[i][j], COLOR_PAIR(0));
                        wclear(M2048[i][j]);
                        wrefresh(M2048[i][j]);
                    }
            nocbreak();
            cbreak();
            clear(); // aici periculos
            refresh();
            // afisare_panou(score);
            menu(menu_win, choices, M2048, matrice, resume, score);
            break;
        case ERR:
            nocbreak();
            cbreak();
            // apelez functia care muta automat
            auto_move(matrice, score, M2048);
        default:
            break;
        };
        nocbreak();
        cbreak();
        // daca mutarea pe care am facut-o este valida, adaug un nou element
        // pe tabla de joc
        if (la_fel(matrice, matrice_anterioara) == 0)
            random_val_atr(matrice);
        M2048_refresh(M2048, matrice);
        // verific daca jocul s-a terminat si daca da afisez
        // mesajul corespunzator
        mesaj_final(menu_win, M2048, matrice, resume, choices, score);
    }
}

void mutare_sus(int matrice[][4])
{
    int i, j, k;
    /* parcurg fiecare coloana in parte si daca este posibil
    mut toate elementele sus*/
    for (j = 0; j < 4; ++j)
    {
        k = 0;
        for (i = k; i < 4; ++i)
            if (matrice[i][j] != 0)
            {
                // daca am gasit un element nenul si nu este deja pe pozitia
                // dorita il mut pe randul k coloana j
                if (i != k)
                {
                    matrice[k][j] = matrice[i][j];
                    matrice[i][j] = 0;
                }
                ++k;
            }
    }
}

void mutare_jos(int matrice[][4])
{
    int i, j, k;
    /*de data asta parcurg pe randuri*/
    for (j = 0; j < 4; ++j)
    {
        k = 3;
        for (i = k; i >= 0; --i)
            if (matrice[i][j] != 0)
            {
                if (i != k)
                {
                    matrice[k][j] = matrice[i][j];
                    matrice[i][j] = 0;
                }
                --k;
            }
    }
}

void mutare_stanga(int matrice[][4])
{
    int i, j, k;
    for (i = 0; i < 4; ++i)
    {
        k = 0;
        for (j = k; j < 4; ++j)
            if (matrice[i][j] != 0)
            {
                if (j != k)
                {
                    matrice[i][k] = matrice[i][j];
                    matrice[i][j] = 0;
                }
                ++k;
            }
    }
}

void mutare_dreapta(int matrice[][4])
{
    int i, j, k;
    for (i = 0; i < 4; ++i)
    {
        k = 3;
        for (j = k; j >= 0; --j)
            if (matrice[i][j] != 0)
            {
                if (j != k)
                {
                    matrice[i][k] = matrice[i][j];
                    matrice[i][j] = 0;
                }
                --k;
            }
    }
}

void adunare_sus(int matrice[][4], int *score)
{
    mutare_sus(matrice);
    int i, j;
    for (j = 0; j < 4; ++j)
    {
        for (i = 0; i < 3; ++i)
            /*daca dupa muatare doua elemente sunt alaturate si egale le unesc*/
            if (matrice[i][j] == matrice[i + 1][j])
            {
                matrice[i][j] *= 2;
                // modific scorul
                *score += matrice[i][j];
                matrice[i + 1][j] = 0;
                // mut din nou elementele
                mutare_sus(matrice);
            }
    }
}

void adunare_jos(int matrice[][4], int *score)
{
    mutare_jos(matrice);
    int i, j;
    for (j = 0; j < 4; ++j)
    {
        for (i = 3; i > 0; --i)
            if (matrice[i][j] == matrice[i - 1][j])
            {
                matrice[i][j] *= 2;
                *score += matrice[i][j];
                matrice[i - 1][j] = 0;
                mutare_jos(matrice);
            }
    }
}

void adunare_stanga(int matrice[][4], int *score)
{
    mutare_stanga(matrice);
    int i, j;
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 3; ++j)
            if (matrice[i][j] == matrice[i][j + 1])
            {
                matrice[i][j] *= 2;
                *score += matrice[i][j];
                matrice[i][j + 1] = 0;
                mutare_stanga(matrice);
            }
    }
}

void adunare_dreapta(int matrice[][4], int *score)
{
    mutare_dreapta(matrice);
    int i, j;
    for (i = 0; i < 4; ++i)
    {
        for (j = 3; j > 0; --j)
            if (matrice[i][j] == matrice[i][j - 1])
            {
                matrice[i][j] *= 2;
                *score += matrice[i][j];
                matrice[i][j - 1] = 0;
                mutare_dreapta(matrice);
            }
    }
}

int la_fel(int matrice[][4], int matrice_anterioara[][4])
{
    int i, j;
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            if (matrice_anterioara[i][j] != matrice[i][j])
                return 0;
    return 1;
}

void sterge_ecran(WINDOW *win)
{
    if (win != NULL)
    {
        wbkgd(win, COLOR_PAIR(0));
        wclear(win);
        wrefresh(win);
        delwin(win);
    }
}

int is_winner(int matrice[][4])
{
    int i, j;
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            if (matrice[i][j] == 2048)
                return 1;
    return 0;
}

int is_loser(int matrice[][4], int *score)
{
    int i, j, copie[4][4], plin = 1;
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
        {
            copie[i][j] = matrice[i][j];
            if (matrice[i][j] == 0)
                plin = 0;
        }
    /*daca matricea este plina si nici o mutare nu ii mai modifica valorile
    jocul este terinat*/
    if (plin == 1)
    {
        adunare_sus(copie, score);
        adunare_jos(copie, score);
        adunare_stanga(copie, score);
        adunare_dreapta(copie, score);
        if (la_fel(copie, matrice) == 1)
            return 1;
    }
    return 0;
}

void mesaj_final(WINDOW *menu_win, WINDOW *M2048[][4],
                 int matrice[][4], int *resume, char **choices, int *score)
{
    /*daca o conditie de terminare a jocului este indeplinita
    afisez mesajul corespunzator si resetez resune la 0 pt a fi nevoie de
    crearea unui nou joc*/
    if (is_winner(matrice) == 1)
    {
        *resume = 0;
        mvwprintw(stdscr, 10, 10, "Ai castigat");
        wrefresh(stdscr);
        getch();
        werase(stdscr);
        wrefresh(stdscr);
        clear();
        refresh();
        menu(menu_win, choices, M2048, matrice, resume, score);
    }
    else if (is_loser(matrice, score) == 1)
    {
        *resume = 0;
        mvwprintw(stdscr, 10, 10, "Ai pierdut");
        wrefresh(stdscr);
        getch();
        werase(stdscr);
        wrefresh(stdscr);
        clear();
        refresh();
        menu(menu_win, choices, M2048, matrice, resume, score);
    }
}
// pt. ora si minut
char *timestr(struct tm t, char *time)
{
    sprintf(time, "%d:%d", t.tm_hour, t.tm_min);
    return time;
}
// pt. zi luna si an
char *timestr2(struct tm t, char *time)
{
    sprintf(time, "%d.%d.%d", t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
    return time;
}

void afisare_panou(int *score)
{
    WINDOW *panou;
    int stdy, stdx, pany, panx;
    getmaxyx(stdscr, stdy, stdx);
    pany = (stdy - WIN_HEIGHT * 4) / 2;
    panx = (stdx + WIN_WIDTH * 5 - PAN_WIDTH) / 2;
    panou = newwin(PAN_HEIGHT, PAN_WIDTH, pany, panx);
    wbkgd(panou, COLOR_PAIR(20));
     char ora[100], data[100];
     time_t t;
     struct tm *area;
     t = time(NULL);
     area = localtime(&t);
     /* atribui variabilelor ora si data datele corepunzatoare (folosim buffer pe stiva)
         pentru a evita alocari inutile si scurgeri de memorie la apeluri repetate */
     timestr(*area, ora);
     timestr2(*area, data);
    int x, y;
    getyx(panou, y, x);
    ++y, ++x;
    box(panou, 0, 0);
    mvwprintw(panou, y, x, "Date: %s", data);
    getyx(panou, y, x);
    x = x + 2;
    mvwprintw(panou, y, x, "Time: %s", ora);
    getyx(panou, y, x);
    x = x + 2;
    mvwprintw(panou, y, x, "Score: %d", *score);
    x = 1;
    y = y + 2;
    mvwprintw(panou, y, x, "Press q to go back to menu");
    y = y + 1;
    mvwprintw(panou, y, x, "Up arrow key to move up");
    y = y + 1;
    mvwprintw(panou, y, x, "Left arrwow key to move left");
    y = y + 1;
    mvwprintw(panou, y, x, "Right arrwow key to move right");
    y = y + 1;
    mvwprintw(panou, y, x, "Down arrwow key to move down");
    y = y + 1;
    wrefresh(panou);
    delwin(panou);
}

void auto_move(int matrice[][4], int *score, WINDOW *M2048[][4])
{
    int i, j, st = 0, dr = 0, sus = 0, jos = 0, copie[4][4], max = 0;
    int mutare, score_copie = 0;

    /*copiez valorile matricei "matrice" in matricea "copie"
    inaintea fiecarei modificari si fac fiecare mutare posibila in parte
    pt. a vedea cu cate casute goale voi ramane dupa*/
    copiere_matrice(matrice, copie);
    adunare_sus(copie, &score_copie);
    if (la_fel(matrice, copie) == 0)
        for (i = 0; i < 4; ++i)
            for (j = 0; j < 4; ++j)
                if (copie[i][j] == 0)
                    ++sus;
    copiere_matrice(matrice, copie);
    adunare_jos(copie, &score_copie);
    if (la_fel(matrice, copie) == 0)
        for (i = 0; i < 4; ++i)
            for (j = 0; j < 4; ++j)
                if (copie[i][j] == 0)
                    ++jos;
    copiere_matrice(matrice, copie);
    adunare_stanga(copie, &score_copie);
    if (la_fel(matrice, copie) == 0)
        for (i = 0; i < 4; ++i)
            for (j = 0; j < 4; ++j)
                if (copie[i][j] == 0)
                    ++st;
    copiere_matrice(matrice, copie);
    adunare_dreapta(copie, &score_copie);
    if (la_fel(matrice, copie) == 0)
        for (i = 0; i < 4; ++i)
            for (j = 0; j < 4; ++j)
                if (copie[i][j] == 0)
                    ++dr;
    // aflu ce mutare elibereaza cele mai multe casute
    if (sus > max)
    {
        max = sus;
        mutare = KEY_UP;
    }
    if (st > max)
    {
        max = st;
        mutare = KEY_LEFT;
    }

    if (dr > max)
    {
        max = dr;
        mutare = KEY_RIGHT;
    }

    if (jos > max)
    {
        max = jos;
        mutare = KEY_DOWN;
    }
    /*fac mutarea cu cele mai multe castute eliberate*/
    switch (mutare)
    {
    case KEY_UP:
        adunare_sus(matrice, score);
        M2048_refresh(M2048, matrice);
        afisare_panou(score);
        break;
    case KEY_DOWN:
        adunare_jos(matrice, score);
        M2048_refresh(M2048, matrice);
        afisare_panou(score);
        break;
    case KEY_LEFT:
        adunare_stanga(matrice, score);
        M2048_refresh(M2048, matrice);
        afisare_panou(score);
        break;
    case KEY_RIGHT:
        adunare_dreapta(matrice, score);
        M2048_refresh(M2048, matrice);
        afisare_panou(score);
        break;
    default:
        break;
    }
}

void copiere_matrice(int matrice[][4], int copie[][4])
{
    int i, j;
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            copie[i][j] = matrice[i][j];
}

void printCustomAsciiArt(int y, int x)
{
    mvprintw(y, x, "  _____    ______   __   __      ______      ");
    mvprintw(y + 1, x, " /_____/\\ /_____/\\ /__/\\/__/\\   /_____/\\     ");
    mvprintw(y + 2, x, " \\:::_:\\ \\\\:::_ \\ \\  \\ \\: \\ \\__ \\:::_:\\ \\    ");
    mvprintw(y + 3, x, "     _\\:\\| \\:\\ \\ \\ \\::\\_\\::\\/_/  \\:\\_\\:\\ \\   ");
    mvprintw(y + 4, x, "    /::_/__ \\:\\ \\ \\ \\_:::   __\\/  \\::__:\\ \\  ");
    mvprintw(y + 5, x, "    \\:\\____/\\\\:\\_\\ \\ \\    \\::\\ \\   \\:\\_\\:\\ \\ ");
    mvprintw(y + 6, x, "     \\_____\\/ \\_____\\/     \\__\\/    \\_____\\/ ");
    refresh(); // Refresh the screen to make the changes visible
}