// Fill out your copyright notice in the Description page of Project Settings.


#include "DataSender.h"
#include <Networking/Public/Interfaces/IPv4/IPv4Address.h>
#include <Networking/Public/Interfaces/IPv4/IPv4Endpoint.h>
#include <Networking/Public/Common/UdpSocketBuilder.h>
#include <Serialization/ArrayWriter.h>

// Sets default values
ADataSender::ADataSender()
	: Port(25500)
	, IPAddress("127.0.0.1")
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADataSender::BeginPlay()
{
	Super::BeginPlay();
	PrepareSocket();
}

// Called every frame
void ADataSender::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADataSender::SendData(TArray<FSensorDataStruct> CollectedData)
{
	if (bEnabled)
	{
		FArrayWriter Writer;
		Writer << CollectedData;

		int32 BytesSent = 0;
		UDPSender->SendTo(Writer.GetData(), Writer.Num(), BytesSent, *SenderRemoteAddress);

		if (BytesSent <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("[SENDER] No bytes sent!"));
		}
	}
}

void ADataSender::PrepareSocket()
{
	SenderRemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool bIsValid;
	SenderRemoteAddress->SetIp(*IPAddress, bIsValid);
	SenderRemoteAddress->SetPort(Port);

	UDPSender = FUdpSocketBuilder(FString("Data Sender"))
		.AsReusable()
		.AsNonBlocking()
		.WithReceiveBufferSize(2 * 1024 * 1024)
		.WithSendBufferSize(2 * 1024 * 1024);

	if (UDPSender)
	{
		bEnabled = true;
	}
}

void ADataSender::DestroySocket()
{
	if (bEnabled)
	{
		UDPSender->Close();
		bEnabled = false;
	}
}

