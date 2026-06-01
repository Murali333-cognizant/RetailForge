#include "salestransaction.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

SalesTransaction::SalesTransaction()
    : transactionID(""), productID(""), quantity(0), salePrice(0.0), costPrice(0.0),
      totalAmount(0.0), transactionDate(""), transactionType("Sale") {
}

SalesTransaction::SalesTransaction(const std::string& transID, const std::string& prodID,
                                   int qty, double price, const std::string& date,
                                   const std::string& type)
    : transactionID(transID), productID(prodID), quantity(qty), salePrice(price), costPrice(0.0),
      transactionDate(date), transactionType(type) {
    calculateTotalAmount();
}

SalesTransaction::~SalesTransaction() {
}

std::string SalesTransaction::getTransactionID() const { return transactionID; }
std::string SalesTransaction::getProductID() const { return productID; }
int SalesTransaction::getQuantity() const { return quantity; }
double SalesTransaction::getSalePrice() const { return salePrice; }
double SalesTransaction::getCostPrice() const { return costPrice; }
double SalesTransaction::getTotalAmount() const { return totalAmount; }
std::string SalesTransaction::getTransactionDate() const { return transactionDate; }
std::string SalesTransaction::getTransactionType() const { return transactionType; }

void SalesTransaction::setQuantity(int qty) { 
    quantity = qty;
    calculateTotalAmount();
}

void SalesTransaction::setSalePrice(double price) { 
    salePrice = price;
    calculateTotalAmount();
}

void SalesTransaction::setCostPrice(double price) { 
    costPrice = price;
}

double SalesTransaction::calculateProfit() const {
    // Profit = (Selling Price - Cost Price) * Quantity
    double profit = (salePrice - costPrice) * quantity;
    return profit;
}

void SalesTransaction::calculateTotalAmount() {
    // Total Amount = Selling Price * Quantity (no discount)
    totalAmount = salePrice * quantity;
}

std::string SalesTransaction::toCSVString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << transactionID << "," << productID << "," << quantity << ","
        << costPrice << "," << salePrice << "," << totalAmount << "," 
        << transactionDate << "," << transactionType;
    return oss.str();
}

void SalesTransaction::loadFromCSVString(const std::string& line) {
    std::istringstream iss(line);
    std::string field;
    
    std::getline(iss, transactionID, ',');
    std::getline(iss, productID, ',');
    
    std::getline(iss, field, ',');
    quantity = atoi(field.c_str());
    
    std::getline(iss, field, ',');
    costPrice = atof(field.c_str());
    
    std::getline(iss, field, ',');
    salePrice = atof(field.c_str());
    
    std::getline(iss, field, ',');
    totalAmount = atof(field.c_str());
    
    std::getline(iss, transactionDate, ',');
    std::getline(iss, transactionType, ',');
}

void SalesTransaction::displayInfo() const {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n--- Transaction Information ---\n";
    std::cout << "Transaction ID: " << transactionID << "\n";
    std::cout << "Product ID: " << productID << "\n";
    std::cout << "Quantity: " << quantity << "\n";
    std::cout << "Cost Price per unit: Rs. " << costPrice << "\n";
    std::cout << "Sale Price per unit: Rs. " << salePrice << "\n";
    std::cout << "Total Amount: Rs. " << totalAmount << "\n";
    std::cout << "Profit: Rs. " << calculateProfit() << "\n";
    std::cout << "Date: " << transactionDate << "\n";
    std::cout << "Type: " << transactionType << "\n";
}

std::string SalesTransaction::generateTransactionID() {
    static int counter = 1;
    std::ostringstream oss;
    oss << "T" << counter++;
    return oss.str();
}

std::string SalesTransaction::getCurrentDate() {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    
    char buffer[11];  // "DD-MM-YYYY\0"
    strftime(buffer, sizeof(buffer), "%d-%m-%Y", timeinfo);
    return std::string(buffer);
}
