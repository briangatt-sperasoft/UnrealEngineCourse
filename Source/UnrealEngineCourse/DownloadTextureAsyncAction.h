// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Interfaces/IHttpRequest.h"
#include "DownloadTextureAsyncAction.generated.h"

// Reference: https://nerivec.github.io/old-ue4-wiki/pages/blueprints-creating-c-functions-as-new-blueprint-nodes.html
// Reference: https://github.com/EpicGames/UnrealEngine/blob/7ba7e260a528ffbab6d0157890f8ecbe363925f4/Engine/Plugins/Online/OnlineSubsystemUtils/Source/OnlineSubsystemUtils/Classes/FindSessionsCallbackProxy.h
// Reference: https://github.com/EpicGames/UnrealEngine/blob/7ba7e260a528ffbab6d0157890f8ecbe363925f4/Engine/Plugins/Online/OnlineSubsystemUtils/Source/OnlineSubsystemUtils/Private/FindSessionsCallbackProxy.cpp
// Reference: https://github.com/EpicGames/UnrealEngine/blob/7ba7e260a528ffbab6d0157890f8ecbe363925f4/Engine/Source/Runtime/UMG/Public/Blueprint/AsyncTaskDownloadImage.h
// Reference: https://github.com/EpicGames/UnrealEngine/blob/7ba7e260a528ffbab6d0157890f8ecbe363925f4/Engine/Source/Runtime/UMG/Private/Blueprint/AsyncTaskDownloadImage.cpp

USTRUCT(BlueprintType)
struct FBlueprintDownloadTextureResult
{
	GENERATED_USTRUCT_BODY()

	// public is still required (even though structs are public by default) in order for properties to be exposed by Unreal Header Tool
public:
	UPROPERTY(BlueprintReadOnly)
	UTexture2D* Texture = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBlueprintDownloadTextureResultDelegate, const FBlueprintDownloadTextureResult&, Result);

/**
 * 
 */
UCLASS()
class UNREALENGINECOURSE_API UDownloadTextureAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	// In Blueprint, this is represented as an execution pin
	UPROPERTY(BlueprintAssignable)
	FBlueprintDownloadTextureResultDelegate OnSuccess;
	
	// Using the same delegate for convenience. In case of failure, the result will have the Texture member as nullptr.
	UPROPERTY(BlueprintAssignable)
	FBlueprintDownloadTextureResultDelegate OnFailure;

	// Factory function which creates a UDownloadTextureAsyncAction and populates the internal member for later reference in Activate
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true"))
	static UDownloadTextureAsyncAction* DownloadTexture(FString URL);

	// UBlueprintAsyncActionBase overrides
	void Activate() override;
	// UBlueprintAsyncActionBase overrides

private:
	void OnProcessRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	FString URL;
};
