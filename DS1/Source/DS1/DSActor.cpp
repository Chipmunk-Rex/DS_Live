// Fill out your copyright notice in the Description page of Project Settings.


#include "DSActor.h"

// Sets default values
ADSActor::ADSActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 생성자 활용
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));
	if (Box)
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_ChamferCube.SM_ChamferCube'"));\

		Box->SetStaticMesh(MeshRef.Object);
	}
}

// Called when the game starts or when spawned
void ADSActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADSActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

