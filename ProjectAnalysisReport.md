# ProjectAnalysisReport — cookerlyk/snake

## 1. Uvod

Analizirani projekat je terminalna igra Snake, napisana u Pythonu 3 od strane Kyle Cookerly-ja. Igra se pokreće u terminalu uz pomoć `curses` biblioteke koja omogućava crtanje po ekranu i čitanje unosa sa tastature bez čekanja na Enter. Igrač kontroliše zmiju koristeći tastere W/A/S/D, sa ciljem da pojede što više voća i poveća skor, pri čemu može izabrati između dva moda — solidni zidovi (game over pri udarcu) i prolazni zidovi (zmija prolazi na drugu stranu).

Projekat se sastoji od 5 modula i ukupno oko 300 linija koda:

| Fajl | Opis |
|------|------|
| `snake.py` | Klasa `Snake` — kretanje, kolizija sa repom, rast |
| `board.py` | Klasa `Board` — tabla, pozicioniranje voća |
| `game.py` | Klasa `Game` — glavna petlja, kolizija sa voćem, logika zidova |
| `game_state_screens.py` | Funkcije za crtanje ekrana (meni, game over) |
| `snake_main.py` | Entry point |

Analizirana grana: `master`, commit: `a06cd39`.

---

## 2. Pylint — statička analiza koda

### Opis alata

Pylint je alat za statičku analizu Python koda koji proverava stilske greške, potencijalne logičke probleme i odstupanja od PEP 8 standarda. Daje ukupnu ocenu koda od 0 do 10. 

PEP 8 (Python Enhancement Proposal 8) je zvanični vodič za stil pisanja Python koda. Njegova svrha je da kod bude čitljiv, konzistentan i lak za održavanje.
Najvažnija pravila:
- `Uvlačenje (Indentation)` - Koristi se 4 razmaka, ne tabovi.
- `Dužina linije` - Preporučeno je da linija ne prelazi 79 karaktera.
- `Razmaci oko operatora`
- `Nazivi promenljivih i funkcija` - Treba da se koristi snake_main, a ne SnakeMain
- `Nazivi klasa` - SnakeGame
- `Konstante` - pišu se velikim slovima
- `Prazne linije` - Dve prazne linije između definicija klasa i funkcija na nivou modula. Jedna prazna linija između metoda unutar klase.
- `Import naredbe` - svaki import u posebnoj liniji
- `Poređenje sa None` - koristi is, not is, a ne ==
- `Komentari i docstring-ovi` - unutar funkcija za opis tekst između """ i """ 

### Pokretanje

```bash
pylint snake/*.py > pylint/output.txt
```

### Rezultati

Ukupna ocena: **7.61/10**

Pronađeni problemi po kategorijama:

**Konvencije (C):**
- `C0116` — Nedostajući docstringovi na getter/setter metodama u `snake.py` i `board.py`
- `C0301` — Preduge linije (>100 karaktera) u `snake.py`, `game.py` i `snake_main.py` — ukupno 13 linija
- `C0303` / `C0305` — Trailing whitespace i trailing newline u `game_state_screens.py`
- `C0115` — Nedostajući class docstring u `game.py`

