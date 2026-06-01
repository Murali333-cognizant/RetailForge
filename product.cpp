#include "product.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

// ==================== BASE PRODUCT CLASS ====================

Product::Product()
    : productID(""), name(""), category(""), supplierID(""),
      currentStock(0), reorderLevel(0), costPrice(0.0), unitPrice(0.0), totalSold(0) {
}

Product::Product(const std::string& id, const std::string& n, const std::string& cat,
                 const std::string& supID, int stock, int reorder, double cost, double price, int sold)
    : productID(id), name(n), category(cat), supplierID(supID),
      currentStock(stock), reorderLevel(reorder), costPrice(cost), unitPrice(price), totalSold(sold) {
}

Product::~Product() {
}

std::string Product::getProductID() const { return productID; }
std::string Product::getName() const { return name; }
std::string Product::getCategory() const { return category; }
std::string Product::getSupplierID() const { return supplierID; }
int Product::getCurrentStock() const { return currentStock; }
int Product::getReorderLevel() const { return reorderLevel; }
double Product::getCostPrice() const { return costPrice; }
double Product::getUnitPrice() const { return unitPrice; }
int Product::getTotalSold() const { return totalSold; }

void Product::setCurrentStock(int qty) { currentStock = qty; }
void Product::setName(const std::string& n) { name = n; }
void Product::setCategory(const std::string& cat) { category = cat; }
void Product::setReorderLevel(int level) { reorderLevel = level; }
void Product::setCostPrice(double price) { costPrice = price; }
void Product::setUnitPrice(double price) { unitPrice = price; }
void Product::setTotalSold(int sold) { totalSold = sold; }

void Product::updateStock(int qty) {
    currentStock += qty;
    if (currentStock < 0) currentStock = 0;
}

bool Product::isLowStock() const {
    return currentStock <= reorderLevel;
}

int Product::calculateReorderQty() const {
    if (totalSold == 0) return reorderLevel * 2;
    // Standard reorder quantity: 30 days worth of average sales
    int avgDailySales = (totalSold > 30) ? (totalSold / 30) : 1;
    return avgDailySales * 30;
}

bool Product::isNearExpiry() const {
    return false;  // Override in derived classes
}

std::string Product::getType() const {
    return "Standard";
}

void Product::displayInfo() const {
    std::cout << "\n--- Product Information ---\n";
    std::cout << "ID: " << productID << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Category: " << category << "\n";
    std::cout << "Supplier ID: " << supplierID << "\n";
    std::cout << "Current Stock: " << currentStock << "\n";
    std::cout << "Reorder Level: " << reorderLevel << "\n";
    std::cout << "Cost Price: Rs. " << costPrice << "\n";
    std::cout << "Selling Price: Rs. " << unitPrice << "\n";
    std::cout << "Total Sold: " << totalSold << "\n";
    std::cout << "Type: " << getType() << "\n";
}

std::string Product::toCSVString() const {
    std::ostringstream oss;
    oss << productID << "," << name << "," << category << "," << supplierID << ","
        << currentStock << "," << reorderLevel << "," << costPrice << "," 
        << unitPrice << ",,," << "0,0," << totalSold;
    return oss.str();
}

void Product::loadFromCSVString(const std::string& line) {
    std::istringstream iss(line);
    std::string field;
    
    std::getline(iss, productID, ',');
    std::getline(iss, name, ',');
    std::getline(iss, category, ',');
    std::getline(iss, supplierID, ',');
    
    std::getline(iss, field, ',');
    currentStock = atoi(field.c_str());
    std::getline(iss, field, ',');
    reorderLevel = atoi(field.c_str());
    std::getline(iss, field, ',');
    costPrice = atof(field.c_str());
    std::getline(iss, field, ',');
    unitPrice = atof(field.c_str());
    
    // Skip remaining fields
    for (int i = 0; i < 4; ++i) {
        std::getline(iss, field, ',');
    }
    
    std::getline(iss, field, ',');
    totalSold = atoi(field.c_str());
}

void Product::saveToFile(std::ofstream& file) const {
    file << toCSVString() << "\n";
}

