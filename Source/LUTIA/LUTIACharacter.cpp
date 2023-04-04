// Copyright Epic Games, Inc. All Rights Reserved.

#include "LUTIACharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ALUTIACharacter

ALUTIACharacter::ALUTIACharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = DefaultArmLength; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALUTIACharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ALUTIACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ALUTIACharacter::ShiftPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ALUTIACharacter::ShiftReleased);
	PlayerInputComponent->BindAction("Communicate", IE_Pressed, this, &ALUTIACharacter::CommuPressed);
	PlayerInputComponent->BindAction("Communicate", IE_Released, this, &ALUTIACharacter::CommuReleased);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ALUTIACharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ALUTIACharacter::MoveRight);
	

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ALUTIACharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ALUTIACharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("Camera Zoom In / Out", this, &ALUTIACharacter::CameraZoom);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ALUTIACharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ALUTIACharacter::TouchStopped);
}

void ALUTIACharacter::Jump()
{
	if (!IsCommunicating) 
	{
		bPressedJump = true;
		JumpKeyHoldTime = 0.0f;
	}
	
}

void ALUTIACharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ALUTIACharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ALUTIACharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ALUTIACharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ALUTIACharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f) && !IsCommunicating)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		if (IsShiftPressed)
			AddMovementInput(Direction, Value);
		else
			AddMovementInput(Direction, Value * 0.4);
	}
}

void ALUTIACharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) && !IsCommunicating)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		if (IsShiftPressed)
			AddMovementInput(Direction, Value);
		else
			AddMovementInput(Direction, Value * 0.4);
	}
}

void ALUTIACharacter::CameraZoom(float Value)
{
	float armLength = CameraBoom->TargetArmLength;

	if (armLength < MaxZoomLength && Value < 0)
		CameraBoom->TargetArmLength -= Value * ZoomStep;
	else if (armLength > MinZoomLength && Value > 0)
		CameraBoom->TargetArmLength -= Value * ZoomStep;
}

void ALUTIACharacter::ShiftPressed()
{
	IsShiftPressed = true;
}

void ALUTIACharacter::ShiftReleased()
{
	IsShiftPressed = false;
}

void ALUTIACharacter::CommuPressed()
{
	
}


void ALUTIACharacter::CommuReleased()
{
}
