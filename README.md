# Analiza projekta "Online Banking System"

U okviru ovog direktorijuma je prikazana dinamička i statička analiza projekta u okviru kursa **Verifikacija softvera** na master studijama na Matematičkom fakultetu Univerziteta u Beogradu.

# O autoru
Ime i prezime: Ana Veličković 

Broj indeksa: 1128/2025

Smer: Informatika

# Projekat "Online Banking System"

Aplikacija je pisana u **C++** programskom jeziku i pokreće se iz terminala, pri pokretanju se otvara glavni meni gde se može izabrati jedan od sledećih modula: 1.login klijenta (trenutno omogućava klijentina uvid u stanje na računu i promenu lozinke), 2. login zaposlenog (pruža funkcionalnosti za rad sa klijentima, uključujući registraciju novih korisnika, pregled postojećih podataka i upravljanje njihovim profilima) i 3. login admina (omogućava adminu kreiranje, modifikaciju i brisanje naloga zaposlenih i klijenata, kao i uvid u kompletan bankovni sistem).

Online Banking System je projekat otvorenog koda dostupnan na adresi [OnlineBankingSystem](https://github.com/Razdeep/OnlineBankingSystem/) 

Grana projekta na kom se radi analiza: master

Komit (hash code) projekta: [dc2130a110212b7faebaaa2dafd7af2f3d2af304](https://github.com/Razdeep/OnlineBankingSystem/commit/dc2130a110212b7faebaaa2dafd7af2f3d2af304)

# Spisak alata korišćenih za analizu

| Alat | Tip verifikacije | Opis |
|------|------------------|------|
| **Catch2 + pcov** | Dinamička verifikacija | Catch2 je C++ framework za pisanje i izvršavanje jediničnih testova, dok pcov služi za merenje pokrivenosti koda tokom izvršavanja testova. |
| **Valgrind Memcheck** | Dinamička verifikacija | Alat za analizu memorije koji detektuje curenje memorije, pristup neinicijalizovanoj memoriji i druge greške upravljanja memorijom tokom izvršavanja programa. |
| **Cppcheck** | Statička verifikacija | Alat za statičku analizu C/C++ koda koji otkriva potencijalne greške, logičke probleme i sigurnosne propuste bez izvršavanja programa. |
| **Clang-Tidy** | Statička verifikacija | Alat zasnovan na Clang-u za statičku analizu i proveru stila, pronalaženje bugova i unapređenje kvaliteta C++ koda. |
| **Flawfinder** | Statička verifikacija | Alat za bezbednosnu analizu C/C++ koda koji identifikuje potencijalne ranjivosti poput buffer overflow i command injection problema. |
| **IWYU (Include What You Use)** | Statička verifikacija | Alat za analizu uključivanja zaglavlja u C++ projektima, koji pomaže da se uklone nepotrebni include-ovi i poboljša struktura koda. |

# Uputstva za reprodukciju rezultata

| Alat | Instalacija | Dodavanje +x privilgije | Pokretanje skripte |
|------|-------------|-------------------------|--------------------|
| **Catch2 + pcov** | sudo apt install **catch2** sudo apt install **lcov** | chmod +x **run_tests.sh** | **./run_tests.sh** |
| **Valgrind Memcheck** | sudo apt install **valgrind** | chmod +x **run_valgrind_memcheck.sh** | **./run_valgrind_memcheck.sh** |
| **Cppcheck** | sudo apt install **cppcheck** | chmod +x **run_cppcheck.sh** | **./run_cppcheck.sh** |
| **Clang-Tidy** | sudo apt install **clang-tidy** | chmod +x **run_clang.sh** | **./run_clang.sh** |
| **Flawfinder** | sudo apt install **flawfinder** | chmod +x **run_flawfinder.sh** | **./run_flawfinder.sh** |
| **IWYU** | sudo apt install **include-what-you-use** | chmod +x **run_iwyu.sh** | **./run_iwyu.sh** |

# Zaključci 

- **Unit tests**: Testiranje je od ključnog značaja za osiguranje ispravnosti sistema, a jedinični testovi su omogućili detaljnu proveru funkcionalnosti ključnih klasa u projektu. Korišćenjem biblioteke Catch2 i alata lcov postignuta je visoka pokrivenost testovima. 
- **Valgrind memcheck**: Glavni problem u programu je korišćenje neinicijalizovanih promenljivih, što može dovesti do nepravilnog upisivanja podataka ili neočekivanog ponašanja programa. Curenje memorije nije pristuno.
- **Cppcheck**: Upozorenja i informacije koje alat pruža pomažu u unapređenju kvaliteta koda i mogu se koristiti za dalju optimizaciju i poboljšanje stila programiranja.
- **Clang-tidy**: identifikovane su uglavnom stilske nedoslednosti, rizične konstrukcije i mogućnosti za unapređenje modernog C++ stila. 
- **Flawfinder**: Rezultati pokazuju da kod sadrži značajan broj potencijalnih ranjivosti, od kojih su najkritičnije one vezane za system() i strcpy(). 
- **Iwyu**: IWYU ukazuju na brojne mogućnosti za poboljšanje upravljanja zavisnostima u projektu. Optimizacija #include direktiva može značajno uticati na performanse kompilacije i održavanje koda.
# Zaključak analize
Sprovedena je statička i dinamička analiza kojom je prikazano da Online Banking System ima dobar nivo funkcionalne ispravnosti, visoku pokrivenost testovima i pravilno upravljanje memorijom. Iako nisu uočeni kritični problemi, identifikovane su određene slabosti u pogledu bezbednosti, inicijalizacije promenljivih i stila koda koje je potrebno unaprediti. Kombinacija korišćenih alata omogućila je sveobuhvatnu procenu kvaliteta softvera i ukazala na pravce daljeg unapređenja, poput refaktorisanja rizičnih delova koda, optimizacije i dodatnog proširenja testiranja.
