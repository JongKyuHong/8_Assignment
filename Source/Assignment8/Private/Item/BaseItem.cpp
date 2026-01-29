#include "Item/BaseItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ABaseItem::ABaseItem()
{
    PrimaryActorTick.bCanEverTick = false;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));

    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

    Collision->SetupAttachment(Scene);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(Collision);

    Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnItemOverlap);
    Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnItemEndOverlap);

    ParticleDestroyDelay = 2.0f;
}

void ABaseItem::OnItemOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        ActivateItem(OtherActor);
    }
}

void ABaseItem::OnItemEndOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
}

void ABaseItem::ActivateItem(AActor* Activator)
{
    UParticleSystemComponent* Particle = nullptr;
    UE_LOG(LogTemp, Warning, TEXT("ActivateItem Called by: %s"), *Activator->GetName());
    if (PickupParticle)
    {
        UE_LOG(LogTemp, Warning, TEXT("Particle Asset is Valid!"));
        Particle = UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            PickupParticle,
            GetActorLocation(),
            GetActorRotation(),
            true
        );
    }

    if (PickupSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            GetWorld(),
            PickupSound,
            GetActorLocation()
        );
    }

    if (Particle)
	{
	    FTimerHandle DestroyParticleTimerHandle;
	    TWeakObjectPtr<UParticleSystemComponent> WeakParticle = Particle;
						
	    GetWorld()->GetTimerManager().SetTimer(
		    DestroyParticleTimerHandle,
		    [WeakParticle]()
		    {
				    if (WeakParticle.IsValid())
				    {
						    WeakParticle->DestroyComponent();
				    }
		    },
            ParticleDestroyDelay,
		    false
	    );
	}
}

void ABaseItem::DestroyItem()
{
    Destroy();
}

FName ABaseItem::GetItemType() const
{
	return ItemType;
}


