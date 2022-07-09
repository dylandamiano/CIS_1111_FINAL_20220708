#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <string>

/*
	Name: Dylan Damiano
	Date: 2022-06-26

	Description:
		* PC part building calculator. Contains predetermined
		pricings of Central and Graphical Processing Units dated
		from the 2000s and higher.

		Will contain:
			Cost, generation (Ryzen 5, DDDR4, GDDR6, etc), etc
	
	REQUIREMENT CHECKLIST:
		* Name
		* Description
		* List of in/outs
		* Flowchart
		* Instructions for Ease of Use
		* Submit documentation as word or PDF
	
	HOW TO UTILIZE:
		* ... to be cont ...

*/

// Array/Vector Declarations w/o Definitions
// NOTE: ALL PRICES DO NOT REFLECT THE ACTUAL PRODUCT IN THE MARKET
const short int productSize = 6;
const short int driveSize = 3;
const short int boardSize = 4;
const short int powerSize = 7;
const short int cpuSize = 2;
const short int memorySize = 1;
const short int gpuSize = 1;

const std::string productCategories[productSize] = { "CPU", "RAM", "GPU", "MOTHERBOARD", "POWERSUPPLY", "DRIVES" };
// SUBLEVEL SIZE
// 2D ARRAY CORRESPONDING TO PRODUCT CATEGORIES

// We're going to have to waste some memory here but a vector might be the best option...
const std::string physicalProducts[productSize][powerSize] = {
	{"AMD Ryzen 5", "Intel i10"},
	{"DDR4 @ 3200 MHz"},
	{"GTX 30 Series"},
	{ "E-ATX", "ATX", "MICRO-ATX", "MINI-ITX" },
	{"2000W", "1250W", "1050W", "850W", "750W", "650W", "40W"},
	{ "M.2", "SSD", "HDD" },
};

const std::string physicalProductsFileFriendly[productSize][powerSize] = {
	{"AMD-Ryzen-5", "Intel-i10"},
	{"DDR4-3200-MHz"},
	{"GTX-30-Series"},
	{ "E-ATX", "ATX", "MICRO-ATX", "MINI-ITX" },
	{"2000W", "1250W", "1050W", "850W", "750W", "650W", "40W"},
	{ "M.2", "SSD", "HDD" },
};

const float productPrices[productSize][powerSize] = {
	{ 500, 600 },
	{ 250 },
	{ 800 },
	{ 200, 150, 100, 50 },
	{ 2000, 1000, 500, 250, 125, 62.50, 31.25 },
	{ 200, 150, 100 },
};
const short int physicalProductSizes[productSize] = { cpuSize, memorySize, gpuSize, boardSize, powerSize, driveSize };

/* 
	const std::string driveCategories[driveSize] = { "M.2", "SSD", "HDD" };
	const std::string boardCategories[boardSize] = { "E-ATX", "ATX", "MICRO-ATX", "MINI-ITX" };
	const std::string powerCategories[powerSize] = {"2000W", "1250W", "1050W", "850W", "750W", "650W", "40W"};
*/

/*
	const float drivePrices[driveSize] = { 200, 150, 100 };
	const float boardPrices[] = { 200, 150, 100, 50 };
	const float powerPrices[powerSize] = {2000, 1000, 500, 250, 125, 62.50, 31.25};
*/

// STOCK AND QUANTITY PURCHASED
const short int purchaseDim2 = 2;

int productPurchases[productSize][powerSize][purchaseDim2] = {
	// FOR TWO 2 ELEMENT ARRAY, THE SIZE IS 2, FIRST ELEMENT IS FOR STOCK REMAINING, SECOND IS FOR QUANTITY PURCHASED

	{ {400, 0}, {410, 0},}, // CPU
	{ {50, 0} }, // MEMORY
	{ {510, 0} }, // CPU // GPU
	{ {100, 0}, {110, 0}, {120, 0} }, // MOTHERBOARD
	{ {200, 0}, {210, 0}, {220, 0}, {230, 0} }, // POWERSUPPLY
	{ {300, 0}, {310, 0}, {320, 0}, {330, 0}, {340, 0}, {350, 0}, {360, 0} } // DRIVES
};

