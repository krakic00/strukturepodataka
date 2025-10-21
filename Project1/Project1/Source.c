#include <stdio.h>
#include <stdlib.h>

#define MAX_DULJINA 100

typedef struct {
    char ime[MAX_DULJINA];
    char prezime[MAX_DULJINA];
    int bodovi;
} Student;

int main() {
    FILE* datoteka;
    char naziv[100];
    char redak[256];
    int brojStudenata = 0;
    int maxBodovi = 0;

    // pitam korisnika da unese ime datoteke koju želim otvoriti
    printf("Unesite naziv datoteke: ");
    scanf("%s", naziv);

    // otvaram datoteku u modu za čitanje
    datoteka = fopen(naziv, "r");
    if (datoteka == NULL) {
        printf("Greska: nije moguce otvoriti datoteku %s\n", naziv);
        return 1;
    }

    // brojim koliko redaka ima (koliko ima studenata u datoteci)
    while (fgets(redak, sizeof(redak), datoteka) != NULL) {
        if (redak[0] != '\n' && redak[0] != '\0')
            brojStudenata++;
    }

    // ako nema redaka, datoteka je prazna pa nema smisla nastaviti
    if (brojStudenata == 0) {
        printf("Datoteka je prazna!\n");
        fclose(datoteka);
        return 1;
    }

    // alociram memoriju za sve studente koje ću učitati iz datoteke
    Student* studenti = (Student*)malloc(brojStudenata * sizeof(Student));
    if (studenti == NULL) {
        printf("Greska: nije moguce alocirati memoriju!\n");
        fclose(datoteka);
        return 1;
    }

    // vraćam pokazivač na početak datoteke da mogu čitati podatke ispočetka
    rewind(datoteka);

    // učitavam podatke o svakom studentu i tražim onog s najviše bodova
    for (int i = 0; i < brojStudenata; i++) {
        fscanf(datoteka, "%s %s %d",
            studenti[i].ime,
            studenti[i].prezime,
            &studenti[i].bodovi);

        if (studenti[i].bodovi > maxBodovi)
            maxBodovi = studenti[i].bodovi;
    }

    fclose(datoteka);

    // ispisujem tablicu s rezultatima i relativnim postotkom bodova
    printf("\n%-15s %-15s %-15s %-20s\n", "Ime", "Prezime", "Bodovi", "Relativni bodovi (%)");
    printf("-----------------------------------------------------------------------\n");

    for (int i = 0; i < brojStudenata; i++) {
        float relativni = (float)studenti[i].bodovi / maxBodovi * 100;
        printf("%-15s %-15s %-15d %-10.2f\n",
            studenti[i].ime,
            studenti[i].prezime,
            studenti[i].bodovi,
            relativni);
    }

    // na kraju oslobađam memoriju koju sam prethodno alocirala
    free(studenti);

    return 0;
}
