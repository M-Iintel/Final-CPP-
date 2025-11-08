#include <iostream>
#include <vector>
#include<Windows.h>
#include <algorithm>
#include <cctype> 
//cctype and algorithm library: for lowering string (Found on the Internet)
using namespace std;



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
	Ingredient(string name, int count, double price) {
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
	Depo() = default;

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
		cout << "New " << count << " " << ingr->GetName() << " added!\n";
	}

	void IncreaseNumberOfIngredients(const string& name, int count) {
		if (count <= 0) {
			throw exception("Increse by postive numbers only!\n");
		}
		int index = FindIngredient(name);
		if (index != -1) {
			int newCount = ingredients[index]->GetCount() + count;
			ingredients[index]->SetCount(newCount);
			cout << "Number of " << name << " was increased in depo by " << count << " to " << newCount << endl;
		}
		else {
			throw exception("There is no such ingredient in stock!\n");
		}
	}
	void DecreaseNumberOfIngredients(const string& name, int count) {
		if (count <= 0) {
			throw exception("Decrease Error: Number must be positive!\n");
		}
		int index = FindIngredient(name);
		if (index != -1) {
			int newCount = ingredients[index]->GetCount() - count;
			if (newCount < 0) {
				throw exception("Can not decrease number of ingredients below zero!\n");
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
		SetPrice(price * amount);
	}

};

class Meal {
protected:
	vector <shared_ptr <ReceiptItem>> ingredients;
	string name;
	double price;
public:
#pragma region Setters&Getters
	void SetName(string name) {
		if (!(name.length() > 3)) {
			throw exception("Invalid name ");
		}
		this->name = name;
	}
	void SetPrice(double price) {
		if (price > 0) {
			this->price = price;
		}
		else {
			throw exception("Invalid count input ");
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

	}
	virtual void PrintIngredients() const {
		int counter = 0;
		for (const auto& ingr : ingredients) {
			cout << ++counter << ". Name: " << ingr->GetName() << " Price: $" << ingr->GetPrice() << " Amount: " << ingr->GetAmount();
		}
	}
	double TotalPriceOfMeal() {
		price = 0;
		for (const auto& ingr : ingredients) {
			price += ingr->GetPrice();
		}
		price *= 1.65; // service fee
		return price;
	}
	virtual ~Meal() = 0 {};
};

Depo d;
struct Tomato : public Ingredient {
	Tomato(int count, double price, string name = "Tomato")
		: Ingredient(name, count, price) {
	}

};


struct Flour : public Ingredient {
	Flour(int count, double price, string name = "Flour")
		: Ingredient(name, count, price) {
	}
};
struct Butter : public Ingredient {
	Butter(int count, double price, string name = "Butter")
		: Ingredient(name, count, price) {
	}
};
struct Cheese : public Ingredient {
	Cheese(int count, double price, string name = "Cheese")
		: Ingredient(name, count, price) {
	}
};
struct Rice : public Ingredient {
	Rice(int count, double price, string name = "Rice")
		: Ingredient(name, count, price) {
	}
};
struct Chicken : public Ingredient {
	Chicken(int count, double price, string name = "Chicken")
		: Ingredient(name, count, price) {
	}
};
struct Beef : public Ingredient {
	Beef(int count, double price, string name = "Beef")
		: Ingredient(name, count, price) {
	}
};
struct Fish : public Ingredient {
	Fish(int count, double price, string name = "Fish")
		: Ingredient(name, count, price) {
	}
};
struct Egg : public Ingredient {
	Egg(int count, double price, string name = "Egg")
		: Ingredient(name, count, price) {
	}
};
struct Pasta : public Ingredient {
	Pasta(int count, double price, string name = "Pasta")
		: Ingredient(name, count, price) {
	}
};

class Pizza : public Meal {

public:
	Pizza() : Meal("Pizza") {
		string name = "Pizza";
	};
	void PrintIngredients() const override {
		int counter = 0;
		for (const auto& ingr : ingredients) {
			cout << ++counter << ". Name: " << ingr->GetName() << "; Price: $" << ingr->GetPrice() << "; Amount: " << ingr->GetAmount() << " Slice(s) ";
		}
	}
};
int main() {
	Pizza p;
	shared_ptr<Ingredient> cheese(new Cheese(100, 2.2, "Cheese"));
	shared_ptr<Ingredient> tomato(new Tomato(5, 4, "Tomato"));
	d.AddIngredient(cheese, cheese->GetCount());
	d.AddIngredient(tomato, tomato->GetCount());
	try {

		p.AddIngredientToMeal(d, tomato, 1);
		p.AddIngredientToMeal(d, cheese, 2);
		cout << "Price of meal " << p.GetName() << " with fees: $" << p.TotalPriceOfMeal() << endl;
		p.PrintIngredients();
		cout << endl;
		d.IncreaseNumberOfIngredients("cheese", 4);
		d.DecreaseNumberOfIngredients("tomato", 1);
		d.ShowSingle("tomato");
		cout << d.TotalValue() <<endl;
		d.DeleteIngredient("tomato");
		
		cout << "ALL:\n";
		d.ShowAll();
	}
	catch (exception ex) {
		cout << ex.what();
	}
	return 0;
}