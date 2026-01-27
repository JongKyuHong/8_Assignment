#include "GameMode/MyGameMode.h"
#include "Character/MyCharacter.h"
#include "Controller/MyPlayerController.h"
#include "GameState/MyGameState.h"

AMyGameMode::AMyGameMode()
{
    DefaultPawnClass = AMyCharacter::StaticClass();
    PlayerControllerClass = AMyPlayerController::StaticClass();
    GameStateClass = AMyGameState::StaticClass();
}