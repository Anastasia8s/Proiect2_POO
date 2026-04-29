#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <exception>

//ExceptiiBancare.h

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


// declaratiile claselor din fisierele .h

//ContBancar.h
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

// ContCurent.h 
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

// ContEconomii.h
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

// ContCredit.h 
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

// Banca.h 
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

// implementariile din fisierele .cpp

// ContBancar.cpp
int ContBancar::numar_total_conturi = 0;  // initializam variabila statica

// implementam constructorul folosind lista de initializare
ContBancar::ContBancar(const std::string& nume, const std::string& cont_iban, double sold_initial)
    : titular(nume), iban(cont_iban), sold(sold_initial) {
    numar_total_conturi++;
}

// depunerea
void ContBancar::depunere(double suma) {
    if (suma > 0) {
        sold += suma;
        std::cout << "s-au adaugat " << suma << "lei in contul " << iban << "\n"
                  << "sold curent: " << sold << "\n\n";
    }
}

// getterul
std::string ContBancar::getIban() const {
    return iban;
}

int ContBancar::getNumarTotalConturi() { // implementarea functiei statice
    return numar_total_conturi;
}

// functia de afisare din clasa de baza
void ContBancar::afisare() const {
    std::cout << "titular: " << titular << "\n"
              << "iban: " << iban << "\n"
              << "sold curent: " << sold << " lei\n";
}

// afis
std::ostream& operator<<(std::ostream& out, const ContBancar& cont) {
    cont.afisare();
    return out;
}

// ContCurent.cpp
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

// ContEconomii.cpp 
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

// ContCredit.cpp 
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

// Banca.cpp 
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


//main.cpp

int main() {
    std::cout << "\n\n";
    std::cout << "Aplicatia Bancara: \n\n";

    // instantiem banca
    Banca BancaING("banca ING");

    // instantiem conturile folosind pointeri inteligenti (std::make_shared)
    auto cont1 = std::make_shared<ContCurent>("ana popescu", "RO11CURENT", 1000);
    auto cont2 = std::make_shared<ContEconomii>("maria ion", "RO22ECONOMII", 5000, 0.05);
    auto cont3 = std::make_shared<ContCredit>("mihai radu", "RO33CREDIT", 0, 2000);

    // adaugam conturile in banca
    BancaING.adaugaCont(cont1);
    BancaING.adaugaCont(cont2);
    BancaING.adaugaCont(cont3);

    // testam functia statica (cate conturi s au creat)
    std::cout << "numar total de conturi deschise: " << ContBancar::getNumarTotalConturi() << "\n\n";

    // afisam toate conturile (testeaza operatorul << si NVI)
    BancaING.afisareConturi();

    // testam exceptiile cu try catch
    std::cout << "testare tranzactii (si exceptii)\n";

    try {
        std::cout << "incercam o retragere normala din contul curent:\n";
        cont1->retragere(200);

        std::cout << "incercam sa retragem mai mult decat avem (eroare):\n";
        cont1->retragere(5000);
    } catch (const ExceptieBancara& e) {
        // va aparea eroarea de fonduri insuficiente
        std::cout << e.what() << "\n\n";
    }

    try {
        std::cout << "incercam sa extragem din contul de credit:\n";
        cont3->retragere(1500);  // va merge limita e 2000

        std::cout << "incercam sa trecem de limita (eroare):\n";
        cont3->retragere(1000);  // asta va da eroare
    } catch (const ExceptieBancara& e) {
        // prindem eroarea de limita depasita
        std::cout << e.what() << "\n\n";
    }

    // testam dynamic_cast
    std::cout << "testare dynamic_cast\n";
    BancaING.extindeLimitaCredit("RO33CREDIT", 3000);  // va merge pt ca e de credit
    BancaING.extindeLimitaCredit("RO11CURENT", 3000);  // va da eroare ot ca e curent

    // testam copy and swap
    std::cout << "\ntestare copy and swap\n";
    Banca bancaClonata = BancaING;  // se va apela constructorul de copiere
    std::cout << "s-a creat o clona a bancii\n";

    std::cout << std::endl
              << std::endl;

    // toate cele 3 conturi sunt ale unei persoane
    auto cont4 = std::make_shared<ContCurent>("Ana Ionescu", "RO11CURENT", 1000);
    auto cont5 = std::make_shared<ContEconomii>("Ana Ionescu", "RO22ECONOMII", 5000, 0.05);
    auto cont6 = std::make_shared<ContCredit>("Ana Ionescu", "RO33CREDIT", 0, 2000);

    // le adaugam in banca
    BancaING.adaugaCont(cont4);
    BancaING.adaugaCont(cont5);
    BancaING.adaugaCont(cont6);

    return 0;
}