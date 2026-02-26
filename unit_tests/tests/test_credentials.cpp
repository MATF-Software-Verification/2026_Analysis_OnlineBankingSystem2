#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../OnlineBankingSystem/utility.h" 
#include <cstring>

TEST_CASE("Testiranje klase Credential", "[Credential]") {

    SECTION("Testiranje defaultnog konstruktora") {
        Credential cred;
        REQUIRE(cred.ID == -1);
        REQUIRE(std::strcmp(cred.pass, "") == 0);
    }

    SECTION("Testiranje parametrizovanog konstruktora") {
        Credential cred(101, "sigurnaLozinka");
        REQUIRE(cred.ID == 101);
        REQUIRE(std::strcmp(cred.pass, "sigurnaLozinka") == 0);
    }

    SECTION("Testiranje setID funkcije") {
        Credential cred;
        cred.setID(500);
        REQUIRE(cred.ID == 500);
    }

    SECTION("Testiranje setPass funkcije") {
        Credential cred;
        cred.setPass("novaSifra123");
        REQUIRE(std::strcmp(cred.pass, "novaSifra123") == 0);
    }

    SECTION("Testiranje operatora == (Autentifikacija)") {
        Credential baza(1, "admin123");
        
        SECTION("Ispravan ID i ispravna lozinka") {
            Credential unos(1, "admin123");
            REQUIRE(baza == unos);
        }

        SECTION("Pogrešan ID, ispravna lozinka") {
            Credential unos(2, "admin123");
            REQUIRE_FALSE(baza == unos);
        }

        SECTION("Ispravan ID, pogrešna lozinka") {
            Credential unos(1, "pogresna");
            REQUIRE_FALSE(baza == unos);
        }

        SECTION("Sve pogrešno") {
            Credential unos(99, "nista");
            REQUIRE_FALSE(baza == unos);
        }
        
        SECTION("Prazna lozinka") {
            Credential bazaPrazna(1, "");
            Credential unosPrazna(1, "");
            REQUIRE(bazaPrazna == unosPrazna);
        }
		
		SECTION("Testiranje Case Sensitivity (Velika i mala slova)") {
        Credential baza(101, "Sifra123");
        Credential unos(101, "sifra123"); 
        
        REQUIRE_FALSE(baza == unos);
    }

    SECTION("Testiranje kopiranja objekta") {
        Credential original(5, "kopirajMe");
        Credential kopija = original; // Poziv copy konstruktora
        
        REQUIRE(kopija.ID == original.ID);
        REQUIRE(std::strcmp(kopija.pass, original.pass) == 0);
        REQUIRE(kopija == original);
    }

    SECTION("Testiranje maksimalne dužine lozinke") {
        // limit je 30 karaktera
        const char* dugaLozinka = "12345678901234567890123456789"; 
        Credential cred(1, dugaLozinka);
        
        REQUIRE(std::strcmp(cred.pass, dugaLozinka) == 0);
    }
    }

}

