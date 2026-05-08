#include "parser.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;

// Función auxiliar para remover espacios en blanco al inicio y final
static inline string trim(string s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) { return !isspace(ch); }));
    s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !isspace(ch); }).base(), s.end());
    return s;
}

vector<string> split(const string& line) {
    vector<string> tokens;
    string token;
    stringstream ss(line);

    while (getline(ss, token, ',')) {
        tokens.push_back(trim(token));
    }

    return tokens;
}

ParseResult parseCSV(const string& filename) {
    ifstream file(filename);
    ParseResult result;
    result.totalRegisters = 0;
    result.totalChargesNulls = 0;

    if (!file.is_open()) {
        cerr << "Error abriendo el archivo\n";
        return result;
    }

    string line;

    getline(file, line); //Saltar titulos

    while (getline(file, line)) {
        vector<string> cols = split(line);

        // Validación básica
        if (cols.size() < 21) continue;

        Solicitud s;

        s.customerID = cols[0];
        s.tenure = stoi(cols[5]);
        s.monthlyCharges = stod(cols[18]);

        if (cols[19].empty() || cols[19] == " " || s.tenure == 0) {
            s.totalCharges = 0.0;
            result.totalChargesNulls++;
        } else {
            s.totalCharges = stod(cols[19]);
        }

        string churnStr = cols[20];
	churnStr.erase(remove(churnStr.begin(), churnStr.end(), '\r'), churnStr.end());
	s.churn = (churnStr == "Yes");

        result.solicitudes.push_back(s);
        result.totalRegisters++;
    }

    file.close();
    return result;
}

void writeCSV(const string& filename, const vector<Solicitud>& solicitudes) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error creando el archivo\n";
        return;
    }

    file << "customerID,tenure,MonthlyCharges,TotalCharges,Churn\n";
    for (const auto& s : solicitudes) {
        file << s.customerID << ","
             << s.tenure << ","
             << s.monthlyCharges << ","
             << s.totalCharges << ","
             << (s.churn ? "Yes" : "No")
             << "\n";
    }

    file.close();
}