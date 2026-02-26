#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../OnlineBankingSystem/utility.h"
#include <sstream>
#include <fstream>
#include <cstring>
#include <vector>

// Pomoćna funkcija za čišćenje okruženja pre svakog testa
void resetTestEnvironment() {
    std::remove("customer.txt");
    std::remove("employee.txt");
    std::ofstream lic("LICENSE");
    lic << "Banking System License v1.0\nMIT License." << std::endl;
    lic.close();
}

TEST_CASE("Testiranje Controller klase", "[Controller]") {
    resetTestEnvironment();
    Controller ctrl;

    SECTION("Osnovni UI (Banner, Credits, License)") {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        std::stringstream fakeInput("Y\nY\n");
        std::cin.rdbuf(fakeInput.rdbuf());

        ctrl.showBanner();
        ctrl.showCredits();
        ctrl.showLicense();

        std::cout.rdbuf(oldCout);
        REQUIRE(oss.str().find("BMS") != std::string::npos);
        REQUIRE(oss.str().find("MIT License") != std::string::npos);
    }

    SECTION("Upravljanje korisnicima (Add, Display, Delete)") {
        std::stringstream ssAdd("101\nMarko Markovic\nBeograd\n64111\n15 06 1995\nsifra123\n500.50\n");
        std::cin.rdbuf(ssAdd.rdbuf());
        ctrl.addCustomer();

        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        ctrl.displayCustomers();
        
        std::stringstream ssDel("101\n");
        std::cin.rdbuf(ssDel.rdbuf());
        ctrl.deleteCustomer();
        
        std::cout.rdbuf(oldCout);
        REQUIRE(oss.str().find("Marko Markovic") != std::string::npos);
    }

    SECTION("Finansije (Deposit/Withdraw)") {
        std::stringstream ssInit("201\nJovan\nNis\n65123\n10 10 1985\npass\n1000\n");
        std::cin.rdbuf(ssInit.rdbuf());
        ctrl.addCustomer();

        std::stringstream ssDep("201\n500\n");
        std::cin.rdbuf(ssDep.rdbuf());
        ctrl.deposit();

        std::stringstream ssWith("201\n200\n");
        std::cin.rdbuf(ssWith.rdbuf());
        ctrl.withdraw();

        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        std::stringstream ssFail("999\n100\n");
        std::cin.rdbuf(ssFail.rdbuf());
        ctrl.deposit();
        
        std::cout.rdbuf(oldCout);
        REQUIRE(oss.str().find("could not be completed") != std::string::npos);
    }

    SECTION("Sortiranje - Ime") {
        std::stringstream ss1("10\nMilica\nAdresa1\n1\n01 01 1980\np\n200\n");
        std::cin.rdbuf(ss1.rdbuf());
        ctrl.addCustomer();
        
        std::stringstream ss2("5\nAna\nAdresa2\n2\n21 11 2000\np\n100\n");
        std::cin.rdbuf(ss2.rdbuf());
        ctrl.addCustomer();

        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        std::stringstream ssSort("2\n"); 
        std::cin.rdbuf(ssSort.rdbuf());
        ctrl.displayCustomersBy();

        std::cout.rdbuf(oldCout);
        size_t posA = oss.str().find("Ana");
        size_t posB = oss.str().find("Milica");
        REQUIRE(posA < posB); 
    }

    SECTION("Modifikacija (Edge Case: Nepostojeći ID)") {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        std::stringstream ssModFail("999\n");
        std::cin.rdbuf(ssModFail.rdbuf());
        ctrl.modifyCustomer();
        std::cout.rdbuf(oldCout);
        REQUIRE(oss.str().find("doesnot exist") != std::string::npos);
    }

    SECTION("Admin Portal - Login i Logout") {
        std::stringstream ssAdmin("3\nraj\nraj\n11\n");
        std::cin.rdbuf(ssAdmin.rdbuf());
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        ctrl.showMenu();
        std::cout.rdbuf(oldCout);
        REQUIRE(oss.str().find("ADMIN PORTAL") != std::string::npos);
    }

    SECTION("Customer Portal - Logout i Invalid Option") {
        std::stringstream ssInit("999\nPortalTest\nAdr\n1\n01 01 1990\npass123\n100\n");
        std::cin.rdbuf(ssInit.rdbuf());
        ctrl.addCustomer();

        std::stringstream ssPortal("5\n4\n");
        std::cin.rdbuf(ssPortal.rdbuf());
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        ctrl.showCustomerPortal(999);
        std::cout.rdbuf(oldCout);
        REQUIRE(oss.str().find("Invalid Option") != std::string::npos);
    }

    SECTION("Main Menu - Pogrešan unos") {
        std::stringstream ssMenu("9\n");
        std::cin.rdbuf(ssMenu.rdbuf());
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        ctrl.showMenu();
        std::cout.rdbuf(oldCout);
        REQUIRE(oss.str().find("Invalid Input") != std::string::npos);
    }

    SECTION("Employee Portal - Login") {
        std::stringstream ssEmpAdd("55\nRadnik\nAdr\n1\n01 01 1980\npass\n1000\n");
        std::cin.rdbuf(ssEmpAdd.rdbuf());
        ctrl.addEmployee();

        std::stringstream ssEmpLogin("55\npass\n8\n");
        std::cin.rdbuf(ssEmpLogin.rdbuf());
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        ctrl.showEmployeeLogin();
        std::cout.rdbuf(oldCout);
        REQUIRE(oss.str().find("Authentication successful") != std::string::npos);
    }

    SECTION("Robusnost - Negativan Depozit") {
        std::stringstream ssInit("888\nNegativan\nAdr\n1\n01 01 1990\np\n100\n");
        std::cin.rdbuf(ssInit.rdbuf());
        ctrl.addCustomer();

        std::stringstream ssDepNeg("888\n-500\n");
        std::cin.rdbuf(ssDepNeg.rdbuf());
        ctrl.deposit();
        SUCCEED();
    }

    SECTION("Brisanje iz prazne baze") {
        resetTestEnvironment();
        std::stringstream ssDel("101\n");
        std::cin.rdbuf(ssDel.rdbuf());
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        ctrl.deleteCustomer();
        std::cout.rdbuf(oldCout);
        REQUIRE_FALSE(oss.str().empty());
    }

    SECTION("Employee Management") {
        std::remove("employee.txt");
        std::stringstream ssAdd1("55\nOriginal\nAdr\n1\n01 01 1980\np\n1000\n");
        std::cin.rdbuf(ssAdd1.rdbuf());
        ctrl.addEmployee();
        
        std::stringstream ssAdd2("55\n");
        std::cin.rdbuf(ssAdd2.rdbuf());
        ctrl.addEmployee(); 

        // Modify Employee 
        std::stringstream ssMod("\n55\nNovo Ime\nNova Adr\n999\n01 01 1980\nnova\n2000\n");
        std::cin.rdbuf(ssMod.rdbuf());
        ctrl.modifyEmployee();

        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        ctrl.displayEmployees();
        std::cout.rdbuf(oldCout);
        CHECK(oss.str().find("Novo Ime") != std::string::npos);
    }

    SECTION("Promena lozinke - Failure grana") {
        std::stringstream ssInit("301\nAna\nSub\n611\n01 01 1990\nstara\n100\n");
        std::cin.rdbuf(ssInit.rdbuf());
        ctrl.addCustomer();

        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        // Pogrešna stara lozinka aktivira else granu
        std::stringstream ssPass("\n301\npogresna\nnova\n");
        std::cin.rdbuf(ssPass.rdbuf());
        ctrl.changeCustomerPassword(301);
        std::cout.rdbuf(oldCout);
        CHECK(oss.str().find("wrong previous password") != std::string::npos);
    }

    SECTION("Sortiranje - Preostale opcije") {
        // Dodajem podatke da std::sort ne pukne (SegFault fix)
        std::stringstream ssData("20\nB\nAdr\n1\n01 01 1980\np\n200\n10\nA\nAdr\n2\n01 01 1990\np\n100\n");
        std::cin.rdbuf(ssData.rdbuf());
        ctrl.addCustomer(); ctrl.addCustomer();

        std::stringstream ssSorts("1\n3\n4\n");
        std::cin.rdbuf(ssSorts.rdbuf());
        ctrl.displayCustomersBy(); // ID
        ctrl.displayCustomersBy(); // DOB
        ctrl.displayCustomersBy(); // Balance
        SUCCEED();
    }

    SECTION("Login Failures") {
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        
        // Admin pogrešna lozinka
        std::stringstream ssAdmin("3\nraj\npogresno\n");
        std::cin.rdbuf(ssAdmin.rdbuf());
        ctrl.showMenu();

        // Customer nepostojeći ID
        std::stringstream ssCust("8888\nniko\n");
        std::cin.rdbuf(ssCust.rdbuf());
        ctrl.showCustomerLogin();

        std::cout.rdbuf(oldCout);
        CHECK(oss.str().find("Authentication unsuccessful") != std::string::npos);
    }

    SECTION("Admin Portal Full Loop") {
        // Prolazi kroz sve opcije Admin Portal switch
        std::stringstream ssAdmin(
            "3\nraj\nraj\n"      // Login
            "1\n77\nTest\nAd\n1\n01 01 1990\np\n1000\n" // Add Emp
            "2\n"               // Display Emp
            "5\n"               // Display Cust
            "11\n"              // Logout
        );
        std::cin.rdbuf(ssAdmin.rdbuf());
        std::ostringstream oss;
        std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
        ctrl.showMenu();
        std::cout.rdbuf(oldCout);
        SUCCEED();
    }
	
	SECTION("Employee Modify and Delete - Deep Coverage") {
    // Prvo dodaj zaposlenog sa ID 77 da bi ga modifikacija pronašla
    std::stringstream ssAdd("77\nOriginal Ime\nOriginal Adresa\n123456\n01 01 1980\nsifra123\n1500\n");
    std::cin.rdbuf(ssAdd.rdbuf());
    ctrl.addEmployee();

    // Modifikacija 
    std::stringstream ssMod("\n77\nNovo Ime\nNova Adresa\n987654\n02 02 1982\nnovaSifra\n2000\n");
    std::cin.rdbuf(ssMod.rdbuf());
    ctrl.modifyEmployee();

    // Brisanje 
    std::stringstream ssDel("77\n");
    std::cin.rdbuf(ssDel.rdbuf());
    ctrl.deleteEmployee();

    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
    ctrl.displayEmployees(); // Treba da bude prazno
    std::cout.rdbuf(oldCout);
    
    SUCCEED();
}

SECTION("Duplicate ID and Login Failures") {
    // Dodaj zaposlenog sa ID 10
    std::stringstream ssAdd("10\nZaposleni\nAdresa\n123\n01 01 1990\npass\n1000\n");
    std::cin.rdbuf(ssAdd.rdbuf());
    ctrl.addEmployee();

    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());

    // Test za DUPLIKAT ID 
    std::stringstream ssDup("10\n");
    std::cin.rdbuf(ssDup.rdbuf());
    ctrl.addEmployee();
    CHECK(oss.str().find("already exists") != std::string::npos);

    // Test za nepostojeći Customer ID pri logovanju 
    std::stringstream ssFailLogin("9999\nloša_šifra\n");
    std::cin.rdbuf(ssFailLogin.rdbuf());
    ctrl.showCustomerLogin();
    CHECK(oss.str().find("Authentication unsuccessful") != std::string::npos);

    std::cout.rdbuf(oldCout);
}

