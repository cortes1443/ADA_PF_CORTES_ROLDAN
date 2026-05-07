#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>

struct Solicitud {
    std::string customerID;
    int tenure;
    double monthlyCharges;
    double totalCharges;
    bool churn; // true = Yes, false = No
};

struct ParseResult {
    std::vector<Solicitud> solicitudes;
    int totalRegisters;
    int totalChargesNulls;
};

ParseResult parseCSV(const std::string& filename);

void writeCSV(const std::string& filename, const std::vector<Solicitud>& solicitudes);

#endif