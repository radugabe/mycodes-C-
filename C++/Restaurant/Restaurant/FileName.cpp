#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <map>
#include <list>
#include <set>
#include <vector>
#include <ctime>
using namespace std;

class ExceptieInput {
	string mesaj;
public:
	ExceptieInput(string mesaj) :mesaj(mesaj) {}
	string what() {
		return this->mesaj;
	}
};

class IFile {
	//clasa interfata
	virtual void saveToFile(fstream& file) = 0;
	virtual void restoreFromFile(fstream& file) = 0;
};

class MateriePrima : IFile {

private:
	string denumire;
	float cantitateDisponibila;
	char* furnizor;

public:

	//CONSTRUCTOR FARA PARAM
	MateriePrima() {
		this->denumire = "Materie neinregistrata";
		this->cantitateDisponibila = 0;
		this->furnizor = nullptr;
	}

	//CONSTRUCTOR CU TOTI PARAM
	MateriePrima(string denumire, float cantitateDisponibila, const char* furnizor) {
		this->denumire = denumire;
		this->cantitateDisponibila = cantitateDisponibila;
		if (furnizor != nullptr) {
			this->furnizor = new char[strlen(furnizor) + 1];
			strcpy(this->furnizor, furnizor);
		}
		else {
			throw ExceptieInput("\nFurnizor incorect");
		}
	}

	//CONSTRUCTOR CU PARAM
	MateriePrima(float cantitateDisponibila) {
		this->cantitateDisponibila = cantitateDisponibila;
	}

	//CONSTRUCTOR COPIERE
	MateriePrima(const MateriePrima& m) {
		this->denumire = m.denumire;
		this->cantitateDisponibila = m.cantitateDisponibila;
		if (m.furnizor != nullptr) {
			this->furnizor = new char[strlen(m.furnizor) + 1];
			strcpy(this->furnizor, m.furnizor);
		}
		else {
			throw ExceptieInput("\nFurnizor incorect");
		}
	}

	//GETTERI
	string getDenumire() {
		return this->denumire;
	}
	float getCantitateDisponibila() {
		return this->cantitateDisponibila;
	}
	const char* getFurnizor() {
		return this->furnizor;
	}

	//SETTERI
	void setDenumire(string denumireNou) {
		if (denumireNou.length() > 1) {
			this->denumire = denumireNou;
		}
		else {
			throw ExceptieInput("\nDenumire incorecta");
		}
	}
	void setCantitateDisponibila(float cantitateDisponibilaNou) {
		if (cantitateDisponibilaNou > 0) {
			this->cantitateDisponibila = cantitateDisponibilaNou;
		}
		throw ExceptieInput("Cantitatea disponibila introdusa este incorecta");
	}
	void setFurnizor(const char* furnizorNou) {
		if (furnizorNou != nullptr) {
			if (this->furnizor != nullptr) {
				delete[] this->furnizor;
			}
			this->furnizor = new char[strlen(furnizorNou) + 1];
			strcpy(this->furnizor, furnizorNou);
		}
		else {
			throw ExceptieInput("\nFurnizor incorect");
		}
	}

	//SUPRAINCARCARE OPERATOR =
	MateriePrima& operator=(const MateriePrima& m) {
		delete[] this->furnizor;
		this->furnizor = nullptr;
		this->denumire = m.denumire;
		this->cantitateDisponibila = m.cantitateDisponibila;
		this->furnizor = new char[strlen(m.furnizor) + 1];
		strcpy(this->furnizor, m.furnizor);
	}

	//SUPRAINCARCARE ISTREAM >>
	friend istream& operator>>(istream& in, MateriePrima& m) {
		//cout << "Introdu denumirea: ";
		in >> m.denumire;
		//cout << "Introdu cantitatea disponibila: ";
		in >> m.cantitateDisponibila;
		char buffer[100];
		//cout << "Introdu furnizorul: ";
		in >> buffer;
		m.setFurnizor(buffer);
		return in;
	}

	//SUPRAINCARCARE OSTREAM <<
	friend ostream& operator<<(ostream& out, const MateriePrima& m) {
		out << "\nDenumire: " << m.denumire;
		out << "\nCantitate disponibila: " << m.cantitateDisponibila;
		out << "\nFurnizor: " << m.furnizor;
		return out;
	}

	//SUPRAINCARCARE OPERATOR <
	bool operator<(const MateriePrima& other) {
		return this->cantitateDisponibila < other.cantitateDisponibila;
	}

	//METODE
	bool verificareStocMinim(float stocMinim) {
		// verifica daca cantitatea disponibila este destula pentru a acoperi un anume stoc minim
		return this->cantitateDisponibila > stocMinim;
	}
	float calculeazaValoareTotala(float pretPerUnitate) {
		// calculeaza valoarea totala a stocului disponibil de materie prima
		if (pretPerUnitate > 0) {
			return this->cantitateDisponibila * pretPerUnitate;
		}
		else {
			throw ExceptieInput("\nPretul per unitate introdus este incorect");
		}
	}
	void saveToFile(fstream& file) override {

		int lgDenumire = denumire.length() + 1;
		file.write((char*)&lgDenumire, sizeof(int));
		file.write(this->denumire.data(), lgDenumire);

		file.write((char*)&this->cantitateDisponibila, sizeof(float));

		int lgFurnizor = (furnizor != nullptr) ? strlen(furnizor) + 1 : 0;
		file.write((char*)&lgFurnizor, sizeof(int));
		if (lgFurnizor > 0) {
			file.write(this->furnizor, lgFurnizor);
		}
	}

	void restoreFromFile(fstream& file) {

		int lgDenumire;
		file.read((char*)&lgDenumire, sizeof(int));

		char* bufferDenumire = new char[lgDenumire];
		file.read(bufferDenumire, lgDenumire);
		this->denumire = bufferDenumire;
		delete[] bufferDenumire;

		file.read((char*)&this->cantitateDisponibila, sizeof(float));

		int lgFurnizor;
		file.read((char*)&lgFurnizor, sizeof(int));

		if (lgFurnizor > 0) {
			char* bufferFurnizor = new char[lgFurnizor];
			file.read(bufferFurnizor, lgFurnizor);
			this->furnizor = bufferFurnizor;
			delete[] bufferFurnizor;
		}
	}