void Product::loadFromFile(std::ifstream& file) {
    std::string line;
    if (std::getline(file, line)) {
        loadFromCSVString(line);
    }
}

// ==================== PERISHABLE PRODUCT CLASS ====================

PerishableProduct::PerishableProduct()
    : Product(), expiryDate(""), shelfLifeDays(0) {
}

PerishableProduct::PerishableProduct(const std::string& id, const std::string& n, 
                                     const std::string& cat, const std::string& supID, 
                                     int stock, int reorder, double cost, double price, int sold,
                                     const std::string& expiry, int shelfDays)
    : Product(id, n, cat, supID, stock, reorder, cost, price, sold),
      expiryDate(expiry), shelfLifeDays(shelfDays) {
}

PerishableProduct::~PerishableProduct() {
}

std::string PerishableProduct::getExpiryDate() const { return expiryDate; }
int PerishableProduct::getShelfLifeDays() const { return shelfLifeDays; }

void PerishableProduct::setExpiryDate(const std::string& date) { expiryDate = date; }
void PerishableProduct::setShelfLifeDays(int days) { shelfLifeDays = days; }

int PerishableProduct::calculateReorderQty() const {
    int baseQty = Product::calculateReorderQty();
    // For perishables, reduce by 20% due to expiry risk
    return (baseQty * 80) / 100;
}

bool PerishableProduct::isNearExpiry() const {
    return daysUntilExpiry() <= 3 && daysUntilExpiry() >= 0;
}

std::string PerishableProduct::getType() const {
    return "Perishable";
}

void PerishableProduct::displayInfo() const {
    std::cout << "\n--- PERISHABLE Product Information ---\n";
    std::cout << "ID: " << productID << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Category: " << category << "\n";
    std::cout << "Supplier ID: " << supplierID << "\n";
    std::cout << "Current Stock: " << currentStock << "\n";
    std::cout << "Reorder Level: " << reorderLevel << "\n";
    std::cout << "Unit Price: Rs. " << unitPrice << "\n";
    std::cout << "Total Sold: " << totalSold << "\n";
    std::cout << "Expiry Date: " << expiryDate << "\n";
    std::cout << "Shelf Life: " << shelfLifeDays << " days\n";
    if (isNearExpiry()) {
        std::cout << "*** WARNING: Product near expiry! ***\n";
    }
}

std::string PerishableProduct::toCSVString() const {
    std::ostringstream oss;
    oss << productID << "," << name << "," << category << "," << supplierID << ","
        << currentStock << "," << reorderLevel << "," << costPrice << "," 
        << unitPrice << "," << expiryDate << "," << shelfLifeDays << ",0,0," << totalSold;
    return oss.str();
}

void PerishableProduct::loadFromCSVString(const std::string& line) {
    std::istringstream iss(line);
    std::string field;
    
    std::getline(iss, productID, ',');
    std::getline(iss, name, ',');
    std::getline(iss, category, ',');
    std::getline(iss, supplierID, ',');
    
    std::getline(iss, field, ',');
    currentStock = atoi(field.c_str());
    std::getline(iss, field, ',');
    reorderLevel = atoi(field.c_str());
    std::getline(iss, field, ',');
    costPrice = atof(field.c_str());
    std::getline(iss, field, ',');
    unitPrice = atof(field.c_str());
    std::getline(iss, expiryDate, ',');
    
    std::getline(iss, field, ',');
    shelfLifeDays = atoi(field.c_str());
    
    // Skip batchNumber and warranty
    std::getline(iss, field, ',');
    std::getline(iss, field, ',');
    
    std::getline(iss, field, ',');
    totalSold = atoi(field.c_str());
}

bool PerishableProduct::isExpired() const {
    return daysUntilExpiry() < 0;
}

