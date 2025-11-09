#include <iostream>
#include<fstream>
#include<string>
#include <vector>
#include<Windows.h>
#include <algorithm>
#include <cctype> 
//cctype and algorithm library: for lowering string (Took from the Internet)
using namespace std;
class BudgetOfRestaurant {
	double budget = 20000;
	double profitLoss = 0;
	double monthlyPNL = 0; // fetch from file
	double dailyPNL = 0; // make it reset in every run(every run == new day so dailyPNL=0)
public:
	BudgetOfRestaurant() {
		try {
			loadData();
		}
		catch (exception ex) {
			cout << ex.what();
			saveData();
		}
	}
	void CalculateProfit(double amountMoney) {
		dailyPNL += amountMoney;
		monthlyPNL += amountMoney;
		//cout << dailyPNL << endl;
		//cout << "MonthlyPNL: " << monthlyPNL << endl;
		budget += amountMoney;
		saveData();
	}
	void ResetDailyPNL() {
		dailyPNL = 0;
	}
	void ResetMontlyPNL() {
		monthlyPNL = 0;
	}
	double ProfitOrLoss() {
		return budget;
	}
	void saveData() {
		//monthlyPNL += dailyPNL;
		ofstream file_budget("Budget.txt", ios::out);
		if (file_budget.is_open()) {
			file_budget << budget << "#" << monthlyPNL;
		}
		else {
			throw exception("File could not be opened\n");
		}
		file_budget.close();
	}
	void loadData() {
		ifstream file_budget("Budget.txt", ios::in);
		if (file_budget.is_open()) {
			while (!file_budget.eof()) {

				string strM_pnl;
				string strBudget;
				string row;
				getline(file_budget, row);
				int counter = 0;
				for (auto& ch_budget : row) {
					if (ch_budget != ':' && counter == 0 && ch_budget != '#') {
						strBudget += ch_budget;
					}
					else if (ch_budget != ':' && counter == 1 && ch_budget != '#') {
						strM_pnl += ch_budget;
					}
					else {
						counter++;
					}
				}
				string newStr_M_pnl;
				for (auto& ch : strM_pnl) {
					if (ch == '-') {
						ch = '-';
					}

				}
				budget = stod(strBudget);
				monthlyPNL = stod(strM_pnl);
			}
		}
		else {
			throw exception("File could not be opened\n");
		}
		file_budget.close();
	}

};
BudgetOfRestaurant budget;

class Ingredient {

	string name;
	int count;
	double price;

public:
#pragma region Setter&Getters
	void SetName(string name) {
		if (name.length() > 2) {
			this->name = name;
		}
		else {
			throw exception("Invalid ingredient's name input\n");
		}
	}

	void SetCount(int count) {
		if (count >= 0) {
			this->count = count;
		}
		else {
			throw exception("Invalid ingredient's count input\n");
		}
	}

	void SetPrice(double price) {
		if (price >= 0) {
			this->price = price;
		}
		else {
			throw exception("Invalid ingredient's price input\n");
		}
	}

	string GetName() const noexcept {
		return name;
	}

	int GetCount() const noexcept {
		return count;
	}

	double GetPrice() const noexcept {
		return price;
	}

#pragma endregion
	Ingredient(string name, double price, int count = 0) {
		SetName(name);
		SetCount(count);
		SetPrice(price);

	}
	virtual void ShowIngredientInfo() const {
		cout << "--------------------" << name << "--------------------" << endl;
		cout << "Name: " << name << endl;
		cout << "Count: " << count << endl;
		cout << "Price(for one): $" << price << endl;
	}
	double GetTotalPrice() const {
		return price * count;
	}
};
class Depo {
	vector<shared_ptr<Ingredient>> ingredients;


public:
	Depo() {
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Tomato", 1.5, 50)));
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Cheese", 2.0, 50)));
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Flour", 1.0, 50)));
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Beef", 5.0, 50)));
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Chicken", 4.0, 50)));
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Rice", 1.2, 50)));
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Fish", 6.0, 50)));
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Egg", 0.8, 50)));
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Cream", 1.3, 50)));
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Bread", 1.0, 50)));
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Oil", 4.0, 50)));
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Ketchup", 1.0, 50)));
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Cucumber", 0.9, 50)));
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Mushroom", 1.9, 50)));
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Pasta", 1.4, 50)));
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Butter", 2.0, 50)));
		ingredients.push_back(shared_ptr<Ingredient>(new Ingredient("Lemon", 1.5, 50)));
	};

