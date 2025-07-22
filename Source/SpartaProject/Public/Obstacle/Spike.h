#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Spike.generated.h"

UCLASS()
class SPARTAPROJECT_API ASpike : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpike();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USceneComponent* RootComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
	float SpikeDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
	float CoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
	float SpikeSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spike")
	float MaxRange;
	

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSpikeHit(
		UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	UFUNCTION()
	void SetSpikeCollision();

private:
	FTimerHandle CollisionTimerHandle;
	bool bIsColliding;
	FVector StartLocation;
};
