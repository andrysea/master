#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cctype>
using namespace std;

// Enum per il tipo di interazione
enum class TipoInterazione { Appuntamento = 1, Contratto = 2};

// Funzione di supporto per stampare il tipo
static string tipoToString(TipoInterazione tipo) {
    switch (tipo) {
        case TipoInterazione::Appuntamento: return "Appuntamento";
        case TipoInterazione::Contratto: return "Contratto";
        default: return "Sconosciuto";
    }
}

// Funzione di supporto, ritorna true se str è un numero intero valido
static bool isInteger(const string& str) {
        if(str.empty()) return false;   // stringa vuota, allora non numero

        size_t start = 0;

        // Gestisce eventuale segno + o -
        if(str[0] == '+' || str[0] == '-') {
            if(str.length() == 1) return false; // solo + o - non va bene
            start = 1;
        }

        // Controlla che tutti i caratteri siano cifre
        for(size_t i = start; i < str.length(); ++i) {
            if(!isdigit(str[i])) return false;
        }

        return true;
}

// Funzione di supporto per vedere se una stringa è vuota o contiene solo spazi
static bool isEmptyOrSpaces(const std::string& str) {
    return str.empty() || std::all_of(str.begin(), str.end(), [](char c){ return std::isspace(c); });
}


// Classe per rappresentare un'interazione
class Interazione {
private:
    TipoInterazione tipo;
    string descrizione;

public:
    // Costruttore
    Interazione(TipoInterazione t, string d){
        tipo = t;
        descrizione = d;
    }

    // Metodi
    // Stampa Interazioni
    void stampaInterazione() const{
        cout << "Tipo interazione: " << tipoToString(tipo) << " - Descrizione: " << descrizione << endl;
    }

    // Confronta due oggetti Interazione, restituisce true se tutti gli attributi (tipo, descrizione) sono uguali
    bool operator==(const Interazione& other) const {
        return tipo == other.tipo &&
               descrizione == other.descrizione;
    }

    // Metodi get & set
    string getTipo() const { return tipoToString(this->tipo); }
    string getDescrizione() const { return this->descrizione; }

    void setTipoInterazione(TipoInterazione tipo) {
        this->tipo = tipo;
    }

    void setDescrizione(string descrizione) {
        this->descrizione = descrizione;
    }
};


// Classe per rappresentare un cliente
class Cliente {
    private:
        string nome;
        string cognome;
        vector<Interazione> interazioni;

    public:
        // Costruttore
        Cliente(string n, string c){
            nome = n;
            cognome = c;
        }

        // Metodi
        // Aggiungere interazione per cliente
        void aggiungiInterazione(const Interazione& i) {
            interazioni.push_back(i);
        }

        // Mostra interazioni del cliente
        void mostraInterazioni() const{
            cout << "Interazioni di " << nome << ":" << endl;
            for (const auto& i : interazioni) {
                i.stampaInterazione();
            }
        }

        // Cerca interazioni per tipo (obbligatorio) e descrizione (opzionale)
        void cercaInterazioniPerTipoEDescrizione(const string& tipo, const string& descrizione = "") {
                bool check = false;
                cout << "Ricerca di interazioni...'" << endl;

                for (const auto& i : interazioni) {
                    bool matchTipo = (i.getTipo() == tipo);
                    bool matchDescrizione = (descrizione.empty() || i.getDescrizione() == descrizione);

                    if (matchTipo && matchDescrizione) {
                        i.stampaInterazione();
                        check = true;
                    }
                }

                if(!check) {
                    cout << "La ricerca non ha prodotto risultati." << endl;
                }
        }

        // Stampare informazioni cliente
        string toString() const {
            return "Nome: " + this->nome +
                   "\nCognome: " + this->cognome;
        }

        // Metodi get & set
        string getNome() const { return this->nome; }
        string getCognome() const{ return this->cognome; }

        void setNome(string nome) {
            this->nome = nome;
        }

        void setCognome(string cognome) {
            this->cognome = cognome;
        }

        void setInterazioni(vector<Interazione> interazioni){
            this->interazioni = interazioni;
        }

