// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMovieActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MediaSource.h"
#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "Components/WidgetComponent.h"
#include "MovieWidget.h"


// Sets default values
AMyMovieActor::AMyMovieActor()
{
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(rootComp);

	screenComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screen"));
	screenComp->SetupAttachment(RootComponent);

	soundComp = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("Sounds"));
	soundComp->SetupAttachment(RootComponent);

	playback_widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	playback_widget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMyMovieActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (playback_widget != nullptr)
	{
		UMovieWidget* movieWidget = Cast<UMovieWidget>(playback_widget->GetWidget());

		if (movieWidget != nullptr)
		{
			movieWidget->movieActor = this;
		}
	}
}

// Called every frame
void AMyMovieActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyMovieActor::PlayMovie()
{
	UE_LOG(LogTemp, Log, TEXT("Play Movie Function!!!!!!!!!"));

	// 미디어 플레이 테스트
	if (videoSouce != nullptr && mediaPlayer != nullptr)
	{
		// 미디어 플레이어에서 실행할 비디오 소스 파일을 연다.
		mediaPlayer->OpenSource(videoSouce);
		mediaPlayer->Play();
	}
}

void AMyMovieActor::StopMovie()
{

	if (mediaPlayer != nullptr)
	{
		mediaPlayer->Close();
	}
}

void AMyMovieActor::ReverseMovie(float second)
{

}

void AMyMovieActor::ForwardMovie(float second)
{

}