int drivePurchases[driveSize][purchaseDim2] = { {100, 0}, {110, 0}, {120, 0} };
int boardPurchases[boardSize][purchaseDim2] = {
	{200, 0},
	{210, 0},
	{220, 0},
	{230, 0},
};
int powerPurchases[powerSize][purchaseDim2] = { {300, 0}, {310, 0}, {320, 0}, {330, 0}, {340, 0}, {350, 0}, {360, 0} };

// Store all purchases individually here!
const short int purchasePartsSize = 4;
std::vector<std::vector<std::string>> purchasedParts;

// Below will be our TWO-DIMENSIONAL ARRAY for COMMAND NAME and DESCRIPTION
const short int commandDim1 = 7;
const short int commandDim2 = 2;

std::string commandName[commandDim1][commandDim2] = {
	{"help","Displays every possible command available to you, the user"}, 
	{"purchase", "Purchase a variety of items ranging from memory to processing components!"},
	{"productInfo", "Get information on a certain product!"},
	{"savePurchase", "Save your purchases to a file!"},
	{"reviewPurchase", "Look at purchases by category made during this appliation's runtime!"},
	{"reset", "Resets the user profile! Does *NOT* reset stock count!"},
	{"exit", "Terminates the program!"},
};

void purchaseComponent(bool test = false);
void purchaseComponent(int category, int part);

void calculatePrice(bool test = false);
void getProductInfo(bool purchasePrompt); // For just getting product information in general... I.E. Name, Category, Price, Etc.
float getProductInfo(short int categoryNum, short int productNum, bool includeStock = false, bool includeCost = false); // For getting the stock availability of the aforementioned product
void help();

void writeFile();
void readFile();

void readFile() {
	std::ifstream fileIn("theFile.txt");

	std::string productName;
	int productQuantity;
	int productPurchased;
	float overallCost;
	std::string productCategory;

	while (fileIn >> productName >> productQuantity >> productPurchased >> overallCost >> productCategory) {
		std::cout << productCategory << " " << productName << " " << productQuantity << " " << productPurchased << " " << overallCost << std::endl;
	};
	
	fileIn.close();
}

void writeFile() {
	std::ofstream fileOut("theFile.txt");

	float totalSpent;

	for (int i = 0; i < productSize; i++) {
		for (int x = 0; x < powerSize; x++) {
			if (physicalProducts[i][x] != "") { // THE "" means that the name is *NOT* declared
				fileOut << physicalProductsFileFriendly[i][x] << "\t"; // Name
				fileOut << productPurchases[i][x][0] << "\t"; // Stock
				fileOut << productPurchases[i][x][1] << "\t"; // Purchased
				fileOut << productPurchases[i][x][1] * productPrices[i][x] << "\t"; // Overall Cost
				fileOut << productCategories[i] << "\t" << std::endl; // Category
			}
		}
	}

	fileOut.close();
}

void getProductInfo(bool purchasePrompt) {
	short int selection;

	if (purchasePrompt == true) {
		std::cout << "For which product category would you like to purchase from?" << std::endl;
	} else if (purchasePrompt == false) {
		std::cout << "For which product do you want information on?" << std::endl;
	}
	

	for (int i = 0; i < productSize; i++) {
		std::cout << "\t" << i << ": " << productCategories[i] << std::endl;
	};

	std::cout << "Selection (ex. 0): ";
	std::cin >> selection;

	for (int i = 0; i < productSize; i++) {
		if (selection == i) {
			selection = 0;
			std::cout << "You have selected: " << productCategories[i] << "... \n";
			std::cout << "Which item would you like to select? \n";

			for (int y = 0; y < physicalProductSizes[i]; y++) {
				std::cout << "\t" << y << ": " << physicalProducts[i][y] << std::endl; // powerCategories[y]
			}

			std::cout << "Selection (ex. 0): ";
			std::cin >> selection;

			std::cout << "You have selected: " << physicalProducts[i][selection] << "\n";

			std::cout << std::setw(16) << "\tProduct Cat.: " << std::setw(15) << productCategories[i] << std::endl;
			std::cout << std::setw(16) << "\tProduct Name: " << std::setw(15) << physicalProducts[i][selection] << std::endl;
			std::cout << std::setw(16) << "\tProduct Cost: " << std::fixed << std::setprecision(2) << std::setw(15) <<  getProductInfo(i, selection, false, true) << std::endl; // productPrices[i][selection]
			std::cout << std::setw(16) << "\tProduct Stck: " << std::fixed << std::setprecision(0) << std::setw(15) << getProductInfo(i, selection, true, false) << std::endl;

			if (purchasePrompt == true) {
				std::string validation;
				std::cout << "Would you like to purchase? (y/n): ";
				std::cin >> validation;

				if (validation == "y") {
					purchaseComponent(i, selection);
				} else {
					std::cout << "Returning to menu..." << std::endl;
				}
			}

			break; // WITHOUT THIS BREAK, IT WILL FALL THROUGH BECAUSE WE CHANGED THE SELECTION VALUE!
		}
	}
}

