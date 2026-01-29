#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UPerkDataAsset;
class UPerkManagerComponent;

UCLASS()
class ASSIGNMENT8_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMyPlayerController();

	#pragma region Input
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;
#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	#pragma region HUD
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
		UUserWidget* HUDWidgetInstance;

		UFUNCTION(BlueprintPure, Category = "HUD")
		UUserWidget* GetHUDWidget() const;

		UFUNCTION(BlueprintCallable, Category = "HUD")
		void ShowGameHUD();
	#pragma endregion

	#pragma region Menu
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
		TSubclassOf<UUserWidget> MainMenuWidgetClass;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
		UUserWidget* MainMenuWidgetInstance;


		UFUNCTION(BlueprintCallable, Category = "Menu")
		void ShowMainMenu(bool bIsRestart);

		UFUNCTION(BlueprintCallable, Category = "Menu")
		void StartGame();
	#pragma endregion


	#pragma region Perk
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Perk")
		TSubclassOf<UUserWidget> PerkWidgetClass;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Perk")
		UPerkDataAsset* MyPerkDataAsset;

		UPROPERTY(EditAnywhere, Category="UI|Perk")
		UPerkManagerComponent* PerkManager;

		UFUNCTION(BlueprintCallable, Category = "UI|Perk")
		void ShowPerkUI();
	#pragma endregion

	virtual void BeginPlay() override;
	
};
