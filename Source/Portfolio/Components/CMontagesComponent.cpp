#include "Components/CMontagesComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCMontagesComponent::UCMontagesComponent()
{

}


void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();
	if (DataTable == NULL) return;

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	TArray<FMontagesData*> datas;
	DataTable->GetAllRows<FMontagesData>("", datas);

	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		for (FMontagesData* data : datas)
		{
			if ((EStateType)i == data->Type)
			{
				Datas[i] = data;
				continue;
			}
		}
	}

#ifdef LOG_UCMontagesComponent
	for (FMontagesData* data : Datas)
	{
		if (data == NULL) continue;

		FString str;
		str.Append(StaticEnum<EStateType>()->GetValueAsString(data->Type));
		str.Append(" / ");
		str.Append(data->Montage->GetPartName());
		CLog::Log(str);
	}
#endif
}

void UCMontagesComponent::PlayDeadMode()
{
	PlayAnimMontage(EStateType::Dead);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InType)
{
	if (OwnerCharacter == NULL) return;

	FMontagesData* data = Datas[(int32)InType];
	
	if (!!data && !!data->Montage)
		OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRatio);
}