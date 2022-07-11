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
		* PC part 'building calculator.' Contains predetermined
		pricings of Central and Graphical Processing Units dated
		from the 2000s and higher. 
		
		* Prices do not reflect actual monetary value *
		*		Not all parts will be contained	      *

		Will contain:
			Cost, generation (Ryzen 5, DDDR4, GDDR5, etc), etc
	
	REQUIREMENT CHECKLIST:
		* Name
		* Description
* 
		* List of in/outs
		* Flowchart
		* Instructions for Ease of Use
		* Submit documentation as word or PDF
	
	HOW TO UTILIZE:
		* All inputs are CaSe SeNsItiVe
		* To get a list of commands type 'help'
			* Each command will be supplemented by a short description detailing it's purpose
		* All contents are outputted to 'theFile.txt'
			* File output is simplified as follows containing:
				* CATEGORY, NAME, STOCK, # PURCHASED, TOTAL SPENT
		* Applications does not revolve around specific users
			* Contents can be reset, however stock listings will carry over
			* Pricings do not change
		* SAVING PURCHASE DOES OVERWRITE THE PURCHASE FILE!
		 
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

// When formatting the sequential file, we will use these values so it is easier to read from
const std::string physicalProductsFileFriendly[productSize][powerSize] = {
	{"AMD-Ryzen-5", "Intel-i10"},
	{"DDR4-3200-MHz"},
	{"GTX-30-Series"},
	{ "E-ATX", "ATX", "MICRO-ATX", "MINI-ITX" },
	{"2000W", "1250W", "1050W", "850W", "750W", "650W", "40W"},
	{ "M.2", "SSD", "HDD" }
};

// These will represent the prices of the product
const float productPrices[productSize][powerSize] = {
	{ 500, 600 }, // CPU
	{ 250 }, // RAM
	{ 800 }, // GPU
	{ 200, 150, 100, 50 }, // MOBO
	{ 2000, 1000, 500, 250, 125, 62.50, 31.25 }, // PSU
	{ 200, 150, 100 } // DRIVES
};
const short int physicalProductSizes[productSize] = { cpuSize, memorySize, gpuSize, boardSize, powerSize, driveSize }; // Stores the sizes of appropriate arrays

// STOCK AND QUANTITY PURCHASED
const short int purchaseDim2 = 2;

int productPurchases[productSize][powerSize][purchaseDim2] = {
	// We may not be able to see in 3D for 2D text but I can indeed see a 3D array :)
	// FOR TWO 2 ELEMENT ARRAY, THE SIZE IS 2, FIRST ELEMENT IS FOR STOCK REMAINING, SECOND IS FOR QUANTITY PURCHASED

	{ {400, 0}, {410, 0} }, // CPU
	{ {50, 0} }, // MEMORY
	{ {510, 0} }, // CPU // GPU
	{ {200, 0}, {210, 0}, {220, 0}, {230, 0} }, // MOTHERBOARD
	{ {300, 0}, {310, 0}, {320, 0}, {330, 0}, {340, 0}, {350, 0}, {360, 0} }, // POWERSUPPLY
	{ {100, 0}, {110, 0}, {120, 0} } // DRIVES
};

// Below will be our TWO-DIMENSIONAL ARRAY for COMMAND NAME and DESCRIPTION
const short int commandDim1 = 8;
const short int commandDim2 = 2;

// Shows the commands and their functionality
std::string commandName[commandDim1][commandDim2] = {
	{"help","Displays every possible command available to you, the user"}, 
	{"purchase", "Purchase a variety of items ranging from memory to processing components!"},
	{"productinfo", "Get information on a certain product!"},
	{"savepurchase", "Save your purchases to a file! WILL OVERWRITE FILE! Does not run on RESET!"},
	{"reviewpurchase", "Look at purchases by category made during this appliation's runtime!"},
	{"reset", "Resets the user profile! Does *NOT* reset stock count!"},
	{"exit", "Terminates the program!"},
	{"tutorial", "Shows how to use the program"}
};

// Brief readable-tutorial for the user
void tutorial() {
	std::cout << "HOW TO UTILIZE ( to see this again, type \"tutorial\")" << std::endl;
	std::cout << "\t* All inputs are CaSe SeNsItiVe" << std::endl;
	std::cout << "\t* To get a list of commands type 'help'" << std::endl;
	std::cout << "\t\t* Each command will be supplemented by a short description detailing it's purpose" << std::endl;
	std::cout << "\t* All contents are outputted to 'theFile.txt'" << std::endl;
	std::cout << "\t\t* File output is simplified as follows containing:" << std::endl;
	std::cout << "\t\t\t*CATEGORY, NAME, STOCK, # PURCHASED, TOTAL SPENT" << std::endl;
	std::cout << "\t* Applications does not revolve around specific users" << std::endl;
	std::cout << "\t\t* Contents can be reset, however stock listings will carry over" << std::endl;
	std::cout << "\t\t* Pricings do not change" << std::endl;
	std::cout << "\t* SAVING PURCHASE DOES OVERWRITE THE PURCHASE FILE! \n" << std::endl;
};

