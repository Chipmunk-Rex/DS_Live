// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/DS1Box.h"
#include "DS1Box.h"

// Sets default values
ADS1Box::ADS1Box()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));
	SetRootComponent(Box);
	
	if (Box)
	{
		Box->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	}
}

// Called when the game starts or when spawned
void ADS1Box::BeginPlay()
{
	Super::BeginPlay();
	
	if (Box)
	{
		Box->OnComponentHit.AddDynamic(this, &ADS1Box::OnBoxHit);
	}
}

// Called every frame
void ADS1Box::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADS1Box::OnBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

