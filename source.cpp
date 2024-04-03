#define _CRT_SECURE_NO_WARNINGS
#include <iostream>#include <iostream>
#include <ctime>
#include <vector>

using namespace std;

class Revizie {
public:
    int kilometraj;
    tm dataReviziei;

    Revizie(int km) : kilometraj(km) {
        time_t now = time(nullptr);
        dataReviziei = *localtime(&now);
    }

    friend istream& operator>>(istream& is, Revizie& revizie);
};

istream& operator>>(istream& is, Revizie& revizie) {
    cout << "Introduceti kilometrajul reviziei: ";
    is >> revizie.kilometraj;
    return is;
}

class Asigurare {
public:
    tm dataExpirare;

    Asigurare() {
        time_t now = time(nullptr);
        dataExpirare = *localtime(&now);
        dataExpirare.tm_year += 1;
        mktime(&dataExpirare);
    }

    friend istream& operator>>(istream& is, Asigurare& asigurare);
};

istream& operator>>(istream& is, Asigurare& asigurare) {
    return is;
}

class TaxaDrum {
public:
    double valoare;
    tm dataScadenta;

    TaxaDrum(double val) : valoare(val) {
        time_t now = time(nullptr);
        dataScadenta = *localtime(&now);
        dataScadenta.tm_year += 1;
        mktime(&dataScadenta);
    }

    friend istream& operator>>(istream& is, TaxaDrum& taxaDrum);
};

istream& operator>>(istream& is, TaxaDrum& taxaDrum) {
    cout << "Introduceti valoarea taxei de drum: ";
    is >> taxaDrum.valoare;
    return is;
}

class Autovehicul : public Revizie, public Asigurare, public TaxaDrum {
public:
    string numarInmatriculare;
    string marca;
    string model;
    int anFabricatie;

    Autovehicul() : Revizie(0), Asigurare(), TaxaDrum(100.0), numarInmatriculare(""), marca(""), model(""), anFabricatie(0) {}

    Autovehicul(string nrInmatriculare, string marca, string model, int anFabricatie)
        : Revizie(0), Asigurare(), TaxaDrum(100.0),
        numarInmatriculare(nrInmatriculare), marca(marca), model(model), anFabricatie(anFabricatie) {}

    friend ostream& operator<<(ostream& os, const Autovehicul& autovehicul);
    friend istream& operator>>(istream& is, Autovehicul& autovehicul);
};

ostream& operator<<(ostream& os, const Autovehicul& autovehicul) {
    os << "Stare Autovehicul - " << autovehicul.numarInmatriculare << endl;
    os << "Marca: " << autovehicul.marca << endl;
    os << "Model: " << autovehicul.model << endl;
    os << "An fabricatie: " << autovehicul.anFabricatie << endl;
    os << "Ultima revizie - Kilometraj: " << autovehicul.kilometraj
        << ", Data: " << autovehicul.dataReviziei.tm_mday << "."
        << autovehicul.dataReviziei.tm_mon + 1 << "."
        << autovehicul.dataReviziei.tm_year + 1900 << endl;
    os << "Asigurare - Data expirare: " << autovehicul.dataExpirare.tm_mday
        << "." << autovehicul.dataExpirare.tm_mon + 1
        << "." << autovehicul.dataExpirare.tm_year + 1900 << endl;
    os << "Taxa de drum - Valoare: " << autovehicul.valoare
        << ", Data scadenta: " << autovehicul.dataScadenta.tm_mday
        << "." << autovehicul.dataScadenta.tm_mon + 1
        << "." << autovehicul.dataScadenta.tm_year + 1900 << endl;
    os << endl;

    return os;
}

istream& operator>>(istream& is, Autovehicul& autovehicul) {
    cout << "Introduceti numarul de inmatriculare: ";
    is >> autovehicul.numarInmatriculare;
    cout << "Introduceti marca: ";
    is >> autovehicul.marca;
    cout << "Introduceti modelul: ";
    is >> autovehicul.model;
    cout << "Introduceti anul fabricatiei: ";
    is >> autovehicul.anFabricatie;

    cout << "Introduceti detalii pentru revizie:" << endl;
    is >> static_cast<Revizie&>(autovehicul);

    cout << "Introduceti valoarea taxei de drum: ";
    is >> autovehicul.valoare;

    return is;
}

class ParcAuto {
public:
    int nrMasini;
    Autovehicul* automobile;

    ParcAuto() : nrMasini(0), automobile(nullptr) {}

    ~ParcAuto() {
        delete[] automobile;
        cout << "Destructor pentru ParcAuto" << endl;
    }

    ParcAuto& operator<<(const Autovehicul& a) {
        Autovehicul* temp = new Autovehicul[nrMasini + 1];
        for (int i = 0; i < nrMasini; ++i) {
            temp[i] = automobile[i];
        }
        temp[nrMasini] = a;
        delete[] automobile;
        automobile = temp;
        nrMasini++;
        return *this;
    }

    friend ostream& operator<<(ostream& os, const ParcAuto& parc);
    friend istream& operator>>(istream& is, ParcAuto& parc);
};

ostream& operator<<(ostream& os, const ParcAuto& parc) {
    os << "Stare Parc Auto (Numar masini: " << parc.nrMasini << "):" << endl;
    for (int i = 0; i < parc.nrMasini; ++i) {
        os << parc.automobile[i];
    }
    return os;
}

istream& operator>>(istream& is, ParcAuto& parc) {
    cout << "Introduceti numarul de masini in parc: ";
    int nr;
    is >> nr;
    parc.nrMasini += nr;
    parc.automobile = new Autovehicul[parc.nrMasini];

    for (int i = 0; i < parc.nrMasini; ++i) {
        cout << "Introduceti detalii pentru autovehiculul " << i + 1 << ":" << endl;
        is >> parc.automobile[i];
    }

    return is;
}

int main() {
    ParcAuto parc;
    Autovehicul a("AB123ABC", "Ford", "Focus", 2018);
    parc << a;
    cout << "Stare initiala parc:\n" << parc;
    cin >> parc;
    cout << "Starea Parcului Auto:\n" << parc;
    return 0;
}