int PerishableProduct::daysUntilExpiry() const {
    // Simple date comparison (DD-MM-YYYY format)
    // For production, use proper date library
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    
    int currentYear = timeinfo->tm_year + 1900;
    int currentMonth = timeinfo->tm_mon + 1;
    int currentDay = timeinfo->tm_mday;
    
    if (expiryDate.empty() || expiryDate.size() < 10) return 999;

    int expDay = atoi(expiryDate.substr(0, 2).c_str());
    int expMonth = atoi(expiryDate.substr(3, 2).c_str());
    int expYear = atoi(expiryDate.substr(6, 4).c_str());
    
    int daysDiff = 0;
    if (expYear > currentYear) {
        daysDiff = (expYear - currentYear) * 365 + (expMonth - currentMonth) * 30 + (expDay - currentDay);
    } else if (expYear < currentYear) {
        daysDiff = -((currentYear - expYear) * 365 + (currentMonth - expMonth) * 30 + (currentDay - expDay));
    } else {
        daysDiff = (expMonth - currentMonth) * 30 + (expDay - currentDay);
    }
    
    return daysDiff;
}

// ==================== NON-PERISHABLE PRODUCT CLASS ====================

NonPerishableProduct::NonPerishableProduct()
    : Product(), batchNumber(""), warrantyMonths(0) {
}

NonPerishableProduct::NonPerishableProduct(const std::string& id, const std::string& n,
                                           const std::string& cat, const std::string& supID,
                                           int stock, int reorder, double cost, double price, int sold,
                                           const std::string& batch, int warranty)
    : Product(id, n, cat, supID, stock, reorder, cost, price, sold),
      batchNumber(batch), warrantyMonths(warranty) {
}

NonPerishableProduct::~NonPerishableProduct() {
}

std::string NonPerishableProduct::getBatchNumber() const { return batchNumber; }
int NonPerishableProduct::getWarrantyMonths() const { return warrantyMonths; }

void NonPerishableProduct::setBatchNumber(const std::string& batch) { batchNumber = batch; }
void NonPerishableProduct::setWarrantyMonths(int months) { warrantyMonths = months; }

int NonPerishableProduct::calculateReorderQty() const {
    int baseQty = Product::calculateReorderQty();
    // For non-perishables, increase by 10% for buffer stock
    return (baseQty * 110) / 100;
}

bool NonPerishableProduct::isNearExpiry() const {
    return false;  // Non-perishables don't expire
}

std::string NonPerishableProduct::getType() const {
    return "Non-Perishable";
}

void NonPerishableProduct::displayInfo() const {
    std::cout << "\n--- NON-PERISHABLE Product Information ---\n";
    std::cout << "ID: " << productID << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Category: " << category << "\n";
    std::cout << "Supplier ID: " << supplierID << "\n";
    std::cout << "Current Stock: " << currentStock << "\n";
    std::cout << "Reorder Level: " << reorderLevel << "\n";
    std::cout << "Unit Price: Rs. " << unitPrice << "\n";
    std::cout << "Total Sold: " << totalSold << "\n";
    std::cout << "Batch Number: " << batchNumber << "\n";
    std::cout << "Warranty: " << warrantyMonths << " months\n";
}

std::string NonPerishableProduct::toCSVString() const {
    std::ostringstream oss;
    oss << productID << "," << name << "," << category << "," << supplierID << ","
        << currentStock << "," << reorderLevel << "," << costPrice << "," 
        << unitPrice << ",,," << batchNumber << "," << warrantyMonths << "," << totalSold;
    return oss.str();
}

void NonPerishableProduct::loadFromCSVString(const std::string& line) {
    std::istringstream iss(line);
    std::string field;
    
    std::getline(iss, productID, ',');
    std::getline(iss, name, ',');
    std::getline(iss, category, ',');
    std::getline(iss, supplierID, ',');
    
    std::getline(iss, field, ',');
    currentStock = atoi(field.c_str());
    std::getline(iss, field, ',');
    reorderLevel = atoi(field.c_str());
    std::getline(iss, field, ',');
    costPrice = atof(field.c_str());
    std::getline(iss, field, ',');
    unitPrice = atof(field.c_str());
    
    // Skip expiry and shelf life
    std::getline(iss, field, ',');
    std::getline(iss, field, ',');
    
    std::getline(iss, batchNumber, ',');
    
    std::getline(iss, field, ',');
    warrantyMonths = atoi(field.c_str());
    
    std::getline(iss, field, ',');
    totalSold = atoi(field.c_str());
}