	//DESTRUCTOR
	~MateriePrima() {
		delete[] this->furnizor;
		this->furnizor = nullptr;
	}
};

class MateriePrimaComanda {
public:

	map <string, float> material;

	void add(string denumire, float gramaj) {
		auto it = material.find(denumire);
		if (it != material.end()) {
			material[denumire] += gramaj;
		}
		else {
			material[denumire] = gramaj;
		}
	}

	float get(string denumire) {
		auto it = material.find(denumire);
		if (it != material.end()) {
			return material[denumire];
		}
		else {
			return 0;
		}
	}

};

class Preparat :IFile {

private:
	string denumire;
	string tip;
	int nrMateriiNecesare;
	MateriePrima** materiiNecesare;
	float* gramajNecesar;
	float pret;

public:

	//CONSTRUCTOR FARA PARAM
	Preparat() {
		this->denumire = "Preparat neinregistrat";
		this->tip = "Fara tip";
		this->nrMateriiNecesare = 0;
		this->materiiNecesare = nullptr;
		this->gramajNecesar = nullptr;
		this->pret = 0;
	}

	//CONSTRUCTOR CU TOTI PARAM
	Preparat(string denumire, string tip, int nrMateriiNecesare, MateriePrima** materiiNecesare, float* gramajNecesar, float pret) {
		if (tip == "Mancare" || tip == "Bautura") {
			this->denumire = denumire;
			this->tip = tip;
			if (nrMateriiNecesare > 0 && materiiNecesare != nullptr && gramajNecesar != nullptr) {
				this->nrMateriiNecesare = nrMateriiNecesare;
				this->materiiNecesare = new MateriePrima * [this->nrMateriiNecesare];
				for (int i = 0; i < this->nrMateriiNecesare; i++) {
					this->materiiNecesare[i] = new MateriePrima(*materiiNecesare[i]);
				}
				this->gramajNecesar = new float[nrMateriiNecesare];
				for (int j = 0; j < this->nrMateriiNecesare; j++) {
					this->gramajNecesar[j] = gramajNecesar[j];
				}
			}
			this->pret = pret;
		}
		else cout << "\nTipul introdus este gresit. Introdu Mancare sau Bautura";
	}

	//CONSTRUCTOR CU PARAM
	Preparat(int nrMateriiNecesare, MateriePrima** materiiNecesare, float* gramajNecesar) {
		if (nrMateriiNecesare > 0 && materiiNecesare != nullptr && gramajNecesar != nullptr) {
			this->nrMateriiNecesare = nrMateriiNecesare;
			this->materiiNecesare = new MateriePrima * [this->nrMateriiNecesare];
			for (int i = 0; i < this->nrMateriiNecesare; i++) {
				this->materiiNecesare[i] = new MateriePrima(*materiiNecesare[i]);
			}
			this->gramajNecesar = new float[this->nrMateriiNecesare];
			for (int j = 0; j < this->nrMateriiNecesare; j++) {
				this->gramajNecesar[j] = gramajNecesar[j];
			}
		}
	}

	//CONSTRUCTOR COPIERE
	Preparat(const Preparat& p) {
		this->denumire = p.denumire;
		this->tip = p.tip;
		if (p.nrMateriiNecesare > 0 && p.materiiNecesare != nullptr) {
			this->nrMateriiNecesare = p.nrMateriiNecesare;
			this->materiiNecesare = new MateriePrima * [this->nrMateriiNecesare];
			for (int i = 0; i < this->nrMateriiNecesare; i++) {
				this->materiiNecesare[i] = new MateriePrima(*p.materiiNecesare[i]);
			}
			this->gramajNecesar = new float[this->nrMateriiNecesare];
			for (int j = 0; j < this->nrMateriiNecesare; j++) {
				this->gramajNecesar[j] = p.gramajNecesar[j];
			}
			this->pret = p.pret;
		}
	}

	//GETTERI
	string getDenumire() {
		return this->denumire;
	}
	string getTip() {
		return this->tip;
	}
	int getNrMateriiNecesare() {
		return this->nrMateriiNecesare;
	}
	MateriePrima** getMateriiNecesare() {
		return this->materiiNecesare;
	}
	float* getGramajNecesar() {
		return this->gramajNecesar;
	}
	float getPret() {
		return this->pret;
	}

	//SETTERI
	void setDenumire(string denumireNou) {
		if (denumireNou.length() > 1) {
			this->denumire = denumireNou;
		}
		else {
			throw ExceptieInput("Denumirea este incorecta");
		}
	}
	void setTipNou(string tipNou) {
		if (tipNou != "Mancare" && tipNou != "Bautura") {
			throw ExceptieInput("\nTipul introdus este gresit. Introdu Mancare sau Bautura");
		}
		else {
			this->tip = tipNou;
		}
	}
	void setNrMateriiNecesare(int nrMateriiNecesareNou) {
		if (nrMateriiNecesareNou > 0) {
			this->nrMateriiNecesare = nrMateriiNecesareNou;
		}
		else {
			throw ExceptieInput("\nNumarul de materii necesare nu poate fi nul sau negativ");
		}
	}
	void setMateriiNecesareNou(MateriePrima** materiiNecesareNou) {
		if (materiiNecesareNou != nullptr) {
			this->materiiNecesare = materiiNecesareNou;
		}
		else {
			throw ExceptieInput("\nObiectul este gol");
		}
	}
	void setGramajNecesar(float* gramajNecesarNou) {
		for (int i = 0; i < this->nrMateriiNecesare; i++) {
			if (gramajNecesarNou[i] <= 0) {
				throw ExceptieInput("\nMinim un gramaj este nul sau negativ. Reintrodu.");
			}
		}
		delete[] this->gramajNecesar;
		this->gramajNecesar = new float[this->nrMateriiNecesare];
		for (int i = 0; i < this->nrMateriiNecesare; i++) {
			this->gramajNecesar[i] = gramajNecesarNou[i];
		}
	}
	void setPret(float pretNou) {
		if (pretNou <= 0) {
			this->pret = pretNou;
		}
		else {
			throw ExceptieInput("Pretul este incorect");
		}
	}