        const vector<Interazione>& getInterazioni() const {
            return interazioni;
        }
};

// Classe CRM
class CRM {
    private:
        vector<Cliente> clienti;

    // Costruttore
    public:
        CRM(){}

		// Metodi
        // Metodi per i clienti
        // Aggiunta cliente
        void aggiungiCliente(const Cliente& cliente) {
            clienti.push_back(cliente);
        }

        // Visualizza clienti
        void visualizzaClienti() {
            int i = 1;
            if(clienti.size() > 0)
	            for (const auto& cliente : clienti) {
	                cout << i << " - Cliente: " << cliente.getNome() << " " << cliente.getCognome() << endl;
	                i++;
	            }
	        else{
	        	cout << "Nessun cliente registrato." << endl;
			}
        }

        // Modifica cliente
        void modificaCliente(int indice, Cliente& clienteNew){
            indice --;

            if (indice >= 0 && indice < clienti.size()) {
                Cliente& clienteOld = this->clienti[indice];

                if(clienteOld.getNome() != clienteNew.getNome() && !isEmptyOrSpaces(clienteNew.getNome())){
                clienteOld.setNome(clienteNew.getNome());
               }

               if(clienteOld.getCognome() != clienteNew.getCognome() && !isEmptyOrSpaces(clienteNew.getCognome())){
                    clienteOld.setCognome(clienteNew.getCognome());
               }

               cout << "Cliente modificato correttamente." << endl;
            }
            else{
                cout << "ERRORE --- Valore indice non corretto." << endl;
            }
        }

        // Ricerca cliente
		vector<Cliente> ricercaCliente(const string& nome, const string& cognome) {
		    vector<Cliente> clientiTrovati;

		    for (const auto& cliente : clienti) {

		        // Se nome è valorizzato, cerca per nome
		        if (!nome.empty()) {
		            if (cliente.getNome() == nome) {
		                clientiTrovati.push_back(cliente);
		            }
		        }
		        // Altrimenti cerca per cognome
		        else if (!cognome.empty()) {
		            if (cliente.getCognome() == cognome) {
		                clientiTrovati.push_back(cliente);
		            }
		        }
		    }

		    return clientiTrovati;
		}


        // Elimina cliente
        void eliminaCliente(int indice){
            indice --;
            if (indice >= 0 && indice < clienti.size()) {
                this->clienti.erase(clienti.begin() + indice);
            }
            else{
                cout << "ERRORE --- Valore indice non corretto." << endl;
            }
        }

        // Metodi get e set
        vector<Cliente>& getClienti() {
            return this->clienti;
        }

        void setClienti(const vector<Cliente>& nuoviClienti) {
            this->clienti = nuoviClienti;
        }
};

// Namespace per raggruppare le funzioni legate alla gestione del file
namespace CRMStorage {

    const string NOME_FILE = "dati_crm.txt";

    // Caricamento dati clienti
    vector<Cliente> caricaTutto() {
        vector<Cliente> clientiLista;
        ifstream file(NOME_FILE);

        // Se il file non esiste, lo creo vuoto e ritorno lista vuota
        if (!file) {
            ofstream nuovoFile(NOME_FILE);
            nuovoFile.close();
            return clientiLista;
        }

        string tag;

        while (getline(file, tag, ';')) {

            // Prendo informazioni cliente
            if (tag == "CLIENTE") {

                string nome, cognome;

                getline(file, nome, ';');
                getline(file, cognome);
                Cliente cliente(nome, cognome);

                string countTag;
                int count = 0;

                if (getline(file, countTag, ';') && countTag == "COUNT") {

                    file >> count;
                    file.ignore(1000, '\n');

                    for (int i = 0; i < count; ++i) {

                        string tipoStr, descrizione;

                        getline(file, tipoStr, ';');
                        getline(file, descrizione);

                        TipoInterazione tipo =
                            (tipoStr == "Appuntamento") ?
                            TipoInterazione::Appuntamento :
                            TipoInterazione::Contratto;

                        cliente.aggiungiInterazione(
                            Interazione(tipo, descrizione)
                        );
                    }
                }

                clientiLista.push_back(cliente);
            }
        }

        file.close();

        return clientiLista;
    }

