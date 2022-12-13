#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

UINTERFACE(MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

class PORTFOLIO_API IICharacter
{
	GENERATED_BODY()

public:
	virtual void End_Hitted() = 0;
	virtual void End_Dead() = 0;

public:
	virtual void Change_Color(class ACharacter* InCharacter, FLinearColor InColor);
};
