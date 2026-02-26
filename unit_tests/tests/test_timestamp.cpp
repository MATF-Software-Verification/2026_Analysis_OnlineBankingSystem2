#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../OnlineBankingSystem/utility.h"

TEST_CASE("Testiranje klase TimeStamp", "[TimeStamp]") {
    
    SECTION("Konstruktori i osnovni Set") {
        TimeStamp t;
        t.set(10, 2, 2024);
        REQUIRE(t.toString() == "10/2/2024");
    }

    SECTION("Logika poređenja (Indirektni test za timeWeight)") {
        TimeStamp rano, kasno, isto;
        rano.set(1, 1, 2023);
        kasno.set(2, 1, 2023); 
        isto.set(1, 1, 2023);

        REQUIRE(rano < kasno);  
        REQUIRE(kasno > rano);  
        REQUIRE(rano == isto);  

        REQUIRE_FALSE(kasno < rano);
        REQUIRE_FALSE(rano > kasno);
        REQUIRE_FALSE(rano == kasno);
    }

    SECTION("Logika prelaza godine") {
        TimeStamp decembar, januar;
        decembar.set(31, 12, 2023);
        januar.set(1, 1, 2024);

        // YYYYMMDD format
        // 20240101 mora biti veće od 20231231
        REQUIRE(januar > decembar);
    }

    SECTION("Copy Konstruktor") {
        TimeStamp original;
        original.set(25, 12, 2025);
        TimeStamp kopija(original);
        
        REQUIRE(kopija == original);
        REQUIRE(kopija.toString() == original.toString());
    }

}

