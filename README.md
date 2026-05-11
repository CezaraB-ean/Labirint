# Labirint
Generator și rezolvator de labirinturi - Descriere Generală

Acastă aplicație poate genera labirinturi folosind algoritmi de parcurgere a grafurilor (precum DFS - Depth-First-Search), algoritmi de determinare a arborilor de acoperire (precum algoritmul lui Kruskal sau algoritmul lui Prim) și algoritmi de generare a arborilor uniform aleși (precum algoritmul lui Wilson).

Acționând asupra unei matrici de noduri, algoritmii menționați generează, în timp real, conexiunile dintre nodurile grafului și evidențiază astfel căile posibile în labirintul final.

Datorită generării în timp real a labirintului, se pot observa modurile de funcționare ale algoritmilor evidențiind astfel diferențele dintre aceștia.

După generare utilizatorul are opțiunea de a schimba modul de vizualizare între modul Labirint, în care se observă pereții labirintului, și modul Graf, în care se observă căile de parcurgere posibile.
Tot după generare, în cazul în care utilizatorul dorește să vadă soluția, fie pentru a se verifica, fie doar pentru a testa funcționalitatea rezolvatorului, soluția labirintului, cu tot cu nodurile care duc la o înfundătură, poate fi afișată prin apăsarea unui buton . 


Mod de Utilizare:

După lansarea aplicației, utilizatorul poate alege, ca prim pas, numărul de noduri (nr. de căsuțe ale labirintului) pe o linie a matricei.
După alegerea unui algoritm pentru generarea labirintului, butonul 'Generare' poate fi apăsat pentru a genera un nou labirint.

La finalul generării, utilizatorul va avea posibilitatea de a schimba între modurile de vizualizare a labirintului (prin folosirea check-box-ului) și de a rezolva curentul labirint, sau de a lăsa aplicația să rezolve labirintul prin apăsarea butonului 'Rezolvare'. Aceasta folosește un algoritm BFS (Breadth-First-Search) pentru a găsi cel mai scurt drum de la nodul/căsuța de început (colțul stânga-sus) până la nodul/căsuța de sfârșit (colțul dreapta-jos).

Pentru încercarea aplicației, se poate instala fișierul executabil: Labirint.exe, iar pe calculatoarele cu sistem de operare Windows rularea acestui executabil este suficientă pentru a folosi aplicația; (pentru alte sisteme de operare nu este garantată funcționalitatea acesteia). 
