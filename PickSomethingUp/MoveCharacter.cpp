// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveCharacter.h"
#include <Engine/Classes/GameFramework/SpringArmComponent.h>
#include <Engine/Classes/Camera/CameraComponent.h>
#include <Engine/Classes/GameFramework/CharacterMovementComponent.h>
#include <Engine/Classes/Components/CapsuleComponent.h>
#include <Engine/Classes/Components/ActorComponent.h>
#include <Engine/Classes/GameFramework/Actor.h>


// Sets default values
AMoveCharacter::AMoveCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoReceiveInput = EAutoReceiveInput::Player0;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 170.0f));
	Camera->SetRelativeRotation(FRotator(-10.0f, 0.0f, 0.0f));


	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMoveCharacter::OnHit);
}

// Called when the game starts or when spawned
void AMoveCharacter::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BeginPlay Testing Message"));
	SpringArm->TargetArmLength = SpringArmLength;
}

// Called every frame
void AMoveCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMoveCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMoveCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMoveCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMoveCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMoveCharacter::LookUp);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMoveCharacter::Jump_Start);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMoveCharacter::Jump_Stop);
	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &AMoveCharacter::PickUp);
}

void AMoveCharacter::MoveForward(float ScaleValue)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, ScaleValue);
}

void AMoveCharacter::MoveRight(float ScaleValue)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, ScaleValue);
}

void AMoveCharacter::Turn(float ScaleValue)
{
	AddControllerYawInput(ScaleValue);
}

void AMoveCharacter::LookUp(float ScaleValue)
{
	AddControllerPitchInput(ScaleValue);
}

void AMoveCharacter::Jump_Start()
{
	bPressedJump = true;
}

void AMoveCharacter::Jump_Stop()
{
	bPressedJump = false;
}

void AMoveCharacter::PickUp()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("BePickingUpNow..."));
	if (BeHitActor->ActorHasTag("Actor.Gun.PickSomethingUp"))
	{
		Attach_Something(BeHitActor, "GripPoint");
	}
}

void AMoveCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	BeHitActor = OtherActor;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Detected Collision"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("OtherActor: %s"), *OtherActor->GetName()));
}

void AMoveCharacter::Attach_Something(AActor* TargetActor, FName SocketName)
{
	UStaticMeshComponent* TargetActorCollisionComponent = TargetActor->GetComponentByClass<UStaticMeshComponent>();
	TargetActorCollisionComponent->SetCollisionProfileName("NoCollision", true);

//	FVector TargetActorScale = TargetActor->GetActorScale();
//	TargetActor->SetActorScale3D(TargetActorScale);

	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	TargetActor->AttachToComponent(GetMesh(), AttachmentTransformRules, "GripPoint");
}