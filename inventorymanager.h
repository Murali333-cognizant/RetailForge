#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include "product.h"
#include "supplier.h"
#include "salestransaction.h"
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <fstream>

class InventoryManager {
private:
    std::map<std::string, Product*> products;
    std::map<std::string, Supplier*> suppliers;
    std::vector<SalesTransaction> salesHistory;
    std::queue<std::string> pendingAlerts;
    std::set<std::string> categories;
    
    // File paths
    std::string productsFile;
    std::string suppliersFile;
    std::string salesLogFile;
    std::string transactionsFile;
    std::string forecastFile;
    std::string backupFile;
    
    // Private helpers
    std::string determineProductType(const std::string& line);
    void loadSalesData();
    void saveSalesData();
    void updateCategories();

public:
    InventoryManager(const std::string& dataDir = ".");
    ~InventoryManager();
    
    // Load and Save operations
    void loadAllData();
    void saveAllData();
    void backupSystem();
    void restoreSystem();
    
    // Product operations
    void addProduct(Product* product);
    void updateProduct(const std::string& productID, Product* newProduct);
    void deleteProduct(const std::string& productID);
    Product* findProduct(const std::string& productID) const;
    Product* findProductByName(const std::string& name) const;
    
    // Supplier operations
    void addSupplier(Supplier* supplier);
    void updateSupplier(const std::string& supplierID, Supplier* newSupplier);
    void deleteSupplier(const std::string& supplierID);
    Supplier* findSupplier(const std::string& supplierID) const;
    
    // Sales operations
    void recordSale(const std::string& productID, int quantity);
    void recordPurchase(const std::string& productID, int quantity);
    
    // Reporting operations
    std::vector<Product*> getLowStockItems() const;
    std::vector<Product*> getNearExpiryItems() const;
    std::vector<Product*> getTopSellingProducts(int n) const;
    std::vector<Product*> getSlowMovingProducts(int daysSinceLastSale) const;
    std::map<std::string, double> getCategoryProfitLoss() const;
    
    // Search and filter operations
    std::vector<Product*> searchProductsByCategory(const std::string& category) const;
    std::vector<Product*> filterProductsByStock(int minStock, int maxStock) const;
    std::vector<Product*> filterProductsByPrice(double minPrice, double maxPrice) const;
    
    // Forecasting operations
    double calculateSimpleMovingAverage(const std::string& productID, int days) const;
    double calculateLinearTrend(const std::string& productID) const;
    int predictNextDemand(const std::string& productID, int daysAhead) const;
    int suggestReorderQuantity(const std::string& productID) const;
    
    // Alert operations
    void generateAlerts();
    void displayAlerts();
    std::string getNextAlert();
    int getPendingAlertCount() const;
    
    // Supplier performance
    void generateSupplierPerformanceReport() const;
    
    // Data retrieval
    int getTotalProducts() const;
    int getTotalSuppliers() const;
    int getTotalSalesTransactions() const;
    double getTotalRevenue() const;
    double calculateTotalInventoryValue() const;
    
    // Display operations
    void displayAllProducts() const;
    void displayAllSuppliers() const;
    void displaySalesHistory(int limit = 20) const;
    
    // Utility
    void clearAllData();
    // Remove expired perishable products from inventory and disk
    void removeExpiredProducts();
};

#endif // INVENTORYMANAGER_H
