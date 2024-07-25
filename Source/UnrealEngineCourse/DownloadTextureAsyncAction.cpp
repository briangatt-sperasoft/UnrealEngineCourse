// Fill out your copyright notice in the Description page of Project Settings.


#include "DownloadTextureAsyncAction.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

#include "IImageWrapperModule.h"

#include "Logging/LogMacros.h"

UDownloadTextureAsyncAction* UDownloadTextureAsyncAction::DownloadTexture(FString URL)
{
	UDownloadTextureAsyncAction* Action = NewObject<UDownloadTextureAsyncAction>();

	Action->LoadConfig();

	if (!URL.IsEmpty())
	{
		Action->URL = MoveTemp(URL);
	}

	return Action;
}

void UDownloadTextureAsyncAction::Activate()
{
	if (URL.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Download Texture Async. Invalid URL."));
		return OnFailure.Broadcast(FBlueprintDownloadTextureResult{});
	}

	// Reference: https://forums.unrealengine.com/t/how-to-make-an-http-request-from-unreal/27577

	FHttpRequestRef HttpRequest = FHttpModule::Get().CreateRequest();

	HttpRequest->SetVerb(TEXT("GET"));
	HttpRequest->SetURL(URL);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UDownloadTextureAsyncAction::OnProcessRequestCompleted);

	const bool bSuccess = HttpRequest->ProcessRequest();

	if (!bSuccess)
	{
		return OnFailure.Broadcast(FBlueprintDownloadTextureResult {});
	}
}

void UDownloadTextureAsyncAction::OnProcessRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	FBlueprintDownloadTextureResult Result;

	bConnectedSuccessfully = bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode());

	if (!bConnectedSuccessfully)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to GET URL content. Error: %d"), (Response != nullptr) ? Response->GetResponseCode() : -1);
		return OnFailure.Broadcast(Result);
	}

	IImageWrapperModule* ImageWrapperModule = FModuleManager::LoadModulePtr<IImageWrapperModule>(FName("ImageWrapper"));

	if (ImageWrapperModule == nullptr)
	{
		return OnFailure.Broadcast(Result);
	}

	FImage Image;
	const bool bSuccess = ImageWrapperModule->DecompressImage(Response->GetContent().GetData(), Response->GetContentLength(), Image);

	if (!bSuccess)
	{
		return OnFailure.Broadcast(Result);
	}

	Image.ChangeFormat(ERawImageFormat::BGRA8, EGammaSpace::sRGB);

	// Reference: https://nerivec.github.io/old-ue4-wiki/pages/asynchronous-image-loading-from-disk.html

	// TODO Is it necessary to encapsulate texture creation in ENQUEUE_RENDER_COMMAND (to perform the operation on the render thread)?

	Result.Texture = UTexture2D::CreateTransient(Image.GetWidth(), Image.GetHeight(), PF_B8G8R8A8);

	Result.Texture->SRGB = true;
	auto& BulkData = Result.Texture->GetPlatformData()->Mips[0].BulkData;

	void* TextureData = BulkData.Lock(LOCK_READ_WRITE);	
	FMemory::Memcpy(TextureData, Image.RawData.GetData(), Image.RawData.Num());
	BulkData.Unlock();

	Result.Texture->UpdateResource();

	return OnSuccess.Broadcast(Result);
}
