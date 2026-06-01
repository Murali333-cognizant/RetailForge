#ifndef SALESTRANSACTION_H
#define SALESTRANSACTION_H

#include <string>
#include <fstream>
#include <sstream>
#include <ctime>

class SalesTransaction {
private:
    std::string transactionID;
    std::string productID;
    int quantity;
    double salePrice;        // Per unit (selling price)
    double costPrice;        // Per unit (cost price)
    double totalAmount;
    std::string transactionDate;  // DD-MM-YYYY
    std::string transactionType;  // "Sale" or "Purchase"

public:
    SalesTransaction();
    SalesTransaction(const std::string& transID, const std::string& prodID,
                     int qty, double price, const std::string& date,
                     const std::string& type);
    
    ~SalesTransaction();
    
    // Getters
    std::string getTransactionID() const;
    std::string getProductID() const;
    int getQuantity() const;
    double getSalePrice() const;
    double getCostPrice() const;
    double getTotalAmount() const;
    std::string getTransactionDate() const;
    std::string getTransactionType() const;
    
    // Setters
    void setQuantity(int qty);
    void setSalePrice(double price);
    void setCostPrice(double price);
    
    // Calculations
    double calculateProfit() const;  // Profit = (salePrice - costPrice) * quantity
    void calculateTotalAmount();     // Total = salePrice * quantity
    
    // File operations
    std::string toCSVString() const;
    void loadFromCSVString(const std::string& line);
    
    // Display
    void displayInfo() const;
    
    // Utility
    static std::string generateTransactionID();
    static std::string getCurrentDate();
};

#endif // SALESTRANSACTION_H
