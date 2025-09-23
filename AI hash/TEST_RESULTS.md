# CUSTOM HASH ALGORITMO TESTAVIMO REZULTATAI

## Testavimo data: 2025-09-23

---

## 1. IŠVEDIMO DYDŽIO TESTAS ✅

**Rezultatas: PAVYKO**

Visi testai parodė, kad nepriklausomai nuo įvesties dydžio (0, 1, 11, 1000, 10000 simbolių), hash visada yra **64 simbolių ilgio** (256 bitai hex formatu).

**Testavimo duomenys:**
- Tuščias stringas: `7f727cf94d34563aa3f90465c65d4c483408bad86b46990d208e4d6a2135407e`
- Vienas simbolis 'a': `ceadf5c071431e7e754aed043b54b1147dd58e5f72d7580634ed248d7ed49239`
- Vienas simbolis 'b': `61f698946c64f66861e727897c7d5876bff90abdaee1dc8a905605d5bf765b27`

---

## 2. DETERMINISTIŠKUMO TESTAS ✅

**Rezultatas: PAVYKO**

Tas pats įvesties tekstas visada duoda identišką hash reikšmę.

**Patikrinti tekstai:**
- "test"
- "longer test string"  
- "123456789"

Visi testai parodė 100% deterministiškumą per kelis kartojimus.

---

## 3. EFEKTYVUMO TESTAS ✅

**Rezultatai:**

| Dydis (simbolių) | Vidutinis laikas (μs) | Santykis |
|------------------|----------------------|----------|
| 100              | 0.00                 | -        |
| 500              | 0.00                 | -        |
| 1000             | 0.00                 | -        |
| 5000             | 0.00                 | -        |
| 10000            | 100.70               | -        |
| 50000            | 213.60               | x2.12    |

**Išvados:**
- Algoritmas labai efektyvus mažiems failams (<10k simbolių)
- Tiesinis augimas didiems failams
- Tinamas praktiniam naudojimui

---

## 4. KOLIZIJŲ TESTAS ✅

**Rezultatas: PUIKUS**

Iš **10,000 testų** kiekvienam ilgiui:

| Stringų ilgis | Kolizijos | Dažnis |
|---------------|-----------|--------|
| 10 simbolių   | 0         | 0.0000% |
| 100 simbolių  | 0         | 0.0000% |
| 500 simbolių  | 0         | 0.0000% |
| 1000 simbolių | 0         | 0.0000% |

**Išvada:** Labai stiprus atsparumas kolizijoms testuotame diapazone.

---

## 5. LAVINOS EFEKTO TESTAS ✅

**Rezultatas: PUIKUS**

Testavimo metodas: 1000 porų, kurios skiriasi tik vienu bitu.

### Bitų skirtumai:
- **Minimalus:** 102 bitai (39.8%)
- **Maksimalus:** 156 bitai (60.9%)
- **Vidutinis:** 128.0 bitai (50.0%)

### Hex simbolių skirtumai:
- **Minimalus:** 51 simboliai (79.7%)
- **Maksimalus:** 64 simboliai (100.0%)
- **Vidutinis:** 60.0 simboliai (93.8%)

**Išvada:** Puikus lavinos efektas - mažas įvesties pokytis sukelia ~50% hash pokyčio.

---

## 6. NEGRĮŽTAMUMO DEMONSTRACIJA ✅

**Salt testavimas:**

Tekstas "secret_password" su skirtingais salt:

| Salt              | Hash |
|-------------------|------|
| "salt1"           | `e7c318a86e96d6196eb4e0af9969ca12f583c848b4ae29c51b6c4bae468eb0c8` |
| "salt2"           | `5e099513b1c17ec1e7fbb56896d568d10d84f27e90f0bd9a2fe20a246b112396` |
| "random_salt_12345" | `59ea5f39039553287787943feb6fd179b52a0c76f3da1b8d5010bcee7308f3a8` |
| ""                | `f11fa9a4335e520b5a620fc223421a32a486fc7021aacc146b3e281504e39f62` |

**Panašių tekstų testavimas su salt "mysalt":**

| Tekstas    | Hash |
|------------|------|
| "password" | `9d22c8987ad8aa09b10e645d7aba3308eeb1b502a3991c20f73f1d4fba677196` |
| "Password" | `672c5812dffaf1a6f400ca2adbd8cc54b256df6212636d76f8065ed45ab31470` |
| "password1"| `8479afc4492175efd0fbe1cc3e08c6861faa0d432da14144c47378247d8803db` |
| "passwor"  | `bb970dc4cb4eae57860d377ba15445a1a52adff94b03e82209a69605dcf97057` |

**Išvada:** Visiškai skirtingi hash'ai net ir labai panašiems tekstams.

---

## 7. PAPILDOMI TESTAI

### Realių failų testavimas:
- **Tuščias failas:** Hash sėkmingai generuotas
- **1500 simbolių failas:** Hash: `fe1d8dc407abd6f6b417fd9bbe66f3df4def274db2a56e02883d01aeafdce2fd`
- **Pilnas konstitucijos tekstas (75,595 simboliai):** Hash: `f114c55859791334523c0c803e6acc29af2a7864208ab104d01e1f53c1c35c97`

### Našumo testas su konstitucijos failu:

| Dydis (simbolių) | Laikas (μs) | Našumas (MB/s) | Hash |
|------------------|-------------|----------------|------|
| 18,898 (0.25x)   | <1          | >1000         | `022bc348...` |
| 37,797 (0.5x)    | 210         | 171.65        | `3e7035d9...` |
| 56,696 (0.75x)   | 320         | 168.76        | `6c64da1d...` |
| 75,595 (1.0x)    | 216         | **334.38**    | `f114c558...` |
| 151,190 (2.0x)   | 603         | 239.04        | `edb18559...` |
| 302,380 (4.0x)   | 1,812       | 159.13        | `82aaddff...` |
| 604,760 (8.0x)   | 4,249       | 135.74        | `d2762fb5...` |

**Našumo išvados:**
- Pilna konstitucija (75k simbolių) apdorojama per 216μs
- Našumas: **334 MB/s** (labai greitas!)
- Tiesinis našumo mažėjimas didiems failams
- Algoritmas efektyviai dirba su realiais tekstais

### Programos stabilumas:
- Jokių klaidų ar sužlugimų
- Stabilus darbas su įvairių dydžių failais
- Tinkamas simbolių kodavimas

---

## BENDROJI ĮVERTINIMAS

### STIPRYBĖS ✅
1. **Puikus lavinos efektas** (~50% bitų pokyčio)
2. **Nėra kolizijų** testuotame diapazone
3. **Stabilus 256-bitų išvedimas**
4. **Efektyvus našumas**
5. **Deterministiškas**
6. **Geras salt palaikymas**

### RIBOJIMAI ⚠️
1. **Nėra kriptografiškai įrodytas**
2. **Reikalingi didesni kolizijų testai**
3. **Neprofesionalus kriptografinis sprendimas**

### GALIMAS NAUDOJIMAS ✅
- Duomenų vientisumui tikrinti
- Hash lentelėms
- Mokymosi tikslams
- Ne-kritinėms saugumo užduotims

### NEGALIMAS NAUDOJIMAS ❌
- Slaptažodžių hash'avimui
- Kriptografinėms aplikacijoms
- Aukšto saugumo sistemoms
- Produkcijos kriptografijai