// Overload (2; 1st - No Return ; 2nd - Returns relevant float)
void getProductInfo(bool purchasePrompt); // For just getting product information in general... I.E. Name, Category, Price, Etc.
float getProductInfo(short int categoryNum, short int productNum, bool includeStock = false, bool includeCost = false); // For getting the stock availability of the aforementioned product

// Lets declare the variables for the functions
void purchaseComponent(int category, int part);
void help(bool brief);
void reset();
void writeFile();
void readFile();
void tutorial();

// Now we're going to write out statements for our function...
void readFile() {
	std::ifstream fileIn("purchaseFile.txt");

	std::string productName;
	int productQuantity;
	int productPurchased;
	float overallCost;
	std::string productCategory;

	std::cout << std::setw(15) << "Category" << std::setw(15) << "Name" << std::setw(15) << "Stock" << std::setw(15) << "Purchased" << std::setw(15) << "Total Spent" << std::endl;
	while (fileIn >> productName >> productQuantity >> productPurchased >> overallCost >> productCategory) {
		std::cout << std::setw(15) << productCategory << std::setw(15) << productName << std::setw(15) << productQuantity << std::setw(15) << productPurchased << std::setw(15) << std::fixed << std::setprecision(2) << overallCost << std::endl;
	};
	
	fileIn.close();
}

// Writes to the sequential data file
void writeFile() {
	std::ofstream fileOut("purchaseFile.txt");

	float totalSpent;

	for (int i = 0; i < productSize; i++) {
		for (int x = 0; x < powerSize; x++) {
			if (physicalProducts[i][x] != "") { // THE "" means that the name is *NOT* declared
				fileOut << std::setw(15) << physicalProductsFileFriendly[i][x]; // Name
				fileOut << std::setw(15) <<productPurchases[i][x][0]; // Stock
				fileOut << std::setw(15) << productPurchases[i][x][1] << "\t"; // Purchased
				fileOut << std::setw(15) << std::fixed << std::setprecision(2) << productPurchases[i][x][1] * productPrices[i][x] << "\t"; // Overall Cost
				fileOut << std::setw(15) << productCategories[i] << "\t" << std::endl; // Category
			}
		}
	}

	fileOut.close();
}

// Returns product information
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

	std::cout << "\nSelection (ex. 0): ";
	std::cin >> selection;

	for (int i = 0; i < productSize; i++) {
		if (selection == i) {
			selection = 0;
			// std::cout << "You have selected: " << productCategories[i] << "... \n";
			std::cout << "\nWhich item would you like to select? \n";

			for (int y = 0; y < physicalProductSizes[i]; y++) {
				std::cout << "\t" << y << ": " << physicalProducts[i][y] << std::endl; // powerCategories[y]
			}

			std::cout << "\nSelection (ex. 0): ";
			std::cin >> selection;

			std::cout << "\nYou have selected: " << physicalProducts[i][selection] << "\n";

			std::cout << "\t" << std::setw(16) << "Product Cat.: " << std::setw(15) << productCategories[i] << std::endl;
			std::cout << "\t" << std::setw(16) << "Product Name: " << std::setw(15) << physicalProducts[i][selection] << std::endl;
			std::cout << "\t" << std::setw(16) << "Product Cost: " << std::setw(15) << std::fixed << std::setprecision(2) <<  getProductInfo(i, selection, false, true) << std::endl; // productPrices[i][selection]
			std::cout << "\t" << std::setw(16) << "Product Stck: " << std::setw(15) << std::fixed << std::setprecision(0) << getProductInfo(i, selection, true, false) << std::endl;
			std::cout << "\t" << std::setw(16) << "Stock Total: " << std::setw(15) << std::fixed << std::setprecision(2) << (productPrices[i][selection] * (float)productPurchases[i][selection][0]) << std::endl;

			if (purchasePrompt == true) {
				std::string validation;
				std::cout << "\nWould you like to purchase? (y/n): ";
				std::cin >> validation;

				if (validation == "y") {
					purchaseComponent(i, selection);
				} else {
					std::cout << "\nReturning to menu..." << std::endl;
				}
			}

			break; // WITHOUT THIS BREAK, IT WILL FALL THROUGH BECAUSE WE CHANGED THE SELECTION VALUE!
		}
	}
}

