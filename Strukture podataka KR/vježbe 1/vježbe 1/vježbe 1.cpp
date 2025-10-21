#define _CRT_SECURE_NO_WARNINGS

/*
    Zadatak:
    1. Pročitati koliko redaka ima datoteka (broj studenata).
    2. Dinamički alocirati memoriju za niz struktura studenata.
    3. Učitati sve zapise iz datoteke (ime, prezime, bodovi).
    4. Ispisati ime, prezime, apsolutni i relativni broj bodova.

    Napomena:
    Svaki redak datoteke sadrži: ime prezime bodovi
    relativniBodovi = (bodovi / maxBodovi) * 100
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>   // for _getcwd

#define DATOTEKA_NIJE_OTVORENA (-1)   // Greška ako se datoteka ne može otvoriti
#define MEMORIJA_NIJE_ALOCIRANA (-2)  // Greška ako malloc ne uspije
#define MAX_LINE (1024)               // Maksimalna duljina jednog retka
#define MAX_NAME (50)                 // Maksimalna duljina imena i prezimena

// Struktura za spremanje podataka o studentu
struct _Student;
typedef struct _Student* PozicijaStudenta;
typedef struct _Student
{
    char ime[MAX_NAME];
    char prezime[MAX_NAME];
    int bodovi;
} Student;

// Deklaracije funkcija
int prebrojiRedkeUDatoteci(char* imeDatoteke);
int ucitajStudenteIzDatoteke(char* imeDatoteke, Student* studenti, int brojStudenata, int* maxBodovi);
int ispisiStudente(Student* studenti, int brojStudenata, int maxBodovi);

/* Helper: print message, optional errno info, wait for Enter and exit */
static void ExitWithMessageAndCode(const char* msg, int code)
{
    if (msg && msg[0]) printf("%s\n", msg);
    if (errno != 0) {
        printf("System error: ");
        perror(NULL); /* prints message for errno */
    }
    printf("Pritisnite Enter za izlaz...\n");
    fflush(stdout);
    getchar();
    exit(code);
}

int main()
{
    char imeDatoteke[MAX_NAME] = "studenti.txt"; // Datoteka koju program čita
    int brojStudenata = 0;  // Broj studenata u datoteci
    int maxBodovi = 0;      // Najveći broj bodova
    int povratnaVrijednost = 0; // Za provjeru povratnih vrijednosti funkcija

    char cwd[_MAX_PATH];
    if (_getcwd(cwd, sizeof(cwd)) == NULL) {
        /* show reason and stop so user can see the message */
        ExitWithMessageAndCode("Greška: nije moguće dohvatiti trenutni direktorij.", DATOTEKA_NIJE_OTVORENA);
    }
    printf("CWD: %s\n", cwd);

    // 1. Prebroji koliko redaka ima u datoteci
    brojStudenata = prebrojiRedkeUDatoteci(imeDatoteke);
    if (brojStudenata < 1) {
        /* prebrojiRedkeUDatoteci vraća negativnu vrijednost kad fopen ne uspije
           errno će biti postavljen od strane fopen; koristimo helper da prikažemo poruku i pauziramo */
        ExitWithMessageAndCode("Greška: datoteka nije otvorena ili je prazna.", DATOTEKA_NIJE_OTVORENA);
    }

    // 2. Dinamički alociraj memoriju za studente
    Student* poljeStudenata = (Student*)malloc(brojStudenata * sizeof(Student));
    if (poljeStudenata == NULL) {
        ExitWithMessageAndCode("Greška: memorija nije alocirana.", MEMORIJA_NIJE_ALOCIRANA);
    }

    // 3. Učitaj studente iz datoteke
    povratnaVrijednost = ucitajStudenteIzDatoteke(imeDatoteke, poljeStudenata, brojStudenata, &maxBodovi);
    if (povratnaVrijednost != 0) {
        free(poljeStudenata);
        ExitWithMessageAndCode("Greška pri učitavanju studenata iz datoteke.", povratnaVrijednost);
    }

    // 4. Ispiši studente s apsolutnim i relativnim bodovima
    povratnaVrijednost = ispisiStudente(poljeStudenata, brojStudenata, maxBodovi);
    if (povratnaVrijednost != 0) {
        free(poljeStudenata);
        ExitWithMessageAndCode("Greška pri ispisu studenata.", povratnaVrijednost);
    }

    // 5. Oslobodi memoriju
    free(poljeStudenata);

    // Pause so console stays visible when run outside debugger
    printf("Pritisnite Enter za izlaz...\n");
    getchar();

    return 0; // Program je uspješno završio
}

/*
    Funkcija: prebrojiRedkeUDatoteci
    Ulaz: ime datoteke
    Izlaz: broj redaka (studenata) u datoteci
    Povrat: >0 broj studenata, -1 ako datoteka nije otvorena
*/
int prebrojiRedkeUDatoteci(char* imeDatoteke)
{
    FILE* pokazivacDatoteke = fopen(imeDatoteke, "r");
    if (pokazivacDatoteke == NULL) {
        return DATOTEKA_NIJE_OTVORENA;
    }

    int brojac = 0;
    char buffer[MAX_LINE] = { 0 };

    while (fgets(buffer, MAX_LINE, pokazivacDatoteke) != NULL) {
        brojac++; // Svaki redak = jedan student
    }

    fclose(pokazivacDatoteke);

    return brojac;
}

/*
    Funkcija: ucitajStudenteIzDatoteke
    Ulaz: ime datoteke, polje studenata, broj studenata, pokazivač na maxBodovi
    Izlaz: puni polje studenata, pronalazi maksimalne bodove
    Povrat: 0 ako je sve u redu, -1 ako je greška s datotekom
*/
int ucitajStudenteIzDatoteke(char* imeDatoteke, Student* studenti, int brojStudenata, int* maxBodovi)
{
    FILE* pokazivacDatoteke = fopen(imeDatoteke, "r");
    if (pokazivacDatoteke == NULL) {
        return DATOTEKA_NIJE_OTVORENA;
    }

    int i = 0;
    *maxBodovi = 0; // inicijaliziraj maxBodovi

    for (i = 0; i < brojStudenata; i++) {
        if (fscanf(pokazivacDatoteke, " %s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi) != 3) {
            fclose(pokazivacDatoteke);
            return DATOTEKA_NIJE_OTVORENA;
        }

        if (studenti[i].bodovi > *maxBodovi) {
            *maxBodovi = studenti[i].bodovi; // ažuriraj maksimalne bodove
        }
    }

    fclose(pokazivacDatoteke);

    return 0; // uspjeh
}

/*
    Funkcija: ispisiStudente
    Ulaz: polje studenata, broj studenata, maxBodovi
    Izlaz: ispisuje ime, prezime, bodove i relativne bodove
    Povrat: 0 ako je sve u redu
*/
int ispisiStudente(Student* studenti, int brojStudenata, int maxBodovi)
{
    int i = 0;
    printf("Ime\tPrezime\tBodovi\tRelativni bodovi (%%)\n");
    for (i = 0; i < brojStudenata; i++) {
        double relativniBodovi = (double)studenti[i].bodovi / maxBodovi * 100.0;
        printf("%s\t%s\t%d\t%.2f%%\n",
            studenti[i].ime,
            studenti[i].prezime,
            studenti[i].bodovi,
            relativniBodovi);
    }

    return 0; // uspjeh
}
