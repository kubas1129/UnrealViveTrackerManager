// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataSender.generated.h"

UENUM(BlueprintType)
enum class ESensorType : uint8
{
	Anchor			UMETA(DisplayName = "Anchor"),
	Player			UMETA(DisplayName = "Player")
};

USTRUCT(BlueprintType)
struct FSensorDataStruct
{
	GENERATED_BODY()

	FSensorDataStruct() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SensorID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESensorType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Position;
};

FORCEINLINE FArchive& operator<<(FArchive& Ar, FSensorDataStruct& Structure)
{
	Ar << Structure.SensorID;
	Ar << Structure.Type;
	Ar << Structure.Position;
	Ar << Structure.PlayerID;
	return Ar;
}

UCLASS()
class SENSORDATAGETTER_API ADataSender : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADataSender();

	UPROPERTY(EditAnywhere)
	uint32 Port;

	class FSocket* UDPSender;

	UPROPERTY(VisibleAnywhere)
	bool bEnabled = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TSharedPtr<FInternetAddr> SenderRemoteAddress;
	FString IPAddress;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SendData(TArray<FSensorDataStruct> CollectedData);

	void PrepareSocket();
	void DestroySocket();
};