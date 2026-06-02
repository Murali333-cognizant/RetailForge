#include "inventorymanager.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <limits>

// Function prototypes
void clearScreen();
void pauseScreen();
void printHeader(const std::string& title);
void printSectionTitle(const std::string& title);
void printInputPrompt(const std::string& text);

void displayMainMenu();

void displayProductMenu();
void displaySupplierMenu();
void displaySalesMenu();
void displayReportMenu();
void displayForecastMenu();

InventoryManager* manager = NULL;

void clearScreen() {
    system("clear");
}

void pauseScreen() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "\nPress ENTER to continue...";
    std::cin.get();
}

void printHeader(const std::string& title) {
    std::cout << "========================================" << std::endl;
    std::cout << "        " << title << std::endl;
    std::cout << "========================================" << std::endl;
}

void printSectionTitle(const std::string& title) {
    std::cout << "\n";
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

void printInputPrompt(const std::string& text) {
    std::cout << ">> " << text;
}

void displayMainMenu() {
    clearScreen();
    printHeader("RETAILFORGE MAIN MENU");
    std::cout << "1. Product Management\n";
    std::cout << "2. Supplier Management\n";
    std::cout << "3. Sales & Purchases\n";
    std::cout << "4. Reports & Analytics\n";
    std::cout << "5. Forecasting & Alerts\n";
    std::cout << "6. Backup & Restore\n";
    std::cout << "0. Exit\n";
    std::cout << "========================================\n";
    printInputPrompt("Enter your choice: ");
}

void displayProductMenu() {
    clearScreen();
    printHeader("PRODUCT MANAGEMENT");
    std::cout << "1. Add New Product\n";
    std::cout << "2. View All Products\n";
    std::cout << "3. Search Product by ID\n";
    std::cout << "4. Search Product by Name\n";
    std::cout << "5. Update Product\n";
    std::cout << "6. Delete Product\n";
    std::cout << "7. Filter by Category\n";
    std::cout << "8. Filter by Stock Level\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "========================================\n";
    printInputPrompt("Enter your choice: ");
}

void displaySupplierMenu() {
    clearScreen();
    printHeader("SUPPLIER MANAGEMENT");
    std::cout << "1. Add New Supplier\n";
    std::cout << "2. View All Suppliers\n";
    std::cout << "3. Search Supplier by ID\n";
    std::cout << "4. Update Supplier\n";
    std::cout << "5. Delete Supplier\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "========================================\n";
    printInputPrompt("Enter your choice: ");
}

void displaySalesMenu() {
    clearScreen();
    printHeader("SALES & PURCHASES");
    std::cout << "1. Record a Sale\n";
    std::cout << "2. Record a Purchase\n";
    std::cout << "3. View Sales History\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "========================================\n";
    printInputPrompt("Enter your choice: ");
}

void displayReportMenu() {
    clearScreen();
    printHeader("REPORTS & ANALYTICS");
    std::cout << "1. Low Stock Items\n";
    std::cout << "2. Near Expiry Items\n";
    std::cout << "3. Top 20 Best-Selling Products\n";
    std::cout << "4. Slow-Moving Products\n";
    std::cout << "5. Category-wise Profit & Loss\n";
    std::cout << "6. Supplier Performance Report\n";
    std::cout << "7. System Dashboard\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "========================================\n";
    printInputPrompt("Enter your choice: ");
}

void displayForecastMenu() {
    clearScreen();
    printHeader("FORECASTING & ALERTS");
    std::cout << "1. View Pending Alerts\n";
    std::cout << "2. Calculate Moving Average\n";
    std::cout << "3. Predict Demand\n";
    std::cout << "4. Suggest Reorder Quantity\n";
    std::cout << "5. Generate All Alerts\n";
    std::cout << "0. Back to Main Menu\n";
    std::cout << "========================================\n";
    printInputPrompt("Enter your choice: ");
}

void productManagementModule() {
    while (true) {
        displayProductMenu();
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 1) {
            printSectionTitle("ADD NEW PRODUCT");
            printInputPrompt("Is this product perishable? (y/n): ");
            char isPerishable;
            std::cin >> isPerishable;
            std::cin.ignore();
            
            printInputPrompt("Enter Product ID: ");
            std::string prodID;
            std::getline(std::cin, prodID);
            
            printInputPrompt("Enter Product Name: ");
            std::string name;
            std::getline(std::cin, name);
            
            printInputPrompt("Enter Category: ");
            std::string category;
            std::getline(std::cin, category);
            
            printInputPrompt("Enter Supplier ID: ");
            std::string suppID;
            std::getline(std::cin, suppID);
            
            printInputPrompt("Enter Current Stock: ");
            int stock;
            std::cin >> stock;
            
            printInputPrompt("Enter Reorder Level: ");
            int reorderLevel;
            std::cin >> reorderLevel;
            
            printInputPrompt("Enter Cost Price: ");
            double costPrice;
            std::cin >> costPrice;
            
            printInputPrompt("Enter Selling Price: ");
            double price;
            std::cin >> price;
            std::cin.ignore();
            
            Product* prod = NULL;
            
            if (isPerishable == 'y' || isPerishable == 'Y') {
                printInputPrompt("Enter Expiry Date (DD-MM-YYYY): ");
                std::string expiryDate;
                std::getline(std::cin, expiryDate);
                
                printInputPrompt("Enter Shelf Life (days): ");
                int shelfDays;
                std::cin >> shelfDays;
                std::cin.ignore();
                
                prod = new PerishableProduct(prodID, name, category, suppID, stock,
                                           reorderLevel, costPrice, price, 0, expiryDate, shelfDays);
            } else {
                printInputPrompt("Enter Batch Number: ");
                std::string batch;
                std::getline(std::cin, batch);
                
                printInputPrompt("Enter Warranty (months): ");
                int warranty;
                std::cin >> warranty;
                std::cin.ignore();
                
                prod = new NonPerishableProduct(prodID, name, category, suppID, stock,
                                              reorderLevel, costPrice, price, 0, batch, warranty);
            }
            
            manager->addProduct(prod);
            
        } else if (choice == 2) {
            // View all products
            manager->displayAllProducts();
            
        } else if (choice == 3) {
            // Search by ID
            printSectionTitle("SEARCH PRODUCT BY ID");
            printInputPrompt("Enter Product ID: ");
            std::string prodID;
            std::getline(std::cin, prodID);
            
            Product* prod = manager->findProduct(prodID);
            if (prod != NULL) {
                prod->displayInfo();
            } else {
                std::cout << "Product not found.\n";
            }
            
        } else if (choice == 4) {
            // Search by name
            printSectionTitle("SEARCH PRODUCT BY NAME");
            printInputPrompt("Enter Product Name: ");
            std::string name;
            std::getline(std::cin, name);
            
            Product* prod = manager->findProductByName(name);
            if (prod != NULL) {
                prod->displayInfo();
            } else {
                std::cout << "Product not found.\n";
            }
            
        } else if (choice == 5) {
            // Update product
            printSectionTitle("UPDATE PRODUCT");
            printInputPrompt("Enter Product ID to update: ");
            std::string prodID;
            std::getline(std::cin, prodID);
            
            Product* prod = manager->findProduct(prodID);
            if (prod == NULL) {
                std::cout << "Product not found.\n";
                continue;
            }
            
            prod->displayInfo();
            printInputPrompt("Enter new Stock: ");
            int stock;
            std::cin >> stock;
            prod->setCurrentStock(stock);
            
            printInputPrompt("Enter new Reorder Level: ");
            int reorder;
            std::cin >> reorder;
            prod->setReorderLevel(reorder);
            
            printInputPrompt("Enter new Unit Price: ");
            double price;
            std::cin >> price;
            prod->setUnitPrice(price);
            std::cin.ignore();
            
            manager->saveAllData();
            std::cout << "Product updated successfully.\n";
            
        } else if (choice == 6) {
            // Delete product
            printSectionTitle("DELETE PRODUCT");
            printInputPrompt("Enter Product ID to delete: ");
            std::string prodID;
            std::getline(std::cin, prodID);
            
            manager->deleteProduct(prodID);
            
        } else if (choice == 7) {
            // Filter by category
            printSectionTitle("FILTER BY CATEGORY");
            printInputPrompt("Enter Category: ");
            std::string category;
            std::getline(std::cin, category);
            
            std::vector<Product*> filtered = manager->searchProductsByCategory(category);
            if (filtered.empty()) {
                std::cout << "No products found in this category.\n";
            } else {
                std::cout << "\nProducts in category '" << category << "':\n";
                for (size_t i = 0; i < filtered.size(); ++i) {
                    filtered[i]->displayInfo();
                }
            }
            
        } else if (choice == 8) {
            // Filter by stock
            printSectionTitle("FILTER BY STOCK LEVEL");
            printInputPrompt("Enter minimum stock: ");
            int minStock;
            std::cin >> minStock;
            
            printInputPrompt("Enter maximum stock: ");
            int maxStock;
            std::cin >> maxStock;
            std::cin.ignore();
            
            std::vector<Product*> filtered = manager->filterProductsByStock(minStock, maxStock);
            if (filtered.empty()) {
                std::cout << "No products found within this stock range.\n";
            } else {
                std::cout << "\nProducts with stock between " << minStock << " and " << maxStock << ":\n";
                for (size_t i = 0; i < filtered.size(); ++i) {
                    std::cout << "  " << filtered[i]->getProductID() << " - " << filtered[i]->getName()
                              << " (Stock: " << filtered[i]->getCurrentStock() << ")\n";
                }
            }
            
        } else if (choice == 0) {
            break;
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
        
        pauseScreen();
    }
}

void supplierManagementModule() {
    while (true) {
        displaySupplierMenu();
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 1) {
            printSectionTitle("ADD NEW SUPPLIER");
            printInputPrompt("Enter Supplier ID: ");
            std::string suppID;
            std::getline(std::cin, suppID);
            
            printInputPrompt("Enter Supplier Name: ");
            std::string name;
            std::getline(std::cin, name);
            
            printInputPrompt("Enter Contact Number: ");
            std::string contact;
            std::getline(std::cin, contact);
            
            printInputPrompt("Enter Lead Time (days): ");
            int leadTime;
            std::cin >> leadTime;
            
            printInputPrompt("Enter Reliability Score (0.0-1.0): ");
            double reliability;
            std::cin >> reliability;
            std::cin.ignore();
            
            Supplier* supp = new Supplier(suppID, name, contact, leadTime, reliability);
            manager->addSupplier(supp);
            
        } else if (choice == 2) {
            // View all suppliers
            manager->displayAllSuppliers();
            
        } else if (choice == 3) {
            // Search by ID
            printSectionTitle("SEARCH SUPPLIER BY ID");
            printInputPrompt("Enter Supplier ID: ");
            std::string suppID;
            std::getline(std::cin, suppID);
            
            Supplier* supp = manager->findSupplier(suppID);
            if (supp != NULL) {
                supp->displayInfo();
            } else {
                std::cout << "Supplier not found.\n";
            }
            
        } else if (choice == 4) {
            // Update supplier
            printSectionTitle("UPDATE SUPPLIER");
            printInputPrompt("Enter Supplier ID to update: ");
            std::string suppID;
            std::getline(std::cin, suppID);
            
            Supplier* supp = manager->findSupplier(suppID);
            if (supp == NULL) {
                std::cout << "Supplier not found.\n";
                continue;
            }
            
            supp->displayInfo();
            printInputPrompt("Enter new Lead Time (days): ");
            int leadTime;
            std::cin >> leadTime;
            supp->setLeadTimeDays(leadTime);
            
            printInputPrompt("Enter new Reliability Score (0.0-1.0): ");
            double reliability;
            std::cin >> reliability;
            supp->setReliabilityScore(reliability);
            std::cin.ignore();
            
            manager->saveAllData();
            std::cout << "Supplier updated successfully.\n";
            
        } else if (choice == 5) {
            // Delete supplier
            printSectionTitle("DELETE SUPPLIER");
            printInputPrompt("Enter Supplier ID to delete: ");
            std::string suppID;
            std::getline(std::cin, suppID);
            
            manager->deleteSupplier(suppID);
            
        } else if (choice == 0) {
            break;
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
        
        pauseScreen();
    }
}

void salesModule() {
    while (true) {
        displaySalesMenu();
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 1) {
            printSectionTitle("RECORD SALE");
            printInputPrompt("Enter Product ID: ");
            std::string prodID;
            std::getline(std::cin, prodID);
            
            Product* prod = manager->findProduct(prodID);
            if (prod == NULL) {
                std::cout << "Product not found.\n";
                continue;
            }
            
            std::cout << "Current stock: " << prod->getCurrentStock() << "\n";
            std::cout << "Unit price: Rs. " << prod->getUnitPrice() << "\n";
            
            printInputPrompt("Enter Quantity sold: ");
            int qty;
            std::cin >> qty;
            
            manager->recordSale(prodID, qty);
            
        } else if (choice == 2) {
            printSectionTitle("RECORD PURCHASE");
            printInputPrompt("Enter Product ID: ");
            std::string prodID;
            std::getline(std::cin, prodID);
            
            Product* prod = manager->findProduct(prodID);
            if (prod == NULL) {
                std::cout << "Product not found.\n";
                continue;
            }
            
            printInputPrompt("Enter Quantity purchased: ");
            int qty;
            std::cin >> qty;
            
            manager->recordPurchase(prodID, qty);
            
        } else if (choice == 3) {
            // View sales history
            manager->displaySalesHistory(20);
            
        } else if (choice == 0) {
            break;
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
        
        pauseScreen();
    }
}
void reportsModule() {
    while (true) {
        displayReportMenu();
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 1) {
            std::vector<Product*> lowStock = manager->getLowStockItems();
            if (lowStock.empty()) {
                std::cout << "\nNo low stock items.\n";
            } else {
                printSectionTitle("LOW STOCK ITEMS");
                std::cout << "+------------+---------------------------+------------+------------+\n";
                std::cout << "| Product ID | Name                      | Current    | Reorder    |\n";
                std::cout << "+------------+---------------------------+------------+------------+\n";
                for (size_t i = 0; i < lowStock.size(); ++i) {
                    std::cout << "| " << std::left << std::setw(10) << lowStock[i]->getProductID()
                              << " | " << std::setw(25) << lowStock[i]->getName()
                              << " | " << std::setw(10) << lowStock[i]->getCurrentStock()
                              << " | " << std::setw(10) << lowStock[i]->getReorderLevel() << " |\n";
                }
                std::cout << "+------------+---------------------------+------------+------------+\n";
            }
        } else if (choice == 2) {
            std::vector<Product*> nearExpiry = manager->getNearExpiryItems();
            if (nearExpiry.empty()) {
                std::cout << "\nNo near-expiry items.\n";
            } else {
                printSectionTitle("NEAR EXPIRY ITEMS");
                for (size_t i = 0; i < nearExpiry.size(); ++i) {
                    std::cout << "[⚠] ";
                    nearExpiry[i]->displayInfo();
                }
            }
        } else if (choice == 3) {
            std::vector<Product*> topSelling = manager->getTopSellingProducts(20);
            if (topSelling.empty()) {
                std::cout << "\nNo sales data available.\n";
            } else {
                printSectionTitle("TOP 20 BEST-SELLING PRODUCTS");
                std::cout << "+------------+---------------------------+-----------------+------------+\n";
                std::cout << "| Product ID | Name                      | Category        | Units Sold |\n";
                std::cout << "+------------+---------------------------+-----------------+------------+\n";
                for (size_t i = 0; i < topSelling.size(); ++i) {
                    std::cout << "| " << std::left << std::setw(10) << topSelling[i]->getProductID()
                              << " | " << std::setw(25) << topSelling[i]->getName()
                              << " | " << std::setw(15) << topSelling[i]->getCategory()
                              << " | " << std::setw(10) << topSelling[i]->getTotalSold() << " |\n";
                }
                std::cout << "+------------+---------------------------+-----------------+------------+\n";
            }
        } else if (choice == 4) {
            std::vector<Product*> slowMoving = manager->getSlowMovingProducts(30);
            if (slowMoving.empty()) {
                std::cout << "\nNo slow-moving products.\n";
            } else {
                printSectionTitle("SLOW-MOVING PRODUCTS");
                for (size_t i = 0; i < slowMoving.size(); ++i) {
                    std::cout << "[!] " << std::left << std::setw(10) << slowMoving[i]->getProductID()
                              << " - " << slowMoving[i]->getName()
                              << " (Sold: " << slowMoving[i]->getTotalSold() << ")\n";
                }
            }
        } else if (choice == 5) {
            std::map<std::string, double> categoryPL = manager->getCategoryProfitLoss();
            if (categoryPL.empty()) {
                std::cout << "\nNo category data available.\n";
            } else {
                printSectionTitle("CATEGORY-WISE PROFIT & LOSS");
                std::cout << "+---------------------------+---------------+\n";
                std::cout << "| Category                  | Profit/Loss   |\n";
                std::cout << "+---------------------------+---------------+\n";
                for (std::map<std::string, double>::iterator it = categoryPL.begin();
                     it != categoryPL.end(); ++it) {
                    std::cout << "| " << std::left << std::setw(25) << it->first
                              << " | Rs. " << std::right << std::setw(10) << std::fixed
                              << std::setprecision(2) << it->second << " |\n";
                }
                std::cout << "+---------------------------+---------------+\n";
            }
        } else if (choice == 6) {
            manager->generateSupplierPerformanceReport();
        } else if (choice == 7) {
            printHeader("SYSTEM DASHBOARD");
            std::cout << "Total Products: " << manager->getTotalProducts() << "\n";
            std::cout << "Total Suppliers: " << manager->getTotalSuppliers() << "\n";
            std::cout << "Total Sales Transactions: " << manager->getTotalSalesTransactions() << "\n";
            std::cout << "Total Revenue: Rs. " << std::fixed << std::setprecision(2)
                      << manager->getTotalRevenue() << "\n";
            std::cout << "Total Inventory Value: Rs. " << manager->calculateTotalInventoryValue() << "\n";
            std::cout << "Pending Alerts: " << manager->getPendingAlertCount() << "\n";
        } else if (choice == 0) {
            break;
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
        pauseScreen();
    }
}

void forecastingModule() {
    while (true) {
        displayForecastMenu();
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 1) {
            // View alerts
            printSectionTitle("PENDING ALERTS");
            manager->displayAlerts();
            
        } else if (choice == 2) {
            // Moving average
            printSectionTitle("CALCULATE MOVING AVERAGE");
            printInputPrompt("Enter Product ID: ");
            std::string prodID;
            std::getline(std::cin, prodID);
            
            printInputPrompt("Enter Number of days: ");
            int days;
            std::cin >> days;
            std::cin.ignore();
            
            double avg = manager->calculateSimpleMovingAverage(prodID, days);
            std::cout << "Simple Moving Average (" << days << " days): " << std::fixed 
                      << std::setprecision(2) << avg << " units/day\n";
            
        } else if (choice == 3) {
            // Predict demand
            printSectionTitle("PREDICT DEMAND");
            printInputPrompt("Enter Product ID: ");
            std::string prodID;
            std::getline(std::cin, prodID);
            
            printInputPrompt("Enter Days ahead to predict: ");
            int daysAhead;
            std::cin >> daysAhead;
            std::cin.ignore();
            
            int predicted = manager->predictNextDemand(prodID, daysAhead);
            std::cout << "Predicted demand for next " << daysAhead << " days: " 
                      << predicted << " units\n";
            
        } else if (choice == 4) {
            // Suggest reorder
            printSectionTitle("SUGGEST REORDER QUANTITY");
            printInputPrompt("Enter Product ID: ");
            std::string prodID;
            std::getline(std::cin, prodID);
            
            int suggested = manager->suggestReorderQuantity(prodID);
            if (suggested > 0) {
                std::cout << "Suggested Reorder Quantity: " << suggested << " units\n";
            } else {
                std::cout << "Product not found or insufficient data.\n";
            }
            
        } else if (choice == 5) {
            // Generate all alerts
            manager->generateAlerts();
            std::cout << "Alerts generated. Total pending alerts: " 
                      << manager->getPendingAlertCount() << "\n";
            manager->displayAlerts();
            
        } else if (choice == 0) {
            break;
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
        
        pauseScreen();
    }
}

int main() {
    manager = new InventoryManager(".");
    
    // Load existing data
    manager->loadAllData();
    
    int mainChoice;
    while (true) {
        displayMainMenu();
        std::cin >> mainChoice;
        std::cin.ignore();
        
        if (mainChoice == 1) {
            productManagementModule();
        } else if (mainChoice == 2) {
            supplierManagementModule();
        } else if (mainChoice == 3) {
            salesModule();
        } else if (mainChoice == 4) {
            reportsModule();
        } else if (mainChoice == 5) {
            forecastingModule();
        } else if (mainChoice == 6) {
            clearScreen();
            printHeader("SYSTEM BACKUP & RESTORE");
            std::cout << "1. Create Backup\n";
            std::cout << "2. Restore from Backup\n";
            std::cout << "0. Cancel\n";
            std::cout << "========================================\n";
            printInputPrompt("Enter your choice: ");
            
            int backupChoice;
            std::cin >> backupChoice;
            std::cin.ignore();
            
            if (backupChoice == 1) {
                manager->backupSystem();
            } else if (backupChoice == 2) {
                manager->restoreSystem();
            }
            
        } else if (mainChoice == 0) {
            std::cout << "\nSaving all data before exit...\n";
            manager->saveAllData();
            std::cout << "Thank you for using RetailForge!\n";
            delete manager;
            break;
        } else {
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
    
    return 0;
}
