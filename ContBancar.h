#pragma once
#include <iostream>
#include <memory>
#include <string>

class ContBancar {
   private:
    static int numar_total_conturi;  // variabila statica pt a numara conturile

   public:
    static int getNumarTotalConturi();// functie statica pt a citi numarul

   protected:
    std::string titular; //persoana care detine contul
    std::string iban;   //ibanul unic
    double sold;        // banii din cont

    // functia de afisare(virtuala)
    virtual void afisare() const;

   public:
    // constructorul
    ContBancar(const std::string& nume, const std::string& cont_iban, double sold_initial);

    std::string getIban() const;

    // destructorul virtual
    virtual ~ContBancar() = default;

    // functie normala
    void depunere(double suma);

    // functie virtuala
    virtual void retragere(double suma) = 0;

    // friend pt cout
    friend std::ostream& operator<<(std::ostream& out, const ContBancar& cont);

    // constructor virtual (clone)
    virtual std::shared_ptr<ContBancar> clone() const = 0;
};