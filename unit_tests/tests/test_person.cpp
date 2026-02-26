#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../OnlineBankingSystem/utility.h"
#include <cstring>

TEST_CASE("Testiranje implementacije person.cpp", "[Person]") {
    TimeStamp dob(1, 1, 1990);
    Customer original(100, "Marko", "Beograd", 64123456, dob, "sifra123", 500.0f);

    SECTION("Potpuna provera svih getera") {
        REQUIRE(original.getID() == 100);
        REQUIRE(std::strcmp(original.getName(), "Marko") == 0);
        REQUIRE(std::strcmp(original.getAddress(), "Beograd") == 0);
        REQUIRE(original.getPhone() == 64123456);
        REQUIRE(std::strcmp(original.getPass(), "sifra123") == 0);
        
        // Provera TimeStamp-a 
        REQUIRE(original.getDob().toString() == "1/1/1990");
    }

    SECTION("Modifikacija lozinke") {
        original.setPass("novaSifra456");
        REQUIRE(std::strcmp(original.getPass(), "novaSifra456") == 0);
    }

    SECTION("Copy Konstruktor (Person level)") {
        Customer copy(original); 

        REQUIRE(copy.getID() == original.getID());
        REQUIRE(std::strcmp(copy.getName(), original.getName()) == 0);
        REQUIRE(std::strcmp(copy.getAddress(), original.getAddress()) == 0);
        REQUIRE(copy.getPhone() == original.getPhone());
        
        // Provera da li je TimeStamp ispravno iskopiran
        REQUIRE(copy.getDob().toString() == original.getDob().toString());
        
        // Provera memorijske nezavisnosti 
        REQUIRE((void*)copy.getName() != (void*)original.getName());
    }

}