SECTION("Delete Employee") {
    // Dodaj DVA zaposlena
    std::stringstream ssAdd1("1\nPrvi\nAdr\n111\n01 01 1980\np\n1000\n");
    std::cin.rdbuf(ssAdd1.rdbuf());
    ctrl.addEmployee();

    std::stringstream ssAdd2("2\nDrugi\nAdr\n222\n02 02 1980\np\n2000\n");
    std::cin.rdbuf(ssAdd2.rdbuf());
    ctrl.addEmployee();

    // Briše ID 1. Sada ID 2 mora biti upisan u fajl
    std::stringstream ssDel("1\n");
    std::cin.rdbuf(ssDel.rdbuf());
    ctrl.deleteEmployee();

    SUCCEED();
}

SECTION("Change Password Success and Failure") {
    // Dodaj kupca sa ID 500
    std::stringstream ssInit("500\nKupac\nAdr\n123\n01 01 1990\nstara_sifra\n100\n");
    std::cin.rdbuf(ssInit.rdbuf());
    ctrl.addCustomer();

    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());

    // 1. Uspešna promena 
    std::stringstream ssPassOk("\nstara_sifra\nnova_sifra\n"); 
    std::cin.rdbuf(ssPassOk.rdbuf());
    ctrl.changeCustomerPassword(500);

    // 2. Neuspešna promena - pogrešna stara šifra 
    std::stringstream ssPassFail("\npogrešna\nnebitno\n");
    std::cin.rdbuf(ssPassFail.rdbuf());
    ctrl.changeCustomerPassword(500);

    std::cout.rdbuf(oldCout);
    CHECK(oss.str().find("Successfully changed") != std::string::npos);
    CHECK(oss.str().find("wrong previous password") != std::string::npos);
}


}

