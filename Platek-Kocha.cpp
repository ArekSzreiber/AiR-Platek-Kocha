//POP 2016-01-19 projekt 2 Szreiber Arkadiusz AIR 3 155497,  Code::Blocks 13.12,  GNU GCC Compiler

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <string>

const int NIE=0;
const int TAK=1;

using namespace std;

    struct xy{
        double x;  //wspolzedna pozioma
        double y;  //wspolzedna pionowa
        };

    xy kursor;//do pliku sa wysylane wartosci wspolzednych kursora, a nastepnie "lineto" lub "moveto"

    //aby zniwelowac roznice powstajace przy dodawaniu katow M_PI/3 oraz -2*M_PI/3
    //utworzylem tablice mozliwych katow, jakie mozna otrzymac dodajac wielokrotnosci M_PI/3 do 0
    //tablica ma 14 katow, a nie tylko 6, by zmniejszyc liczbe dzielenia z reszta przez 6 w funkcji "rysuj_krzywa_Kocha"
    const double mozliwe_katy[14]={ 0, M_PI/3.0, (M_PI/3.0)*2, M_PI, (M_PI/3.0)*4, (M_PI/3.0)*5,
                                    0, M_PI/3.0, (M_PI/3.0)*2, M_PI, (M_PI/3.0)*4, (M_PI/3.0)*5,
                                    0, M_PI/3.0};



//moje wersje znanych prostych funkcji:

void moja_tolower(string &napis){//zamienia wielkie litery w napisie na male
    int dlugosc_napisu=napis.length();

    for(int i=0;i<dlugosc_napisu;i++){
        if(napis[i]<='Z' && napis[i]>='A'){//jesli znak jest wielka litera
            napis[i]= napis[i]-'A'+'a';//to zamien ja na mala litere
        }
    }
}

int potega(const int podstawa, const int wykladnik){
    int wynik=1;
    for(int i=0;i<wykladnik;i++){
        wynik=wynik*podstawa;
    }
    return wynik;
}


//funkcje sprawdzajace skladnie napisow:

bool czy_string_na_double(const string slowo){//sprawdza, czy mozna przekonwertowac string na liczbe zmiennoprzecinkowa
    int dlugosc_slowa=slowo.length();
    bool czy_jest_przecinek=NIE;

    if(dlugosc_slowa==0){//aby zbior pusty nie dawal pozytywnego wyniku
        return NIE;
    }

    for(int i=0;i<dlugosc_slowa;i++){//sprawdza, czy skladnia zmiennej jest poprawna
        if(slowo[i]>='0' && slowo[i]<='9'){
            continue;
        }else{
            if(slowo[i]=='.'  || slowo[i]==','){//napis moze zawierac co najwyzej 1 przecinek lub kropke
                if(czy_jest_przecinek==NIE){
                    czy_jest_przecinek=TAK;
                }else{
                    return NIE;
                }
            }else{
                return NIE;
            }
        }
    }
    return TAK;
}

bool czy_string_na_naturalna(const string napis){//czy mozna przekonwertowac string na liczbe calkowita nieujemna
    int dlugosc_slowa=napis.length();

    if(dlugosc_slowa==0){//zabezpiecza przed zwroceniem "TAK", jesli wcisnieto [ENTER] nie podajac wczesnij innych znakow
        return NIE;
    }

    for(int i=0;i<dlugosc_slowa;i++){//sprawdza, czy zmienna zawiera tylko cyfry
        if(napis[i]<'0' || napis[i]>'9'){
            return NIE;
        }
    }
    return TAK;
}


//konwertery:

void napis_na_double(const string napis, double &liczba){//dziala poprawnie tylko przy odpowiedniej zawartosci elementu "napis"

    liczba=0;

    const int dlugosc_napisu=napis.length();
    const int podstawa=10;//system dziesietny
    double czynnik=podstawa;

    int j=0;//numer indeksu znaku w napisie

    while(j<dlugosc_napisu){//tworzy czesc calkowita liczby
        if(napis[j]=='.' || napis[j]==','){//jesli napotka znak przecinka (lub kropki), konczy tworzenie czesci calkowitej
            j++; // by pominac znak przecinka
            break;
        }
        liczba = liczba*podstawa + (napis[j]-'0') ;
        j++;
    }

    while(j<dlugosc_napisu){//tworzy czesc ulamkowa liczby
        liczba = liczba + (napis[j]-'0')/czynnik;
        czynnik = czynnik*podstawa;
        j++;
    }
}

void napis_na_liczbe_naturalna(const string napis, int &liczba){//konwertuje napis na nieujemna liczbe calkowita

    const int podstawa=10;//system dziesietny
    int dl_napisu=napis.length();

    liczba=0;

    for(int i=0;i<dl_napisu;i++){
        liczba = (liczba*podstawa + (napis[i]-'0')) ;
    }
}


//funkcje wypisujace komunikaty na ekranie:

void drukuj_ostrzezenie(const int stopien){//wywolywana, jesli uzytkownik zechce wygenerowac fraktal wysokiego stopnia (8 i wiecej)

    long int liczba = 3*potega(4, stopien);

    cout<<"\nFraktal "<<stopien<<" stopna sklada sie z "<<liczba<<" pojedynczych kresek.\n";
    cout<<"Aplikacja EPS Viewer prawdopodobnie nie bedzie w stanie go wyswietlic.\n";
    cout<<"\nCzy na pewno chcesz wygenerowac fraktal "<<stopien<<". stopnia?\n";
    cout<<"\t\t\t\t(Przerwanie dzialania programu: klawisze CTRL + C)\n";
    cout<<"T - tak, N - nie\n\t";
}

void wypisz_menu(const string nazwa_pliku,const double szerokosc_fraktalu,const int stopien){
    //glowne menu programu
    cout<<"\n";
    cout<<"  1 - Generuj fraktal\n";
    cout<<"  2 - Stopien: "<<stopien<<"\n";
    cout<<"  3 - Szerokosc: "<<szerokosc_fraktalu<<" px \n";
    cout<<"  4 - Nazwa pliku: "<<nazwa_pliku<<"\n";
    cout<<"  0 - Wyjscie z programu\n";
}

void drukuj_upewnienie(const int dolna_granica,const int wartosc,const int gorna_granica){//drukuje komunikat, jesli podana szerokosc ma nietypowa wartosc
    if(dolna_granica>=wartosc || gorna_granica<=wartosc){
        cout<<"Szerokosc fraktalu rowna "<<wartosc<<" jest wyjatkowo ";
        if(wartosc<=dolna_granica){
            cout<<"niewielka";
        }else{
            cout<<"duza";
        }
        cout<<".\nProsze upewnic sie, ze jest to wartosc oczekiwana.\n";
    }
}


//funkcje pobierajace parametry od uzytkownika:

void pobierz_nazwe_pliku_od_urzytkownika(string &nazwa_pliku){

    cout<<"Nowa nazwa pliku: ";

    getline(cin, nazwa_pliku, '\n');

    int dlugosc=nazwa_pliku.length();

    //jesli nazwa nie konczy sie rozszerzeniem ".ps", zostaje ono dodane
    if( !(nazwa_pliku[dlugosc-3]=='.' && nazwa_pliku[dlugosc-2]=='p' && nazwa_pliku[dlugosc-1]=='s') ){
        nazwa_pliku=nazwa_pliku+".ps";
    }
}

