#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../OnlineBankingSystem/utility.h"
#include <sstream>

TEST_CASE("Testiranje klase Employee", "[Employee]") {
    TimeStamp dob(12, 8, 1988);
    Employee emp(501, "Nikola", "Novi Sad", 98765, dob, "adminPass", 75000.0);

    SECTION("Inicijalizacija i Geteri (Provera nasleđivanja)") {
        REQUIRE(emp.getID() == 501);
        REQUIRE(std::string(emp.getName()) == "Nikola");
        REQUIRE(std::string(emp.getAddress()) == "Novi Sad"); // Testiranje Person dela
        REQUIRE(emp.getPhone() == 98765);                    // Testiranje Person dela
        REQUIRE(emp.getSalary() == Approx(75000.0f));       // Upotreba Approx za float
    }

    SECTION("Testiranje lozinke (getPass)") {
        // Controller koristi getPass() za login
        REQUIRE(std::string(emp.getPass()) == "adminPass");
    }

    SECTION("Testiranje podrazumevanog (default) konstruktora") {
        Employee emptyEmp;
        // Proverava da li Person() konstruktor zaista inicijalizuje ID na -1
        //REQUIRE(emptyEmp.getID() == -1); - ovde nema inicijalizacije kao kod test_credentials
        REQUIRE(emptyEmp.getSalary() == Approx(0.0f));
    }

    SECTION("Testiranje showDetails (Struktura ispisa)") {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());

        emp.showDetails();

        std::cout.rdbuf(oldCout);
        std::string output = oss.str();
        
        // Provera redosleda ispisa 
        // showDetails ispisuje sve u novom redu: ID, name, address...
        std::string expectedPart = "501\nNikola\nNovi Sad\n";
        REQUIRE(output.find(expectedPart) != std::string::npos);
    }

}


