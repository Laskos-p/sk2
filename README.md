Piotr Laskowski 156020


Sprawozdanie sk2


Gra w staki


1. Opis projektu
  Projekt implementuje serwer oraz klienta do obsługi gry w statki. 
  Klienci mogą tworzyć pokoje, dołączać do nich oraz wchodzić w interakcję między sobą w ramach jednego pokoju.

  Użyte języki:
    Serwer: C++
    Klient: C++


2. Opis komunikacji pomiędzy serwerem i klientem
	Komunikacja odbywa się za pomocą socketów TCP z wykorzystaniem send() i recv().
	Klient w pętli oczekuje na dwie wiadomości od serwera. Jedna z treścią do wyświetlenia klientowi, a druga z informacją czy klient w danej pętli powinien wysłać wiadomość. Jeżeli dostanie informacje o tym, że ma wysłać wiadomość, pozwala użytkownikowi wysłać wprowadzone dane.
	Serwer korzysta z wątków, aby obsłużyć poszczególnych klientów. Każdy klient przypisywany jest do jednego wątku, aby równocześnie ich obsługiwać. 
	Dla synchornizacji w grze serwer korzysta również z wątków do obsługi pokoi. Zapewnia to możliwość prowadzenia wiele rozgrywek naraz.
	Serwer zarządza komunikacją wysyłając odpowiednie wiadomości do graczy. Oczekuje na ich informacje zwrotne oraz informuje ich o aktualnych wynikach.


3. Podsumowanie
	Serwer działa w oparciu o sockety TCP i wykorzystuje wątki do równoczesnej obsługi wielu klientów. Dzięki temu każdy gracz może niezależnie od innych tworzyć lub dołączać do pokoju gry.
	Klient działa w pętli, oczekując na wiadomości od serwera. Obsługuje instrukcje wyświetlane użytkownikowi oraz umożliwia interakcję z grą.
	Obsługa pokoju gry została zaimplementowana w taki sposób, że dwóch graczy może prowadzić rozgrywkę w pełni zsynchronizowaną i odseparowaną od innych pokoi.


Kompilacja servera i klienta:


g++ -std=c++11 Board.cpp Square.cpp Ship.cpp Player.cpp main.cpp -o main.out


g++ -std=c++11 ship_client.cpp -o ship_client.out 




   Trudności:
      Zamykanie połączeń z użytkownikami 
