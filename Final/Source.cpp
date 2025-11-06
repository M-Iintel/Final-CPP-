#include <iostream>
#include <vector>
#include<Windows.h>
using namespace std;

struct Ingredient {
private:
	string name;
	int count;
	float price;

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

	void SetPrice(float price) {
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

	float GetPrice() const noexcept {
		return price;
	}

#pragma endregion
	Ingredient(string name, int count, float price) {
		SetName(name);
		SetCount(count);
		SetPrice(price);
	}
	void ShowIngredientInfo() const {
		cout << "--------------------" << name << "--------------------" << endl;
		cout << "Name: " << name;
		cout << "Count: " << count;
		cout << "Price(for kg): $" << price;
	}
	float GetTotalPrice() const {
		return price * count;
	}
};
class Depo {
	vector<Ingredient> ingredients;
public:

	void AddIngredients(Ingredient ingr) {
		int index = FindIngredient(ingr.GetName());
		if (index != -1) {
			ingredients[index].SetCount((ingredients[index].GetCount() + 1));
			cout << "Ingredient already existed so count increased by one!\n";
		}
		else {
			ingredients.push_back(ingr);
			cout << "New ingredient added!\n";
		}
		//WTF
	}

	void IncreaseNumberOfIngredients(string name, int count) {
		if (count == 0) {
			throw exception("Increasing ingredient by zero is not accepted!\n ");
		}
		else if (count < 0) {
			throw exception("Negative count is not allowed.\n");
		}
		int index = FindIngredient(name);

		if (index != -1) {
			ingredients[index].SetCount((ingredients[index].GetCount() + count));
			cout << "Number of " << name << " was increased by " << count << "\n";
		}
		else {
			throw exception("There is no such ingredient in the inventory\n");
		}
		//WTF(write to file)
	}
	void DecreaseNumberOfIngredients(string name, int count) {
		if (count == 0) {
			throw exception("Decreasing ingredient by zero is not accepted!\n ");
		}

		if (count > 0) {
			throw exception("Positive count is not allowed!\n");
		}

		int index = FindIngredient(name);

		if (index != -1) {
			int newCount = ingredients[index].GetCount() + count;

			if (newCount < 0) {
				throw exception("Cannot decrease below zero!\n");
			}

			ingredients[index].SetCount(newCount);
			cout << "Number of " << name << " was decreased by " << -count << "\n";
		}
		else {
			throw exception("There is no such ingredient in the inventory\n");
		}
		//WTF

	}
	void DeleteIngredient(string name) {
		int index = FindIngredient(name);
		if (index != -1) {
			ingredients.erase(ingredients.begin() + index);
			cout << name << " was removed from the inventory\n";
		}
		else {
			throw exception("There is no such ingredient in the inventory\n");
		}
	}

	int FindIngredient(string name) {
		for (int i = 0; i < ingredients.size(); i++) {
			if (ingredients[i].GetName() == name) {
				return i;
			}
		}
		return -1;
	}

	void ShowAll() {
		for (auto& ingr : ingredients) {
			ingr.ShowIngredientInfo();
		}
	}
	void ShowSingle(string name) {
		int index = FindIngredient(name);
		if (index != -1) {
			ingredients[index].ShowIngredientInfo();
		}
		else {
			throw exception("There is no such ingredient in the inventory\n");
		}
	}

	void TotalValue() {
		double totalVal = 0;
		for (auto& ingr : ingredients) {
			totalVal += ingr.GetTotalPrice();
		}
		cout << "Total value: $" << totalVal << endl;
	}
};
int main() {
	return 0;
}