#include "Obstacle/Spike.h"

#include "SpartaCharacter.h"
#include "Kismet/GameplayStatics.h"

ASpike::ASpike()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComp);
	MeshComp->SetNotifyRigidBodyCollision(true);
	MeshComp->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	SpikeDamage = 20.0f;
	CoolTime = 1.0f;
	bIsColliding = true;
	SpikeSpeed = 300.0f;
	MaxRange = 300.0f;
	StartLocation = FVector::ZeroVector;
}

void ASpike::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->OnComponentHit.AddDynamic(this, &ASpike::OnSpikeHit);
	StartLocation = GetActorLocation();
}

void ASpike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldOffset(FVector(0.0f, 0.0f, -SpikeSpeed * DeltaTime));
	if (FVector::Dist(GetActorLocation(), StartLocation) > MaxRange || GetActorLocation().Z > StartLocation.Z)
	{
		SpikeSpeed *= -1.0f;
	}
}

void ASpike::OnSpikeHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bIsColliding) return;
	
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		ASpartaCharacter* Player = Cast<ASpartaCharacter>(OtherActor);

		if (Player)
		{
			UGameplayStatics::ApplyDamage(Player, SpikeDamage, nullptr, this, UDamageType::StaticClass());

			//MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			bIsColliding = false;

			GetWorldTimerManager().SetTimer(CollisionTimerHandle, this, &ASpike::SetSpikeCollision, CoolTime);
		}
	}
}

void ASpike::SetSpikeCollision()
{
	//MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bIsColliding = true;
}

