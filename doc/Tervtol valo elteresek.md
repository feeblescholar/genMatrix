# <center> A tervtől való eltérések az implementációban </center>

## A főprogram
A főprogramban konzolos UI helyett 3 db, a gyakorlati alkalmazást szemléltető, teszt fut le. A tesztelést itt a `gtest_lite` végzi. A választás véleményem szerint logikusabb egy könyvtár esetében, mintsem a felhaszálótól bekérni 25 db számot egy 5x5-ös mátrix 2 ms lefutó tesztjéhez. A felhasználói útmutató pedig nem az UI, hanem a könyvtár használatát részletezi.

## A tesztkeretrendszer
A genMatrix típusos kialakítása miatt a tesztprogramban a GoogleTest kerül használatra. A GoogleTest ugyanis rendelkezik típusok tesztelésével(`TYPED_TEST`), valamint egyéni epszilonos lebegőpontos összehasonlítással (`EXPECT_NEAR`). Az értékek összehasonlításánál továbbá jelzi az abszolút eltérést is bukott tesztek esetében, ami nagyon hasznos, főleg olyan esetekben, ahol nem az implementáció hibás, hanem a lebegőpontos egység által felhalmozott hibák történt epszilonnák nagyobb eltérés.

## Extrák
Az alábbiak nem szerepeltek az eredeti tervben, de mégis helyet tudtak kapni az implementációban:
* Egész mátrixok determináns számítása a Bareiss-algoritmussal
* Mátrixok (és vektorok) 1-es, végtelen és Frobenius-normái
* Egy <b>teljes</b> `Vector` osztály, ami oda-vissza konvertálható a mátrixszal
* Általánosított `Matrix_Iterator` : a `RandomIterator` osztály, amely képes bármely 1D-s tömböt tároló osztálynak egy teljesen implementált `std::random_access_iterator_tag`-nek megfelelő iterátort adni.
* Hibákat összefogó absztrakt osztály: `genMatrix_Error`, így (majdnem) az összes, a könyvtár által dobott hibát tisztán el lehet kapni (1 `catch` blokkal)
* Saját sablonok a típusok vizsgálatára
* SFINAE (Substitution Failure Is Not An Error) sablonok használata, a `concepts` kiváltására (a C(J)PORTA a C++ 2017-es szabványt használja)
* SBO (Small Buffer Optimization) használata a mátrixok tárolására
* Fuse-Multiply Add használata a gyorsabb futás érdekében

Ezen felül a cross-platform tesztek is elérhetőek a projekt GitHubon a GitHub Actions segítségével.