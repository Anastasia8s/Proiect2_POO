#pragma once
#include "ContBancar.h"

class ContEconomii : public ContBancar {
   private:
    double rata_dobanda;

   protected:
    void afisare() const override;

   public:
    // constructorul
    ContEconomii(const std::string& nume, const std::string& cont_iban, double sold_initial, double dobanda);

    // suprascrierea functiei de retragere
    void retragere(double suma) override;

    // functie specifica pentru contul de econimii
    void adaugaDobanda();

    // suprascriere clone
    std::shared_ptr<ContBancar> clone() const override;
};