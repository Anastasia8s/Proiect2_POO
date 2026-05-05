# Proiect POO: Sistem de Gestiune Bancară (C++)

## 1. Descrierea Aplicației

### Obiectivul Proiectului
Acest proiect reprezintă o aplicație de management bancar dezvoltată în C++, având ca scop demonstrarea conceptelor avansate de Programare Orientată pe Obiecte (POO). Sistemul simulează funcționarea unei bănci care gestionează un portofoliu de clienți și tipuri diferite de conturi bancare (Curent, Economii, Credit), fiecare având un comportament distinct la retrageri și reguli specifice de business.

### Cerințe Tehnice Respectate
Arhitectura a fost construită pe baza unei ierarhii de clase cu o clasă de bază proprie și cel puțin 3 clase derivate. Gestionarea erorilor se face printr-o ierarhie de excepții custom (minim 3 tipuri distincte). Proiectul utilizează polimorfismul la execuție, funcții virtuale pure, interfețe non-virtuale (NVI) și pointeri inteligenți (`std::shared_ptr`). De asemenea, sunt aplicate principii moderne de C++ precum pattern-ul *Copy-and-Swap*, *dynamic_cast* cu sens logic, eliminarea variabilelor globale și utilizarea structurilor standard (`std::string`, `std::vector`). 

---

## 2. Design și Arhitectură Orientată pe Obiecte

### 2.1. Ierarhia Claselor

#### **Clasa de Bază: `ContBancar`**
Reprezintă scheletul oricărui cont din sistem, definind interfața de comunicare și starea de bază.
- **Încapsulare strictă:** Atribute protejate (`titular`, `iban`, `sold`). Am eliminat complet getterii și setterii clasici, implementând principiul *„Tell, Don't Ask”* (ex: metoda `verificaIban()`).
- **NVI (Non-Virtual Interface):** Funcția publică `afisare()` apelează în spate metoda virtuală protejată `print()`, permițând derivatelor să customizeze output-ul.
- **Membri statici:** O variabilă `static int numarTotalConturi` monitorizează global entitățile active.

#### **Clasele Derivate:**
1. **`ContCurent`**: Contul standard. Permite retrageri doar în limita soldului disponibil.
2. **`ContEconomii`**: Introduce `rata_dobanda`. Formatează afișarea polimorfă pentru a include informații despre dobândă.
3. **`ContCredit`**: Modifică radical logica prin suprascrierea funcției virtuale `retragere()`, permițând scăderea soldului sub pragul de zero, până la o `limita_inferioara_sold`.

---

### 2.2. Polimorfism și Cast-uri

#### **Pointeri de bază și Colectii**
Clasa principală `Banca` administrează o colecție eterogenă: `std::vector<std::shared_ptr<ContBancar>>`. Toate operațiunile (afișare, procesare tranzacții) sunt efectuate uniform, deciziile fiind luate la runtime.

#### **Utilizarea `dynamic_cast`**
Downcast-ul este folosit punctual și cu sens logic. În operațiuni specifice băncilor (cum ar fi actualizarea limitelor de credit pentru un client), sistemul folosește `std::dynamic_pointer_cast<ContCredit>` pentru a determina dacă contul procesat este eligibil pentru extinderea creditului, evitând alterarea conturilor de economii sau curente.

---

### 2.3. Gestiunea Sigură a Memoriei (Copy-and-Swap)

Clasa `Banca` este "Resource Manager-ul" aplicației. Pentru a garanta *Strong Exception Safety* și a preveni scurgerile de memorie, am implementat:
- **Metoda virtuală `clone()`** în ierarhia de conturi pentru *Deep Copy* polimorfic.
- **Funcția `swap`** (`noexcept`) pentru a interschimba rapid resursele.
- **Operatorul de atribuire (`operator=`)** implementat prin *Copy-and-Swap*, care asigură că baza de date a băncii rămâne intactă în cazul eșecului unei operațiuni de duplicare.

---

### 2.4. Managementul Excepțiilor

Pentru a separa logica de business de tratarea erorilor, am creat o ierarhie custom de excepții, derivată din `std::exception`:
- `ExceptieBancara` (Clasa de bază, reține mesajul).
- `FonduriInsuficiente` (Când o retragere depășește fondurile).
- `LimitaCreditDepasita` (Specifică pentru comportamentul contului de credit).
- `SumaInvalida` (Prevenirea introducerii valorilor negative).

Aplicația folosește blocuri `try-catch` în `main()` pentru a intercepta aceste excepții, garantând că programul rulează fără întreruperi neașteptate.

---

## 3. Checklist Cerințe Implementate

| Cerință POO | Status implementare | Context în proiect |
| :--- | :---: | :--- |
| **Separare fișiere** | ✅ | Structură curată `.h` și `.cpp` pentru fiecare clasă. |
| **Ierarhie clase (1+3)** | ✅ | `ContBancar` (Bază) + `Curent`, `Economii`, `Credit` (Derivate). |
| **Metode Virtuale Pure / NVI** | ✅ | `retragere()`, `clone()` + implementare NVI pe afișare. |
| **Excepții Custom (3 tipuri)** | ✅ | Ierarhia `ExceptieBancara` apelată la tranzacții invalide. |
| **Polimorfism & shared_ptr** | ✅ | Vectorul de conturi stocat în clasa `Banca`. |
| **dynamic_cast** | ✅ | Validarea obiectelor specifice de tip Credit. |
| **Pattern Copy-and-Swap** | ✅ | Implementat pentru gestiunea instanțelor clasei `Banca`. |
| **Date/Metode Statice** | ✅ | `numarTotalConturi` pentru tracking global instanțe. |
| **Supraîncărcare Operator `<<`**| ✅ | Afișare directă (`cout << *cont`) delegată spre NVI. |
| **Fără variabile globale** | ✅ | Date izolate și încapsulate strict. |

---

## 4. Instrucțiuni de Compilare și Rulare

Nu este necesară utilizarea fișierelor CMake. Proiectul poate fi compilat standard cu orice compilator C++ (suport minim C++11 recomandat pentru `shared_ptr`).

**Compilare din terminal:**
```bash
g++ *.cpp -o banca_app
