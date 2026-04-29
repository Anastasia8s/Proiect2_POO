#pragma once
#include <exception>
#include <string>

// clasa de baza pentru toate erorile bancii noastre
class ExceptieBancara : public std::exception {
   protected:
    std::string mesaj;

   public:
    ExceptieBancara(const std::string& m) : mesaj(m) {}

    // functia what() este cea care returneaza textul erorii
    virtual const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

// exceptie specifica pentru bani putini
class FonduriInsuficiente : public ExceptieBancara {
   public:
    FonduriInsuficiente(const std::string& iban)
        : ExceptieBancara("eroare: fonduri insuficiente in contul " + iban) {}
};

// exceptie specifica pentru sume negative
class SumaInvalida : public ExceptieBancara {
   public:
    SumaInvalida() : ExceptieBancara("eroare: suma introdusa trebuie sa fie pozitiva") {}
};

// exceptie pentru cand se trece de limita pe minus
class LimitaCreditDepasita : public ExceptieBancara {
   public:
    LimitaCreditDepasita(const std::string& iban)
        : ExceptieBancara("eroare: limita de credit a fost depasita pentru contul " + iban) {}
};