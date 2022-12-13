#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMagazine.generated.h"

UCLASS()
class PORTFOLIO_API ACMagazine : public AActor
{
	GENERATED_BODY()
	
public:	
	ACMagazine();
	
private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh_Empty;

protected:
	virtual void BeginPlay() override;

public:
	void Eject();
	

private:
	bool bEject = false;
};
