#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../OnlineBankingSystem/utility.h"
#include <sstream>

TEST_CASE("Testiranje klase Customer", "[Customer]") {
    TimeStamp dob(15, 6, 1995);
    Customer cust(101, "Marko", "Beograd", 12345, dob, "sifra123", 1000.0);

    SECTION("Geteri i Nasleđivanje") {
        REQUIRE(cust.getID() == 101);
        REQUIRE(std::string(cust.getName()) == "Marko");
        REQUIRE(cust.getBalance() == Approx(1000.0f));
    }

    SECTION("Finansijske transakcije") {
        // Test deposit
        cust.deposit(500.55f);
        REQUIRE(cust.getBalance() == Approx(1500.55f));
        
        // Test withdraw
        cust.withdraw(100.0f);
        REQUIRE(cust.getBalance() == Approx(1400.55f));
    }

    SECTION("showDetails - Vizuelna provera") {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());

        cust.showDetails();

        std::cout.rdbuf(oldCout);
        std::string output = oss.str();
        
        // Provera formata ispisa (ID, ime, adresa, telefon, datum, balans)
        // Redosled potreban: ID -> name -> address -> phone -> dob -> balance
        REQUIRE(output.find("101") != std::string::npos);
        REQUIRE(output.find("Marko") != std::string::npos);
        REQUIRE(output.find("Beograd") != std::string::npos);
        REQUIRE(output.find("15/6/1995") != std::string::npos); // BITNO: Provera TimeStamp-a
        REQUIRE(output.find("1000") != std::string::npos);
    }

    SECTION("Copy Konstruktor i integritet podataka") {
        Customer copyCust = cust;
        // Provera da li je balans ispravno prenet
        REQUIRE(copyCust.getBalance() == Approx(cust.getBalance()));
        // Provera da li je TimeStamp ispravno prenet
        REQUIRE(std::string(copyCust.getDob().toString()) == "15/6/1995");
    }

}