	//SUPRAINCARCARE OPERATOR =
	Preparat& operator=(const Preparat& p) {
		if (this != &p) {
			for (int i = 0; i < this->nrMateriiNecesare; i++)
			{
				delete this->materiiNecesare[i];
				this->materiiNecesare[i] = nullptr;
			}
			delete[] this->materiiNecesare;
			this->materiiNecesare = nullptr;
			delete[] this->gramajNecesar;
			this->gramajNecesar = nullptr;
			this->denumire = p.denumire;
			this->tip = p.tip;
			if (p.nrMateriiNecesare > 0 && p.materiiNecesare != nullptr && p.gramajNecesar != nullptr) {
				this->nrMateriiNecesare = p.nrMateriiNecesare;
				this->materiiNecesare = new MateriePrima * [this->nrMateriiNecesare];
				for (int i = 0; i < this->nrMateriiNecesare; i++) {
					this->materiiNecesare[i] = new MateriePrima(*p.materiiNecesare[i]);
				}
				this->gramajNecesar = new float[this->nrMateriiNecesare];
				for (int j = 0; j < this->nrMateriiNecesare; j++) {
					this->gramajNecesar[j] = p.gramajNecesar[j];
				}
				this->pret = p.pret;
			}
			return *this;
		}
	}

	//SUPRAINCARCARE ISTREAM >>
	friend istream& operator>>(istream& in, Preparat& p) {
		cout << "\nIntrodu denumirea preparatului: ";
		in >> p.denumire;
		cout << "\nIntrodu tipul preparatului (Mancare/Bautura): ";
		in >> p.tip;
		cout << "\nIntrodu numarul de materii necesare: ";
		in >> p.nrMateriiNecesare;
		cout << "\nIntrodu materiile necesare (denumire, cantitate disponibila si furnizor pentru fiecare): ";
		p.materiiNecesare = new MateriePrima * [p.nrMateriiNecesare];
		p.gramajNecesar = new float[p.nrMateriiNecesare];
		for (int i = 0; i < p.nrMateriiNecesare; i++) {
			cout << "\nMateria " << i + 1 << ": ";
			p.materiiNecesare[i] = new MateriePrima();
			in >> *p.materiiNecesare[i];
			cout << "\nIntrodu gramajul necesar: ";
			in >> p.gramajNecesar[i];
		}
		in >> p.pret;
		return in;
	}

	//SUPRAINCARCARE OSTREAM <<
	friend ostream& operator<<(ostream& out, const Preparat& p) {
		out << "\nDenumire: " << p.denumire;
		out << "\nTipul: " << p.tip;
		out << "\nNumarul de materii necesare: " << p.nrMateriiNecesare;
		out << "\nMateriile necesare: ";
		for (int i = 0; i < p.nrMateriiNecesare; i++)
			out << " " << *p.materiiNecesare[i] << "\nGramaj necesar: " << p.gramajNecesar[i] << "g";
		out << "\nPretul este: " << p.pret;
		return out;
	}

	bool operator==(const Preparat& p) {
		if (this->denumire != p.denumire) {
			return false;
		}
		if (this->tip != p.tip) {
			return false;
		}
		if (this->nrMateriiNecesare != p.nrMateriiNecesare) {
			return false;
		}
		for (int i = 0; i < this->nrMateriiNecesare; i++) {
			if (this->gramajNecesar[i] != p.gramajNecesar[i]) {
				return false;
			}
		}
		if (this->pret != p.pret) {
			return false;
		}
		return true;
	}

	//METODE
	bool contineMaterie(MateriePrima& materie) {
		//verifica daca un preparat contine o anumita materie prima
		for (int i = 0; i < this->nrMateriiNecesare; i++) {
			if (this->materiiNecesare[i]->getDenumire() == materie.getDenumire()) {
				return true;
			}
		}
		return false;
	}
	void actualizeazaGramajCuProcentaj(float procentaj) {
		//actualizeaza gramajul necesar pentru toate materiile prime necesare în funcție de un procentaj dat
		if (procentaj >= 0 && procentaj <= 100) {
			for (int i = 0; i < this->nrMateriiNecesare; i++) {
				this->gramajNecesar[i] *= (procentaj / 100.0);
			}
		}
		else {
			throw ExceptieInput("\nProcentajul introdus este incorect");
		}
	}
	void saveToFile(fstream& file) {
		file.write((char*)&this->nrMateriiNecesare, sizeof(nrMateriiNecesare));

		int lgDenumire = denumire.length() + 1;
		file.write((char*)&lgDenumire, sizeof(int));
		file.write(this->denumire.data(), lgDenumire);

		int lgTip = tip.length() + 1;
		file.write((char*)&lgTip, sizeof(int));
		file.write(this->tip.data(), lgTip);

		for (int i = 0; i < nrMateriiNecesare; ++i) {
			materiiNecesare[i]->saveToFile(file);
			file.write((const char*)&this->gramajNecesar[i], sizeof(gramajNecesar[i]));
		}

		file.write((char*)&this->pret, sizeof(pret));
	}

	void restoreFromFile(fstream& file) {
		file.read((char*)&this->nrMateriiNecesare, sizeof(nrMateriiNecesare));

		int lgDenumire;
		file.read((char*)&lgDenumire, sizeof(int));

		char* bufferDenumire = new char[lgDenumire];
		file.read(bufferDenumire, lgDenumire);
		this->denumire = bufferDenumire;
		delete[] bufferDenumire;

		int lgTip;
		file.read((char*)&lgTip, sizeof(int));

		char* bufferTip = new char[lgTip];
		file.read(bufferTip, lgTip);
		this->tip = bufferTip;
		delete[] bufferTip;

		for (int i = 0; i < nrMateriiNecesare; ++i) {
			MateriePrima* materie = new MateriePrima(); 
			materie->restoreFromFile(file);
			materiiNecesare[i] = materie;

			file.read((char*)&this->gramajNecesar[i], sizeof(gramajNecesar[i]));
		}

		file.read((char*)&this->pret, sizeof(pret));
	}