**Upozorenja (W):**
- `W1401` — Anomalne escape sekvence u ASCII art stringu u `draw_start_window()` (`game_state_screens.py`, linije 20–24). Stringovi poput `" / _____\ |"` sadrže `\` karaktere koje Python 3.12 interpretira kao nevažeće escape sekvence. Ispravka: dodati `r` prefiks ili duplirati backslash.
- `W0511` — 4 TODO komentara u kodu — autor je obeležio da `move_position()` treba premestiti logiku unosa u `Game` klasu i da neke metode imaju neodgovarajuća imena.

**Refaktorisanje (R):**
- `R0912` — `move_position()` metoda u `snake.py` ima 18 grana (limit je 12). Metoda rukuje unosom tastature i kretanjem u jednom mestu.
- `R0902` — Klasa `Game` ima 8 atributa (limit je 7).
- `R1705` — Nepotreban `else` nakon `return` u `draw_option_select_window()`.

### Zaključak

Kod je čitljiv i funkcionalan, ali ima vidljivih stilskih problema. Najznačajniji nalaz su nevažeće escape sekvence u ASCII art stringu koje u Python 3.12+ podižu `SyntaxWarning` i mogle bi postati greška u budućim verzijama.

---

## 3. Mypy — provera tipova

### Opis alata

Mypy je alat za opcionu statičku proveru tipova u Pythonu. Analizira type annotations i prijavljuje nekonzistentnosti u tipovima pre izvršavanja koda.

### Pokretanje

```bash
mypy snake/*.py --ignore-missing-imports > mypy/output.txt
```

### Rezultati

```
Success: no issues found in 5 source files
```

### Zaključak

Mypy nije pronašao greške jer projekat nema type annotations ni na jednoj funkciji ni metodi. Ovo nije greška, ali znači da mypy nema šta da proveri — ceo projekat je u dinamičkom modu. Zanimljivo je da neke metode poput `set_snake_head_x(self, x_val: int)` imaju anotaciju parametra ali ne i povratnog tipa (`-> None`), što je nedoslednost. Dodavanje anotacija bi poboljšalo dokumentovanost koda i omogućilo otkrivanje grešaka u tipovima pre pokretanja.

---

## 4. Bandit — bezbednosna analiza

### Opis alata

Bandit je alat za pronalazak bezbednosnih propusta u Python kodu. Skenira apstraktno sintaksno stablo i traži poznate obrasce koji mogu predstavljati bezbednosni rizik.

### Pokretanje

```bash
bandit -r snake/ > bandit/output.txt
```

### Rezultati

Pronađeno **6 nalaza**, svi kategorije **Low severity / High confidence**:

| Lokacija | Pravilo | Opis |
|----------|---------|------|
| `board.py:19` | B311 | `randint` za inicijalizaciju `fruit_position` (2 poziva) |
| `board.py:38-39` | B311 | `randint` unutar `update_fruit_position()` (2 poziva) |
| `board.py:45-46` | B311 | `randint` unutar `else` grane (2 poziva) |

Svi nalazi se odnose na upotrebu `random.randint` iz standardne Python biblioteke. Bandit ga flaguje pravilom **B311 (CWE-330)** jer `random` modul nije kriptografski siguran generator pseudo-slučajnih brojeva i ne bi smeo da se koristi u bezbednosno-kritičnom kontekstu (npr. generisanje tokena, lozinki).

### Zaključak

Svi nalazi su lažno pozitivni u kontekstu ove igre — pozicioniranje voća na tabli nema nikakve veze sa bezbednošću. Da je projekat web aplikacija koja generiše sesijske tokene pomoću `random`, ovo bi bio stvaran problem. Za igru, upotreba `random.randint` je potpuno ispravna.

---

## 5. Black — stilska provera formatiranja

### Opis alata

Black je Python code formatter koji nameće konzistentno formatiranje koda. Korišćen u `--check --diff` modu koji ne menja fajlove već samo prijavljuje šta bi promenio.

### Pokretanje

```bash
black --check --diff snake/*.py > black/output.txt
```

### Rezultati

Black bi reformatovao svih **5 fajlova**. Najčešće promene:

- Uklanjanje viška razmaka u poravnatim komentarima (npr. `# For RNG` poravnat na kolonu 37)
- Prelom dugih linija uz Black-ov stil (operatori na početku novog reda)
- Prelom `addch()` i `Snake()` poziva sa višestrukim argumentima na više redova
- Uklanjanje trailing whitespace i viška praznih redova u `game_state_screens.py`

### Zaključak

Projekat ne prati Black-ov standard formatiranja, što je vidljivo posebno u stilu poravnavanja komentara i dugim linijama. Ovo je stilska primedba bez uticaja na funkcionalnost.

---

## 6. Pytest — jedinični testovi

### Opis

Jedinični testovi testiraju svaku metodu klasa `Snake` i `Board` izolovano, bez zavisnosti od stvarnog terminala. Curses `window` objekat je zamenjen `MockWindow` klasom koja simulira `addch()`, `inch()`, `getch()` i `border()` pozive.

### Struktura testova

| Fajl | Klasa | Opis |
|------|-------|------|
| `test_board.py` | `Board` | Inicijalna pozicija voća, `display_fruit`, `update_fruit_position` sa monkeypatching `randint` |
| `test_snake.py` | `Snake` | Kretanje, `jump_snake_position`, `did_go_back_on_self`, `check_tail_collision`, `move_position` sa svim granama |

### Pokretanje

```bash
cd pytest-unit && bash run.sh
```

### Rezultati

**45/45 testova prošlo.**

| Modul | Pokrivenost |
|-------|-------------|
| `snake.py` | **100%** |
| `board.py` | 94% |
| `game.py` | 0% |
| `game_state_screens.py` | 0% |
| **Ukupno** | **57%** |

`game.py` i `game_state_screens.py` nisu pokriveni jediničnim testovima jer su njihove metode testirane integracionim testovima, odnosno zahtevaju interakciju između klasa.

### Zaključak

Sve metode klasa `Snake` i `Board` koje sadrže čistu logiku su pokrivene sa 100% (`snake.py`) odnosno 94% (`board.py`). Nepokrivene linije u `board.py` su getter/setter metode koje nisu direktno pozivane u testovima, ali su indirektno testirane.

---

## 7. Pytest — integracioni testovi

### Opis

Integracioni testovi proveravaju interakcije između klasa, pre svega klase `Game` koja koordinira `Snake` i `Board`. Pošto `Game.__init__` direktno poziva `curses.newwin()`, korišćen je `unittest.mock.patch` da se zameni stvarni poziv i spreči otvaranje terminala.

### Struktura testova

| Fajl | Opis |
|------|------|
| `test_game.py` | Inicijalizacija `Game`, kolizija voća, logika zidova oba moda, prolaz kroz zidove, brzina, game over propagacija, `run_game` orkestrizacija |

### Pokretanje

```bash
cd pytest-integration && bash run.sh
```

### Rezultati

**19/19 testova prošlo.**

| Modul | Pokrivenost |
|-------|-------------|
| `game.py` | **98%** |
| `board.py` | 84% |
| `snake.py` | 28% |
| `game_state_screens.py` | 0% |
| **Ukupno** | **46%** |

Jedina nepokrivena linija u `game.py` je linija 54 (`self.snake.grow_snake()` unutar `check_fruit_collision`) — metoda je pokrivena ali grow_snake poziv nije direktno testiran kroz Game instancu.

### Pronađeni bug — `pass_through_if_wall_hit`

Tokom pisanja integracionih testova uočen je bug u metodi `pass_through_if_wall_hit()` u `game.py`:

```python
def pass_through_if_wall_hit(self):
    if self.snake.get_snake_head_x() == 0:
        self.snake.set_snake_head_x(self.board.get_board_width() - 1)
    elif self.snake.get_snake_head_y() == 0:          # elif !!
        ...
    elif self.snake.get_snake_head_x() == self.board.get_board_width() - 1:
        ...
    elif self.snake.get_snake_head_y() == self.board.get_board_height() - 1:
        ...
```

Zbog `elif` lanca, ako zmija dospe u ugao table (npr. `x=0, y=0`), biće obrađen samo prvi uslov (`x=0`), dok `y=0` neće biti ispravljen. U narednom frame-u zmija će biti na poziciji `x=59, y=0` što može uzrokovati neočekivano ponašanje. Ispravka: zameniti `elif` sa zasebnim `if` naredbama.

---

## 8. Vulture — detekcija mrtvog koda

#### Opis alata

Vulture je alat za statičku analizu Python koda koji pronalazi delove koda koji su definisani ali se nikada ne koriste — tzv. mrtvi kod (*dead code*). To uključuje nekorišćene funkcije, metode, klase, varijable i uvezene module. Alat analizira apstraktno sintaksno stablo bez izvršavanja koda.

#### Pokretanje

```bash
vulture snake/*.py > vulture/output.txt 2>&1
```

#### Rezultati

Tokom pokretanja alata, Python interpreter je prijavio `SyntaxWarning` upozorenja za fajl `game_state_screens.py`, linije 20–24. Radi se o nevažećim escape sekvencama (`\ ` i `\_`) unutar ASCII art stringova koji čine naslovni ekran igre. Python 3.12+ tretira ove sekvence kao grešku u stilu, a u budućim verzijama Pythona mogle bi postati `SyntaxError`. Ispravka podrazumeva dodavanje `r` prefiksa ispred stringa (raw string) ili dupliranje backslash karaktera.

Vulture nije pronašao nekorišćene funkcije, klase niti module. Sve definisane metode u klasama `Snake`, `Board` i `Game` su referencisane u toku izvršavanja igre.

#### Zaključak

Projekat ne sadrži mrtvi kod — sve definisane komponente se aktivno koriste. Jedini nalaz su `SyntaxWarning` upozorenja vezana za escape sekvence u ASCII art stringu, koja su već identifikovana i opisana u sekciji o Pylint analizi. Ovaj nalaz potvrđuje konzistentnost između alata.

---

## 9. Opšti zaključci

| # | Nalaz | Ozbiljnost | Alat |
|---|-------|-----------|------|
| 1 | Deljeni klasni atributi u `Snake` — bug pri višestrukim instancama | Visoka | Ručna analiza |
| 2 | `elif` umesto `if` u `pass_through_if_wall_hit` — bug u ugaonom slučaju | Srednja | Integracioni testovi |
| 3 | Nevažeće escape sekvence u ASCII art stringu — `SyntaxWarning` u Python 3.12+ | Srednja | Pylint, Bandit |
| 4 | Duplirani kod u `update_fruit_position` — redundantno postavljanje koordinata | Niska | Pylint |
| 5 | `check_tail_collision` — 4 identična bloka koja se mogu uprosti | Niska | Pylint |
| 6 | Nedostajuće type annotations | Informativna | Mypy |
| 7 | Stilske nekonzistentnosti — poravnati komentari, preduge linije | Informativna | Black, Pylint |
| 8 | `random.randint` flagovan od Bandit — lažno pozitivno za igru | Informativna | Bandit |
| 9 | Mrtvi kod nije pronađen — sve definisane komponente se koriste | Informativna | Vulture |

Projekat je funkcionalan i čitljiv. Ima mesta na kojima autor treba da poboljša svoj projekat — nedostatak type annotations, TODO komentari koji ukazuju na planirana poboljšanja koja nisu realizovana, i par logičkih propusta koji bi se manifestovali u značajnim slučajevima tokom igranja.
