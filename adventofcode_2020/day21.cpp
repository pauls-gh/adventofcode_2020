#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

struct Food {
	vector<string> ingredients;
	vector<string> allergens;
};

vector<Food> foods;

// map allergen => array of array of ingredients
struct AllergenInfo {
	vector<vector<string>> ingredientsArray;
	bool determined = false;
	string ingredient;		// when determined, this contains the actual ingredient
};
unordered_map <string, AllergenInfo> allergenMap;
unordered_set<string> ingredientSet;

void printFoods()
{
	for (auto &food : foods) {
		for (auto& ingredient : food.ingredients) {
			cout << ingredient << " ";
		}
		cout << " Allergens: ";
		for (auto& allergen : food.allergens) {
			cout << allergen << " ";
		}
		cout << endl;
	}

	cout << "Allergen map" << endl;

	for (auto& pair : allergenMap) {
		auto& allergen = pair.first;
		auto& allergenInfo = pair.second;
		cout << allergen << ": ";
		if (allergenInfo.determined) {
			cout << allergenInfo.ingredient;
		}
		else {
			cout << "undetermined";
		}
		cout << endl;
		for (auto& ingredients : allergenInfo.ingredientsArray) {
			for (auto& ingredient : ingredients) {
				cout << ingredient << " ";
			}
			cout << endl;
		}
	}
}

void removeIngredient(string &ingredientToDelete)
{
	for (auto& pair : allergenMap) {
		auto& allergen = pair.first;
		auto& allergenInfo = pair.second;
		for (auto& ingredients : allergenInfo.ingredientsArray) {
			// use remove_if as deleting elements in a vector will invalidate references
			ingredients.erase(remove_if(ingredients.begin(), ingredients.end(),
				[&ingredientToDelete](string& ingredient) {
					bool bMatch = (ingredient == ingredientToDelete);
					return bMatch;
				}),
				ingredients.end());
		}
	}
}

void determineAllergens()
{
	// Algorithm
	// for each allergen
	//		keep list of foods containing this allergen
	// while (!done)
	//		done = true;
	//		for each undetermined allergen
	//			count ingredient frequencies for all food containing allergen
	//			if ingredient frequency == # foods  and is unique
	//				then this ingredient is the allergen
	//					 now remove this ingredient from all foods
	//
	//			else
	//				still can't determine for this allergen
	//				done = false;

	bool bDone = false;

	while (!bDone) {
		bDone = true;

		for (auto& pair : allergenMap) {
			auto& allergen = pair.first;
			auto& allergenInfo = pair.second;

			if (allergenInfo.determined) {
				continue;
			}
			bDone = false;

			unordered_map<string, int> countIngredients;
			for (auto& ingredients : allergenInfo.ingredientsArray) {
				for (auto& ingredient : ingredients) {
					countIngredients[ingredient]++;
				}
			}

			vector<string> matches;
			for (auto& pair : countIngredients) {
				auto& ingredient = pair.first;
				auto& count = pair.second;

				if (count == allergenInfo.ingredientsArray.size()) {
					matches.push_back(ingredient);
				}
			}
			//cout << "matches = " << matches.size() << " " << matches[0] << endl;
			if (matches.size() == 1) {
				// this ingredient is an allergen
				allergenInfo.determined = true;
				allergenInfo.ingredient = matches[0];

				// remove this ingredient from all foods
				removeIngredient(matches[0]);
			}
		}
	}
}

void day21()
{
	fstream myfile("day21_input.txt", ios_base::in);
	string str;

	// read rules
	while (getline(myfile, str)) {
		//cout << str << endl;
		stringstream ss(str);
		string token;
		Food food;

		while (getline(ss, token, ' ')) {
			if (token == "(contains") {
				break;
			}
			food.ingredients.push_back(token);
		}

		while (getline(ss, token, ' ')) {
			food.allergens.push_back(token.substr(0, token.length() - 1));
		}
		foods.push_back(food);
	}

	// Create map of allergens => foods
	for (auto& food : foods) {
		for (auto& allergen : food.allergens) {
			allergenMap[allergen].ingredientsArray.push_back(food.ingredients);
		}
	}

	// Create set of all ingredients
	for (auto& food : foods) {
		for (auto& ingredient : food.ingredients) {
			ingredientSet.insert(ingredient);
		}
	}

	printFoods();

	determineAllergens();

	// determine non allergen ingredients
	unordered_set nonAllergenIngredientsSet = ingredientSet;
	for (auto& pair : allergenMap) {
		auto& allergen = pair.first;
		auto& allergenInfo = pair.second;

		cout << allergen << " = " << allergenInfo.ingredient << endl;
		nonAllergenIngredientsSet.extract(allergenInfo.ingredient);
	}

	cout << "Number of non allergen ingredients = " << nonAllergenIngredientsSet.size() << endl;

	unordered_map<string, int> countNonAllergen;
	for (auto& food : foods) {
		for (auto& ingredient : food.ingredients) {
			if (nonAllergenIngredientsSet.find(ingredient) != nonAllergenIngredientsSet.end()) {
				countNonAllergen[ingredient]++;
			}
		}
	}

	int total = 0;
	for (auto& pair : countNonAllergen) {
		auto& ingredient = pair.first;
		auto& count = pair.second;
		total += count;
	}

	cout << "Part 1 = Non allergen ingredient, # times = " << total << endl;

	// Part 2
	vector<string> allergens;
	for (auto& pair : allergenMap) {
		auto& allergen = pair.first;
		auto& allergenInfo = pair.second;
		allergens.push_back(allergen);
	}
	sort(allergens.begin(), allergens.end());

	cout << "Part 2" << endl;
	for (auto& allergen : allergens) {
		cout << allergenMap[allergen].ingredient << ",";
	}
	cout << endl;
}