#include "ContEconomii.h"

#include <iostream>

#include "ExceptiiBancare.h"

// apelam constructorul din baza si initializam dobanda
ContEconomii::ContEconomii(const std::string& nume, const std::string& cont_iban, double sold_initial, double dobanda)
    : ContBancar(nume, cont_iban, sold_initial), rata_dobanda(dobanda) {}

// retragerea
void ContEconomii::retragere(double suma) {
    if (suma <= 0)
        throw SumaInvalida();

    if (suma > sold)
        throw FonduriInsuficiente(iban);

    sold -= suma;
    std::cout << "s-au retras " << suma << "lei din contul " << iban << "\n"
              << "sold curent: " << sold << "\n\n";
}

std::shared_ptr<ContBancar> ContEconomii::clone() const {
    return std::make_shared<ContEconomii>(*this);
}

// afisarea
void ContEconomii::afisare() const {
    std::cout << "cont de economii:\n";
    ContBancar::afisare();
    std::cout << "rata dobanda: " << rata_dobanda * 100 << "%\n\n";
}

// aplicam dobanzii
void ContEconomii::adaugaDobanda() {
    double profit = sold * rata_dobanda;
    depunere(profit);  // refolosim functia depunere din clasa ContBancar
}