# Analiza projekta "Snake"

U okviru ovog direktorijuma je prikazana dinamička i statička analiza projekta u okviru kursa **Verifikacija softvera** na master studijama na Matematičkom fakultetu Univerziteta u Beogradu.

# O autoru
Ime i prezime: Ana Veličković 

Broj indeksa: 1128/2025

Smer: Informatika

# Projekat "Snake"

Aplikacija je pisana u **Python** programskom jeziku i pokreće se iz terminala. Analizirani projekat je igra Snake, napisana od strane Kyle Cookerly-ja. Igra se pokreće u terminalu uz pomoć curses biblioteke koja omogućava crtanje po ekranu i čitanje unosa sa tastature bez čekanja na Enter. Igrač kontroliše zmiju koristeći tastere W/A/S/D, sa ciljem da pojede što više voća i poveća skor, pri čemu može izabrati između dva moda — solidni zidovi (game over pri udarcu) i prolazni zidovi (zmija prolazi na drugu stranu). 

Snake je projekat otvorenog koda dostupnan na adresi [Snake](https://github.com/cookerlyk/snake) 

Grana projekta na kom se radi analiza: master

Komit (hash code) projekta: [a06cd390d64edcb4e9505f0c7fd5737c6128d716](https://github.com/cookerlyk/snake/commit/a06cd390d64edcb4e9505f0c7fd5737c6128d716)

# Spisak alata korišćenih za analizu i uputstva za reprodukciju

| Alat | Tip verifikacije | Opis | Reprodukcija |
|------|------------------|------|--------------------------|
| **Pylint** | Statička verifikacija | Pylint je alat za statičku analizu Python koda koji proverava stilske greške, logičke probleme i moguće bagove. | bash pylint/run.sh |
| **Mypy** | Statička verifikacija | Mypy je alat za statičku proveru tipova u Pythonu. Projekat nema type annotations, pa mypy ne prijavljuje greške, što je i samo po sebi zaključak o nedostatku anotacija. | bash mypy/run.sh |
|**Bandit**|Statička verifikacija|Bandit je alat za pronalazak bezbednosnih propusta u Python kodu.|bash bandit/run.sh|
|**Black**|Statička verifikacija|Black je Python code formatter koji proverava da li kod prati standardizovano formatiranje.|bash black/run.sh|
|**Pytest**|Dinamička verifikacija|Jedinični testovi za klase `Snake` i `Board` — svaka metoda se testira izolovano uz mock `curses` window objekta.|bash pytest-unit/run.sh|
|**Pytest**|Dinamička verifikacija-|Integracioni testovi koji proveravaju interakcije između klasa `Snake`, `Board` i `Game` — kolizija sa voćem, prolaz kroz zidove, promena brzine, game over logika.|bash pytest-integration/run.sh|
|**Vulture**|Statička verifikacija|Vulture je alat za statičku analizu Python koda koji pronalazi delove koda koji su definisani ali se nikada ne koriste — tzv. mrtvi kod (dead code).|bash vulture/run.sh|

## Zaključci

1. **Deljeni klasni atributi (potencijalni bug):** U klasi `Snake`, atributi `snake_position`, `snake_body`, `key`, `game_over` i `last_valid_key` su definisani kao klasni atributi (deljeni između svih instanci), a ne instancni. Ovo bi uzrokovalo neočekivano ponašanje pri kreiranju više instanci `Snake` klase.

2. **Duplirani kod u `update_fruit_position`:** Unutar `while` petlje u `board.py`, postavljanje koordinata voca se ponavlja dva puta (jednom na početku petlje i jednom u `else` grani), što je redudantno.

3. **`check_tail_collision` sa 4 identična if bloka:** Sva četiri if/elif bloka u ovoj metodi imaju identičan sadržaj — mogu se uprosti u jedan uslov.

4. **`pass_through_if_wall_hit` koristi `elif` pa može samo jedan zid biti obrađen po frame-u:** Ako zmija neke frame udari u ugao, samo jedan od zidova će biti obrađen zbog `elif` lanca.

5. **Nedostajuće type annotations:** Mypy ne pronalazi greške jer nema anotacija — kod bi imao bolju dokumentovanost i mogućnost statičke provere uz dodavanje tipova.

6. **`random.randint` — lažno pozitivan bezbednosni nalaz:** Bandit flaguje sve pozive `random.randint` kao potencijalni bezbednosni propust, što je za kontekst igre nebitno, ali je korisno znati za projekte gde je slučajnost sigurnosno relevantna.

7. **`game_state_screens.py` — invalid escape sekvence u ASCII art stringu:** Stringovi u `draw_start_window` funkciji sadrže `\` karaktere koji Python 3.12 interpretira kao nevažeće escape sekvence (SyntaxWarning), što bi moglo uzrokovati probleme u budućim verzijama Pythona.
