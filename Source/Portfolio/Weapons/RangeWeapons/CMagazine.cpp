#include "CMagazine.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"

ACMagazine::ACMagazine()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh", Root);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh_Empty, "Mesh_Empty", Root);
}

void ACMagazine::BeginPlay()
{	
	Super::BeginPlay();
	Mesh->SetCollisionProfileName("Magazine");
	Mesh_Empty->SetCollisionProfileName("Magazine");
		
	if (bEject == false)
	{
		Mesh->SetVisibility(true);
		Mesh_Empty->SetVisibility(false);
	}
}

void ACMagazine::Eject()
{
	bEject = true;

	Mesh->SetVisibility(false);
	Mesh_Empty->SetVisibility(true);
	Mesh_Empty->SetSimulatePhysics(true);
}