	//DESTRUCTOR
	~Preparat() {
		//for (int i = 0; i < this->nrMateriiNecesare; i++)
		//{
			//delete this->materiiNecesare[i];
			//this->materiiNecesare[i] = nullptr;
		//}
		//delete[] this->materiiNecesare;
		//this->materiiNecesare = nullptr;
		delete[] this->gramajNecesar;
		this->gramajNecesar = nullptr;
	}
};

class Meniu {

private:
	int nrPreparate; //cate preparate distincte sunt in meniu
	Preparat** preparate;

public:

	//CONSTRUCTOR FARA PARAM
	Meniu() {
		this->nrPreparate = 0;
		this->preparate = nullptr;
	}

	//CONSTRUCTOR CU TOTI PARAM
	Meniu(int nrPreparate, Preparat** preparate) {
		if (nrPreparate > 0 && preparate != nullptr) {
			this->nrPreparate = nrPreparate;
			this->preparate = new Preparat * [this->nrPreparate];
			for (int i = 0; i < this->nrPreparate; i++) {
				this->preparate[i] = new Preparat(*preparate[i]);
			}
		}
	}

	//CONSTRUCTOR CU PARAM
	Meniu(int nrPreparate) {
		if (nrPreparate > 0) {
			this->nrPreparate = nrPreparate;
		}
	}

	//CONSTRUCTOR COPIERE
	Meniu(const Meniu& m) {
		if (m.nrPreparate > 0 && m.preparate != nullptr) {
			this->nrPreparate = m.nrPreparate;
			this->preparate = new Preparat * [this->nrPreparate];
			for (int i = 0; i < this->nrPreparate; i++) {
				this->preparate[i] = new Preparat(*m.preparate[i]);
			}
		}
	}

	//SUPRAINCARCARE OPERATOR =
	Meniu& operator=(const Meniu& m) {
		if (this != &m) {
			for (int i = 0; i < this->nrPreparate; i++)
			{
				delete this->preparate[i];
				this->preparate[i] = nullptr;
			}
			delete[] this->preparate;
			this->preparate = nullptr;
			if (m.nrPreparate > 0 && m.preparate != nullptr) {
				this->nrPreparate = m.nrPreparate;
				this->preparate = new Preparat * [this->nrPreparate];
				for (int i = 0; i < this->nrPreparate; i++) {
					this->preparate[i] = new Preparat(*m.preparate[i]);
				}
			}
			return *this;
		}
	}

	//SUPRAINCARCARE ISTREAM >>
	friend istream& operator>>(istream& in, Meniu& m) {
		cout << "\nIntrodu numarul de preparate in meniu: ";
		in >> m.nrPreparate;
		cout << "\nIntrodu preparatele din meniu: ";
		m.preparate = new Preparat * [m.nrPreparate];
		for (int i = 0; i < m.nrPreparate; i++) {
			cout << "\nPreparatul " << i + 1 << ": ";
			m.preparate[i] = new Preparat();
			in >> *m.preparate[i];
		}
		return in;
	}

	//SUPRAINCARCARE OSTREAM <<
	friend ostream& operator<<(ostream& out, const Meniu& m) {
		//out << "\nNumarul de preparate: " << m.nrPreparate;
		//out << "\nPreparatele din meniu: ";
		for (int i = 0; i < m.nrPreparate; i++) {
			out << " " << *m.preparate[i];
			out << "\n";
		}
		return out;
	}

	//SUPRAINCARCARE OPERATOR []
	Preparat& operator[](int index) {
		if (index >= 0 && index < this->nrPreparate) {
			return *this->preparate[index];
		}
		else {
			throw ExceptieInput("\nIndexul nu este valid.");
		}
	}

	//GETTERI
	int getNrPreparate() {
		return this->nrPreparate;
	}
	Preparat** getPreparate() {
		return this->preparate;
	}

	//SETTERI
	void setNrPreparate(int nrPreparateNou) {
		if (nrPreparateNou > 0) {
			this->nrPreparate = nrPreparateNou;
		}
		throw ExceptieInput("\nNumarul de preparate nu poate fi nul sau negativ");
	}
	void setPreparate(Preparat** preparateNou) {
		if (preparateNou != nullptr) {
			this->preparate = preparateNou;
		}
		else {
			throw ExceptieInput("\nObiectul nu poate fi gol");
		}
	}

	//METODE
	void adaugarePreparat(Preparat* preparatNou) {
		if (this->preparate != nullptr) {
			Preparat** temp = new Preparat * [this->nrPreparate + 1];
			for (int i = 0; i < this->nrPreparate; i++) {
				temp[i] = this->preparate[i];
			}
			temp[this->nrPreparate] = preparatNou;
			delete[] this->preparate;
			this->preparate = temp;
			this->nrPreparate++;
		}
		else {
			throw ExceptieInput("\nObiectul nu poate fi gol");
		}
	}
	int cautarePreparatDupaNume(string nume) {
		for (int i = 0; i < this->nrPreparate; i++) {
			if (this->preparate[i]->getDenumire() == nume) {
				return i;
			}
		}
		return -1;
	}

	//DESTRUCTOR
	~Meniu() {
		for (int i = 0; i < this->nrPreparate; i++)
		{
			delete this->preparate[i];
			this->preparate[i] = nullptr;
		}
		delete[] this->preparate;
		this->preparate = nullptr;
	}

};

class Comanda {

private:
	const int id;
	int nrProduse; //cate produse distincte dorim
	Preparat** produse;
	int* cantitati;

public:

	//CONSTRUCTOR FARA PARAM
	Comanda(int id) : id(id) {
		this->nrProduse = 0;
		this->produse = nullptr;
		this->cantitati = nullptr;
	}

	//CONSTRUCTOR CU TOTI PARAM
	Comanda(int id, int nrProduse, Preparat** produse, int* cantitati) : id(id) {
		if (nrProduse > 0 && produse != nullptr) {
			this->nrProduse = nrProduse;
			this->produse = new Preparat * [this->nrProduse];
			for (int i = 0; i < this->nrProduse; i++) {
				this->produse[i] = new Preparat(*produse[i]);
			}
			this->cantitati = new int[this->nrProduse];
			for (int i = 0; i < this->nrProduse; i++) {
				this->cantitati[i] = cantitati[i];
			}
		}
	}

