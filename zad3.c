#include <stdio.h>      // uključujem biblioteku za ispis i unos (printf, scanf)
#include <stdlib.h>     // uključujem biblioteku za malloc i free
#include <string.h>     // uključujem biblioteku za rad s tekstom (strcpy, strcmp)

// ovdje definiram strukturu koja predstavlja jednu osobu u listi
typedef struct Osoba {
    char ime[50];               // ime osobe
    char prezime[50];           // prezime osobe
    int godinaRodenja;          // godina rođenja
    struct Osoba* next;         // pokazivač na sljedeću osobu u listi
} Osoba;

// funkcija koja pravi (dinamički stvara) novu osobu
Osoba* kreirajOsobu(const char* ime, const char* prezime, int godina) {
    Osoba* nova = (Osoba*)malloc(sizeof(Osoba));   // alociram memoriju za novu osobu
    if (!nova) {                                   // provjeravam je li malloc uspio
        printf("Greska pri alokaciji memorije!\n");
        return NULL;                               // ako nije, vraćam NULL
    }
    strcpy(nova->ime, ime);                        // kopiram ime u strukturu
    strcpy(nova->prezime, prezime);                // kopiram prezime
    nova->godinaRodenja = godina;                  // upisujem godinu rođenja
    nova->next = NULL;                             // nova osoba zasad ne pokazuje ni na koga
    return nova;                                   // vraćam pokazivač na novu osobu
}

// funkcija koja dodaje novu osobu na početak liste
void dodajNaPocetak(Osoba** head, const char* ime, const char* prezime, int godina) {
    Osoba* nova = kreirajOsobu(ime, prezime, godina);  // prvo napravim novu osobu
    if (nova == NULL) return;                          // ako nije uspjelo, izlazim iz funkcije
    nova->next = *head;                                // nova osoba pokazuje na staru prvu osobu
    *head = nova;                                      // nova osoba postaje nova “glava” liste
}

// funkcija koja ispisuje sve osobe u listi
void ispisiListu(Osoba* head) {
    Osoba* temp = head;                                // pomoćni pokazivač za prolazak kroz listu
    if (temp == NULL) {                                // ako je lista prazna
        printf("Lista je prazna!\n");
        return;                                        // ne radim ništa
    }

    // ispisujem zaglavlje (naslove stupaca)
    printf("\n%-15s %-15s %-15s\n", "Ime", "Prezime", "Godina rodenja");
    printf("---------------------------------------------\n");

    // prolazim kroz sve osobe i ispisujem ih redom
    while (temp != NULL) {
        printf("%-15s %-15s %-15d\n", temp->ime, temp->prezime, temp->godinaRodenja);
        temp = temp->next;                             // prelazim na sljedeću osobu
    }
}

// funkcija koja dodaje novu osobu na kraj liste
void dodajNaKraj(Osoba** head, const char* ime, const char* prezime, int godina) {
    Osoba* nova = kreirajOsobu(ime, prezime, godina);  // kreiram novu osobu
    if (nova == NULL) return;                          // ako nije uspjelo, prekidam

    if (*head == NULL) {                               // ako je lista prazna
        *head = nova;                                  // nova osoba postaje jedina u listi
        return;
    }

    Osoba* temp = *head;                               // krećem od početka liste
    while (temp->next != NULL)                         // idem do kraja (dok next nije NULL)
        temp = temp->next;

    temp->next = nova;                                 // zadnja osoba pokazuje na novu osobu
}

// funkcija koja traži osobu po prezimenu
Osoba* pronadiOsobu(Osoba* head, const char* prezime) {
    Osoba* temp = head;                                // počinjem od prve osobe
    while (temp != NULL) {                             // dok ima osoba u listi
        if (strcmp(temp->prezime, prezime) == 0)       // ako se prezime poklapa
            return temp;                               // vraćam pokazivač na tu osobu
        temp = temp->next;                             // inače idem dalje
    }
    return NULL;                                       // ako nisam našla, vraćam NULL
}

