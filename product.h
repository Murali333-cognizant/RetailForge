#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

// Abstract base class for all products
class Product {
protected:
    std::string productID;
    std::string name;
    std::string category;
    std::string supplierID;
    int currentStock;
    int reorderLevel;
    double costPrice;        // Cost price (buying price)
    double unitPrice;        // Selling price
    int totalSold;

public:
    Product();
    Product(const std::string& id, const std::string& n, const std::string& cat,
            const std::string& supID, int stock, int reorder, double cost, double price, int sold);
    
    virtual ~Product();
    
    // Getters
    std::string getProductID() const;
    std::string getName() const;
    std::string getCategory() const;
    std::string getSupplierID() const;
    int getCurrentStock() const;
    int getReorderLevel() const;
    double getCostPrice() const;
    double getUnitPrice() const;
    int getTotalSold() const;
    
    // Setters
    void setCurrentStock(int qty);
    void setName(const std::string& n);
    void setCategory(const std::string& cat);
    void setReorderLevel(int level);
    void setCostPrice(double price);
    void setUnitPrice(double price);
    void setTotalSold(int sold);
    
    // Core operations
    void updateStock(int qty);  // Positive or negative
    bool isLowStock() const;
    
    // Virtual methods
    virtual int calculateReorderQty() const;
    virtual bool isNearExpiry() const;
    virtual std::string getType() const;
    virtual void displayInfo() const;
    virtual std::string toCSVString() const;
    virtual void loadFromCSVString(const std::string& line);
    
    // File operations
    virtual void saveToFile(std::ofstream& file) const;
    virtual void loadFromFile(std::ifstream& file);
};

// Derived class for perishable products
class PerishableProduct : public Product {
private:
    std::string expiryDate;  // Format: DD-MM-YYYY
    int shelfLifeDays;

public:
    PerishableProduct();
    PerishableProduct(const std::string& id, const std::string& n, const std::string& cat,
                      const std::string& supID, int stock, int reorder, double cost, double price, int sold,
                      const std::string& expiry, int shelfDays);
    
    virtual ~PerishableProduct();
    
    // Getters
    std::string getExpiryDate() const;
    int getShelfLifeDays() const;
    
    // Setters
    void setExpiryDate(const std::string& date);
    void setShelfLifeDays(int days);
    
    // Override virtual methods
    virtual int calculateReorderQty() const;
    virtual bool isNearExpiry() const;
    virtual std::string getType() const;
    virtual void displayInfo() const;
    virtual std::string toCSVString() const;
    virtual void loadFromCSVString(const std::string& line);
    
    // Utility
    bool isExpired() const;
    int daysUntilExpiry() const;
};

// Derived class for non-perishable products
class NonPerishableProduct : public Product {
private:
    std::string batchNumber;
    int warrantyMonths;

public:
    NonPerishableProduct();
    NonPerishableProduct(const std::string& id, const std::string& n, const std::string& cat,
                         const std::string& supID, int stock, int reorder, double cost, double price, int sold,
                         const std::string& batch, int warranty);
    
    virtual ~NonPerishableProduct();
    
    // Getters
    std::string getBatchNumber() const;
    int getWarrantyMonths() const;
    
    // Setters
    void setBatchNumber(const std::string& batch);
    void setWarrantyMonths(int months);
    
    // Override virtual methods
    virtual int calculateReorderQty() const;
    virtual bool isNearExpiry() const;
    virtual std::string getType() const;
    virtual void displayInfo() const;
    virtual std::string toCSVString() const;
    virtual void loadFromCSVString(const std::string& line);
};

#endif // PRODUCT_H