	//CONSTRUCTOR CU PARAM
	Comanda(int id, int nrProduse, Preparat** produse) : id(id) {
		if (nrProduse > 0 && produse != nullptr) {
			this->nrProduse = nrProduse;
			this->produse = new Preparat * [this->nrProduse];
			for (int i = 0; i < this->nrProduse; i++) {
				this->produse[i] = new Preparat(*produse[i]);
			}
		}
	}

	//CONSTRUCTOR COPIERE
	Comanda(const Comanda& c) : id(c.id) {
		if (c.nrProduse > 0 && c.produse != nullptr) {
			this->nrProduse = c.nrProduse;
			this->produse = new Preparat * [this->nrProduse];
			for (int i = 0; i < this->nrProduse; i++) {
				this->produse[i] = new Preparat(*c.produse[i]);
			}
			this->cantitati = new int[this->nrProduse];
			for (int i = 0; i < this->nrProduse; i++) {
				this->cantitati[i] = c.cantitati[i];
			}
		}
	}

	//GETTERI
	int getId() {
		return this->id;
	}
	int getNrProduse() {
		return this->nrProduse;
	}
	Preparat** getProduse() {
		return this->produse;
	}
	int* getCantitati() {
		return this->cantitati;
	}

	//SETTERI
	void setNrProduse(int nrProduseNou) {
		if (nrProduseNou > 0) {
			this->nrProduse = nrProduseNou;
		}
		throw ExceptieInput("\nNumarul de produse nu poate fi 0 sau negativ");
	}
	void setProduse(Preparat** produseNou) {
		if (produseNou != nullptr) {
			this->produse = produseNou;
		}
		else {
			throw ExceptieInput("\nObiectul nu poate fi gol.");
		}
	}
	void setCantitati(int* cantitatiNou) {
		for (int i = 0; i < this->nrProduse; i++) {
			if (cantitatiNou[i] <= 0) {
				throw ExceptieInput("\nMinim o cantitate este nula sau negativa. Reintrodu.");
			}
		}
		delete[] this->cantitati;
		this->cantitati = new int[this->nrProduse];
		for (int i = 0; i < this->nrProduse; i++) {
			this->cantitati[i] = cantitati[i];
		}
	}

	//SUPRAINCARCARE ISTREAM >>
	friend istream& operator>>(istream& in, Comanda& c) {
		cout << "\nIntrodu numarul de produse din comanda: ";
		in >> c.nrProduse;
		c.produse = new Preparat * [c.nrProduse];
		c.cantitati = new int[c.nrProduse];
		for (int i = 0; i < c.nrProduse; i++) {
			c.produse[i] = new Preparat();
			cout << "\nIntrodu produsul " << i + 1 << ": ";
			in >> *c.produse[i];
			cout << "\nIntrodu cantitatea pentru produsul " << i + 1 << ": ";
			in >> c.cantitati[i];
		}
		return in;
	}

	//SUPRAINCARCARE OSTREAM <<
	friend ostream& operator<<(ostream& out, const Comanda& c) {
		out << "\nId-ul comenzii: " << c.id;
		out << "\nNumarul de produse: " << c.nrProduse;
		out << "\nProdusele din comanda: ";
		for (int i = 0; i < c.nrProduse; i++)
			out << " " << *c.produse[i];
		out << "\nCantitatea din fiecare produs: ";
		for (int i = 0; i < c.nrProduse; i++)
			out << " " << c.cantitati[i];
		return out;
	}

	//SUPRAINCARCARE OPERATOR =
	Comanda& operator=(const Comanda& c) {
		if (this != &c) {
			for (int i = 0; i < this->nrProduse; i++)
			{
				delete this->produse[i];
				this->produse[i] = nullptr;
			}
			delete[] this->produse;
			this->produse = nullptr;
			delete[] this->cantitati;
			this->cantitati = nullptr;
			if (c.nrProduse > 0 && c.produse != nullptr) {
				this->nrProduse = c.nrProduse;
				this->produse = new Preparat * [this->nrProduse];
				for (int i = 0; i < this->nrProduse; i++) {
					this->produse[i] = new Preparat(*c.produse[i]);
				}
				this->cantitati = new int[this->nrProduse];
				for (int i = 0; i < this->nrProduse; i++) {
					this->cantitati[i] = c.cantitati[i];
				}
			}
			return *this;
		}
	}

	//SUPRAINCARCARE OPERATOR ++ pre-incrementare
	Comanda& operator++() {
		for (int i = 0; i < this->nrProduse; i++) {
			this->cantitati[i]++;
		}
		return *this;
	}
	//SUPRAINCARCARE OPERATOR ++ post-incrementare
	Comanda operator++(int) {
		Comanda copie(*this);
		for (int i = 0; i < this->nrProduse; i++) {
			this->cantitati[i]++;
		}
		return copie;
	}

	//SUPRAINCARCARE OPERATOR !
	bool operator!() {
		return this->nrProduse == 0;
	}

	//METODE
	bool continePreparat(Preparat& preparatCautat) {
		for (int i = 0; i < this->nrProduse; i++) {
			if (*this->produse[i] == preparatCautat) {
				return true;
			}
		}
		return false;
	}
	MateriePrimaComanda materialeNecesare() {
		int nrProduse = this->getNrProduse();
		Preparat** produse = this->getProduse();
		int* cantitati = this->getCantitati();
		MateriePrimaComanda materialeComanda;
		for (int i = 0; i < nrProduse; i++) {
			float* gramaj = produse[i]->getGramajNecesar();
			MateriePrima** materiiNecesare = produse[i]->getMateriiNecesare();
			int nrMateriiNecesare = produse[i]->getNrMateriiNecesare();
			for (int j = 0; j < nrMateriiNecesare; j++) {
				materialeComanda.add(materiiNecesare[j]->getDenumire(), cantitati[i] * gramaj[j]);
			}
		}
		return materialeComanda;
	}