void pobierz_double_od_urzytkownika(double &liczba){

    string slowo;
    const int dolna_granica=10;//jesli szerokosc fraktalu jest zbyt mala
    const int gorna_granica=3000;//albo zbyt duza, to zostanie wyswietlony dodatkowy komunikat

    while(1){//pobiera dane od uzytkownika dopoki nie otrzyma poprawnych danych
        cout<<"\nNowa wartosc w px: ";
        getline(cin, slowo, '\n');//by pobierana byla cala zawartosc buforu

        //kiedy zawartosc zmiennej "slowo" mozna przekonwetrowac na liczbe zmiennoprzecinkowa, petla sie zakonczy
        if(czy_string_na_double(slowo)==TAK){
            break;
        }
        cout<<"Wystapily niepoprawne znaki / brak znakow.\n";
    }

    napis_na_double(slowo, liczba);

    //tylko upewnienie, bo fraktale o nietypowym rozmiarze nie spowoduja niestabilnosci dzialania programu
    drukuj_upewnienie(dolna_granica, liczba, gorna_granica);

}

void pobierz_stopien_od_urzytkownika(int &stopien){

    string slowo;

    while(1){

        while(1){//przerywa sie, kiedy uzytkownik poda lancuch znakow, ktory mozna przekonwertowac na niujemna liczbe calkowita
            cout<<"Nowa liczba krokow rozbudowy krzywej: ";
            getline(cin, slowo, '\n');

            if(czy_string_na_naturalna(slowo)){
                break;
            }
            cout<<"Wystapily niepoprawne znaki\n";
        }

        napis_na_liczbe_naturalna(slowo, stopien);

        if(stopien<=7){ //jesli fraktal nie ma byc wyjatkowo rozbudowany
            return;
        }

        drukuj_ostrzezenie(stopien);

        while(1){
            getline(cin, slowo, '\n');
            moja_tolower(slowo);//by wielkie litery dawaly taki sam efekt jak male
            if( 1==slowo.length() && (slowo[0]=='t' || slowo[0]=='n') ){//jesli uzytkownik podal oczekiwane znaki, to przestanie go ponownie o nie pytac
                break;
            }
        }

        if(slowo[0]=='t')//jestli uzytkownik jest pewny tego, co robi
            return;

    }
}


//funkcje drukujace do pliku:

void wypisz_naglowek(fstream &plik, const string nazwa_pliku){// usuwa zawartosc pliku i wypisuje naglowek

    plik.open(nazwa_pliku.c_str(), ios::out);
    plik<<"%!PS\n/Courier 12 selectfont\n";
    plik.close();

    cout<<"\n Prosze czekac, trwa wysylanie danych do pliku...";

}

void przenies_kursor_na_poczatek(fstream &plik, const string nazwa_pliku, const double szerokosc_sniezki_w_px){
    //przenosi kursor na miejsce, w ktorym zaczyna sie rysowanie fraktalu

    const int margines=20;//aby fraktal nie byl tuz przy krawedzi

    kursor.x=margines;
    kursor.y=szerokosc_sniezki_w_px*(sqrt(3)/2.0) + margines*3;

    plik<<kursor.x<<" "<<kursor.y<<" moveto\n";

}

void wypisz_zakonczenie(fstream &plik, const string nazwa_pliku, const int stopien_fraktalu){//konczy generowanie fraktalu

    plik<<"stroke\nshowpage\n";

    //zastapienie komunikatu podczas pracy funkcji na komunikat o zakonczeniu jej dzialania
    cout<<"\r\t\t\t\t\t\r\tWygenerowano fraktal "<<stopien_fraktalu<<" stopnia do pliku \""<<nazwa_pliku<<"\".\n";

}

