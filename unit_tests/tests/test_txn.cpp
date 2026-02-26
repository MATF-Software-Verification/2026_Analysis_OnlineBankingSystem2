#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../OnlineBankingSystem/utility.h"
#include <sstream>
#include <iostream>

TEST_CASE("Testiranje klase Txn (Transakcije)", "[Txn]") {
    TimeStamp ts;
    ts.set(5, 1, 2026);
    
    Customer cust(101, "Jovan Jovanovic", "Bulevar 10", 63111222, ts, "lozinka", 50000.0f);

    SECTION("Testiranje ispisa Debit transakcije") {
        // REDOSLED: id, ts, cust, amount, type
        Txn t(999, ts, cust, 1500.0f, 'd');

        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());

        t.printTxn();

        std::cout.rdbuf(oldCout);
        std::string output = oss.str();

        REQUIRE(output.find("999") != std::string::npos);
        REQUIRE(output.find("Jovan Jovanovic") != std::string::npos);
        REQUIRE(output.find("Debit") != std::string::npos);
    }

    SECTION("Testiranje ispisa Credit transakcije") {
        // REDOSLED: id, ts, cust, amount, type
        Txn t(1000, ts, cust, 3000.0f, 'c');

        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());

        t.printTxn();

        std::cout.rdbuf(oldCout);
        
        REQUIRE(oss.str().find("Credit") != std::string::npos);
        REQUIRE(oss.str().find("3000") != std::string::npos);
    }

    SECTION("Testiranje case-insensitivity ('D' vs 'd')") {
        // Provera za veliko slovo 'D'
        Txn t(1001, ts, cust, 100.0f, 'D'); 

        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());

        t.printTxn();

        std::cout.rdbuf(oldCout);
        REQUIRE(oss.str().find("Debit") != std::string::npos);
    }

}
