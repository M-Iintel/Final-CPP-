#include <iostream>
#include<fstream>
#include<string>
#include<sstream> // for checking whether inputted number is int or no (Took from the Internet)
#include <vector>
#include<windows.h>
#include<conio.h>
#include <algorithm> //algorithm library: for lowering string (Took from the Internet)

using namespace std;
void SetColor(int textColor)
{
	cout << "\033[" << textColor << "m";
}

void ResetColor() { cout << "\033[0m"; }
string getHiddenInput(const string& prompt = "Enter password: ") {
	cout << prompt;
	string password;
	char ch;

	while (true) {
		ch = _getch();
		if (ch == 13) {
			cout << endl;
			break;
		}
		else if (ch == 8) {
			if (!password.empty()) {
				cout << "\b \b";
				password.pop_back();
			}
		}
		else if (isprint(ch)) {
			password += ch;
			cout << '*';
		}
	}
	return password;
}
// SetColor,ResetColor, and getHiddenInput were taken from the Internet

bool isInteger(const string input) {
	if (input.empty()) {
		return false;
	}

	stringstream ss(input);
	int number;
	ss >> number;

	if (!ss.eof()) {
		return false;
	}

	return true;
}
void loadingAnimation() {
	for (size_t i = 0; i < 3; i++)
	{
		cout << ".";
		Sleep(750);
	}
}
class BudgetOfRestaurant {
	double budget = 20000;
	double default_budget = 20000;
	double monthlyPNL = 0; // will be gotten from file
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
		budget += amountMoney;
		if (budget <= 0) {
			SetColor(31);
			cout << "Budget is not enough. Please, restart application!\n";
			ResetColor();
			terminate();
		}
		saveData();
	}
	void ResetDailyPNL() {
		dailyPNL = 0;
	}
	void ResetMontlyPNL() {
		monthlyPNL = 0;
	}
	double ProfitOrLoss() noexcept {
		return budget - default_budget;
	}
	double GetDailyPNL()const noexcept {
		return dailyPNL;
	}
	double GetMonthlyPNL()const noexcept {
		return monthlyPNL;
	}
	double GetDefaultBudget()const noexcept {
		return default_budget;
	}
	double GetRemainedBudget() const noexcept {
		return budget;
	}
	void saveData() {
		//monthlyPNL += dailyPNL;
		ofstream file_budget("Budget.txt", ios::out);
		if (file_budget.is_open()) {
			file_budget << budget << "#" << monthlyPNL;
		}
		else {
			throw exception("Budget file could not be opened\n");
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
				if (row.length() == 0) {
					throw exception("Empty file\n");
				}
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
			throw exception("Budget file could not be opened\n");
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
			for (size_t i = 0; i < name.length(); i++)
			{
				if (int(name[i]) >= 48 && int(name[i]) <= 57) {
					throw exception("Invalid ingredient's name input: Integers are not allowed\n");
				}
			}
			this->name = name;
		}
		else {
			throw exception("Invalid ingredient's name length input\n");
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
		if (!name.empty()) {
			name[0] = toupper(name[0]);
		}
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
		try {
			loadDataDepo();
		}
		catch (exception ex) {
			cout << ex.what();
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
		}

	};
	void saveDataDepo() {
		ofstream file_depo("Depo.txt", ios::out);
		if (file_depo.is_open()) {
			for (size_t i = 0; i < ingredients.size(); i++)
			{
				if (i == ingredients.size() - 1) {
					file_depo << ingredients[i]->GetName() << "#" << ingredients[i]->GetPrice() << "#" << ingredients[i]->GetCount();
					continue;
				}
				file_depo << ingredients[i]->GetName() << "#" << ingredients[i]->GetPrice() << "#" << ingredients[i]->GetCount() << "\n";

			}
		}
		else {
			throw exception("Depo File could not be opened\n");
		}
		file_depo.close();
	}
	void loadDataDepo() {
		ifstream file_depo("Depo.txt", ios::in);
		if (file_depo.is_open()) {
			while (!file_depo.eof()) {
				string row;
				getline(file_depo, row);
				if (row.length() == 0) {
					throw exception("File is empty\n");
				}
				int counter = 0;
				string name;
				string str_price;
				string str_count;
				for (auto& ch : row) {
					if (ch != '#' && counter == 0) {
						name += ch;
					}
					else if (ch != '#' && counter == 1) {
						str_price += ch;
					}
					else if (ch != '#' && counter == 2) {
						str_count += ch;
					}
					else {
						counter++;
					}
				}
				double price_file = stod(str_price);
				int count_file = stoi(str_count);
				string name_file = name;
				shared_ptr<Ingredient> ingredient_file(new Ingredient(name_file, price_file, count_file));
				ingredients.push_back(ingredient_file);
			}
		}
		else {
			throw exception("Depo File could not be opened\n");
		}
		file_depo.close();

	}
	void AddIngredient(shared_ptr<Ingredient> ingr, int count = 1) {
		if (!ingr) {
			throw exception("Null pointer can not be ingredient!\n");
		}
		if (count <= 0) {
			throw exception("Count must be positive!\n");
		}
		if (!ingr->GetName().empty()) {
			ingr->GetName()[0] = toupper(ingr->GetName()[0]);
		}

		int index = FindIngredient(ingr->GetName());
		if (index != -1) {
			cout << ingr->GetName();
			throw exception(" Ingredient already exists\n");
		}

		ingr->SetCount(count);
		budget.CalculateProfit(-(ingr->GetPrice() * ingr->GetCount()));
		ingredients.push_back(ingr);
		cout << "New " << count << " " << ingr->GetName() << " added to the stock!\n";
	}

	void IncreaseNumberOfIngredients(string name, int count) {
		if (count <= 0) {
			throw exception("Increse by postive numbers only!\n");
		}
		int index = FindIngredient(name);

		if (index != -1) {
			budget.CalculateProfit(-(ingredients[index]->GetPrice() * count));
			int newCount = ingredients[index]->GetCount() + count;
			ingredients[index]->SetCount(newCount);
			cout << "Number of " << name << " was increased in depo by " << count << " to " << newCount << endl;
			saveDataDepo();
		}
		else {
			throw exception("There is no such ingredient in stock!\n");
		}
	}
	void DecreaseNumberOfIngredients(const string& name, int count) {
		if (count <= 0) {
			throw exception("Decrease ERROR: Number must be positive!\n");
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
				saveDataDepo();
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
			budget.CalculateProfit(-(ingredients[index]->GetPrice() * count));
			ingredients[index]->SetCount(count);
			cout << "Number of " << name << " was changed in depo by " << count << " to " << count << endl;
			saveDataDepo();
		}
		else {
			throw exception("There is no such ingredient in stock!\n");
		}
	}

	void DeleteIngredient(const string& name) {
		int index = FindIngredient(name);
		if (index != -1) {
			ingredients.erase(ingredients.begin() + index);
			cout << name << " was removed from the stock!\n";
			saveDataDepo();
		}
		else {
			throw exception("There is no such ingredient in the stock!\n");
		}
	}

	int FindIngredient(const string name) const {
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
	void AddIngredientToMeal(Depo& d, shared_ptr<Ingredient> ingr, int amount) {
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
			pizza->AddIngredientToMeal(d, d.GetIngredient("Cheese"), 2);
			pizza->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Tomato", 1.0)), 1);
			meals.push_back(pizza);
		}
		catch (exception& ex) {
			cout << "ERROR Pizza: " << ex.what() << endl;
		}

		try {
			auto pasta = shared_ptr<Meal>(new Meal("Pasta"));
			pasta->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Pasta", 0.8)), 2);
			pasta->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Cream", 1.2)), 1);
			pasta->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Cheese", 1.5)), 1);
			meals.push_back(pasta);
		}
		catch (exception& ex) {
			cout << "ERROR Pasta: " << ex.what() << endl;
		}

		try {
			auto burger = shared_ptr<Meal>(new Meal("Burger"));
			burger->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Bread", 0.5)), 2);
			burger->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Beef", 3.0)), 1);
			meals.push_back(burger);
		}
		catch (exception& ex) {
			cout << "ERROR Burger: " << ex.what() << endl;
		}

		try {
			auto riceMeal = shared_ptr<Meal>(new Meal("RiceMeal"));
			riceMeal->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Rice", 0.5)), 2);
			riceMeal->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Cream", 0.8)), 1);
			meals.push_back(riceMeal);
		}
		catch (exception& ex) {
			cout << "ERROR RiceMeal: " << ex.what() << endl;
		}

		try {
			auto chicken = shared_ptr<Meal>(new Meal("ChickenSteak"));
			chicken->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Chicken", 2.5)), 1);
			chicken->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Ketchup", 0.5)), 1);
			meals.push_back(chicken);
		}
		catch (exception& ex) {
			cout << "ERROR ChickenSteak: " << ex.what() << endl;
		}

		try {
			auto beef = shared_ptr<Meal>(new Meal("BeefSteak"));
			beef->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Beef", 3.5)), 1);
			beef->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Oil", 0.8)), 1);
			meals.push_back(beef);
		}
		catch (exception& ex) {
			cout << "ERROR BeefSteak: " << ex.what() << endl;
		}

		try {
			auto salad = shared_ptr<Meal>(new Meal("Salad"));
			salad->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Tomato", 0.5)), 2);
			salad->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Cucumber", 0.6)), 2);
			salad->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Oil", 1)), 1);
			meals.push_back(salad);
		}
		catch (exception& ex) {
			cout << "ERROR Salad: " << ex.what() << endl;
		}

		try {
			auto omelette = shared_ptr<Meal>(new Meal("Omelette"));
			omelette->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Egg", 0.4)), 3);
			omelette->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Butter", 0.8)), 1);
			meals.push_back(omelette);
		}
		catch (exception& ex) {
			cout << "ERROR Omelette: " << ex.what() << endl;
		}

		try {
			auto sushi = shared_ptr<Meal>(new Meal("Sushi"));
			sushi->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Rice", 0.4)), 2);
			sushi->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Fish", 3.0)), 1);
			meals.push_back(sushi);
		}
		catch (exception& ex) {
			cout << "ERROR Sushi: " << ex.what() << endl;
		}

		try {
			auto grilledFish = shared_ptr<Meal>(new Meal("GrilledFish"));
			grilledFish->AddIngredientToMeal(d, shared_ptr<Ingredient>(new Ingredient("Fish", 3.0)), 1);
			meals.push_back(grilledFish);
		}
		catch (exception& ex) {
			cout << "ERROR GrilledFish: " << ex.what() << endl;
		}

	}
	int FindMeal(string name) {
		if (!name.empty()) {
			name[0] = toupper(name[0]);
		}
		for (int i = 0; i < meals.size(); i++)
		{
			if (meals[i]->GetName() == name) {
				return i;
			}
		}
		return -1;
	}
	void AddMeal(Depo& d, const string name, int count = 1) {
		for (auto& ch : name) {
			if (int(ch) >= 48 && int(ch) <= 57) {
				throw exception("Meal name ERROR: Meal name can not contain integer\n");
			}
		}
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
	void DeleteMeal(const string name) {
		int index = FindMeal(name);

		if (index != -1) {
			string real_name = meals[index]->GetName();
			meals.erase(meals.begin() + index);
			cout << real_name;
			SetColor(32);
			cout << " was successfully deleted\n";
			ResetColor();
		}
		else {
			throw exception("There is no such meal\n");
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
					throw string("File is empty\n");
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
			SetColor(32);
			cout << "Logged in" << endl;
			ResetColor();
		}
		else {
			throw exception("Failed to log in, incorrect password or username\n");
		}
	}

};
void ShowModifyIngredientsMenu(Depo& d) {

	while (true) {
	ModifyIngredientsStart:
		cout << "========== MODIFYING INGREDIENTS ==========" << endl;
		cout << "1. Add New Ingredient To Depo" << endl;
		cout << "2. Increase Number Of Existing Ingredient" << endl;
		SetColor(31);
		cout << "3. Decrease Number Of Existing Ingredient" << endl;
		cout << "4. Delete Ingredient From Depo" << endl;
		ResetColor();
		cout << "5. Back To Admin Panel" << endl;
		cout << "===========================================" << endl;
		cout << "Enter your choice: ";

		char choice = _getch();
		_putch(choice);

		if (int(choice) >= 49 && int(choice) <= 57) {
			switch (int(choice)) {
			case 49: {
				system("cls");
				string name, count_str, price_str;

				cout << "Enter ingredient name: ";
				cin >> name;
				cout << "Enter count: ";
				cin >> count_str;
				cout << "Enter price per unit: ";
				cin >> price_str;
				if (isInteger(count_str) && stod(price_str) > 0) {
					try {
						auto ingr = shared_ptr<Ingredient>(new Ingredient(name, stod(price_str)));
						SetColor(32);
						cout << "Adding ingredient";
						loadingAnimation();
						cout << endl;
						d.AddIngredient(ingr, stoi(count_str));
						ResetColor();
					}
					catch (exception& ex) {
						SetColor(31);
						cout << ex.what() << endl;
						ResetColor();
					}
				}
				else {
					SetColor(31);
					cout << "Invalid input ERROR: ";
					ResetColor();
					cout << "Count and price must be a positive number.\n";
				}
				break;
			}

			case 50: {
				system("cls");
				string name, count_str;
				cout << "Enter ingredient name to increase: ";
				cin >> name;
				cout << "Enter number to add to the existent ingredient: ";
				cin >> count_str;

				if (isInteger(count_str)) {
					try {
						SetColor(32);
						cout << "Increasing count";
						loadingAnimation();
						cout << endl;
						d.IncreaseNumberOfIngredients(name, stoi(count_str));
						ResetColor();
					}
					catch (exception& ex) {
						SetColor(31);
						cout << ex.what() << endl;
						ResetColor();
					}
				}
				else {
					SetColor(31);
					cout << "Invalid input! Enter positive integer only.\n";
					ResetColor();
				}
				break;
			}

			case 51: {
				system("cls");
				string name, count_str;
				cout << "Enter ingredient name to decrease: ";
				cin >> name;
				cout << "Enter number to exclude from the existent ingredient: ";
				cin >> count_str;

				if (isInteger(count_str) > 0) {
					try {
						SetColor(32);
						cout << "Decreasing count";
						loadingAnimation();
						cout << endl;
						d.DecreaseNumberOfIngredients(name, stoi(count_str));
						ResetColor();
					}
					catch (exception& ex) {
						SetColor(31);
						cout << ex.what() << endl;
						ResetColor();
					}
				}
				else {
					SetColor(31);
					cout << "Invalid input! Enter positive integer only.\n";
					ResetColor();
				}
				break;
			}

			case 52: {
			DeleteStart:
				system("cls");
				cout << "Are you sure you want to delete an ingredient?(";
				SetColor(32);
				cout << "Y";
				ResetColor();
				cout << "/";
				SetColor(31);
				cout << "N";
				ResetColor();
				cout << "): ";

				char yes_no = _getch();
				_putch(yes_no);
				cout << endl;
				if (int(yes_no) == 89 || int(yes_no) == 121) {
					string name;
					cout << "\nEnter ingredient name: ";
					cin >> name;
					try {
						d.DeleteIngredient(name);
						SetColor(31);
						cout << "\nIngredient removed successfully.\n";
						ResetColor();
					}
					catch (exception& ex) {
						SetColor(31);
						cout << ex.what() << endl;
						ResetColor();
					}
				}
				else if (int(yes_no) == 78 || int(yes_no) == 110 || int(yes_no) == 27) {
					continue;
				}
				else {
					goto DeleteStart;
				}
				break;
			}


			case 53: {
				cout << "\nReturning to Admin Panel";
				loadingAnimation();
				Sleep(150);
				system("cls");
				return;
			}

			default:
				SetColor(31);
				cout << "\nInvalid choice.\n";
				Sleep(800);
				system("cls");
				ResetColor();
				break;
			}
		}
		else if (int(choice) == 27) {
			return;
		}
		else {
			system("cls");
			goto ModifyIngredientsStart;
		}
	}
}
void ShowSalesReport(BudgetOfRestaurant& b) {
	system("cls");
	SetColor(32);
	cout << "Fetching Data";
	loadingAnimation();
	cout << endl;
	Sleep(300);
SaleReportStart:
	while (true) {

		SetColor(33);
		cout << "======================= BUDGET MANAGEMENT PANEL =======================" << endl;
		ResetColor();

		cout << "1. Show Default (Base) Budget" << endl;
		cout << "2. Show Remaining Budget" << endl;
		cout << "3. Show Daily Profit / Loss" << endl;
		cout << "4. Show Monthly Profit / Loss" << endl;
		cout << "5. Show Profit / Loss Percentage" << endl;
		cout << "6. Show All Data Combined" << endl;
		cout << "7. Back to Admin Panel" << endl;

		SetColor(33);
		cout << "=======================================================================" << endl;
		ResetColor();
		cout << "Enter your choice: ";
		char choice = _getch();
		_putch(choice);
		Sleep(250);
		cout << endl;

		if (int(choice) >= 48 && int(choice) <= 57) {
			double defaultBudget, remainedBudget, dailyPnL, monthlyPnL, totalPnL;
			defaultBudget = b.GetDefaultBudget();
			remainedBudget = b.GetRemainedBudget();
			switch (int(choice)) {
			case 49: {
				system("cls");

				cout << "Base Budget: $";
				SetColor(32);
				cout << defaultBudget;
				cout << endl;
				Sleep(1500);
				ResetColor();
				break;
			}
			case 50: {
				system("cls");
				cout << "Remainig Budget : $";
				if (remainedBudget >= 0) {
					SetColor(32);
					cout << remainedBudget;
					cout << endl;
					Sleep(1500);
					ResetColor();
				}
				else if (remainedBudget < 0) {
					SetColor(31);
					cout << remainedBudget;
					cout << endl;
					Sleep(1500);
					ResetColor();
				}
				break;
			}
			case 51: {
				system("cls");
				dailyPnL = b.GetDailyPNL();
				cout << "Today's Profit / Loss: $";
				if (dailyPnL > 0) {
					SetColor(32);
					cout << dailyPnL;
					cout << endl;
					Sleep(1500);
					ResetColor();
				}
				else if (dailyPnL < 0) {
					SetColor(31);
					cout << dailyPnL;
					cout << endl;
					Sleep(1500);
					ResetColor();
				}
				else {
					SetColor(94);
					cout << dailyPnL;
					cout << endl;
					Sleep(1500);
					ResetColor();
				}
				break;
			}
			case 52: {
				system("cls");
				monthlyPnL = b.GetMonthlyPNL();
				cout << "Today's Profit / Loss: $";
				if (monthlyPnL > 0) {
					SetColor(32);
					cout << monthlyPnL;
					cout << endl;
					Sleep(1500);
					ResetColor();
				}
				else if (monthlyPnL < 0) {
					SetColor(31);
					cout << monthlyPnL;
					cout << endl;
					Sleep(1500);
					ResetColor();
				}
				else {
					SetColor(94);
					cout << monthlyPnL;
					cout << endl;
					Sleep(1500);
					ResetColor();
				}
				break;
			}
			case 53: {
				system("cls");
				double change = ((remainedBudget - defaultBudget) / defaultBudget) * 100.0;
				if (change < 0) {
					cout << "You are not doing well, since your financial performance is negative. ";
					cout << "\nTotal Loss: ";
					SetColor(31);
					cout << change << "%";
					cout << endl;
					Sleep(1500);
					ResetColor();
				}
				else if (change > 0) {
					cout << "You are doing great, your financial performance is positive. ";
					cout << "\nTotal Gain: ";
					SetColor(32);
					cout << change << "% profit";
					cout << endl;
					Sleep(1500);
					ResetColor();
				}
				else {
					cout << "Your financial performance is stable. No profit or loss recorded.\n";
				}
				break;
			}
			case 54: {
				system("cls");

				double defaultBudget = b.GetDefaultBudget();
				double remainedBudget = b.GetRemainedBudget();
				double dailyPnL = b.GetDailyPNL();
				double monthlyPnL = b.GetMonthlyPNL();
				double change = ((remainedBudget - defaultBudget) / defaultBudget) * 100.0;

				cout << "================== FULL FINANCIAL OVERVIEW ==================" << endl;

				cout << "Base Budget: $";
				SetColor(32);
				cout << defaultBudget;
				cout << endl;
				Sleep(1500);
				ResetColor();

				cout << "Remaining Budget: $";
				if (remainedBudget >= 0) {
					SetColor(32);
					cout << remainedBudget;
					cout << endl;
					Sleep(1500);
					ResetColor();
				}
				else {
					SetColor(31);
					cout << remainedBudget;
					cout << endl;
					Sleep(1500);
					ResetColor();
				}

				cout << "Today's Profit / Loss: $";
				if (dailyPnL > 0) {
					SetColor(32);
					cout << dailyPnL;
					cout << endl;
					Sleep(1500);
					ResetColor();
				}
				else if (dailyPnL < 0) {
					SetColor(31);
					cout << dailyPnL;
					cout << endl;
					Sleep(1500);
					ResetColor();
				}
				else {
					SetColor(94);
					cout << dailyPnL;
					cout << endl;
					Sleep(1500);
					ResetColor();
				}

				cout << "Monthly Profit / Loss: $";
				if (monthlyPnL > 0) {
					SetColor(32);
					cout << monthlyPnL;
					cout << endl;
					Sleep(1500);
					ResetColor();
				}
				else if (monthlyPnL < 0) {
					SetColor(31);
					cout << monthlyPnL;
					cout << endl;
					Sleep(1500);
					ResetColor();
				}
				else {
					SetColor(94);
					cout << monthlyPnL;
					cout << endl;
					Sleep(1500);
					ResetColor();
				}

				if (change < 0) {
					cout << "You are not doing well, since your financial performance is negative. ";
					cout << "\nTotal Loss: ";
					SetColor(31);
					cout << change << "%";
					cout << endl;
					Sleep(500);
					ResetColor();
				}
				else if (change > 0) {
					cout << "You are doing great, your financial performance is positive. ";
					cout << "\nTotal Gain: ";
					SetColor(32);
					cout << change << "% profit";
					cout << endl;
					Sleep(500);
					ResetColor();
				}
				else {
					cout << "Your financial performance is stable. No profit or loss recorded.\n";
				}

				cout << "=============================================================" << endl;
				Sleep(1500);
				break;
			}

			case 55: {
				cout << "Returning to Admin Panel";
				loadingAnimation();
				Sleep(155);
				system("cls");
				return;
			}
			default:
				SetColor(31);
				cout << "\nInvalid choice.\n";
				Sleep(800);
				system("cls");
				ResetColor();
				break;
			}
		}
		else if (int(choice) == 27) {
			return;
		}
		else {

			system("cls");
			goto SaleReportStart;
		}

	}
}
void ShowAdminPanel(Depo& d, BudgetOfRestaurant& b, Menu& m) {

	while (true) {
	AdminPanelStart:
		cout << "\n========== ADMIN PANEL ==========" << endl;
		cout << "1. Show Menu" << endl;
		cout << "2. Show All Ingredients In The Depo" << endl;
		cout << "3. Add New Meal To The menu" << endl;
		SetColor(31);
		cout << "4. Remove Meal From Menu" << endl;
		ResetColor();
		cout << "5. Modify Ingredients" << endl;
		cout << "6. Show Sales Report" << endl;
		cout << "7. Back to Main Menu" << endl;
		cout << "=================================" << endl;
		cout << "Enter your choice: ";

		char choice = _getch();
		_putch(choice);
		if (int(choice) >= 48 && int(choice) <= 57) {
			switch (int(choice)) {
			case 49: {

				Sleep(150);
				system("cls");
				SetColor(32);
				cout << "================================= Showing Menu =================================\n";
				ResetColor();
				m.PrintMenu();
				break;
			}

			case 50: {

				Sleep(150);
				system("cls");

				SetColor(32);
				cout << "================================= Showing Ingredients... =================================\n";
				ResetColor();
				d.ShowAll();
				break;
			}

			case 51: {
			AddMealStart:
				string meal_name = "";
				string amountOfIngredients = "";
				Sleep(150);
				system("cls");
				cout << "Enter name of the meal: ";
				cin >> meal_name;
				cout << "Enter number of INGREDIENTS that will be in " << meal_name << ": ";
				cin >> amountOfIngredients;
				if (isInteger(amountOfIngredients) && stoi(amountOfIngredients) > 0) {
					try {
						SetColor(32);
						cout << "\n--- Adding New Meal ---";
						for (size_t i = 0; i < 3; i++)
						{
							cout << ".";
							Sleep(750);
						}
						ResetColor();
						cout << endl;
						m.AddMeal(d, meal_name, stoi(amountOfIngredients));
					}
					catch (exception ex) {
						SetColor(31);
						cout << ex.what();
						Sleep(500);
						ResetColor();
						goto AddMealStart;

					}
					break;
				}
				else {
					SetColor(31);
					cout << "Ingredient Number ERROR: ";
					cout << "Input positive integers only\n";
					ResetColor();
					goto AddMealStart;
				}
			}

			case 52: {
			removeStart:
				cout << "\nAre you sure to remove a meal?(";
				SetColor(32);
				cout << "Y";
				ResetColor();
				cout << "/";
				SetColor(31);
				cout << "N";
				ResetColor();
				cout << "):  ";
				char yes_no = _getch();
				cout << yes_no << endl;

				if (int(yes_no) == 89 || int(yes_no) == 89 + 32) {
					string name_of_meal;
					m.PrintMenu();
					cout << "Enter name of the meal: ";
					cin >> name_of_meal;
					try {
						SetColor(31);
						cout << "\n--- Removing Meal ---";
						for (size_t i = 0; i < 3; i++)
						{
							cout << ".";
							Sleep(750);
						}
						ResetColor();
						cout << endl;
						m.DeleteMeal(name_of_meal);
					}
					catch (exception ex) {
						SetColor(31);
						cout << "Meal Name ERROR: ";
						cout << ex.what();
						ResetColor();
					}
				}
				else if (int(yes_no) == 78 || int(yes_no) == 78 + 32 || int(yes_no) == 27) {
					Sleep(150);
					system("cls");

					continue;
				}
				else {
					Sleep(200);
					system("cls");
					goto removeStart;
				}
				break;
			}

			case 53: {

				system("cls");
				ShowModifyIngredientsMenu(d);
				//system("cls");
				break;
			}

			case 54: {
				ShowSalesReport(b);
				system("cls");
				break;
			}

			case 55: {

			ToMainMenuStart:
				system("cls");
				cout << "\nAre you sure to return to Main Menu?(you will have to login again)(";
				SetColor(32);
				cout << "Y";
				ResetColor();
				cout << "/";
				SetColor(31);
				cout << "N";
				ResetColor();
				cout << "):  ";
				char main_menu_no = _getch();
				cout << main_menu_no << endl;
				if (int(main_menu_no) == 89 || int(main_menu_no) == 89 + 32) {
					return;
				}
				else if (int(main_menu_no) == 78 || int(main_menu_no) == 78 + 32 || int(main_menu_no) == 27) {
					Sleep(400);
					system("cls");
					continue;
				}
				else {
					goto ToMainMenuStart;
				}
			}
			default:
				cout << "Invalid choice. Try again.\n";
				break;
			}
		}
		else if (int(choice) == 27) {
			return;
		}
		else {
			continue;
		}
	}
}
int main() {
	Depo depo;
	Menu menu(depo);
	while (true) {
	Start:
		system("cls");
		cout << "====================================\n";
		cout << " Welcome to the Restaurant: Byte & Bite \n";
		cout << "====================================\n";
		cout << "1. Admin Login\n";
		cout << "2. User Menu\n";
		cout << "3. Exit\n";
		cout << "Enter your choice: ";
		string strchoice;
		int intchoice;
		cin >> strchoice;
		if (isInteger(strchoice)) {
			intchoice = stoi(strchoice);
		}
		else {
			continue;
		}
		switch (intchoice) {
		case 1: {
			system("cls");
			cout << "--- Admin Login ---\n";
			string username, password;
			cout << "Enter username: ";
			cin >> username;
			password = getHiddenInput();
			try {
				Admin(username, password, budget, depo);
				ShowAdminPanel(depo, budget, menu);

			}
			catch (exception ex) {
				SetColor(31);
				cout << "Login ERROR: ";
				cout << ex.what();
				ResetColor();
				Sleep(1500);
			}
		}
		}
	}
	return 0;
}