void rysuj_krzywa_Kocha(fstream &plik, const string nazwa_pliku, const double dl_kreski_w_px, int indeks_kata, int stopien){

    indeks_kata=indeks_kata%6;//by wartosc po pozniejszych dodawaniach nadal wskazywala na element tablicy

    for(int licznik=0; licznik<4; licznik++){
        if(stopien==1){
            //wyznaczenie nowej pozycji kursora i wyslanie jej do pliku:
            kursor.x =kursor.x + dl_kreski_w_px*cos(mozliwe_katy[indeks_kata]);
            kursor.y =kursor.y + dl_kreski_w_px*sin(mozliwe_katy[indeks_kata]);
            plik<<kursor.x<<" "<<kursor.y<<" lineto\n";
        }else{
            //rekurencja
            rysuj_krzywa_Kocha(plik, nazwa_pliku, dl_kreski_w_px, indeks_kata, (stopien-1));
        }

        //wyznaczenie kata wzgledem poprzedniej kreski
        if(licznik%2==0){
            indeks_kata++;
        }else{
            indeks_kata = indeks_kata+4;
        }
    }

}

void rysuj_kreske(fstream &plik, const string nazwa_pliku, const double szerokosc_kreski_w_px, int indeks_kata){

    plik.open(nazwa_pliku.c_str(), ios::out | ios::app);
    plik<<setiosflags(ios::fixed);

    //ustalenie nowej pozycji kursora
    kursor.x=kursor.x + szerokosc_kreski_w_px*cos(mozliwe_katy[indeks_kata]);
    kursor.y=kursor.y + szerokosc_kreski_w_px*sin(mozliwe_katy[indeks_kata]);

    plik<<kursor.x<<" "<<kursor.y<<" lineto\n";

}


//glowna funkcja odpowiedzialna za rysowanie fraktalu

void generuj_fraktal(fstream &plik, const string nazwa_pliku, const double szerokosc_fraktalu, int stopien){

    int indeks=0;//indeks kata pierwszego rysowanego elementu

    const double dl_kreski=szerokosc_fraktalu/potega(3, stopien);

    plik.open(nazwa_pliku.c_str(), ios::out | ios::app);
    if(!plik){//jesli plik zostanie otwarty raz, to juz nie trzeba wiecej razy sprawdzac, czy zostal otwarty
        cout<<"\tNie udalo sie otwrzyc pliku.\n";
        return;
    }
    plik.close();

    wypisz_naglowek(plik, nazwa_pliku);

    plik.open(nazwa_pliku.c_str(), ios::out | ios::app);

    przenies_kursor_na_poczatek(plik, nazwa_pliku, szerokosc_fraktalu);

    //rysowanie 3 kresek lub 3 czesci sniezki:
    for(int i=0;i<3;i++){
        switch(stopien){
            case 0:     rysuj_kreske(plik, nazwa_pliku, szerokosc_fraktalu, indeks);
                        break;

            default:    rysuj_krzywa_Kocha(plik, nazwa_pliku, dl_kreski, mozliwe_katy[indeks], stopien);

        }
        indeks=(indeks+4)%6;
    }

    wypisz_zakonczenie(plik, nazwa_pliku, stopien);

    plik.close();

}

main(){
    //trzy domyslne wartosci:
    string nazwa_pliku="platek Kocha.ps";
    double szerokosc_fraktalu=720;
    int stopien=3;

    string slowo; int liczba;//obydwie zmienne sluza do pobierania numerow od urzytkownika

    fstream plik;

    do{
        wypisz_menu(nazwa_pliku, szerokosc_fraktalu, stopien);
        do{
            getline(cin, slowo, '\n');

            if(czy_string_na_naturalna(slowo)==NIE){
                cout<<"Wystapily niepoprawne znaki\n";
            }else{
                break;
            }

        }while(1);

        napis_na_liczbe_naturalna(slowo, liczba);

        switch(liczba){
            case 1:   generuj_fraktal(plik, nazwa_pliku, szerokosc_fraktalu, stopien);
                            break;
            case 2:   pobierz_stopien_od_urzytkownika(stopien);
                            break;
            case 3:   pobierz_double_od_urzytkownika(szerokosc_fraktalu);
                            break;
            case 4:   pobierz_nazwe_pliku_od_urzytkownika(nazwa_pliku);
                            break;
            case 0:   return 1;
        }
    }while(1);

}
