#ifndef SUPPLIER_H
#define SUPPLIER_H

#include <string>
#include <fstream>
#include <sstream>

class Supplier {
private:
    std::string supplierID;
    std::string name;
    std::string contact;
    int leadTimeDays;
    double reliabilityScore;  // 0.0 to 1.0

public:
    Supplier();
    Supplier(const std::string& id, const std::string& n, const std::string& cont,
             int leadTime, double reliability);
    
    ~Supplier();
    
    // Getters
    std::string getSupplierID() const;
    std::string getName() const;
    std::string getContact() const;
    int getLeadTimeDays() const;
    double getReliabilityScore() const;
    
    // Setters
    void setName(const std::string& n);
    void setContact(const std::string& cont);
    void setLeadTimeDays(int days);
    void setReliabilityScore(double score);
    
    // File operations
    std::string toCSVString() const;
    void loadFromCSVString(const std::string& line);
    
    // Display
    void displayInfo() const;
};

#endif // SUPPLIER_H
