#!/bin/bash
# Windows PowerShell version
# CUSTOM HASH TESTING SUMMARY SCRIPT

Write-Host "=== CUSTOM HASH ALGORITMO TESTAVIMO SANTRAUKA ===" -ForegroundColor Green
Write-Host ""

Write-Host "1. Kompiliuojami testai..." -ForegroundColor Yellow
g++ -O2 custom_hash.cpp -o custom_hash
g++ -O2 hash_test.cpp -o hash_test

Write-Host ""
Write-Host "2. Paleidžiami pagrindiniai testai..." -ForegroundColor Yellow
./hash_test

Write-Host ""
Write-Host "3. Testuojami specifiniai failai..." -ForegroundColor Yellow

Write-Host "   - Tuščias failas:"
./custom_hash test_empty.txt

Write-Host "   - Vienas simbolis 'a':"
./custom_hash test_single_a.txt

Write-Host "   - Vienas simbolis 'b':"
./custom_hash test_single_b.txt

Write-Host ""
Write-Host "4. Lavinos efekto demonstracija..." -ForegroundColor Yellow
Write-Host "   Panašių tekstų hash'ai:"

$text1 = "Hello World"
$text2 = "Hello world"  # tik vienos raidės skirtumas

Write-Host "   '$text1' -> " -NoNewline
echo $text1 | ./custom_hash

Write-Host "   '$text2' -> " -NoNewline  
echo $text2 | ./custom_hash

Write-Host ""
Write-Host "5. Deterministiškumo testas..." -ForegroundColor Yellow
$test_text = "Consistency Test"
Write-Host "   Tekstas: '$test_text'"
Write-Host "   1-as bandymas: " -NoNewline
echo $test_text | ./custom_hash
Write-Host "   2-as bandymas: " -NoNewline
echo $test_text | ./custom_hash
Write-Host "   3-as bandymas: " -NoNewline
echo $test_text | ./custom_hash

Write-Host ""
Write-Host "=== TESTAVIMAS BAIGTAS ===" -ForegroundColor Green
Write-Host "Peržiūrėkite:"
Write-Host "- README.md - algorito aprašymui"
Write-Host "- TEST_RESULTS.md - detalūs rezultatai"
Write-Host "- hash_test.cpp - testavimo kodui"