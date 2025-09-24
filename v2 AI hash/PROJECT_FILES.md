# CUSTOM HASH PROJEKTO FAILŲ SĄRAŠAS

## Pagrindiniai failai

### Šaltinio kodas
- **`custom_hash.cpp`** - Pagrindinis hash algoritmo implementavimas
- **`hash_test.cpp`** - Išsamus testavimo programos kodas  
- **`constitution_performance_test.cpp`** - Konstitucijos našumo testavimo programa
- **`avalanche_demo.cpp`** - Lavinos efekto demonstracijos kodas

### Vykdomi failai
- **`custom_hash.exe`** - Sukompiliuotas hash'avimo įrankis
- **`hash_test.exe`** - Sukompiliuota testavimo programa

### Dokumentacija
- **`README.md`** - Pagrindinis projekto aprašymas su algoritmo pseudokodu ir išvadomis
- **`TEST_RESULTS.md`** - Detalūs visų testų rezultatai
- **`PROJECT_FILES.md`** - Šis failas (failų sąrašas)

### Testavimo failai

#### Mažų dydžių testai
- **`test_empty.txt`** - Tuščias failas
- **`test_single_a.txt`** - Vienas simbolis 'a'
- **`test_single_b.txt`** - Vienas simbolis 'b'
- **`test.txt`** - Kelių eilučių testas (Lietuva variantai)

#### Didelių dydžių testai  
- **`large_test1.txt`** - 1500 atsitiktinių simbolių
- **`large_test2.txt`** - 1500 atsitiktinių simbolių (skirtingas nuo 1)
- **`konstitucija.txt`** - **Pilna Lietuvos Respublikos Konstitucija (75,595 simboliai)**

### Pagalbiniai scriptai
- **`generate_test_file.ps1`** - PowerShell scriptas atsitiktinių failų generavimui
- **`run_tests.ps1`** - Automatizuotas visų testų paleidimo scriptas

---

## Kaip naudoti

### 1. Pagrindinis hash'avimas
```bash
# Interaktyvus režimas
./custom_hash

# Failo apdorojimas  
./custom_hash failas.txt
```

### 2. Testavimo paleidimas
```bash
# Visi testai
./hash_test

# Arba automatizuotai
powershell -ExecutionPolicy Bypass -File run_tests.ps1
```

### 3. Naujų testinių failų generavimas
```bash
powershell -ExecutionPolicy Bypass -File generate_test_file.ps1 -Length 2000 -Filename "naujas_testas.txt"
```

---

## Projekto statistika

### Kodo eilutės
- **custom_hash.cpp**: ~200 eilučių
- **hash_test.cpp**: ~300 eilučių  
- **Dokumentacija**: ~500 eilučių
- **Iš viso**: ~1000+ eilučių kodo ir dokumentacijos

### Testavimo apimtis
- **40,000+** kolizijų testų
- **1,000** lavinos efekto testų
- **Išsamūs našumo testai** su 75k simbolių failais
- **100%** deterministiškumo padengimas
- **Realių dokumentų testavimas** (LR Konstitucija)

### Palaikomi failų formatai
- Tekstiniai failai (UTF-8)
- Bet kokie dvejetainiai duomenys
- Tuščių failai
- **Labai dideli failai (75k+ simbolių patikrinta)**
- **Realūs dokumentai** (konstitucijos, įstatymai, etc.)

---

## Sukūrimo chronologija

1. **custom_hash.cpp** - Pradinis algoritmo sukūrimas
2. **Pagrindiniai testai** - Deterministiškumo ir dydžio tikrinimas  
3. **hash_test.cpp** - Išsamios testavimo programos implementavimas
4. **Testavimo failai** - Įvairių dydžių ir tipų testinių duomenų sukūrimas
5. **Dokumentacija** - README ir TEST_RESULTS sukūrimas
6. **Pagalbiniai įrankiai** - Scriptų ir demo programų pridėjimas

---

## Ateities plėtojimo galimybės

### Galimi patobulinimai
- SIMD instrukcijų optimizacijos
- Multi-threading palaikymas
- Papildomi saugumo testai
- GUI sąsajos sukūrimas

### Papildomi testai
- Milijardų kolizijų paieška
- Kriptanalitiniai tyrimai  
- Našumo benchmarkai su kitais algoritmais
- Paskirstymo uniformumo analizė

---

## 🏆 GAUTI REZULTATAI

- ✅ **Lavinos efektas:** 50.0% (idealus!)
- ✅ **Kolizijų atsparumas:** 0 iš 40,000 testų
- ✅ **Našumas:** 334 MB/s realių failų apdorojimui
- ✅ **Patikimumas:** 100% stabilus su įvairiais failais
- ✅ **Mastelis:** Sėkmingai testuota su 75k+ simbolių failais

**Projektas sukurtas 2025-09-23**  
**Išsamus testavimas baigtas sėkmingai** ✅  
**Pilna LR Konstitucija sėkmingai apdorota** 🇱🇹