float getProductInfo(short int categoryNum, short int productNum, bool includeStock, bool includeCost) {
	if (includeStock == true) {
		// STOCK NOT A FEATURE AS OF RIGHT NOW!
		return productPurchases[categoryNum][productNum][0];
	}

	if (includeCost == true) {
		return productPrices[categoryNum][productNum];
	}
}

void purchaseComponent(bool test) {
	if (test == true) {
		std::cout << "Testing purchase function!" << std::endl;
	}
}

void purchaseComponent(int categoryNum, int partNum) {
	int quantity;
	std::cout << "How many items would you like to purchase? (ex: 0): ";
	std::cin >> quantity;

	if (quantity >= 0) {
		// [0] += quantity;
		if ((productPurchases[categoryNum][partNum][0] - quantity) >= 0) {
			productPurchases[categoryNum][partNum][0] -= quantity;
			productPurchases[categoryNum][partNum][1] += quantity;
			
		} else {
			std::cout << "Too many items are being purchased, try with a lesser quantity!" << std::endl;
		}
	} else {
		// CANNOT PURCHASE LESS THAN ZERO!
		std::cout << "Something went wrong OR you were purchasing less than ZERO items!";
	}

}

void calculatePrice(bool test) {

}

void help() {
	
	// ESCAPE SEQUENCES: https://en.cppreference.com/w/cpp/language/escape
	std::cout << "COMMAND LIST: \n";

	for (int i=0; i < commandDim1; i++) {
		for (int y = 0; y < commandDim2; y++) {
			switch(y){
				case 0:
					std::cout << "\t" << commandName[i][y] << ": ";
					break;
				case 1:
					std::cout << commandName[i][y] << "\n";
					break;
			}
		}
	}

	std::cout << "\n";
}


int main() {
	std::cout << "Welcome to DC Central! \n" << std::endl;
	std::string orderName;
	std::string orderDate[3]; // YYYY, MM, DD provided by user ( may god help us all, with no error handling D: 

	while (true) { // Beginning of main program loop which includes basic setup...
		std::string name;
		std::string localInput;
		localInput = "";

		std::cout << "Enter your name: ";
		std::getline(std::cin, name);
		
		std::cout << "Enter the year (YYYY ex. 2010): ";
		std::cin >> orderDate[0];
		std::cout << "Enter the month (MM ex. 08 for August): ";
		std::cin >> orderDate[1];
		std::cout << "Enter the day (DD ex. 16): ";
		std::cin >> orderDate[2];

		std::cout << "\nType \"help\" for more info!\n";

		while (true) {
			localInput = "";
			std::cout << ">> ";
			std::cin >> localInput;

			if (localInput == "help") {
				help();
				std::cout << "\n";
			} else if (localInput == "purchase") {
				getProductInfo(true);
				std::cout << "\n";
			} else if (localInput == "productInfo") {
				getProductInfo(false);
				std::cout << "\n";
			}
			else if (localInput == "savePurchase") {
				writeFile();
				std::cout << "\n";
			} else if (localInput == "reviewPurchase") {
				readFile();
				std::cout << "\n";
			} else if (localInput == "reset") {
				std::cout << "Resetting profile!" << std::endl;
				std::cout << "\n";
				break;
			} else if (localInput == "exit") {
				std::cout << "Terminating program!";
				exit(0);		
			}
		}
	}; 
	
	return 0;
}