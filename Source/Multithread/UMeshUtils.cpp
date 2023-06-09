// Fill out your copyright notice in the Description page of Project Settings.


#include "UMeshUtils.h"

#include "ProceduralMeshComponent.h"
#include "ProceduralMeshConversion.h"
#include "StaticMeshDescription.h"


UStaticMesh* UUMeshUtils::ConvertProceduralMeshComponentToSM(UProceduralMeshComponent* ProcMesh)
{
	if (!ProcMesh)
		return nullptr;

	FMeshDescription MeshDescription = BuildMeshDescription(ProcMesh);

	// If we got some valid data.
	if (MeshDescription.Polygons().Num() > 0)
	{
		// Create StaticMesh object
		FName PMC_Name = FName(*ProcMesh->GetName());
		UStaticMesh* StaticMesh = NewObject<UStaticMesh>(GetTransientPackage(), PMC_Name, EObjectFlags::RF_Transient);
		//StaticMesh->bAllowCPUAccess = true;
		StaticMesh->NeverStream = true;
		StaticMesh->InitResources();
		StaticMesh->SetLightingGuid();

		// Add source to new StaticMesh
		auto Desc = StaticMesh->CreateStaticMeshDescription();
		Desc->SetMeshDescription(MeshDescription);
		// buildSimpleCol = false, cause it creates box collision based on mesh bounds or whatever :(
		StaticMesh->BuildFromStaticMeshDescriptions({ Desc }, false);

		//// SIMPLE COLLISION
		if (!ProcMesh->bUseComplexAsSimpleCollision)
		{
			StaticMesh->CreateBodySetup();
			UBodySetup* NewBodySetup = StaticMesh->GetBodySetup();
			NewBodySetup->BodySetupGuid = FGuid::NewGuid();
			NewBodySetup->AggGeom.ConvexElems = ProcMesh->ProcMeshBodySetup->AggGeom.ConvexElems;
			NewBodySetup->bGenerateMirroredCollision = false;
			NewBodySetup->bDoubleSidedGeometry = true;
			// Play around with the flag below if you struggle with collision not working
			NewBodySetup->CollisionTraceFlag = CTF_UseDefault;
			NewBodySetup->CreatePhysicsMeshes();
		}

		// Commented out cause I don't need it
		//// MATERIALS
		TSet<UMaterialInterface*> UniqueMaterials;
		const int32 NumSections = ProcMesh->GetNumSections();
		for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
		{
			FProcMeshSection* ProcSection =
				ProcMesh->GetProcMeshSection(SectionIdx);
			UMaterialInterface* Material = ProcMesh->GetMaterial(SectionIdx);

			//StaticMesh->AddMaterial(Material);
			UniqueMaterials.Add(Material);
		}
		// Copy materials to new mesh
		for (auto* Material : UniqueMaterials)
		{
			StaticMesh->AddUVChannel(0);
			FStaticMaterial staticMaterial(Material);
			staticMaterial.UVChannelData.bInitialized = true;

			StaticMesh->GetStaticMaterials().Add(staticMaterial);
		}

		StaticMesh->Build(false);

		return StaticMesh;
	}

	return nullptr;
}