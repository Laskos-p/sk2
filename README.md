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
   