// funkcija koja briše osobu po prezimenu
void obrisiOsobu(Osoba** head, const char* prezime) {
    Osoba* temp = *head;                               // trenutna osoba
    Osoba* prethodna = NULL;                           // prethodna osoba (za povezivanje)

    // prolazim listu dok ne nađem traženo prezime
    while (temp != NULL && strcmp(temp->prezime, prezime) != 0) {
        prethodna = temp;                              // pamtim prethodnu
        temp = temp->next;                             // idem dalje
    }

    if (temp == NULL) {                                // ako nisam našla osobu
        printf("Osoba s prezimenom '%s' nije pronadena.\n", prezime);
        return;
    }

    if (prethodna == NULL) {                           // ako je osoba na početku
        *head = temp->next;                            // pomaknem glavu liste
    }
    else {
        prethodna->next = temp->next;                  // prespajam listu da preskoči tu osobu
    }

    free(temp);                                        // oslobađam memoriju
    printf("Osoba '%s' uspjesno obrisana.\n", prezime);
}

// funkcija koja dodaje osobu iza određene osobe (po prezimenu)
void dodajIza(Osoba* head, const char* prezimeTarget, const char* ime, const char* prezime, int godina) {
    Osoba* trazena = pronadiOsobu(head, prezimeTarget); // tražim osobu iza koje želim dodati
    if (trazena == NULL) {                              // ako ne postoji
        printf("Osoba '%s' nije pronadena.\n", prezimeTarget);
        return;
    }

    Osoba* nova = kreirajOsobu(ime, prezime, godina);   // kreiram novu osobu
    if (nova == NULL) return;

    nova->next = trazena->next;                         // nova pokazuje na osobu iza cilja
    trazena->next = nova;                               // ciljana osoba pokazuje na novu
    printf("Osoba '%s' dodana iza osobe '%s'.\n", prezime, prezimeTarget);
}

// funkcija koja dodaje osobu ispred određene osobe (po prezimenu)
void dodajIspred(Osoba** head, const char* prezimeTarget, const char* ime, const char* prezime, int godina) {
    if (*head == NULL) {                                // ako je lista prazna
        printf("Lista je prazna.\n");
        return;
    }

    if (strcmp((*head)->prezime, prezimeTarget) == 0) { // ako je ciljna osoba prva u listi
        dodajNaPocetak(head, ime, prezime, godina);     // dodajem na početak
        printf("Osoba '%s' dodana ispred osobe '%s'.\n", prezime, prezimeTarget);
        return;
    }

    Osoba* temp = *head;                                // inače prolazim listu
    while (temp->next != NULL && strcmp(temp->next->prezime, prezimeTarget) != 0)
        temp = temp->next;

    if (temp->next == NULL) {                           // ako nisam našla
        printf("Osoba '%s' nije pronadena.\n", prezimeTarget);
        return;
    }

    Osoba* nova = kreirajOsobu(ime, prezime, godina);   // kreiram novu osobu
    if (nova == NULL) return;

    nova->next = temp->next;                            // nova pokazuje na ciljnu
    temp->next = nova;                                  // prethodna pokazuje na novu
    printf("Osoba '%s' dodana ispred osobe '%s'.\n", prezime, prezimeTarget);
}

