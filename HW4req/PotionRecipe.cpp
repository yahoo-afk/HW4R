#include <iostream>
#include <vector>
#include <string>
#include <map>

class PotionRecipe
{
public:
	std::string potionName;
	std::vector<std::string> ingredients;

	PotionRecipe(const std::string& name, const std::vector<std::string>& ingredients)
		:potionName(name), ingredients(ingredients) {
	}
};

class StockManager
{
private:	
	std::map<std::string, int> potionStock;
public:
	int Max_Stock = 3;

	int GetStock(const std::string& potionname)const
	{
		auto it = potionStock.find(potionname);
		if (it != potionStock.end())
		{
			return it->second;
		}
		return 0;
	}

	void InitializeStock(const std::string& potionName)
	{
		potionStock[potionName] = Max_Stock;
	}

	void AddStock(const std::string& potionname)
	{
		if (0 <= potionStock[potionname] && potionStock[potionname] < Max_Stock)
		{
			potionStock[potionname] += 1;
		}
		else
		{
			std::cout << "포션 갯수는 3개초과일수 없습니다.  현재 보유중인 포션갯수는:" << potionStock[potionname] << "개입니다" << std::endl;
		}
	}

	bool ReduceStock(std::string potionname)
	{
		if (0 < potionStock[potionname])
		{
			potionStock[potionname] -= 1;
			return true;
		}
		else
		{
			std::cout << "남은 포션이없습니다" << std::endl;
			return false;
		}
	}
};

class RecipeManager
{
private:
	std::vector<PotionRecipe> recipes_;
public:

	PotionRecipe* AddRecipe(const std::string& name, const std::vector<std::string>& ingredients)
	{
		for (int i = 0; i < recipes_.size(); ++i)
		{
			if (name == recipes_[i].potionName)
			{
				return &recipes_[i];
			}
		}
		recipes_.push_back(PotionRecipe(name, ingredients));
		return &recipes_.back();
	}
	
	PotionRecipe* FindRecipeByName(const std::string& name)
	{
		for (int i = 0; i < recipes_.size(); ++i)
		{
			if (name == recipes_[i].potionName)
			{
				return &recipes_[i];
			}
		}
		return nullptr;
	}

	std::vector<PotionRecipe> FindRecipesByIngredient(const std::string& ingredient) const
	{
		std::vector<PotionRecipe> result;
		for (int i = 0; i < recipes_.size(); ++i)
		{
			for (int j = 0; j < recipes_[i].ingredients.size(); ++j)
			{
				if (ingredient == recipes_[i].ingredients[j])
				{
					result.push_back(recipes_[i]);
					break;
				}
			}
		}
		return result;
	}

	const std::vector<PotionRecipe>& GetAllRecipes()const
	{
		return recipes_;
	}

};

class AlchemyWorkshop
{
private:
	RecipeManager recipeManager_;
	StockManager stockManager_;
public:
	int GetStockByName(const std::string& name) const
	{
		return stockManager_.GetStock(name);
	}

	bool DispensePotionByName(const std::string& name)
	{
		return stockManager_.ReduceStock(name);
	}

	void AddRecipe(const std::string& name, const std::vector<std::string>ingredients)
	{
		PotionRecipe* newRecipe= recipeManager_.AddRecipe(name, ingredients);
		if(newRecipe != nullptr)
		{
			stockManager_.InitializeStock(name);
			std::cout << "레시피가 등록되었습니다." << std::endl;
			std::cout << " 현재 재고는:" << stockManager_.GetStock(name) << "입니다." << std::endl;
		}
		else
		{
			std::cout<<"레시피 등록실패했습니다"<<std::endl;
		}
	}

	void ReturnPotionByName(const std::string& name)
	{
		stockManager_.AddStock(name);
	}

	std::vector<std::string> DispensePotionsByIngredient(const std::string& ingredient)
	{
		std::vector<std::string> dispensedPotions;
		
		std::vector<PotionRecipe> targetRecipes = recipeManager_.FindRecipesByIngredient(ingredient);

		for (int i = 0; i < targetRecipes.size(); ++i)
		{
			if (stockManager_.ReduceStock(targetRecipes[i].potionName))
			{
				dispensedPotions.push_back(targetRecipes[i].potionName);
			}
		}
		return dispensedPotions;
	}