	void AddIngredient(shared_ptr<Ingredient> ingr, int count = 1) {
		if (!ingr) {
			throw exception("Null pointer passed as ingredient!\n");
		}
		if (count <= 0) {
			throw exception("Count must be positive!\n");
		}

		int index = FindIngredient(ingr->GetName());
		if (index != -1) {
			cout << ingr->GetName();
			throw exception(" Ingredient already exists\n");
		}

		ingr->SetCount(count);
		ingredients.push_back(ingr);
		cout << "New " << count << " " << ingr->GetName() << " added to the stock!\n";
		budget.CalculateProfit(-(ingr->GetPrice() * ingr->GetCount()));
	}

	void IncreaseNumberOfIngredients(const string& name, int count) {
		if (count <= 0) {
			throw exception("Increse by postive numbers only!\n");
		}
		int index = FindIngredient(name);
		int newCount = ingredients[index]->GetCount() + count;
		if (index != -1) {
			ingredients[index]->SetCount(newCount);
			cout << "Number of " << name << " was increased in depo by " << count << " to " << newCount << endl;
		}
		else {
			throw exception("There is no such ingredient in stock!\n");
		}
		budget.CalculateProfit(-(ingredients[index]->GetPrice() * newCount));
	}
	void DecreaseNumberOfIngredients(const string& name, int count) {
		if (count <= 0) {
			throw exception("Decrease Error: Number must be positive!\n");
		}
		int index = FindIngredient(name);
		if (index != -1) {
			int newCount = ingredients[index]->GetCount() - count;
			if (newCount < 0) {
				cout << name;
				throw exception("; We do not have that much in the stock, sorry!\n");
			}
			else {
				ingredients[index]->SetCount(newCount);
				cout << "Number of " << name << " was decreased from depo by " << count << " to " << newCount << "!\n";
			}
		}
		else {
			throw exception("There is no such ingredient in stock!\n");
		}
	}

	void SetNumberOfIngredients(const string& name, int count) {
		if (count <= 0) {
			throw exception("Only positive numbers are accepted\n");
		}
		int index = FindIngredient(name);
		if (index != -1) {
			ingredients[index]->SetCount(count);
			cout << "Number of " << name << " was changed in depo by " << count << " to " << count << endl;
		}
		else {
			throw exception("There is no such ingredient in stock!\n");
		}
		budget.CalculateProfit(-(ingredients[index]->GetPrice() * count));
	}

	void DeleteIngredient(const string& name) {
		int index = FindIngredient(name);
		if (index != -1) {
			ingredients.erase(ingredients.begin() + index);
			cout << name << " was removed from the stock!\n";
		}
		else {
			throw exception("There is no such ingredient in the stock!\n");
		}
	}

	int FindIngredient(const string& name) const {
		for (int i = 0; i < ingredients.size(); i++) {
			string nameOfIngredient = ingredients[i]->GetName();
			string copyOfTakenName = name;
			transform(nameOfIngredient.begin(), nameOfIngredient.end(), nameOfIngredient.begin(), ::tolower);
			transform(copyOfTakenName.begin(), copyOfTakenName.end(), copyOfTakenName.begin(), ::tolower);
			if (nameOfIngredient == copyOfTakenName)
				return i;
		}

		return -1;
	}
	shared_ptr<Ingredient> GetIngredient(const string name) {
		int index = FindIngredient(name);
		if (index != -1) {
			return ingredients[index];
		}
		else {
			throw exception("There is no such ingredient in depo to add to the meal\n");
		}
	}
	void ShowAll() const {
		if (ingredients.empty()) {
			throw exception("Empty Depo\n");
		}
		for (const auto& ingr : ingredients)
			ingr->ShowIngredientInfo();
	}

	void ShowSingle(string name) {
		int index = FindIngredient(name);
		if (index != -1) {
			ingredients[index]->ShowIngredientInfo();

		}
		else {
			throw exception("There is no such ingredient in stock!\n");
		}
	}
	double TotalValue() {
		double total_value = 0;
		for (const auto& ingr : ingredients) {
			total_value += ingr->GetTotalPrice();
		}
		return total_value;
	}
};