// Returns product info pertaining to stock or cost
float getProductInfo(short int categoryNum, short int productNum, bool includeStock, bool includeCost) {
	if (includeStock == true) {
		// STOCK NOT A FEATURE AS OF RIGHT NOW!
		return productPurchases[categoryNum][productNum][0]; // STOCK REMAINING
	}

	if (includeCost == true) { // COST OF PRODUCT
		return productPrices[categoryNum][productNum];
	}
}

// Something that is useless...
void purchaseComponent(bool test) {
	if (test == true) {
		std::cout << "Testing purchase function!" << std::endl;
	}
}

// Function below will allow us to purchase items
void purchaseComponent(int categoryNum, int partNum) {
	int quantity;
	std::cout << "\nHow many items would you like to purchase? (ex: 0): ";
	std::cin >> quantity;

	if (quantity >= 0) {
		// [0] += quantity;
		if ((productPurchases[categoryNum][partNum][0] - quantity) >= 0) {
			productPurchases[categoryNum][partNum][0] -= quantity; // Subtraction Assignment
			productPurchases[categoryNum][partNum][1] += quantity; // Addition Assignment
			
		} else {
			std::cout << "Too many items are being purchased, try with a lesser quantity!" << std::endl;
		}
	} else {
		// CANNOT PURCHASE LESS THAN ZERO!
		std::cout << "Something went wrong OR you were purchasing less than ZERO items!";
	}

}

// Displays the command documentation
void help(bool brief) {
	
	// ESCAPE SEQUENCES: https://en.cppreference.com/w/cpp/language/escape
	std::cout << "COMMAND LIST: \n";

	if (brief == true) {
		for (int i = 0; i < commandDim1; i++) {
			for (int y = 0; y < commandDim2; y++) {
				switch (y) {
				case 0:
					std::cout << "\t" << commandName[i][y] << ": ";
					break;
				case 1:
					std::cout << commandName[i][y] << "\n";
					break;
				}
			}
		}
	} else {
		for (int i = 0; i < commandDim1; i++) {
			for (int y = 0; y < commandDim2; y++) {
				switch (y) {
				case 0:
					std::cout << "\t" << commandName[i][y] << "\n";
					break;
				}
			}
		}
	}

	

	std::cout << "\n";
}

// Resets data provided by the user
void reset() {
	for (int i = 0; i < productSize; i++) {
		for (int x = 0; x < powerSize; x++) {
			productPurchases[i][x][1] = 0;
		}
	}
}

int main() {
	std::cout << "Welcome to DC Central! \n" << std::endl;
	tutorial();

	while (true) { // Beginning of main program loop which includes basic setup...
		std::string localInput; // Will be utilized to gather informatoin from user
		localInput = "";

		std::cout << "\nType \"help\" for more info!\n";

		// Command loop
		while (true) {
			localInput = "";
			std::cout << ">> ";
			std::cin >> localInput;

			if (localInput == "help") { // Displays command documentation
				std::cout << "\n***********************************************\n";

				std::cout << "Do you want the description included? ([y]/n): ";
				localInput = "";

				std::cin >> localInput;

				std::cout << "\n";

				if (localInput == "n") { // Shortened documentation
					help(false);
				}
				else { // Detailed documentation
					help(true);
				}

				std::cout << "\n***********************************************\n";
				std::cout << "\n";
			} else if (localInput == "purchase") { // Purchase process
				std::cout << "\n***********************************************\n";
				getProductInfo(true);
				std::cout << "\n***********************************************\n";
				std::cout << "\n";
			} else if (localInput == "productinfo") { // Product information request
				std::cout << "\n***********************************************\n";
				getProductInfo(false);
				std::cout << "\n***********************************************\n";
				std::cout << "\n";
			} else if (localInput == "savepurchase") { // Save to sequential file
				writeFile();
				std::cout << "Your purchase has been saved! \n" << std::endl;
			} else if (localInput == "reviewpurchase") { // Read from sequential file
				std::cout << "\n***********************************************\n";
				readFile();
				std::cout << "\n***********************************************\n";
				std::cout << "\n";
			} else if (localInput == "reset") { // Reset user-provided data
				reset();
				std::cout << "Resetting profile!" << std::endl;
				std::cout << "\n";
				break;
			} else if (localInput == "exit") { // Terminate program
				std::cout << "Terminating program!";
				exit(0);		
			} else if (localInput == "tutorial") { // Show a tutorial
				std::cout << "\n***********************************************\n";
				tutorial();
				std::cout << "\n***********************************************\n";
				std::cout << "\n";
			}
		}
	}; 
	
	return 0;
}