# Tema 2 POO - Sistem de Gestiune Bancara

## Descriere

Programul permite simularea operatiunilor financiare de baza si gestionarea mai multor tipuri de conturi apartinand clientilor unei banci.

## Ierarhie de clase

* ContBancar - clasa de baza (abstracta)
  * ContCurent - pentru uz zilnic, permite retrageri in limita soldului
  * ContEconomii - are rata de dobanda si genereaza profit
  * ContCredit - are limita de descoperire si permite extrageri pe minus

## Alte clase importante
* Banca - gestioneaza centralizat toate conturile folosind pointeri inteligenti si realizeaza operatiuni de back-up (copy-and-swap).
* ExceptieBancara - clasa de baza pentru tratarea erorilor (ex: FonduriInsuficiente, SumaInvalida).