# vstupni_demo

Zopár dodatkov ku kódu:
-cesty ku knižniciam catch2 a entt sú dané do poznámok, keďže uloženie stiahnutej knižnice sa môže meniť od zariadenia, a nenašiel som momentálne lepší spôsob prepojenia, ospravedlňujem sa
-knižnica catch2 spôsobuje chyby, a nepodarilo sa mi prísť na chybu. Testovanie je z toho dôvodu pridané ako komentár, a nie je skontrolované či je napísané správne

Kód je rozdelený na niekoľko častí a funkcií.
-začiatočné importy a definície, slúžili mi ako kontrola a jednoducho ovládané testovanie, keďže som mal problémy s knižnicou catch2
-komponenty
-systémy - vytvoril som jeden systém, ktorý simuluje pohyb strely v čase, za pomoci určeného časového kroku, ktorý dokáže rozoznať zásah. Momentálne však iba za predpokladu že sú uhly pri výstrele nastavené správne
-main - obsahuje vytvorenie registra, vloženie potrebných entít, rozhodnutie o tom či strela dosiahne na cieľ, zavolanie výpočtu uhlov, výpis výsledkov a vpísanie smeru do príslušného komponentu
-volanie systému simulácie výstrelu je dané do komentáru, z dôvodu prehľadnosti pri spúšťaní
-funkcie počítajú uhly a kontrolujú dostrel
-testovanie nie je funkčné a teda nie je aktívne
