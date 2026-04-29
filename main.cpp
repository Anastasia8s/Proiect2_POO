#include <iostream>
#include <memory>

#include "Banca.h"
#include "ContCredit.h"
#include "ContCurent.h"
#include "ContEconomii.h"
#include "ExceptiiBancare.h"

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
// g++ *.cpp -o proiect_banca
// .\proiect_banca.exe