class ReceiptItem {
	shared_ptr<Ingredient> ingredient;
	int amount;
	double pricePerKg;
public:
#pragma region Setters&Getters
	void SetAmount(int amount) {
		if (amount > 0) {
			this->amount = amount;
			return;
		}
		throw exception("Amount of ingredients in receipt must be more than zero!\n");
	}
	void SetPrice(double price) {
		if (price > 0) {
			pricePerKg = price;
			return;
		}
		throw exception("Price(per kg) must be more than zero!\n");
	}
	int GetAmount() const noexcept {
		return amount;
	}
	double GetPrice() const noexcept {
		return pricePerKg;
	}
	string GetName() {
		return ingredient->GetName();
	}

#pragma endregion
	ReceiptItem(shared_ptr <Ingredient> ingr, int amount, double price) {
		if (ingr == nullptr) {
			throw exception("Ingredient is null!\n");
		}
		ingredient = ingr;
		SetAmount(amount);
		SetPrice(price);
	}

};



class Meal {

protected:
	vector <shared_ptr <ReceiptItem>> ingredients;
	string name;
	double price = 0;
public:
#pragma region Setters&Getters
	void SetName(string name) {
		if (!(name.length() > 3)) {
			throw exception("Invalid name length\n ");
		}
		this->name = name;
	}
	void SetPrice(double price) {
		if (price > 0) {
			this->price = price;
		}
		else {
			throw exception("Invalid count input\n ");
		}
	}
	string GetName() const noexcept {
		return this->name;
	}
	double GetPrice() const noexcept {
		return this->price;
	}
#pragma endregion
	Meal(string name) {
		this->name = name;
	}
	void AddIngredientToMeal(Depo& d, shared_ptr<Ingredient> ingr, int amount = 1) {
		//Ingredient* mealIngr = ingr;
		if (d.FindIngredient(ingr->GetName()) == -1) {
			throw exception("Sorry, this ingredient is not available in our depo at the moment.\n");

		}
		double price = ingr->GetPrice();
		shared_ptr<ReceiptItem> rItem(new ReceiptItem(ingr, amount, price));
		ingredients.push_back(rItem);
		d.DecreaseNumberOfIngredients(ingr->GetName(), amount);
		cout << amount << " " << ingr->GetName() << " added to meal: " << name << endl;
		//budget.CalculateProfit(*this);

	}
	void PrintIngredients() const {
		int counter = 0;
		if (ingredients.empty()) {
			cout << name;
			throw exception(" has no ingredients \n");
		}
		cout << "Ingredients of " << name << ": \n";
		for (const auto& ingr : ingredients) {


			cout << " " << ++counter << ". " << ingr->GetName() << "; Price: $" << ingr->GetPrice() << "; Amount: " << ingr->GetAmount() << " Slice(s)\n";
		}
	}
	double TotalPriceOfMeal() {
		price = 0;
		for (const auto& ingr : ingredients) {
			price += ingr->GetPrice() * ingr->GetAmount();
		}
		price *= 3.65; // service fee and profit of restaurant
		budget.CalculateProfit(price);
		return price;
	}
	double TotalPriceOfIngredients() {
		double price_ingr = 0;
		for (const auto& ingr : ingredients) {
			price_ingr += ingr->GetPrice() * ingr->GetAmount();
		}
		return price_ingr;
	}
	int FindIngredient(string name) {
		for (int i = 0; i < ingredients.size(); i++)
		{
			if (ingredients[i]->GetName() == name) {
				return i;
			}
		}
		return -1;
	}
	void DeleteIngredient(string name) {
		int index = FindIngredient(name);
		if (index != -1) {
			ingredients.erase(ingredients.begin() + index);
			return;
		}
		cout << name;
		throw exception("; There is no such ingredient\n");
	}
	shared_ptr <ReceiptItem> GetIngredient(string name) {
		int index = FindIngredient(name);
		if (index != -1) {
			return ingredients[index];

		}
		throw exception("No such ingredient\n");
	}
	void ChangeNumberOfIngredient(string name, int count) {
		int index = FindIngredient(name);
		if (index != -1) {
			if (count > 0) {
				ingredients[index]->SetAmount(count);
			}
			else {
				cout << "Use delete function instead" << endl;
			}
		}
	}
};
class Menu {
	vector<shared_ptr<Meal>> meals;
public:
	Menu(Depo& d) {
		try {
			auto pizza = shared_ptr<Meal>(new Meal("Pizza"));
			pizza->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Cheese", 1.5)), 2);
			pizza->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Tomato", 1.0)), 1);
			meals.push_back(pizza);
		}
		catch (exception& ex) {
			cout << "Error Pizza: " << ex.what() << endl;
		}

		try {
			auto pasta = shared_ptr<Meal>(new Meal("Pasta"));
			pasta->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Pasta", 0.8)), 2);
			pasta->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Cream", 1.2)), 1);
			pasta->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Cheese", 1.5)), 1);
			meals.push_back(pasta);
		}
		catch (exception& ex) {
			cout << "Error Pasta: " << ex.what() << endl;
		}

		try {
			auto burger = shared_ptr<Meal>(new Meal("Burger"));
			burger->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Bread", 0.5)), 2);
			burger->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Beef", 3.0)), 1);
			meals.push_back(burger);
		}
		catch (exception& ex) {
			cout << "Error Burger: " << ex.what() << endl;
		}

		try {
			auto riceMeal = shared_ptr<Meal>(new Meal("RiceMeal"));
			riceMeal->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Rice", 0.5)), 2);
			riceMeal->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Cream", 0.8)), 1);
			meals.push_back(riceMeal);
		}
		catch (exception& ex) {
			cout << "Error RiceMeal: " << ex.what() << endl;
		}

		try {
			auto chicken = shared_ptr<Meal>(new Meal("ChickenSteak"));
			chicken->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Chicken", 2.5)), 1);
			chicken->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Ketchup", 0.5)), 1);
			meals.push_back(chicken);
		}
		catch (exception& ex) {
			cout << "Error ChickenSteak: " << ex.what() << endl;
		}

		try {
			auto beef = shared_ptr<Meal>(new Meal("BeefSteak"));
			beef->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Beef", 3.5)), 1);
			beef->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Oil", 0.8)), 1);
			meals.push_back(beef);
		}
		catch (exception& ex) {
			cout << "Error BeefSteak: " << ex.what() << endl;
		}

		try {
			auto salad = shared_ptr<Meal>(new Meal("Salad"));
			salad->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Tomato", 0.5)), 2);
			salad->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Cucumber", 0.6)), 2);
			salad->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Oil", 1)), 1);
			meals.push_back(salad);
		}
		catch (exception& ex) {
			cout << "Error Salad: " << ex.what() << endl;
		}

		try {
			auto omelette = shared_ptr<Meal>(new Meal("Omelette"));
			omelette->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Egg", 0.4)), 3);
			omelette->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Butter", 0.8)), 1);
			meals.push_back(omelette);
		}
		catch (exception& ex) {
			cout << "Error Omelette: " << ex.what() << endl;
		}

		try {
			auto sushi = shared_ptr<Meal>(new Meal("Sushi"));
			sushi->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Rice", 0.4)), 2);
			sushi->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Fish", 3.0)), 1);
			meals.push_back(sushi);
		}
		catch (exception& ex) {
			cout << "Error Sushi: " << ex.what() << endl;
		}

		try {
			auto grilledFish = shared_ptr<Meal>(new Meal("GrilledFish"));
			grilledFish->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Fish", 3.0)), 1);
			meals.push_back(grilledFish);
		}
		catch (exception& ex) {
			cout << "Error GrilledFish: " << ex.what() << endl;
		}

	}
	int FindMeal(string name) {
		for (int i = 0; i < meals.size(); i++)
		{
			if (meals[i]->GetName() == name) {
				return i;
			}
		}
		return -1;
	}
	void AddMeal(Depo& d, const string name, int count = 1) {
		auto new_meal = shared_ptr<Meal>(new Meal(name));
		for (size_t i = 0; i < count; i++)
		{
			string name_of_ingredient;
			cout << "Enter name of ingredient to add for " << new_meal->GetName() << ": ";
			cin >> name_of_ingredient;
			int index = d.FindIngredient(name_of_ingredient);
			if (index != -1) {
				int amount = 0;
				auto ingredients = d.GetIngredient(name_of_ingredient);
				cout << "Set amount of this ingredient: ";
				cin >> amount;
				new_meal->AddIngredientToMeal(d, ingredients, amount);
			}
			else {
				throw exception("There is no such ingredient\n");
			}

		}
		meals.push_back(new_meal);
		cout << "Meal " << name << " added to menu.\n";
	}
	void DeleteIngredientOfMeal(Depo& d, const string name) {
		int index = FindMeal(name);

		string ingredientName;
		if (index != -1) {
			meals[index]->PrintIngredients();
			cout << "Which ingredient to delete: ";
			cin >> ingredientName;
			auto ingredient = meals[index]->GetIngredient(ingredientName);
			meals[index]->DeleteIngredient(ingredientName);
			d.IncreaseNumberOfIngredients(ingredientName, ingredient->GetAmount());
		}
		else {
			throw exception("There is no such meal\n");
		}
	}
	void ChangeNumOfIngredients(Depo& d, const string name) {
		int index = FindMeal(name);
		int count = 0;
		string ingredientName;
		if (index != -1) {
			meals[index]->PrintIngredients();
			cout << "Which ingredient to change: ";
			cin >> ingredientName;
			cout << "To what number?: ";
			cin >> count;

			auto ingredient = meals[index]->GetIngredient(ingredientName);
			if (ingredient->GetAmount() < count) {
				d.DecreaseNumberOfIngredients(ingredientName, count - ingredient->GetAmount());
			}
			else if (ingredient->GetAmount() > count) {
				d.IncreaseNumberOfIngredients(ingredientName, ingredient->GetAmount() - count);
			}
			else {
				cout << "Same set number";
				return;
			}
			meals[index]->ChangeNumberOfIngredient(name, count);
		}
	}
	void PrintMenu() {
		cout << "---MENU ---(CONSIDER 465% fee for restaurant for each price): \n";
		int counter = 0;
		for (auto& MenuItem : meals) {
			cout << ++counter << ". " << MenuItem->GetName() << " $" << MenuItem->TotalPriceOfIngredients() << endl;
		}
	}

};


