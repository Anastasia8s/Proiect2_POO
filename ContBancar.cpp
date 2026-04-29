#include "ContBancar.h"

#include <iostream>

int ContBancar::numar_total_conturi = 0;  // initializam variabila statica

// implementam constructorul folosind lista de initializare
ContBancar::ContBancar(const std::string& nume, const std::string& cont_iban, double sold_initial)
    : titular(nume), iban(cont_iban), sold(sold_initial) {
    numar_total_conturi++;
}

// depunerea
void ContBancar::depunere(double suma) {
    if (suma > 0) {
        sold += suma;
        std::cout << "s-au adaugat " << suma << "lei in contul " << iban << "\n"
                  << "sold curent: " << sold << "\n\n";
    }
}

// getterul
std::string ContBancar::getIban() const {
    return iban;
}

int ContBancar::getNumarTotalConturi() { // implementarea functiei statice
    return numar_total_conturi;
}

// functia de afisare din clasa de baza
void ContBancar::afisare() const {
    std::cout << "titular: " << titular << "\n"
              << "iban: " << iban << "\n"
              << "sold curent: " << sold << " lei\n";
}

// afis
std::ostream& operator<<(std::ostream& out, const ContBancar& cont) {
    cont.afisare();
    return out;
}