#include "Banca.h"

#include <iostream>

#include "ContCredit.h"  //librarie pt dynamic_cast

// constructor
Banca::Banca(const std::string& nume) : nume_banca(nume) {}

// copy constructor: folosim clone pentru a copia obiectele corect
Banca::Banca(const Banca& other) : nume_banca(other.nume_banca) {
    for (const auto& cont : other.lista_conturi) {
        lista_conturi.push_back(cont->clone());
    }
}

// functia de swap
void swap(Banca& a, Banca& b) noexcept {
    std::swap(a.nume_banca, b.nume_banca);
    std::swap(a.lista_conturi, b.lista_conturi);
}

// operatorul =
Banca& Banca::operator=(Banca other) {
    swap(*this, other);
    return *this;
}

void Banca::adaugaCont(std::shared_ptr<ContBancar> cont) {
    lista_conturi.push_back(cont);
}

void Banca::afisareConturi() const {
    std::cout << nume_banca << "\n";
    for (const auto& cont : lista_conturi) {
        std::cout << *cont;  // folosim operatorul << virtual (NVI)
    }
}

// implementare dynamic_cast cu sens
void Banca::extindeLimitaCredit(const std::string& iban, double limita_noua) {
    for (auto& cont : lista_conturi) {
        if (cont->getIban() == iban) {
            // incercam sa convertim pointerul de baza in pointer de credit
            auto cont_credit = std::dynamic_pointer_cast<ContCredit>(cont);
            if (cont_credit) {
                // daca a reusit inseamna ca este un cont de credit

                std::cout << "limita a fost extinsa pentru contul de credit " << iban << "\n";
            } else {
                std::cout << "eroare: acest cont nu este unul de credit!\n";
            }
            return;
        }
    }
}