	//DESTRUCTOR
	~Comanda() {
		for (int i = 0; i < this->nrProduse; i++)
		{
			delete this->produse[i];
			this->produse[i] = nullptr;
		}
		delete[] this->produse;
		this->produse = nullptr;
		delete[] this->cantitati;
		this->cantitati = nullptr;
	}
};

class Restaurant {

private:
	static int codCaen;
	string denumire;
	Meniu meniu;
	float* profitAnualAnterior;

public:

	//CONSTRUCTOR FARA PARAM
	Restaurant() {
		this->denumire = "Anonim";
		Meniu m;
		this->meniu = m;
		this->profitAnualAnterior = nullptr;
	}

	//CONSTRUCTOR CU TOTI PARAM
	Restaurant(string denumire, Meniu meniu, float* profitAnualAnterior) {
		this->denumire = denumire;
		this->meniu = meniu;
		this->profitAnualAnterior = new float[12];
		for (int i = 0; i < 12; i++) {
			this->profitAnualAnterior[i] = profitAnualAnterior[i];
		}
	}

	//CONSTRUCTOR CU PARAM
	Restaurant(string denumire) {
		this->denumire = denumire;
	}

	//CONSTRUCTOR COPIERE
	Restaurant(const Restaurant& r) {
		this->denumire = r.denumire;
		this->meniu = r.meniu;
		this->profitAnualAnterior = new float[12];
		for (int i = 0; i < 12; i++) {
			this->profitAnualAnterior[i] = r.profitAnualAnterior[i];
		}
	}

	//GETTERI
	static int getCodCaen() {
		return Restaurant::codCaen;
	}
	string getDenumire() {
		return this->denumire;
	}
	Meniu& getMeniu() {
		return this->meniu;
	}
	float* getProfitAnualAnterior() {
		if (this->profitAnualAnterior != nullptr) {
			return profitAnualAnterior;
		}
		else {
			throw ExceptieInput("\nIntroducere incorecta.");
		}
	}

	//SETTERI
	static void setCodCaen(int codCaenNou) {
		if (codCaenNou > 0) {
			Restaurant::codCaen = codCaenNou;
		}
		else {
			throw ExceptieInput("\nCodul CAEN nu este valid");
		}
	}
	void setDenumire(string denumireNou) {
		if (denumireNou.length() > 1) {
			this->denumire = denumireNou;
		}
		else {
			throw ExceptieInput("\nDenumire invalida.");
		}
	}
	void setMeniu(Meniu meniuNou) {
		//if (meniuNou != nullptr) {
		this->meniu = meniuNou;
		//}
	}
	void setProfitAnualAnterior(float ProfitAnualAnteriorNou) {
		if (this->profitAnualAnterior == nullptr) {
			this->profitAnualAnterior = new float[12];
		}
		for (int i = 0; i < 12; i++) {
			this->profitAnualAnterior[i] = profitAnualAnterior[i];
		}
	}

	//SUPRAINCARCARE OPERATOR =
	Restaurant& operator=(const Restaurant& r) {
		if (&r != this) {
			delete[] this->profitAnualAnterior;
			this->profitAnualAnterior = nullptr;
			this->denumire = r.denumire;
			this->meniu = r.meniu;
			this->profitAnualAnterior = new float[12];
			for (int i = 0; i < 12; i++) {
				this->profitAnualAnterior[i] = r.profitAnualAnterior[i];
			}
		}
		return *this;
	}

	//SUPRAINCARCARE ISTREAM >>
	friend istream& operator>>(istream& in, Restaurant& r) {
		cout << "\nIntrodu denumirea restaurantului: ";
		in >> r.denumire;
		cout << "\nIntrodu meniul: ";
		in >> r.meniu;
		cout << "\nIntrodu profitul anual anterior: ";
		r.profitAnualAnterior = new float[12];
		for (int i = 0; i < 12; i++) {
			cout << "\nLuna " << i + 1 << ": ";
			in >> r.profitAnualAnterior[i];
		}
		return in;
	}

	//SUPRAINCARCARE OSTREAM <<
	friend ostream& operator<<(ostream& out, const Restaurant& r) {
		out << "\nCodul CAEN al afacerii: " << Restaurant::codCaen;
		out << "\nDenumire: " << r.denumire;
		out << "\nMeniul restaurantului: " << r.meniu;
		out << "\nProfitul anual anterior: ";
		for (int i = 0; i < 12; i++) {
			cout << "\nLuna " << i + 1 << ": ";
			out << r.profitAnualAnterior[i];
		}
	}

	//SUPRAINCARCARE OPERATOR CAST INT
	operator int() {
		return Restaurant::codCaen;
	}

	//SUPRAINCARCARE OPERATOR +
	Restaurant operator+(float valoare) {
		Restaurant rezultat(*this);
		for (int i = 0; i < 12; ++i) {
			rezultat.profitAnualAnterior[i] += valoare;
		}
		return rezultat;
	}

	//METODE
	float calculeazaProfitAnualTotal() {
		//calculeaza profitul anual total al restaurantului
		float profitTotal = 0.0;
		for (int i = 0; i < 12; i++) {
			profitTotal += this->profitAnualAnterior[i];
		}
		return profitTotal;
	}
	void actualizeazaProfitLunar(int luna, float profitNou) {
		//actualizeaza profitul pentru o luna specifica
		if (luna >= 1 && luna <= 12) {
			this->profitAnualAnterior[luna - 1] = profitNou;
		}
		else {
			throw ExceptieInput("\nNumarul lunii este invalid.");
		}
	}

	//DESTRUCTOR
	~Restaurant() {
		delete[] this->profitAnualAnterior;
		this->profitAnualAnterior = nullptr;
	}
};

int Restaurant::codCaen = 5610;

// Functie pentru a obtine valoarea contorului din fisier
int citesteContor(const char* numeFisier) {
	ifstream fisier(numeFisier);
	int valoareContor = 0;

	if (fisier.is_open()) {
		fisier >> valoareContor;
		fisier.close();
	}

	return valoareContor;
}
// Functie pentru a actualiza si scrie valoarea contorului înapoi în fisier
void actualizeazaContor(const char* numeFisier, int nouaValoare) {
	ofstream fisier(numeFisier);

	if (fisier.is_open()) {
		fisier << nouaValoare;
		fisier.close();
	}
	else {
		cerr << "Eroare la scrierea in fisier!" << endl;
	}
}

