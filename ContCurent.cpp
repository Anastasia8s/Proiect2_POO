#include "ContCurent.h"
#include "ExceptiiBancare.h"
#include <iostream>

// apelam constructorul din clasa de baza
ContCurent::ContCurent(const std::string& nume, const std::string& cont_iban, double sold_initial)
    : ContBancar(nume, cont_iban, sold_initial) {}  // nu mai trebuie sa construim de mana

// ratragerea pentru contul curent
void ContCurent::retragere(double suma) {

    //aici se verifica daca suma este una corespunzatoare actiunii(o valoare pozitiva)
    //in caz contrar, se da eroare
    if (suma <= 0)
        throw SumaInvalida(); 

    if (suma > sold) 
        throw FonduriInsuficiente(iban);

    sold -= suma;
    std::cout << "s-au retras " << suma << "lei din contul " << iban << "\n"
              << "sold curent: " << sold << "\n\n";
}

std::shared_ptr<ContBancar> ContCurent::clone() const {
    return std::make_shared<ContCurent>(*this);
}

// afisarea
void ContCurent::afisare() const {
    std::cout << "cont curent \n";
    ContBancar::afisare();
}