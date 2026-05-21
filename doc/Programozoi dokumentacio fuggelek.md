# <center>genMatrix - Programozói dokumentáció függeléke</center>

## UML diagram
Az UML diagram PlantUML kódból készült. A PlantUML editor <a href=https://editor.plantuml.com>itt</a> érhető el. A forrásfájl, amiből generálták, a vektorgrafikás képpel együtt elérhető a projekt GitHubon <a href=https://github.com/feeblescholar/genMatrix>itt</a>.

<center><img src="UML current rendered.svg"></img></center>

## A főprogram
Mivel a genMatrix egy mátrixkönyvtár, a főprogram felhasználói felület helyett 3 db példafeldat megoldását tartalmazza 3 különböző felhasználási területről:
* Egy 5 ismeretlenes lineáris egyenletrendszer megoldása 
* Egy egyszerűsített Google PageRank számítás (Markov-lánc stacionárius eloszlása)
* Egy 3D-s pont transzformációja (Számítógépes-grafika)

## A projekt bővíthetősége
A genMatrix úgy lett tervezve, hogy bármikor bővíthető legyen új osztályokkal a meglévő kód jelentősebb átalakítása nélkül. Ezt a `custom_type_traits.hpp` és `utils.hpp`-ben található függvény wrapperek biztosítják. Az új számosztályok felvétele esetén az ott található függvényeket kell túlterhelni az új osztálynak megfelelővel. Ha esetleg szükséges hiperbolikus vagy duális számokkal bővíteni a projektet, akkor az `Hypercomplex2D` osztály öröklésével megtehető, mivel így a `Complex` osztályon végzett műveleteket pl. az `utils::abs` függvényt nem kell újra megírni és pl. determinás vagy norma számítás automatikusan működni is fog (természetesen a megfelelő operátorok meglétével). Új hibaosztályt fel lehet venni a `genMatrix_Error` öröklésével. A `RandomIterator` osztálysablon bármely 1D-s tömböt tároló típusnak legyárt egy `random_access_iterator_tag`-nek megfelelő iterátort, STL algoritmusok kompatibilitása érdekében.

## genMatrix kompatibilis új osztály felvétele
Ahhoz, hogy egy új típus <b>teljesen</b> a genMatrix `Matrix` illetve `Vektor` osztályaival az alábbiakkal kell rendelkeznie:
* Default 0 paraméteres konstruktor
* 1 paraméteres konstruktor
* A típus lehessen másolható és értékül adható
* A típus rendelkezzen a `+, *, +=, *=` túlterhelt operátorokkal
* A típus rendelkezzen a `/, /=` túlterhelt operátorokkal a determináns/inverz számításhoz
* A típus rendelkezzen a `<<, >>` túlterhelt operátorokkal a fájlíráshoz
* A típusának megfelelően vegye fel magát a `custom_type_traits.hpp` fájlban található type traitekhez
    * pl. a double típust tárolók vegyék fel a `__has_double_precision()` függvényhez magukat, hogy a megfelelő determináns algoritmus fusson le
* Az osztálynak megfelően implementáljanak egy `abs, fma és eq` függvényt az abszolútérték, Fuse-Multiply Add és egyenlőségvizsgálat helyes működéséhez. Ezek a `utils.ipp` fájlban találhatóak.
* (Opcionális, szükség esetén) Konverziós operátorok

A fentieken kívül bizonyos jellegű viselkedést is elvárnak a tárolók:
* A default konstruktor publikus számértéket tároló változóinak értéke 0 legyen
* Az egyparaméteres konstruktor az <b>első</b> publikus számértéket tároló változót a paraméterre, a többiekét 0-ra állítsa
* Az abszolútérték függvénynek elemi típust kell visszaadni
* Az osztály által lefoglalt erőforrásokért az adott osztály felel (magáért az osztályért pedig a tároló)

A helyes működés tesztelésében segítenek a GoogleTest-el implementált tesztek. A `test_*tároló*_random.cpp` fájl tetején kell az adott osztályt hozzádni. <i>Szükség esetén természetesen további tesztek is implementálhatóak. Determinisztikus fix típusú teszteket a megfelő `test_*tároló*_base.cpp` fájlba érdemes tenni.</i>

## Felhasznált források
Az implementáció során a meglévő lineáris algebra ismereteimre támaszkodtam, ezen felül sok ötletet adott az Eigen mátrixkönyvtár, melynek dokumentációja <a href="https://libeigen.gitlab.io/eigen/docs-nightly/">itt</a> érhető el. Egyes determinisztikus tesztekben felhasznált számértékek generálásához, a Google Gemini 3.5-ös Flash modelljét használtam, az tesztek eredményeinek utánaszámoltam (természetesen a megfelelő kerekítést alkalmazva), így (remélhetőleg) nincs számítási hiba bennük.