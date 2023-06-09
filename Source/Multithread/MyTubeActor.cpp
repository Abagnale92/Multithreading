// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTubeActor.h"
#include <ProceduralMeshComponent/Public/ProceduralMeshComponent.h>
#include "UMeshUtils.h"
#include <Kismet/KismetMathLibrary.h>
// Sets default values
AMyTubeActor::AMyTubeActor()
{
	MeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = MeshComp;
	myMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/StarterContent/Materials/M_Metal_Steel.M_Metal_Steel'")).Object;
	MeshComp->bUseAsyncCooking = true;

}

// Called when the game starts or when spawned
void AMyTubeActor::BeginPlay()
{
	Super::BeginPlay();
   // CreateMesh();

}

void AMyTubeActor::CreateMesh()
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;

    // Incremento angolare tra ogni slice del cilindro
    const float AngleStep = 2.0f * PI / Slices;

    // Genera i vertici laterali del cilindro
    for (int32 SliceIndex = 0; SliceIndex <= Slices; SliceIndex++)
    {
        float Angle = AngleStep * SliceIndex;
        float X = Radius * FMath::Cos(Angle);
        float Z = Radius * FMath::Sin(Angle);

        // Vertice superiore
        FVector TopVertex = FVector(X, Height / 2, Z);
        Vertices.Add(TopVertex);
        Normals.Add(FVector(X, 0.0f, Z).GetSafeNormal());
        UVs.Add(FVector2D(1.0f - static_cast<float>(SliceIndex) / Slices, 1.0f));

        // Vertice inferiore
        FVector BottomVertex = FVector(X, -Height / 2, Z);
        Vertices.Add(BottomVertex);
        Normals.Add(FVector(X, 0.0f, Z).GetSafeNormal());
        UVs.Add(FVector2D(1.0f - static_cast<float>(SliceIndex) / Slices, 0.0f));
    }

    // Genera gli indici per creare i lati del cilindro
    for (int32 SliceIndex = 0; SliceIndex < Slices; SliceIndex++)
    {
        int32 CurrentSlice = SliceIndex * 2;
        int32 NextSlice = (SliceIndex + 1) * 2;

        // Triangolo superiore
        Triangles.Add(CurrentSlice);
        Triangles.Add(CurrentSlice + 1);
        Triangles.Add(NextSlice);

        // Triangolo inferiore
        Triangles.Add(CurrentSlice + 1);
        Triangles.Add(NextSlice + 1);
        Triangles.Add(NextSlice);
    }

    // Genera i vertici per la faccia superiore
    int32 TopCenterIndex = Vertices.Add(FVector(0.0f, Height / 2, 0.0f));
    Normals.Add(FVector(0.0f, 1.0f, 0.0f));
    UVs.Add(FVector2D(0.0f, 1.0f));

    // Genera i vertici per la faccia inferiore
    int32 BottomCenterIndex = Vertices.Add(FVector(0.0f, -Height / 2, 0.0f));
    Normals.Add(FVector(0.0f, -1.0f, 0.0f));
    UVs.Add(FVector2D(0.0f, 0.0f));

    // Genera gli indici per creare le facce superiori e inferiori
    for (int32 SliceIndex = 0; SliceIndex < Slices; SliceIndex++)
    {
        int32 CurrentSlice = SliceIndex * 2;
        int32 NextSlice = (SliceIndex + 1) * 2;

        // Faccia superiore
        Triangles.Add(TopCenterIndex);
        Triangles.Add(CurrentSlice);
        Triangles.Add(NextSlice);

        // Faccia inferiore
        Triangles.Add(BottomCenterIndex);
        Triangles.Add(NextSlice + 1);
        Triangles.Add(CurrentSlice + 1);
    }

    // Crea una sezione della mesh procedurale e assegna i dati
    MeshComp->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
    //GenerateIstancedMesh();
}

void AMyTubeActor::ComputeProperties(FRotator const& rotation, FVector const& pos)
{
    MeshComp->SetWorldTransform(FTransform(rotation + FRotator(0, 90, 0), pos, FVector(1, 1, 1)));
    MeshComp->SetMaterial(0, myMaterial);
}

void AMyTubeActor::GenerateIstancedMesh()
{
    //_StaticMesh = UUMeshUtils::ConvertProceduralMeshComponentToSM(MeshComp);
    //MeshComp->UnregisterComponent();
    //MeshComp->DestroyComponent();
    //_InstanceMeshComp = NewObject<UInstancedStaticMeshComponent>(this, TEXT("InstancedMesh"));
    //_InstanceMeshComp->SetMobility(EComponentMobility::Type::Movable);
    //_InstanceMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    //_InstanceMeshComp->RegisterComponent();
    //_InstanceMeshComp->SetStaticMesh(_StaticMesh);
    ////_InstanceMeshComp->AddRelativeLocation(FVector(0, 0, this->GetActorLocation().Z + _TotalHeight));
    //_InstanceMeshComp->SetMaterial(0, myMaterial);
    //this->AddInstanceComponent(_InstanceMeshComp);
}

