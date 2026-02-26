#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../OnlineBankingSystem/utility.h"
#include <sstream>

TEST_CASE("Testiranje klase Rajdeep", "[Rajdeep]") {
    Rajdeep r;

    SECTION("drawLine - Razne varijacije") {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());

        r.drawLine(5);             // 5 crtica  ispisuje samo -----
        r.drawLine('+', 3);        // 3 plusa  ispisuje prosledjeni karakter +++
        
        std::cout.rdbuf(oldCout);
        REQUIRE(oss.str() == "-----\n+++\n");
    }

    SECTION("drawLineTrans - Razne varijacije") {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());

        r.drawLineTrans(5, 1); 
        r.drawLineTrans('#', 3, 1); 

        std::cout.rdbuf(oldCout);
        REQUIRE(oss.str() == "-----\n###\n");
    }

    SECTION("Pomoćne funkcije koje su TODO") {
        // Testira delay bez parametara (prazna petlja)
        REQUIRE_NOTHROW(r.delay());
        
        // Testira delay sa parametrom (trenutno @TODO)
        // Poziva samo zbog pokrivenosti koda, jer ne radi nista
        REQUIRE_NOTHROW(r.delay(100));
        
        // Testira transText (trenutno @TODO)
        // Poziva samo zbog pokrivenosti koda, jer ne radi nista
        REQUIRE_NOTHROW(r.transText("Test", 10));
    }

}

