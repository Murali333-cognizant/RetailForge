#include "supplier.h"
#include <iostream>
#include <cstdlib>

Supplier::Supplier()
    : supplierID(""), name(""), contact(""), leadTimeDays(0), reliabilityScore(0.0) {
}

Supplier::Supplier(const std::string& id, const std::string& n, const std::string& cont,
                   int leadTime, double reliability)
    : supplierID(id), name(n), contact(cont), leadTimeDays(leadTime), 
      reliabilityScore(reliability) {
}

Supplier::~Supplier() {
}

std::string Supplier::getSupplierID() const { return supplierID; }
std::string Supplier::getName() const { return name; }
std::string Supplier::getContact() const { return contact; }
int Supplier::getLeadTimeDays() const { return leadTimeDays; }
double Supplier::getReliabilityScore() const { return reliabilityScore; }

void Supplier::setName(const std::string& n) { name = n; }
void Supplier::setContact(const std::string& cont) { contact = cont; }
void Supplier::setLeadTimeDays(int days) { leadTimeDays = days; }
void Supplier::setReliabilityScore(double score) { 
    if (score >= 0.0 && score <= 1.0) reliabilityScore = score;
}

std::string Supplier::toCSVString() const {
    std::ostringstream oss;
    oss << supplierID << "," << name << "," << contact << ","
        << leadTimeDays << "," << reliabilityScore;
    return oss.str();
}

void Supplier::loadFromCSVString(const std::string& line) {
    std::istringstream iss(line);
    std::string field;
    
    std::getline(iss, supplierID, ',');
    std::getline(iss, name, ',');
    std::getline(iss, contact, ',');
    
    std::getline(iss, field, ',');
    leadTimeDays = atoi(field.c_str());
    
    std::getline(iss, field, ',');
    reliabilityScore = atof(field.c_str());
}

void Supplier::displayInfo() const {
    std::cout << "\n--- Supplier Information ---\n";
    std::cout << "Supplier ID: " << supplierID << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Contact: " << contact << "\n";
    std::cout << "Lead Time: " << leadTimeDays << " days\n";
    std::cout << "Reliability Score: " << reliabilityScore << "\n";
}
