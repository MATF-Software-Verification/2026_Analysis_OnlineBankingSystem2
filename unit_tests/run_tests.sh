#!/bin/bash

cd "$(dirname "$0")"

echo "Čišćenje starih datoteka..."
rm -f testsAll coverage.info coverage-filtered.info
rm -rf coverage-report
find .. -name "*.gcda" -exec rm -f {} \;
find .. -name "*.gcno" -exec rm -f {} \;

TEST_FILES=(./tests/test_*.cpp)

echo "Kompajliranje i pokretanje testova pojedinačno..."

for test_file in "${TEST_FILES[@]}"; do
    test_name=$(basename "$test_file" .cpp)
    echo "-------------------------------------------------------"
    echo "Obrađujem: $test_name"
    
    g++ -std=c++17 --coverage -g -fpermissive "$test_file" -o "$test_name"  -I../OnlineBankingSystem -I./tests
    
    if [ $? -ne 0 ]; then
        echo "Greška pri kompajliranju $test_name. Prekidam."
        exit 1
    fi
    
    ./"$test_name"
    
    rm "$test_name"
done

echo "-------------------------------------------------------"
echo "Svi testovi su izvršeni. Prikupljam podatke o pokrivenosti..."

lcov --capture --directory . --output-file coverage.info --quiet --ignore-errors gcov,source

echo "Filtriram izveštaj..."

lcov --remove coverage.info \
    '/usr/*' \
    '*/catch.hpp' \
    -o coverage-filtered.info --quiet --ignore-errors unused,empty

echo "Generišem HTML izveštaj..."

if [ -f coverage-filtered.info ]; then
    genhtml coverage-filtered.info --output-directory coverage-report --quiet --ignore-errors source
    echo "-------------------------------------------------------"
    echo "Gotovo! Izveštaj se nalazi u: unit_tests/coverage-report/index.html"
else
    echo "Greška: coverage-filtered.info nije kreiran. Proveri da li su .gcda fajlovi generisani."
fi

echo "Čišćenje nepotrebnih fajlova..."

rm -f *.o

rm -f catch_main_helper.cpp

echo "-------------------------------------------------------"
echo "Gotovo! .gcda i .gcno fajlovi su sačuvani, ostalo je obrisano."
