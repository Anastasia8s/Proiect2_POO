#include "ContCredit.h"
#include "ExceptiiBancare.h"
#include <iostream>

// apelam constructorul din clasa de baza si initializam limita
ContCredit::ContCredit(const std::string& nume, const std::string& cont_iban, double sold_initial, double limita)
    : ContBancar(nume, cont_iban, sold_initial), limita_inferioara_sold(limita) {
}

void ContCredit::retragere(double suma) {

    if (suma <= 0)
        throw SumaInvalida();

    // verificam daca duparetragere trecem de limita pe minus setata
    if (sold - suma < -limita_inferioara_sold) 
        throw LimitaCreditDepasita(iban);

    sold -= suma;
    std::cout << "s-au retras " << suma << "lei din contul " << iban << "\n"
              << "sold curent: " << sold << "\n\n";
}

std::shared_ptr<ContBancar> ContCredit::clone() const {
    return std::make_shared<ContCredit>(*this);
}

//  afisarea
void ContCredit::afisare() const {
    std::cout << "cont de credit \n";
    ContBancar::afisare();
    std::cout << "limita descoperire permisa: " << limita_inferioara_sold << "lei\n\n";
}