    // Salvataggio dati clienti
    void salvaTutto(const vector<Cliente>& clientiLista) {

        // Apro il file in modalità trunc, se esiste viene svuotato, se non esiste viene creato
        ofstream file(NOME_FILE, ios::trunc);

        if (!file) {
            cout << "Errore apertura file!" << endl;
            return;
        }

        // Scrivo tutti i clienti presenti in clientiLista sul mio file
        for (const auto& cliente : clientiLista) {

            file << "CLIENTE;"
                 << cliente.getNome() << ";"
                 << cliente.getCognome() << "\n";

            const auto& interazioni = cliente.getInterazioni();

            file << "COUNT;" << interazioni.size() << "\n";

            for (const auto& interazione : interazioni) {
                file << interazione.getTipo() << ";"
                     << interazione.getDescrizione() << "\n";
            }
        }

        file.close();

        cout << "CRM salvato correttamente (" << clientiLista.size() << " clienti)." << endl;
    }
};

// MAIN
int main() {

    // Dichiarazione e inizializzazione
    int pulsante = 0;
    string input;
    CRM crm;
    crm.setClienti(CRMStorage::caricaTutto());

    cout << "Benvenuto nel sistema CRM per la gestione dei clienti!" <<endl;

    while(pulsante != -1){

        cout << "-------------------------------------------------------------------" <<endl;
        cout << "Seleziona 1 -- Aggiunta di un cliente.\n";
        cout << "Seleziona 2 -- Visualizzazione clienti.\n";
        cout << "Seleziona 3 -- Modifica di un cliente.\n";
        cout << "Seleziona 4 -- Eliminazione di un cliente.\n";
        cout << "Seleziona 5 -- Ricerca di un cliente.\n";
        cout << "Seleziona 6 -- Aggiunta interazione per un cliente.\n";
        cout << "Seleziona 7 -- Visualizza interazioni per un cliente.\n";
        cout << "Seleziona 8 -- Ricerca interazione per un cliente.\n";
        cout << "Seleziona 9 -- Salvataggio dati.\n";
        cout << "Seleziona -1 - Termina esecuzione.\n";
        cout << "-------------------------------------------------------------------"  <<endl;

        cout << "Inserisci valore: ";
		getline(cin >> ws, input); // legge tutto, evita newline residuo

		// Controllo se la stringa è un numero intero valido
		if(!isInteger(input)) {
		    cout << "Valore non valido, inserisci un numero!" << endl;
		    continue; // torna all'inizio del ciclo
		}

		// Conversione sicura in int
		pulsante = stoi(input);

        switch (pulsante) {
            case 1:{
                string nome = "";
                string cognome = "";

                cout << "Hai selezionato l'aggiunta di un nuovo cliente nel CRM." << endl << endl;
                // Creazione utente
                do{
                	cout <<"Inserisci i valori di nome e cognome, non vuoti." << endl;
					cout << "Nome: "; getline(cin, nome);
                	cout << "Cognome: "; getline(cin, cognome);
				} while(isEmptyOrSpaces(nome) || isEmptyOrSpaces(cognome));

                Cliente cliente(nome, cognome);
                crm.aggiungiCliente(cliente);

                break;
            }
            case 2:{
                cout << "Hai selezionato la visualizzazione dei clienti presenti nel CRM."  << endl << endl;
                crm.visualizzaClienti();
                break;
            }
            case 3:{
                cout << "Hai selezionato la modifica di un cliente presente nel CRM."  << endl << endl;
                cout << "Ecco gli utenti attualmente presenti:" << endl;
                crm.visualizzaClienti();

                if(crm.getClienti().size()!= 0){
                    cout << "-------------------------------------------------------------------" <<endl;
                    int indice = 0;
                    string nome = "";
                    string cognome = "";

                    cout << "Inserisci l'indice del cliente che vuoi modificare: ";
                    getline(cin >> ws, input);

                    // Controllo se la stringa è un numero valido
                    if(!isInteger(input)) {
                        cout << "Valore non valido, inserisci un numero!\n";
                        continue; // torna all'inizio del ciclo
                    }

                    // Conversione sicura
                    indice = stoi(input);

                    cout <<"Ora inseriremo le nuove informazioni, lasci i campi vuoti per non modificare." << endl << endl;

                    cout <<"Inserisci valore nuovo nome: ";
                    getline(cin, nome);
                    cout <<"Inserisci valore nuovo cognome: ";
                    getline(cin, cognome);

                    Cliente clienteNew(nome, cognome);
                    crm.modificaCliente(indice, clienteNew);
                }
                break;
            }
            case 4:{
                cout << "Hai selezionato l'eliminazione di un cliente presente nel CRM."  << endl << endl;
                crm.visualizzaClienti();

                if(crm.getClienti().size() != 0){
                	int indice = 0;
	                cout << "Inserisci l'indice del cliente che vuoi eliminare: ";
	                getline(cin >> ws, input);

	                // Controllo se la stringa è un numero valido
				    if(!isInteger(input)) {
				        cout << "Valore non valido, inserisci un numero!\n";
				        continue; // torna all'inizio del ciclo
				    }

				    // Conversione sicura
				    indice = stoi(input);

	                crm.eliminaCliente(indice);
				}
                break;
            }
            case 5:{
                cout << "Hai selezionato la ricerca di un cliente presente nel CRM." << endl << endl;
                cout << "La ricerca verra' eseguita tramite nome o cognome." << endl;
                cout << "Se vuoi effettuare la ricerca tramite nome, lascia il valore di cognome vuoto e viceversa." << endl;

                string nome = "";
                string cognome = "";
                vector<Cliente> clientiTrovati;

                while(true) {
                    cout <<"Inserisci valore nuovo nome: ";
                    getline(cin, nome);

                    if (!nome.empty()){
                        clientiTrovati = crm.ricercaCliente(nome, cognome);
                        break;
                    }

                    cout <<"Inserisci valore nuovo cognome: ";
                    getline(cin, cognome);

                    if (!cognome.empty()) {
                         clientiTrovati = crm.ricercaCliente(nome, cognome);
                        break;
                    };

                    cout << "Devi inserire un valore di nome o di cognome." << endl << endl;
                }

                if(clientiTrovati.empty()) {
				    cout << "Nessun cliente trovato." << endl;
				} else {
					cout << "Clienti trovati:" << endl << endl;
				    for(const auto& c : clientiTrovati) {
				        cout << "Nome: " << c.getNome() << " "
				             << "Cognome: " << c.getCognome() << endl;
				    }
				}

                break;
            }
            case 6:{
                cout << "Hai selezionato l'aggiunta delle interazioni per un cliente presente nel CRM."  << endl;
                cout << "Ecco gli utenti attualmente presenti:" << endl;
                crm.visualizzaClienti();

                if(crm.getClienti().size() != 0){
                    cout << "-------------------------------------------------------------------" <<endl;
                	int indice = 0;
	                cout << "Inserisci l'indice del cliente a cui vuoi aggiungere una interazione: ";
	                getline(cin >> ws, input);

	                // Controllo se la stringa è un numero valido
				    if(!isInteger(input)) {
				        cout << "Valore non valido, inserisci un numero!\n";
				        continue;
				    }

				    // Conversione sicura
				    indice = stoi(input);
                    indice --;

                    if (indice >= 0 && indice < crm.getClienti().size()) {
                        string descrizione = "";
                        int tipoInterazione = 0;
                        Cliente& cliente = crm.getClienti().at(indice);

                        // Inserimento informazioni interazione
                        do {
                            cout << "Inserisci la descrizione (non vuota): ";
                            getline(cin, descrizione);
                            if (isEmptyOrSpaces(descrizione)) {
                                cout << "Descrizione non valida.\n";
                            }
                        } while (isEmptyOrSpaces(descrizione));

                        while (true) {
                            cout << "Seleziona tipo interazione: " << endl;
                            cout << "1 - Appuntamento" << endl;
                            cout << "2 - Contratto" << endl;
                            cout << "Scelta: ";
                            getline(cin, input);

                            if (!isInteger(input)) {
                                cout << "Inserisci 1 o 2." << endl;
                                continue;
                            }

                            tipoInterazione = stoi(input);
                            if (tipoInterazione != 1 && tipoInterazione != 2) {
                                cout << "Inserisci 1 o 2." << endl;
                                continue;
                            }
                            break;
                        }

                        TipoInterazione tipoScelto = (tipoInterazione == 1) ? TipoInterazione::Appuntamento : TipoInterazione::Contratto;
                        Interazione nuovaInterazione(tipoScelto, descrizione);
                        cliente.aggiungiInterazione(nuovaInterazione);
                    }
                    else{
                        cout << "ERRORE --- Valore indice non corretto." << endl;
                    }

                }
                break;
            }
            case 7:{
                cout << "Hai selezionato la visualizzazione delle interazioni per un cliente presente nel CRM."  << endl << endl;
                cout << "Ecco gli utenti attualmente presenti:" << endl;
                crm.visualizzaClienti();

                if(crm.getClienti().size() != 0){
                    cout << "-------------------------------------------------------------------" <<endl;
                	int indice = 0;
	                cout << "Inserisci l'indice del cliente a cui vuoi aggiungere una interazione: ";
	                getline(cin >> ws, input);

				    if(!isInteger(input)) {
				        cout << "Valore non valido, inserisci un numero!\n";
				        continue;
				    }

				    indice = stoi(input);
                    indice --;

                    if (indice >= 0 && indice < crm.getClienti().size()) {
                        Cliente cliente = crm.getClienti().at(indice);
                        vector<Interazione> listInterazioni = cliente.getInterazioni();

                        if (listInterazioni.empty()) {
                            cout << "Nessuna interazione presente.\n";
                        } else {
                            for (const auto& interazione : listInterazioni) {
                                interazione.stampaInterazione();
                            }
                        }
                    }
                    else{
                        cout << "ERRORE --- Valore indice non corretto." << endl;
                    }
                }
                break;
            }
            case 8:{
                cout << "Hai selezionato la ricerca delle interazioni per ogni cliente presente nel CRM."  << endl;

                string descrizione = "";
                int tipoInterazione = 0;

                if(crm.getClienti().size() != 0){
                    // Obbligatorio inserire il tipo
                    while (true) {
                        cout << "Seleziona tipo interazione: " << endl;
                        cout << "1 - Appuntamento" << endl;
                        cout << "2 - Contratto" << endl;
                        cout << "Scelta: ";
                        getline(cin, input);

                        if (!isInteger(input)) {
                            cout << "Inserisci 1 o 2." << endl;
                            continue;
                        }

                        tipoInterazione = stoi(input);
                        if (tipoInterazione != 1 && tipoInterazione != 2) {
                            cout << "Inserisci 1 o 2." << endl;
                            continue;
                        }
                        break;
                    }
                    TipoInterazione tipoScelto = (tipoInterazione == 1) ? TipoInterazione::Appuntamento : TipoInterazione::Contratto;

                    // Opzionale la descrizione
                    cout << "Inserisci la descrizione: ";
                    getline(cin, descrizione);

                    for (Cliente& cliente : crm.getClienti()) {
                        cliente.cercaInterazioniPerTipoEDescrizione(tipoToString(tipoScelto), descrizione);
                    }
                }
                else{
                    cout << "Nessun cliente registrato." << endl;
                }

                break;
            }
            case 9:{
                cout << "Hai selezionato il salvataggio dei dati dei clienti."  << endl << endl;
                vector<Cliente> clienti = crm.getClienti();
                CRMStorage::salvaTutto(clienti);
                break;
            }
            case -1:{
                cout << "Grazie per aver utilizzato il software CRM."  << endl << endl;
                break;
            }
            default:{
                cout << "Valore inserito non valido." << endl;
                cin.ignore();
                break;
            }
        }
    }

    return 0;
}
