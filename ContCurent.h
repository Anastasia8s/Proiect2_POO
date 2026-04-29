#pragma once
#include "ContBancar.h"

class ContCurent : public ContBancar {
   protected:
    void afisare() const override;

   public:
    // constructorul
    ContCurent(const std::string& nume, const std::string& cont_iban, double sold_initial);

    // suprascrierea functiei de retragere
    void retragere(double suma) override;

    // suprascriere clone
    std::shared_ptr<ContBancar> clone() const override;
};