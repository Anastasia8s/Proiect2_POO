#pragma once
#include <memory>
#include <string>
#include <vector>

#include "ContBancar.h"

class Banca {
   private:
    std::string nume_banca;
    std::vector<std::shared_ptr<ContBancar>> lista_conturi;  // am schimbat in shared_ptr

   public:
    // constructor
    Banca(const std::string& nume);

    // copy constructor (necesar pentru copy and swap)
    Banca(const Banca& other);

    // operatorul = (foloseste copy and swap)
    Banca& operator=(Banca other);

    // functia de swap
    friend void swap(Banca& a, Banca& b) noexcept;

    // functii de gestiune
    void adaugaCont(std::shared_ptr<ContBancar> cont);
    void afisareConturi() const;
    void executaRetragere(const std::string& iban_cautat, double suma);

    // dynamic_cast
    void extindeLimitaCredit(const std::string& iban, double limita_noua);
};