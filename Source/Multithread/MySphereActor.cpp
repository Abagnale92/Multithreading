// Fill out your copyright notice in the Description page of Project Settings.

#include "MySphereActor.h"
#include <ProceduralMeshComponent/Public/ProceduralMeshComponent.h>

// Sets default values
AMySphereActor::AMySphereActor()
{
	MeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = MeshComp;
	myMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Steel.M_Metal_Steel'")).Object;
	MeshComp->bUseAsyncCooking = true;
}

void AMySphereActor::CreateSphere()
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;

    // Genera i vertici della sfera
    for (int32 SliceIndex = 0; SliceIndex <= Slices; SliceIndex++)
    {
        float Y = 2.0f * PI * SliceIndex / Slices;

        for (int32 StackIndex = 0; StackIndex <= Stacks; StackIndex++)
        {
            float X = PI * StackIndex / Stacks;

            FVector Vertex = FVector(
                Radius * FMath::Sin(X) * FMath::Cos(Y),
                Radius * FMath::Sin(X) * FMath::Sin(Y),
                Radius * FMath::Cos(X)
            );

            Vertices.Add(Vertex);
            Normals.Add(Vertex.GetSafeNormal());
            UVs.Add(FVector2D(
                static_cast<float>(SliceIndex) / static_cast<float>(Slices),
                static_cast<float>(StackIndex) / static_cast<float>(Stacks)
            ));
        }
    }

    // Genera gli indici per creare i triangoli della sfera
    for (int32 SliceIndex = 0; SliceIndex < Slices; SliceIndex++)
    {
        for (int32 StackIndex = 0; StackIndex < Stacks; StackIndex++)
        {
            int32 CurrentStack = StackIndex + (SliceIndex * (Stacks + 1));
            int32 NextStack = CurrentStack + Stacks + 1;

            // Triangolo superiore
            Triangles.Add(CurrentStack);
            Triangles.Add(NextStack);
            Triangles.Add(CurrentStack + 1);

            // Triangolo inferiore
            Triangles.Add(NextStack);
            Triangles.Add(NextStack + 1);
            Triangles.Add(CurrentStack + 1);
        }
    }

    // Crea una sezione della mesh procedurale e assegna i dati
    MeshComp->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
    if (myMaterial) {
        MeshComp->SetMaterial(0, myMaterial);
    }
}

// Called when the game starts or when spawned
void AMySphereActor::BeginPlay()
{
	Super::BeginPlay();
}