class Admin {
	BudgetOfRestaurant& budget;
	Depo& depo;
	string username;
	string password;
public:
	void saveData() {
		cout << "Saving..\n";
		ofstream file_Admin("Admin.txt", ios::out);
		if (file_Admin.is_open()) {
			file_Admin << "admin" << "#" << "!admin123";
		}
		else {
			throw exception("File could not be opened\n");
		}
		file_Admin.close();
	}
	void loadData() {
		ifstream file_Admin("Admin.txt", ios::in);
		if (file_Admin.is_open()) {
			while (!file_Admin.eof()) {
				string row;
				getline(file_Admin, row);
				if (row.length() == 0) {
					throw string("File is empty...");
				}
				string str_username;
				string str_password;
				int counter = 0;
				for (auto& ch_Admin : row) {
					if (ch_Admin != '#' && counter == 0) {
						str_username += ch_Admin;
					}
					else if (ch_Admin != '#' && counter == 1) {
						str_password += ch_Admin;
					}
					else {
						counter++;
					}
				}
				password = str_password;
				username = str_username;
			}
		}
		else {
			throw exception("File could not be opened Read\n");
		}
		file_Admin.close();
	}
	Admin(string username, string password, BudgetOfRestaurant& b, Depo& d) : budget(b), depo(d) {
		try {
			loadData();
		}
		catch (exception& ex) {
			saveData();
			loadData();
			cout << ex.what() << endl;
		}
		transform(username.begin(), username.end(), username.begin(), ::tolower);
		if (username == this->username && password == this->password) {
			cout << "Logged in" << endl;
		}
		else {
			cout << "Failed\n";
		}
	}

};


int main() {
	Depo d;
	Admin admin("admin", "!admin123", budget, d);
	Menu m(d);
	m.PrintMenu();
	try {

		//m.AddMeal(d, "Kabab", 1);
	}
	catch (exception ex) {
		cout << ex.what() << endl;
	}
	m.PrintMenu();
	d.ShowAll();
	//m.DeleteIngredientOfMeal(d, "Pasta");
	m.ChangeNumOfIngredients(d, "RiceMeal");
	d.ShowAll();

	return 0;
}