int main() {

	set<string> administratori;
	set<string>::iterator itSet;
	administratori.insert("Radu");
	administratori.insert("Dinu");
	administratori.insert("Gabriel");

	ofstream materieof("materiiPrimeCout.txt");
	ifstream materieif("materiiPrimeCin.txt");
	MateriePrima sare, piper, faina, ulei, carne, cascaval, uleiMasline, ciocolata, oua, zahar, lapte, avocado, sosBarbecue;
	materieif >> sare >> piper >> faina >> ulei >> carne >> cascaval >> uleiMasline >> ciocolata >> oua >> zahar >> lapte >> avocado >> sosBarbecue;
	materieof << sare << endl << piper << endl << faina << endl << ulei << endl << carne << endl << cascaval << endl << uleiMasline
		<< endl << ciocolata << endl << oua << endl << zahar << endl << lapte << endl << avocado << endl << sosBarbecue;
	materieof.close();
	materieif.close();
	MateriePrima stoc[] = { sare, piper, faina, ulei, carne, cascaval, uleiMasline, ciocolata, oua, zahar, lapte, avocado, sosBarbecue };

	list<MateriePrima> listaMaterii;
	listaMaterii.push_back(sare);
	listaMaterii.push_back(piper);
	listaMaterii.push_back(faina);
	listaMaterii.push_back(ulei);
	listaMaterii.push_back(carne);
	listaMaterii.push_back(cascaval);
	listaMaterii.push_back(uleiMasline);
	listaMaterii.push_back(ciocolata);
	listaMaterii.push_back(oua);
	listaMaterii.push_back(zahar);
	listaMaterii.push_back(lapte);
	listaMaterii.push_back(avocado);
	listaMaterii.push_back(sosBarbecue);

	ofstream preparateof("preparateCout.txt");
	ifstream preparateif("preparateCin.txt");
	Preparat pizza, burghir, omleta, salata, tiramisu, blueLagoon;
	preparateif >> pizza >> burghir >> blueLagoon >> salata >> tiramisu >> omleta;
	preparateof << pizza << endl << burghir << endl << blueLagoon << endl << salata << endl << tiramisu << endl << omleta;
	preparateif.close();
	preparateof.close();

	fstream file("meniu.bin", ios::out | ios::binary);
	pizza.saveToFile(file);
	burghir.saveToFile(file);
	blueLagoon.saveToFile(file);
	salata.saveToFile(file);
	tiramisu.saveToFile(file);
	omleta.saveToFile(file);
	file.close();

	const char* numeFisierContor = "contor.txt";
	int valoareContor = citesteContor(numeFisierContor);
	valoareContor++;
	actualizeazaContor(numeFisierContor, valoareContor);

	system("cls");
	ofstream utilizatoriof("utilizatoriof.txt",ios::app);
	string utilizator;
	cout << "\nIntroduceti numele de utilizator: ";
	cin >> utilizator;
	utilizatoriof << citesteContor(numeFisierContor) << ". " << utilizator;
	for (itSet = administratori.begin(); itSet != administratori.end(); itSet++) {
		if (*itSet == utilizator) {
			utilizatoriof << " - Admin";
		}
	}
	utilizatoriof << endl;
	utilizatoriof.close();

	time_t timpCurent = time(nullptr);
	tm* timpStruct = localtime(&timpCurent);
	const int dimensiuneMaxima = 80;
	char timpString[dimensiuneMaxima];
	strftime(timpString, dimensiuneMaxima, "%Y-%m-%d %H:%M:%S", timpStruct);
	ofstream dateLogare("dateLogare.txt", ios::app);
	dateLogare << citesteContor(numeFisierContor) << ". " << timpString << endl;
	dateLogare.close();

	ofstream comenzi("comenzi.txt", ios::app);

	ifstream csv("RecenziiClienti.csv");
	vector<vector<string>> recenziiClienti;
	string linie;
	while (getline(csv, linie)) {
		size_t pos = 0;
		string valoare;
		vector<string> subsir;
		while ((pos = linie.find(',')) < linie.length()) {
			valoare = linie.substr(0, pos);
			subsir.push_back(valoare);
			linie.erase(0, pos + 1);
		}
		subsir.push_back(linie);
		recenziiClienti.push_back(subsir);
	}

	ofstream datamining("RapoarteDataMining.txt", ios::app);
	datamining << "\nUtilizatorul " << utilizator << " cu id-ul " << valoareContor << " a facut urmatoarele actiuni: ";
	int ok1, ok2, ok3;
	ok1 = ok2 = ok3 = 0;

	Preparat* vectorPreparate[] = { &pizza, &burghir, &omleta, &salata, &tiramisu, &blueLagoon };
	map <string, Preparat> materiale;
	materiale["pizza"] = pizza;
	materiale["burger"] = burghir;
	materiale["omleta"] = omleta;
	materiale["salata"] = salata;
	materiale["tiramisu"] = tiramisu;
	materiale["blueLagoon"] = blueLagoon;

	Meniu meniul(6, vectorPreparate);

	system("cls");
	cout << "\nIntroduceti actiunea dorita:";
	cout << "\n1. Consultati meniul.";
	cout << "\n2. Plasati o comanda. ";
	cout << "\n3. Vizualizare recenzii";
	cout << "\n4. Iesire.";
	int op = 0;
	do {
		cout << "\n";
		cin >> op;
		if (op == 1) {
			ok1 = 1;
			system("cls");
			cout << "\n";
			int i;
			cout << "Fel principal: \n";
			for (i = 0; i < 4; i++) {
				cout << i + 1 << ". " << meniul[i].getDenumire() << " - " << meniul[i].getPret() << " lei\n";
			}
			cout << "\nDesert: \n";
			for (i = 4; i < 5; i++) {
				cout << i + 1 << ". " << meniul[i].getDenumire() << " - " << meniul[i].getPret() << " lei\n";
			}
			cout << "\nCocktail: \n";
			for (i = 5; i < 6; i++) {
				cout << i + 1 << ". " << meniul[i].getDenumire() << " - " << meniul[i].getPret() << " lei\n";
			}
			int op1;
			cout << "\nIntrodu numarul preparatului pentru informatii: ";
			cout << "\nSau 0 pentru a te intoarce la pagina princiapala.\n";
			cin >> op1;
			if (op1 == 0) {
				system("cls");
				cout << "\nIntroduceti actiunea dorita:";
				cout << "\n1. Consultati meniul.";
				cout << "\n2. Plasati o comanda. ";
				cout << "\n3. Vizualizare recenzii";
				cout << "\n4. Iesire.";
			}
			else {
				if (op1 < 7 && op1 > 0) {
					system("cls");
					cout << meniul[op1 - 1];
					cout << "\n\nIntroduceti orice valoare pentru a va intoarce.\n";
					int op11;
					cin >> op11;
					system("cls");
					cout << "\nIntroduceti actiunea dorita:";
					cout << "\n1. Consultati meniul.";
					cout << "\n2. Plasati o comanda. ";
					cout << "\n3. Vizualizare recenzii";
					cout << "\n4. Iesire.";
				}
				else {
					do {
						cout << "\nValoarea introdusa este incorecta. Reintrodu.\n";
						cin >> op1;
					} while (op1 < 0 || op1 > 4);
					system("cls");
					cout << meniul[op1 - 1];
					cout << "\n\nIntroduceti orice valoare pentru a va intoarce.\n";
					int op11;
					cin >> op11;
					system("cls");
					cout << "\nIntroduceti actiunea dorita:";
					cout << "\n1. Consultati meniul.";
					cout << "\n2. Plasati o comanda. ";
					cout << "\n3. Vizualizare recenzii";
					cout << "\n4. Iesire.";
				}
			}
		}
		else {
			if (op == 2) {
				comenzi << citesteContor(numeFisierContor) << ". Comanda:";
				ok2 = 1;
				system("cls");
				int idcmd = 1;
				int nrProduse;
				Preparat** produse;
				int* cantitati;
				cout << "\nCate tipuri de preparate ati dori?";
				cout << "\nExemplu: pizza, tiramisu = 2";
				cout << "\n\nIntroduceti valoarea: ";
				cin >> nrProduse;
				system("cls");
				int i;
				cout << "Fel principal: \n";
				for (i = 0; i < 4; i++) {
					cout << i + 1 << ". " << meniul[i].getDenumire() << " - " << meniul[i].getPret() << " lei\n";
				}
				cout << "\nDesert: \n";
				for (i = 4; i < 5; i++) {
					cout << i + 1 << ". " << meniul[i].getDenumire() << " - " << meniul[i].getPret() << " lei\n";
				}
				cout << "\nCocktail: \n";
				for (i = 5; i < 6; i++) {
					cout << i + 1 << ". " << meniul[i].getDenumire() << " - " << meniul[i].getPret() << " lei\n";
				}
				produse = new Preparat * [nrProduse];
				cantitati = new int[nrProduse];
				for (int k = 0; k < nrProduse; k++) {
					cout << "\nIntroduceti preparatul " << k + 1 << "(numele preparatului cu litere mici): ";
					string temp;
					cin >> temp;
					produse[k] = &materiale[temp];
					cout << "Cate doriti din acest fel? ";
					int cantitate;
					cin >> cantitate;
					cantitati[k] = cantitate;
					cout << "\n";
					comenzi << endl << cantitate << " x " << temp;
				}
				comenzi << endl;
				Comanda order(idcmd, nrProduse, produse, cantitati);
				MateriePrimaComanda materiiComanda = order.materialeNecesare();
				int contor = 0;
				for (int j = 0; j < 13; j++) {
					if (materiiComanda.get(stoc[j].getDenumire()) > stoc[j].getCantitateDisponibila()) {
						system("cls");
						cout << "\nNe pare rau, nu avem destula materie prima pentru a onora comanda.";
						cout << "\nVa rugam sa o refaceti.\n";
						op = 4;
					}
					else {
						contor++;
					}
				}
				if (contor == 13) {
					system("cls");
					cout << "\nComanda inregistrata! Multumim " << utilizator << "!";
					cout << "\n\n";
					op = 4;
				}
			}
			else {
				if (op == 3) {
					ok3 = 1;
					system("cls");
					int contoar = 1;
					for (size_t i = 0; i < recenziiClienti.size(); i++) {
						for (size_t j = 0; j < recenziiClienti[i].size(); j++) {
							cout << contoar << ". " << "\"" << recenziiClienti[i][j] << "\"" << endl;
							contoar++;
						}
					}
					cout << "\n\nIntroduceti orice valoare pentru a va intoarce.\n";
					int op112;
					cin >> op112;
					system("cls");
					cout << "\nIntroduceti actiunea dorita:";
					cout << "\n1. Consultati meniul.";
					cout << "\n2. Plasati o comanda. ";
					cout << "\n3. Vizualizare recenzii";
					cout << "\n4. Iesire.";
				}
				else {
					if (op == 4) {
						system("cls");
						cout << "\nO zi buna! Va mai asteptam!\n\n";
						cout << "    *****    " << endl;
						cout << "  *       *  " << endl;
						cout << " * O     O * " << endl;
						cout << " *   \\_/   * " << endl;
						cout << "  *       *  " << endl;
						cout << "    *****    " << endl;
					}
					else {
						cout << "\nValoarea introdusa nu este corecta. Reintrodu.";
					}
				}
			}
		}
	} while (op != 4);

	datamining << "\nVizualizare meniu: ";
	if (ok1 == 0) datamining << "NU";
	else datamining << "DA";
	datamining << "\nPlasare comanda: ";
	if (ok2 == 0) datamining << "NU";
	else datamining << "DA";
	datamining << "\nVizualizare recenzii: ";
	if (ok3 == 0) datamining << "NU";
	else datamining << "DA";
	datamining << endl;

	comenzi << endl;
	comenzi.close();
	datamining.close();
	return 0;
}
