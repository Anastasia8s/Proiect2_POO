#pragma once
#include <memory>

#include "ContBancar.h"

class ContCredit : public ContBancar {
   private:
    double limita_inferioara_sold; //limita minima(in valoare negativa) care poate fi acceptata intr-un cont

   protected:
    void afisare() const override;

   public:
    // constructorul
    ContCredit(const std::string& nume, const std::string& cont_iban, double sold_initial, double limita);

    // functiile virtuale suprascrise
    void retragere(double suma) override;

    // suprascriere clone
    std::shared_ptr<ContBancar> clone() const override;
};