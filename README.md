Zadanie brzmi następująco:
 
Proszę napisać implementację w języku C służącą do zarządzania zbiorem prefiksów IPv4.
Prefiks określa zakres adresów wyrażony za pomocą wartości bazowej oraz maski,
przy czym maska posiada ustawione tylko najbardziej znaczące bity w ilości wyrażonej dla uproszczenia liczbą.
Dla przykładu prefiks 10.20.0.0/16 oznacza zakres adresów 10.20.0.0 - 10.20.255.255,
a prefiks 32.64.128.0/20 oznacza zakres 32.64.128.0 - 32.64.143.255.

Struktura danych powinna przechowywać arbitralnie duży zbiór prefiksów czyli par - baza IP (32 bity) i maska (wartości 0 - 32).
Muszą zostać zaimplementowane następujące funkcje:
int add(unsigned int base, char mask)
Dodanie prefiksu do zbioru. Zwraca 0 lub -1 dla błędnych argumentów wywołania.
int del(unsigned int base, char mask)
Usunięcie prefiksu ze zbioru. Zwraca 0 lub -1 dla błędnych argumentów wywołania.
char check(unsigned int ip)
Sprawdzenie czy adres ip zawiera się w zbiorze prefiksów. Zwraca maskę najmniejszego prefiksu (o największej wartości maski) w zbiorze, który zawiera wskazany adres. Jeżeli ip nie zawiera się w zbiorze prefiksów zwraca -1.

Prefiksy w zbiorze są unikalne czyli dodanie istniejącego już prefiksu nie wywołuje żadnej akcji. Usunięcie prefiksu zawierającego w sobie mniejsze prefiksy nie powoduje ich usunięcia.
Przyjmujemy, że najczęściej wywoływaną funkcją jest check. Należy zwrócić uwagę na złożoność obliczeniową wszystkich metod i dobrać odpowiednie struktury danych. Warto wziąć pod uwagę również rozmiar użytej pamięci.
 
Całość rozwiązania powinna zostać zaimplementowana samodzielnie (łącznie ze strukturą danych). Rozwiązanie powinno zawierać testy dodawania, usuwania i wyszukiwania prefiksów.

Implementacja powinna być przede wszystkim zwięzła, ale też estetyczna oraz czytelna. Kod powinien być przenośny (standard języka C).
 
