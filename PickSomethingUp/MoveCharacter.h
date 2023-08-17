// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Engine/Classes/GameFramework/SpringArmComponent.h>
#include <Engine/Classes/Camera/CameraComponent.h>
#include "MoveCharacter.generated.h"

UCLASS()
class PICKSOMETHINGUP_API AMoveCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMoveCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
		UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float SpringArmLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AActor* BeHitActor;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AActor* PickedActor;

protected:

	UFUNCTION()
		void MoveForward(float ScaleValue);
	UFUNCTION()
		void MoveRight(float ScaleValue);
	UFUNCTION()
		void Turn(float ScaleValue);
	UFUNCTION()
		void LookUp(float ScaleValue);
	UFUNCTION()
		void Jump_Start();
	UFUNCTION()
		void Jump_Stop();
	UFUNCTION()
		void PickUp();

public:

	UFUNCTION(BlueprintCallable, Category = "Pick Up event")
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(BlueprintCallable, Category = "Pick Up event")
		void Attach_Something(AActor* TargetActor, FName SocketName);
	UFUNCTION(BlueprintCallable, Category = "Pick Up event")
		void TransformPickedActor(AActor* TargetActor);
};