	void DisplayAllRecipes() const
	{
		const std::vector<PotionRecipe>& allRecipes = recipeManager_.GetAllRecipes(); 
		if(allRecipes.empty())
		{
			std::cout << "등록된 레시피가 없습니다" << std::endl;
		}
		std::cout << "\n--- [ 전체 레시피 목록 ] ---" << std::endl;
		for(int i=0; i<allRecipes.size();++i)
		{
			std::cout << "- 물약 이름: " << allRecipes[i].potionName << std::endl;
			std::cout << "  > 필요 재료: ";
			for (int j = 0; j< allRecipes[i].ingredients.size(); ++j)
			{
				std::cout << allRecipes[i].ingredients[j];
				if (j < allRecipes[i].ingredients.size() - 1)
				{
					std::cout << ", ";
				}
			}
		std::cout << std::endl;
		}
	}
	
	void SearchRecipeByName(std::string name)
	{
		PotionRecipe* found = recipeManager_.FindRecipeByName(name);
		if(found != nullptr)
		{
			std::cout << "물약이름:" << found->potionName << std::endl;
			std::cout << "제작재료:";
			for(int i=0; i<found->ingredients.size();++i)
			{
				std::cout << found->ingredients[i];
				if(i<found->ingredients.size()-1)
				{
					std::cout << ",";
				}
			}
			std::cout<<std::endl;;
		}
		else 
		{
			std::cout << "찾는 물약은 존재하지않습니다." << std::endl;
		}
	}

	void SearchRecipeByIngredient(std::string ingredients)
	{
		std::vector<PotionRecipe> resultList = recipeManager_.FindRecipesByIngredient(ingredients);

		if (resultList.empty())
		{
			std::cout << "그런재료는 존재하지 않습니다." << std::endl;
			return; 
		}

		for (int i = 0; i < resultList.size(); ++i)
		{
			std::cout << "찾으시는 재료로 만들 수 있는 물약 이름은: "
				<< resultList[i].potionName << "입니다." << std::endl;
		}
	}
};

void main()
{
	AlchemyWorkshop workshop;

	workshop.AddRecipe("Healing Potion", { "Herb", "Water" });
	workshop.AddRecipe("Mana Potion", { "Magic Water", "Crystal" });
	workshop.AddRecipe("Stamina Potion", { "Herb", "Berry" });
	workshop.AddRecipe("Fire Resistance Potion", { "Fire Flower", "Ash" });

	std::cout << "=== 초기 상태 (레시피 추가 + 재고 자동 3개) ===\n";
	workshop.DisplayAllRecipes();

	std::cout << "\n[재고 확인] Healing Potion 재고: "
		<< workshop.GetStockByName("Healing Potion") << "\n";

	std::cout << "\n=== 이름으로 지급 테스트 (Healing Potion 3회 지급) ===\n";
	std::cout << "1회 지급: " << (workshop.DispensePotionByName("Healing Potion") ? "성공" : "실패") << "\n";
	std::cout << "2회 지급: " << (workshop.DispensePotionByName("Healing Potion") ? "성공" : "실패") << "\n";
	std::cout << "3회 지급: " << (workshop.DispensePotionByName("Healing Potion") ? "성공" : "실패") << "\n";

	std::cout << "현재 재고: " << workshop.GetStockByName("Healing Potion") << "\n";

	std::cout << "4회 지급(재고 없으면 실패): "
		<< (workshop.DispensePotionByName("Healing Potion") ? "성공" : "실패") << "\n";

	std::cout << "\n=== 재료로 지급 테스트 (ingredient = Herb) ===\n";
	std::vector<std::string> dispensed = workshop.DispensePotionsByIngredient("Herb");

	std::cout << "지급된 물약 수: " << dispensed.size() << "\n";
	for (const std::string& name : dispensed)
		std::cout << "- " << name << "\n";

	std::cout << "\n=== 공병 반환 테스트 (Healing Potion) ===\n";
	workshop.ReturnPotionByName("Healing Potion");
	workshop.ReturnPotionByName("Healing Potion");
	workshop.ReturnPotionByName("Healing Potion"); 

	std::cout << "반환 후 재고(최대 3 유지): "
		<< workshop.GetStockByName("Healing Potion") << "\n";

	std::cout << "\n=== 최종 상태 ===\n";
	workshop.DisplayAllRecipes();
}
