#include "inventorymanager.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>

static std::string trimString(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

static bool isHeaderLine(const std::string& line, const std::string& expectedHeader) {
    return trimString(line) == expectedHeader;
}

InventoryManager::InventoryManager(const std::string& dataDir)
    : productsFile(dataDir + "/products.csv"),
      suppliersFile(dataDir + "/suppliers.csv"),
      salesLogFile(dataDir + "/sales.log"),
      transactionsFile(dataDir + "/transactions.csv"),
      forecastFile(dataDir + "/forecast.csv"),
      backupFile(dataDir + "/backup.dat") {
    
    std::cout << "InventoryManager initialized with data directory: " << dataDir << "\n";
}

InventoryManager::~InventoryManager() {
    clearAllData();
}

void InventoryManager::clearAllData() {
    for (std::map<std::string, Product*>::iterator it = products.begin();
         it != products.end(); ++it) {
        delete it->second;
    }
    products.clear();
    
    for (std::map<std::string, Supplier*>::iterator it = suppliers.begin();
         it != suppliers.end(); ++it) {
        delete it->second;
    }
    suppliers.clear();
    
    salesHistory.clear();
    while (!pendingAlerts.empty()) {
        pendingAlerts.pop();
    }
    categories.clear();
}

void InventoryManager::loadAllData() {
    std::cout << "\nLoading inventory data...\n";
    
    // Load suppliers first
    std::ifstream suppFile(suppliersFile.c_str());
    if (suppFile.is_open()) {
        std::string line;
        bool firstLine = true;
        while (std::getline(suppFile, line)) {
            if (firstLine) {
                firstLine = false;
                if (isHeaderLine(line, "supplierID,name,contact,leadTimeDays,reliabilityScore")) {
                    continue;
                }
            }
            if (!line.empty()) {
                Supplier* supp = new Supplier();
                supp->loadFromCSVString(line);
                suppliers[supp->getSupplierID()] = supp;
            }
        }
        suppFile.close();
        std::cout << "Loaded " << suppliers.size() << " suppliers.\n";
    } else {
        std::cout << "Note: Suppliers file not found. Starting with empty suppliers.\n";
    }
    
    // Load products
    std::ifstream prodFile(productsFile.c_str());
    if (prodFile.is_open()) {
        std::string line;
        bool firstLine = true;
        while (std::getline(prodFile, line)) {
            if (firstLine) {
                firstLine = false;
                if (isHeaderLine(line, "productID,name,category,supplierID,currentStock,reorderLevel,unitPrice,expiryDate,shelfLifeDays,batchNumber,warrantyMonths,totalSold")) {
                    continue;
                }
            }
            if (!line.empty()) {
                Product* prod = NULL;
                
                // Determine product type by counting commas and checking content
                int commaCount = 0;
                for (size_t i = 0; i < line.length(); ++i) {
                    if (line[i] == ',') commaCount++;
                }
                
                // Check if it's perishable (has expiryDate)
                std::istringstream iss(line);
                std::string field;
                int fieldCount = 0;
                std::vector<std::string> fields;
                while (std::getline(iss, field, ',')) {
                    fields.push_back(field);
                    fieldCount++;
                }
                
                // If field[7] (expiryDate) has content, it's perishable
                if (fieldCount > 7 && !fields[7].empty()) {
                    prod = new PerishableProduct();
                } else if (fieldCount > 9 && !fields[9].empty()) {
                    // If field[9] (batchNumber) has content, it's non-perishable
                    prod = new NonPerishableProduct();
                } else {
                    prod = new Product();
                }
                
                prod->loadFromCSVString(line);
                products[prod->getProductID()] = prod;
                categories.insert(prod->getCategory());
            }
        }
        prodFile.close();
        std::cout << "Loaded " << products.size() << " products.\n";
            // Remove expired perishable products right after loading
            removeExpiredProducts();
    } else {
        std::cout << "Note: Products file not found. Starting with empty inventory.\n";
    }
    
    // Load sales history
    loadSalesData();
}

void InventoryManager::loadSalesData() {
    std::ifstream salesFile(salesLogFile.c_str());
    if (salesFile.is_open()) {
        std::string line;
        bool firstLine = true;
        while (std::getline(salesFile, line)) {
            if (firstLine) {
                firstLine = false;
                if (isHeaderLine(line, "transactionID,productID,quantity,salePrice,totalAmount,date,type")) {
                    continue;
                }
            }
            if (!line.empty()) {
                SalesTransaction trans;
                trans.loadFromCSVString(line);
                salesHistory.push_back(trans);
            }
        }
        salesFile.close();
        std::cout << "Loaded " << salesHistory.size() << " sales transactions.\n";
    }
}

void InventoryManager::saveAllData() {
    std::cout << "\nSaving inventory data...\n";
    
    // Save products
    std::ofstream prodFile(productsFile.c_str());
    if (prodFile.is_open()) {
        prodFile << "productID,name,category,supplierID,currentStock,reorderLevel,unitPrice,expiryDate,shelfLifeDays,batchNumber,warrantyMonths,totalSold\n";
        for (std::map<std::string, Product*>::iterator it = products.begin();
             it != products.end(); ++it) {
            prodFile << it->second->toCSVString() << "\n";
        }
        prodFile.close();
        std::cout << "Saved " << products.size() << " products.\n";
    }
    
    // Save suppliers
    std::ofstream suppFile(suppliersFile.c_str());
    if (suppFile.is_open()) {
        suppFile << "supplierID,name,contact,leadTimeDays,reliabilityScore\n";
        for (std::map<std::string, Supplier*>::iterator it = suppliers.begin();
             it != suppliers.end(); ++it) {
            suppFile << it->second->toCSVString() << "\n";
        }
        suppFile.close();
        std::cout << "Saved " << suppliers.size() << " suppliers.\n";
    }
    
    // Save sales history
    saveSalesData();
}

void InventoryManager::saveSalesData() {
    std::ofstream salesFile(salesLogFile.c_str());
    if (salesFile.is_open()) {
        for (size_t i = 0; i < salesHistory.size(); ++i) {
            salesFile << salesHistory[i].toCSVString() << "\n";
        }
        salesFile.close();
    }
}

void InventoryManager::backupSystem() {
    std::cout << "\nCreating system backup...\n";
    std::string tempFile = backupFile + ".tmp";
    
    // Create backup by reading current files
    std::ifstream prodFile(productsFile.c_str());
    std::ofstream backupProd(tempFile.c_str());
    
    if (prodFile.is_open() && backupProd.is_open()) {
        backupProd << prodFile.rdbuf();
        prodFile.close();
        backupProd.close();
        
        // Rename temp file to actual backup
        remove(backupFile.c_str());
        rename(tempFile.c_str(), backupFile.c_str());
        std::cout << "Backup created successfully at: " << backupFile << "\n";
    } else {
        std::cout << "Error creating backup.\n";
    }
}

void InventoryManager::restoreSystem() {
    std::cout << "\nRestoring from backup...\n";
    
    std::ifstream backupFile_in(backupFile.c_str());
    if (!backupFile_in.is_open()) {
        std::cout << "No backup file found.\n";
        return;
    }
    
    std::ofstream prodFile(productsFile.c_str());
    if (prodFile.is_open()) {
        prodFile << backupFile_in.rdbuf();
        prodFile.close();
        backupFile_in.close();
        
        clearAllData();
        loadAllData();
        std::cout << "System restored successfully.\n";
    }
}

void InventoryManager::addProduct(Product* product) {
    if (product != NULL) {
        products[product->getProductID()] = product;
        categories.insert(product->getCategory());
        saveAllData();
        std::cout << "Product added: " << product->getProductID() << "\n";
    }
}

void InventoryManager::updateProduct(const std::string& productID, Product* newProduct) {
    std::map<std::string, Product*>::iterator it = products.find(productID);
    if (it != products.end()) {
        delete it->second;
        products[productID] = newProduct;
        categories.insert(newProduct->getCategory());
        saveAllData();
        std::cout << "Product updated: " << productID << "\n";
    } else {
        std::cout << "Product not found: " << productID << "\n";
    }
}

void InventoryManager::deleteProduct(const std::string& productID) {
    std::map<std::string, Product*>::iterator it = products.find(productID);
    if (it != products.end()) {
        delete it->second;
        products.erase(it);
        saveAllData();
        std::cout << "Product deleted: " << productID << "\n";
    } else {
        std::cout << "Product not found: " << productID << "\n";
    }
}

void InventoryManager::removeExpiredProducts() {
    std::vector<std::string> toRemove;
    for (std::map<std::string, Product*>::const_iterator it = products.begin();
         it != products.end(); ++it) {
        PerishableProduct* pp = dynamic_cast<PerishableProduct*>(it->second);
        if (pp != NULL) {
            if (pp->isExpired()) {
                toRemove.push_back(it->first);
            }
        }
    }

    if (toRemove.empty()) return;

    for (size_t i = 0; i < toRemove.size(); ++i) {
        std::map<std::string, Product*>::iterator it = products.find(toRemove[i]);
        if (it != products.end()) {
            std::cout << "Removing expired product: " << it->first << "\n";
            delete it->second;
            products.erase(it);
        }
    }

    // Persist changes to products.csv after removals
    saveAllData();
}

Product* InventoryManager::findProduct(const std::string& productID) const {
    std::map<std::string, Product*>::const_iterator it = products.find(productID);
    if (it != products.end()) {
        return it->second;
    }
    return NULL;
}

Product* InventoryManager::findProductByName(const std::string& name) const {
    for (std::map<std::string, Product*>::const_iterator it = products.begin();
         it != products.end(); ++it) {
        if (it->second->getName() == name) {
            return it->second;
        }
    }
    return NULL;
}

void InventoryManager::addSupplier(Supplier* supplier) {
    if (supplier != NULL) {
        suppliers[supplier->getSupplierID()] = supplier;
        saveAllData();
        std::cout << "Supplier added: " << supplier->getSupplierID() << "\n";
    }
}

void InventoryManager::updateSupplier(const std::string& supplierID, Supplier* newSupplier) {
    std::map<std::string, Supplier*>::iterator it = suppliers.find(supplierID);
    if (it != suppliers.end()) {
        delete it->second;
        suppliers[supplierID] = newSupplier;
        saveAllData();
        std::cout << "Supplier updated: " << supplierID << "\n";
    } else {
        std::cout << "Supplier not found: " << supplierID << "\n";
    }
}

void InventoryManager::deleteSupplier(const std::string& supplierID) {
    std::map<std::string, Supplier*>::iterator it = suppliers.find(supplierID);
    if (it != suppliers.end()) {
        delete it->second;
        suppliers.erase(it);
        saveAllData();
        std::cout << "Supplier deleted: " << supplierID << "\n";
    } else {
        std::cout << "Supplier not found: " << supplierID << "\n";
    }
}

Supplier* InventoryManager::findSupplier(const std::string& supplierID) const {
    std::map<std::string, Supplier*>::const_iterator it = suppliers.find(supplierID);
    if (it != suppliers.end()) {
        return it->second;
    }
    return NULL;
}

void InventoryManager::recordSale(const std::string& productID, int quantity) {
    Product* prod = findProduct(productID);
    if (prod == NULL) {
        std::cout << "Product not found: " << productID << "\n";
        return;
    }
    
    if (prod->getCurrentStock() < quantity) {
        std::cout << "Insufficient stock. Available: " << prod->getCurrentStock() << "\n";
        return;
    }
    
    double costPrice = prod->getCostPrice();
    double salePrice = prod->getUnitPrice();
    SalesTransaction trans(SalesTransaction::generateTransactionID(), productID, 
                           quantity, salePrice, SalesTransaction::getCurrentDate(), "Sale");
    trans.setCostPrice(costPrice);
    
    prod->updateStock(-quantity);
    prod->setTotalSold(prod->getTotalSold() + quantity);
    
    salesHistory.push_back(trans);

    // Append to transactions archive (append-only)
    bool addHeader = false;
    std::ifstream transCheck(transactionsFile.c_str());
    if (!transCheck.is_open() || transCheck.peek() == std::ifstream::traits_type::eof()) {
        addHeader = true;
    }
    transCheck.close();

    std::ofstream transOut(transactionsFile.c_str(), std::ios::out | std::ios::app);
    if (transOut.is_open()) {
        if (addHeader) {
            transOut << "transactionID,productID,quantity,salePrice,totalAmount,date,type\n";
        }
        transOut << trans.toCSVString() << "\n";
        transOut.close();
    } else {
        std::cout << "Warning: Unable to append to transactions file: " << transactionsFile << "\n";
    }

    saveSalesData();
    saveAllData();

    std::cout << "Sale recorded successfully.\n";
    trans.displayInfo();
}

void InventoryManager::recordPurchase(const std::string& productID, int quantity) {
    Product* prod = findProduct(productID);
    if (prod == NULL) {
        std::cout << "Product not found: " << productID << "\n";
        return;
    }
    
    double costPrice = prod->getCostPrice();
    SalesTransaction trans(SalesTransaction::generateTransactionID(), productID,
                           quantity, costPrice, 
                           SalesTransaction::getCurrentDate(), "Purchase");
    trans.setCostPrice(costPrice);
    
    prod->updateStock(quantity);
    
    salesHistory.push_back(trans);

    // Append to transactions archive (append-only)
    bool addHeader = false;
    std::ifstream transCheck(transactionsFile.c_str());
    if (!transCheck.is_open() || transCheck.peek() == std::ifstream::traits_type::eof()) {
        addHeader = true;
    }
    transCheck.close();

    std::ofstream transOut(transactionsFile.c_str(), std::ios::out | std::ios::app);
    if (transOut.is_open()) {
        if (addHeader) {
            transOut << "transactionID,productID,quantity,salePrice,totalAmount,date,type\n";
        }
        transOut << trans.toCSVString() << "\n";
        transOut.close();
    } else {
        std::cout << "Warning: Unable to append to transactions file: " << transactionsFile << "\n";
    }

    saveSalesData();
    saveAllData();

    std::cout << "Purchase recorded successfully.\n";
}

std::vector<Product*> InventoryManager::getLowStockItems() const {
    std::vector<Product*> result;
    for (std::map<std::string, Product*>::const_iterator it = products.begin();
         it != products.end(); ++it) {
        if (it->second->isLowStock()) {
            result.push_back(it->second);
        }
    }
    return result;
}

std::vector<Product*> InventoryManager::getNearExpiryItems() const {
    std::vector<Product*> result;
    for (std::map<std::string, Product*>::const_iterator it = products.begin();
         it != products.end(); ++it) {
        if (it->second->isNearExpiry()) {
            result.push_back(it->second);
        }
    }
    return result;
}

std::vector<Product*> InventoryManager::getTopSellingProducts(int n) const {
    std::vector<Product*> result;
    for (std::map<std::string, Product*>::const_iterator it = products.begin();
         it != products.end(); ++it) {
        result.push_back(it->second);
    }
    
    // Sort by totalSold in descending order using bubble sort
    for (size_t i = 0; i < result.size(); ++i) {
        for (size_t j = i + 1; j < result.size(); ++j) {
            if (result[j]->getTotalSold() > result[i]->getTotalSold()) {
                Product* temp = result[i];
                result[i] = result[j];
                result[j] = temp;
            }
        }
    }
    
    if (result.size() > (size_t)n) {
        result.erase(result.begin() + n, result.end());
    }
    
    return result;
}

std::vector<Product*> InventoryManager::getSlowMovingProducts(int daysSinceLastSale) const {
    // For now, return products with low sales
    std::vector<Product*> result;
    for (std::map<std::string, Product*>::const_iterator it = products.begin();
         it != products.end(); ++it) {
        if (it->second->getTotalSold() < 10) {  // Threshold of 10 units
            result.push_back(it->second);
        }
    }
    return result;
}

std::map<std::string, double> InventoryManager::getCategoryProfitLoss() const {
    std::map<std::string, double> categoryPL;
    
    for (std::set<std::string>::const_iterator it = categories.begin();
         it != categories.end(); ++it) {
        categoryPL[*it] = 0.0;
    }
    
    // Simple calculation: (revenue - cost) for each category
    for (size_t i = 0; i < salesHistory.size(); ++i) {
        if (salesHistory[i].getTransactionType() == "Sale") {
            Product* prod = findProduct(salesHistory[i].getProductID());
            if (prod != NULL) {
                double profit = salesHistory[i].calculateProfit();
                categoryPL[prod->getCategory()] += profit;
            }
        }
    }
    
    return categoryPL;
}

std::vector<Product*> InventoryManager::searchProductsByCategory(const std::string& category) const {
    std::vector<Product*> result;
    for (std::map<std::string, Product*>::const_iterator it = products.begin();
         it != products.end(); ++it) {
        if (it->second->getCategory() == category) {
            result.push_back(it->second);
        }
    }
    return result;
}

std::vector<Product*> InventoryManager::filterProductsByStock(int minStock, int maxStock) const {
    std::vector<Product*> result;
    for (std::map<std::string, Product*>::const_iterator it = products.begin();
         it != products.end(); ++it) {
        int stock = it->second->getCurrentStock();
        if (stock >= minStock && stock <= maxStock) {
            result.push_back(it->second);
        }
    }
    return result;
}

std::vector<Product*> InventoryManager::filterProductsByPrice(double minPrice, double maxPrice) const {
    std::vector<Product*> result;
    for (std::map<std::string, Product*>::const_iterator it = products.begin();
         it != products.end(); ++it) {
        double price = it->second->getUnitPrice();
        if (price >= minPrice && price <= maxPrice) {
            result.push_back(it->second);
        }
    }
    return result;
}

double InventoryManager::calculateSimpleMovingAverage(const std::string& productID, int days) const {
    std::vector<int> dailySales(days, 0);
    
    for (size_t i = 0; i < salesHistory.size(); ++i) {
        if (salesHistory[i].getProductID() == productID &&
            salesHistory[i].getTransactionType() == "Sale") {
            // Simplified: just add to the array
            if (i < (size_t)days) {
                dailySales[i] = salesHistory[i].getQuantity();
            }
        }
    }
    
    int total = 0;
    for (int i = 0; i < days; ++i) {
        total += dailySales[i];
    }
    
    return (double)total / days;
}

double InventoryManager::calculateLinearTrend(const std::string& productID) const {
    std::vector<int> sales;
    for (size_t i = 0; i < salesHistory.size(); ++i) {
        if (salesHistory[i].getProductID() == productID &&
            salesHistory[i].getTransactionType() == "Sale") {
            sales.push_back(salesHistory[i].getQuantity());
        }
    }
    
    if (sales.size() < 2) return 0.0;
    
    // Simple trend: average increase per transaction
    double trend = (double)(sales.back() - sales.front()) / (sales.size() - 1);
    return trend;
}

int InventoryManager::predictNextDemand(const std::string& productID, int daysAhead) const {
    double avg = calculateSimpleMovingAverage(productID, 7);
    double trend = calculateLinearTrend(productID);
    
    double predicted = avg + (trend * daysAhead);
    return (predicted > 0) ? (int)predicted : 0;
}

int InventoryManager::suggestReorderQuantity(const std::string& productID) const {
    Product* prod = findProduct(productID);
    if (prod == NULL) return 0;
    
    return prod->calculateReorderQty();
}

void InventoryManager::generateAlerts() {
    // Clear previous alerts
    while (!pendingAlerts.empty()) {
        pendingAlerts.pop();
    }
    
    // Low stock alerts
    std::vector<Product*> lowStock = getLowStockItems();
    for (size_t i = 0; i < lowStock.size(); ++i) {
        std::ostringstream oss;
        oss << "LOW STOCK ALERT: " << lowStock[i]->getProductID() << " - "
            << lowStock[i]->getName() << " (Current: " << lowStock[i]->getCurrentStock() << ")";
        pendingAlerts.push(oss.str());
    }
    
    // Near expiry alerts
    std::vector<Product*> nearExpiry = getNearExpiryItems();
    for (size_t i = 0; i < nearExpiry.size(); ++i) {
        std::ostringstream oss;
        oss << "EXPIRY ALERT: " << nearExpiry[i]->getProductID() << " - "
            << nearExpiry[i]->getName() << " expires soon!";
        pendingAlerts.push(oss.str());
    }
}

void InventoryManager::displayAlerts() {
    if (pendingAlerts.empty()) {
        std::cout << "\nNo pending alerts.\n";
        return;
    }
    
    std::cout << "\n=== PENDING ALERTS ===\n";
    int count = 1;
    
    // We can't iterate through queue directly, so we process them
    std::queue<std::string> temp = pendingAlerts;
    while (!temp.empty()) {
        std::cout << count++ << ". " << temp.front() << "\n";
        temp.pop();
    }
}

std::string InventoryManager::getNextAlert() {
    if (!pendingAlerts.empty()) {
        std::string alert = pendingAlerts.front();
        pendingAlerts.pop();
        return alert;
    }
    return "";
}

int InventoryManager::getPendingAlertCount() const {
    return pendingAlerts.size();
}

void InventoryManager::generateSupplierPerformanceReport() const {
    if (suppliers.empty()) {
        std::cout << "\nNo suppliers to report.\n";
        return;
    }
    
    std::cout << "\n===== SUPPLIER PERFORMANCE REPORT =====\n";
    std::cout << std::left << std::setw(12) << "Supplier ID"
              << std::setw(25) << "Name"
              << std::setw(12) << "Lead Time"
              << std::setw(12) << "Reliability\n";
    std::cout << std::string(61, '-') << "\n";
    
    for (std::map<std::string, Supplier*>::const_iterator it = suppliers.begin();
         it != suppliers.end(); ++it) {
        Supplier* supp = it->second;
        std::cout << std::left << std::setw(12) << supp->getSupplierID()
                  << std::setw(25) << supp->getName()
                  << std::setw(12) << (std::string(1, ' ') + std::string(1, '0' + supp->getLeadTimeDays()))
                  << std::setw(12) << supp->getReliabilityScore() << "\n";
    }
}

int InventoryManager::getTotalProducts() const {
    return products.size();
}

int InventoryManager::getTotalSuppliers() const {
    return suppliers.size();
}

int InventoryManager::getTotalSalesTransactions() const {
    return salesHistory.size();
}

double InventoryManager::getTotalRevenue() const {
    double total = 0.0;
    for (size_t i = 0; i < salesHistory.size(); ++i) {
        if (salesHistory[i].getTransactionType() == "Sale") {
            total += salesHistory[i].getTotalAmount();
        }
    }
    return total;
}

double InventoryManager::calculateTotalInventoryValue() const {
    double total = 0.0;
    for (std::map<std::string, Product*>::const_iterator it = products.begin();
         it != products.end(); ++it) {
        total += (it->second->getUnitPrice() * it->second->getCurrentStock());
    }
    return total;
}

void InventoryManager::displayAllProducts() const {
    if (products.empty()) {
        std::cout << "\nNo products in inventory.\n";
        return;
    }
    
    std::cout << "\n===== ALL PRODUCTS =====\n";
    std::cout << std::left << std::setw(10) << "ID"
              << std::setw(25) << "Name"
              << std::setw(15) << "Category"
              << std::setw(10) << "Stock"
              << std::setw(10) << "Price"
              << std::setw(8) << "Type\n";
    std::cout << std::string(78, '-') << "\n";
    
    for (std::map<std::string, Product*>::const_iterator it = products.begin();
         it != products.end(); ++it) {
        Product* prod = it->second;
        std::cout << std::left << std::setw(10) << prod->getProductID()
                  << std::setw(25) << prod->getName()
                  << std::setw(15) << prod->getCategory()
                  << std::setw(10) << prod->getCurrentStock()
                  << std::setw(10) << prod->getUnitPrice()
                  << std::setw(8) << prod->getType() << "\n";
    }
}

void InventoryManager::displayAllSuppliers() const {
    if (suppliers.empty()) {
        std::cout << "\nNo suppliers registered.\n";
        return;
    }
    
    std::cout << "\n===== ALL SUPPLIERS =====\n";
    std::cout << std::left << std::setw(12) << "ID"
              << std::setw(20) << "Name"
              << std::setw(15) << "Contact"
              << std::setw(10) << "Lead Time"
              << std::setw(12) << "Reliability\n";
    std::cout << std::string(69, '-') << "\n";
    
    for (std::map<std::string, Supplier*>::const_iterator it = suppliers.begin();
         it != suppliers.end(); ++it) {
        Supplier* supp = it->second;
        std::cout << std::left << std::setw(12) << supp->getSupplierID()
                  << std::setw(20) << supp->getName()
                  << std::setw(15) << supp->getContact()
                  << std::setw(10) << supp->getLeadTimeDays()
                  << std::setw(12) << supp->getReliabilityScore() << "\n";
    }
}

void InventoryManager::displaySalesHistory(int limit) const {
    if (salesHistory.empty()) {
        std::cout << "\nNo sales transactions recorded.\n";
        return;
    }
    
    std::cout << "\n===== SALES HISTORY (Last " << limit << " transactions) =====\n";
    std::cout << std::left << std::setw(12) << "Trans ID"
              << std::setw(12) << "Product ID"
              << std::setw(10) << "Qty"
              << std::setw(10) << "Price"
              << std::setw(12) << "Amount"
              << std::setw(15) << "Date"
              << std::setw(10) << "Type\n";
    std::cout << std::string(79, '-') << "\n";
    
    int start = (salesHistory.size() > (size_t)limit) ? (salesHistory.size() - limit) : 0;
    for (size_t i = start; i < salesHistory.size(); ++i) {
        const SalesTransaction& trans = salesHistory[i];
        std::cout << std::left << std::setw(12) << trans.getTransactionID()
                  << std::setw(12) << trans.getProductID()
                  << std::setw(10) << trans.getQuantity()
                  << std::setw(10) << std::fixed << std::setprecision(2) << trans.getSalePrice()
                  << std::setw(12) << trans.getTotalAmount()
                  << std::setw(15) << trans.getTransactionDate()
                  << std::setw(10) << trans.getTransactionType() << "\n";
    }
}