// funkcija koja sortira listu po prezimenima (abecedno)
void sortirajListu(Osoba** head) {
    if (*head == NULL || (*head)->next == NULL)         // ako je lista prazna ili ima samo jednu osobu
        return;

    int zamjena;                                        // varijabla koja prati je li bilo zamjene
    Osoba* i;
    Osoba* j = NULL;                                    // granica do koje je već sortirano

    do {
        zamjena = 0;                                    // na početku pretpostavljam da nema zamjene
        i = *head;                                      // počinjem od glave liste

        while (i->next != j) {                          // idem sve dok ne dođem do “kraja”
            if (strcmp(i->prezime, i->next->prezime) > 0) { // ako je prezime “veće” (abecedno)
                // zamjenjujem podatke između i i i->next
                char tempIme[50], tempPrezime[50];
                int tempGod;

                strcpy(tempIme, i->ime);
                strcpy(tempPrezime, i->prezime);
                tempGod = i->godinaRodenja;

                strcpy(i->ime, i->next->ime);
                strcpy(i->prezime, i->next->prezime);
                i->godinaRodenja = i->next->godinaRodenja;

                strcpy(i->next->ime, tempIme);
                strcpy(i->next->prezime, tempPrezime);
                i->next->godinaRodenja = tempGod;

                zamjena = 1;                             // označavam da je bilo promjena
            }
            i = i->next;                                 // idem na sljedeći par
        }
        j = i;                                           // kraj do kojeg je sada sortirano
    } while (zamjena);                                   // ponavljam dok ima promjena

    printf("Lista je sortirana po prezimenima.\n");
}

// funkcija koja zapisuje listu u datoteku
void upisiUDatoteku(Osoba* head, const char* nazivDatoteke) {
    FILE* fp = fopen(nazivDatoteke, "w");                // otvaram datoteku za pisanje
    if (!fp) {                                           // ako nije otvorena
        printf("Greska pri otvaranju datoteke!\n");
        return;
    }

    Osoba* temp = head;                                  // idem kroz listu
    while (temp != NULL) {
        fprintf(fp, "%s %s %d\n", temp->ime, temp->prezime, temp->godinaRodenja); // upisujem redak
        temp = temp->next;                               // idem dalje
    }

    fclose(fp);                                          // zatvaram datoteku
    printf("Lista uspjesno upisana u datoteku '%s'.\n", nazivDatoteke);
}

// funkcija koja čita listu iz datoteke
void ucitajIzDatoteke(Osoba** head, const char* nazivDatoteke) {
    FILE* fp = fopen(nazivDatoteke, "r");                // otvaram datoteku za čitanje
    if (!fp) {
        printf("Greska pri otvaranju datoteke!\n");
        return;
    }

    char ime[50], prezime[50];
    int godina;

    // brišem staru listu ako već postoji
    while (*head != NULL) {
        Osoba* temp = *head;
        *head = (*head)->next;
        free(temp);
    }

    // čitam red po red dok ima podataka
    while (fscanf(fp, "%s %s %d", ime, prezime, &godina) == 3) {
        dodajNaKraj(head, ime, prezime, godina);         // dodajem svaku osobu na kraj
    }

    fclose(fp);                                          // zatvaram datoteku
    printf("Lista uspjesno ucitana iz datoteke '%s'.\n", nazivDatoteke);
}

// glavna funkcija
int main() {
    Osoba* head = NULL;                                  // na početku je lista prazna

    // dodajem nekoliko osoba da imam nešto za test
    dodajNaPocetak(&head, "Ana", "Anić", 1998);
    dodajNaPocetak(&head, "Marko", "Marković", 2000);
    dodajNaKraj(&head, "Iva", "Ivić", 1997);

    printf("Lista nakon dodavanja:\n");
    ispisiListu(head);                                   // ispisujem listu

    // testiram dodavanje iza i ispred određene osobe
    dodajIza(head, "Anić", "Petra", "Petrić", 2001);
    dodajIspred(&head, "Marković", "Luka", "Lukić", 1995);

    printf("\nLista nakon umetanja:\n");
    ispisiListu(head);                                   // ispisujem ažuriranu listu

    // sada testiram sortiranje
    sortirajListu(&head);
    printf("\nLista nakon sortiranja:\n");
    ispisiListu(head);

    // zapisujem listu u datoteku
    upisiUDatoteku(head, "osobe.txt");

    // testiram čitanje liste iz datoteke
    ucitajIzDatoteke(&head, "osobe.txt");

    printf("\nLista nakon čitanja iz datoteke:\n");
    ispisiListu(head);

    // na kraju oslobađam svu memoriju
    while (head != NULL) {
        Osoba* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;                                            // kraj programa
}

