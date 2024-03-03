#pragma once
#include "memory.h"
#include "class.h"
#include "xor.h"

class UClass
{
public:
	char _padding_0[0x48];
	UClass* SuperClass;
};

class UObject
{
public:
	void* VTableObject;
	int32_t ObjectFlags;
	int32_t InternalIndex;
	UClass* Class;
	FName Name;
	UObject* Outer;
	char _padding[0x8];

	static UObject* StaticFindObject(UObject* Class, UObject* InOuter, const wchar_t* Name, bool ExactClass)
	{
		return reinterpret_cast<UObject * (__fastcall*)(UObject*, UObject*, const wchar_t*, bool)>(base + offset::static_find_object)(Class, InOuter, Name, ExactClass);
	}

	static void ProcessEvent(void* class_, UObject* function, void* params)
	{
		reinterpret_cast<void (*)(void*, UObject*, void*)>(base + offset::process_event)(class_, function, params);
	}

	bool IsA(UObject* _class)
	{
		for (auto super = this->Class; super; super = super->SuperClass) {
			if (super == (void*)_class) {
				return true;
			}
		}

		return true;
	}
};

struct UEngine : UObject {
	char pad_30[0x8]; // 0x30(0x08)
	 UObject* TinyFont; // 0x38(0x08)
	 FSoftObjectPath TinyFontName; // 0x40(0x20)
	 UObject* SmallFont; // 0x60(0x08)
	 FSoftObjectPath SmallFontName; // 0x68(0x20)
	 UObject* MediumFont; // 0x88(0x08)
	 FSoftObjectPath MediumFontName; // 0x90(0x20)
	 UObject* LargeFont; // 0xb0(0x08)
	 FSoftObjectPath LargeFontName; // 0xb8(0x20)
	 UObject* SubtitleFont; // 0xd8(0x08)
	 FSoftObjectPath SubtitleFontName; // 0xe0(0x20)
	 TArray< UObject*> AdditionalFonts; // 0x100(0x10)
	 TArray< FString> AdditionalFontNames; // 0x110(0x10)
};

struct UWorld
{

};

struct UCanvas
{
	FVector2D GetSize()
	{
		float x = (float)*(int*)((uint64_t)this + 0x48);
		float y = (float)*(int*)((uint64_t)this + 0x48 + 0x4);
		return { x,y };
	}

	FVector2D K2_TextSize(UObject* Font, FString RenderText, FVector2D Scale)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.Canvas.K2_TextSize"), false));

		struct {
			UObject* Font;
			FString RenderText;
			FVector2D Scale;
			FVector2D out;
		}Args = { Font, RenderText, Scale };

		Function->ProcessEvent(this, Function, &Args);
		return Args.out;
	}

	void K2_DrawLine(FVector2D ScreenPositionA, FVector2D ScreenPositionB, float Thickness, FLinearColor RenderColor, bool Outline = false)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.Canvas.K2_DrawLine"), false));

		struct {
			FVector2D ScreenPositionA;
			FVector2D ScreenPositionB;
			float Thickness;
			FLinearColor RenderColor;
		}Args;

		if (Outline)
		{
			Args.ScreenPositionA = ScreenPositionA;
			Args.ScreenPositionB = ScreenPositionB;
			Args.Thickness = Thickness + 1.f;
			Args.RenderColor = { 0,0,0,1 };
			Function->ProcessEvent(this, Function, &Args);
		}

		Args.ScreenPositionA = ScreenPositionA;
		Args.ScreenPositionB = ScreenPositionB;
		Args.Thickness = Thickness;
		Args.RenderColor = RenderColor;

		Function->ProcessEvent(this, Function, &Args);
	}

	void K2_DrawText(UObject* Font, FString RenderText, FVector2D ScreenPosition, FVector2D Scale, FLinearColor RenderColor, float Kerning, FLinearColor ShadowColor, FVector2D ShadowOffset, bool bCentreX, bool bCentreY, bool bOutlined, FLinearColor OutlineColor)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.Canvas.K2_DrawText"), false));

		struct 
		{
			UObject* RenderFont;
			FString RenderText;
			FVector2D ScreenPosition;
			FVector2D Scale;
			FLinearColor RenderColor;
			float Kerning;
			FLinearColor ShadowColor;
			FVector2D ShadowOffset;
			bool bCentreX;
			bool bCentreY;
			bool bOutlined;
			FLinearColor OutlineColor;
		} param = { Font, RenderText, ScreenPosition, Scale, RenderColor, Kerning , ShadowColor, ShadowOffset, bCentreX, bCentreY, bOutlined, OutlineColor};

		Function->ProcessEvent(this, Function, &param);
	}

	void K2_DrawTexture(UObject* RenderTexture, FVector2D ScreenPosition, FVector2D ScreenSize, FVector2D CoordinatePosition, FVector2D CoordinateSize, FLinearColor RenderColor, EBlendMode BlendMode, float Rotation, FVector2D PivotPoint)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.Canvas.K2_DrawTexture"), false));

		struct
		{
			UObject* RenderTexture;
			FVector2D ScreenPosition;
			FVector2D ScreenSize;
			FVector2D CoordinatePosition;
			FVector2D CoordinateSize;
			FLinearColor RenderColor;
			EBlendMode BlendMode;
			float Rotation;
			FVector2D PivotPoint;
		} param = { RenderTexture, ScreenPosition, ScreenSize, CoordinatePosition, CoordinateSize, RenderColor, BlendMode, Rotation, PivotPoint };

		Function->ProcessEvent(this, Function, &param);
	}

	void K2_DrawBox(FVector2D initial_pos, float w, float h, FLinearColor color)
	{
		for (float i = 0.0f; i < h; i += 1.0f)
			K2_DrawLine(FVector2D{ initial_pos.X, initial_pos.Y + i }, FVector2D{ initial_pos.X + w, initial_pos.Y + i }, 1.0f, color);
	}
};

struct USkeletalMeshComponent
{
	FVector GetBoneWithRotation(uint32_t index)
	{
		FMatrix matrix{};
		reinterpret_cast<FMatrix* (__fastcall*)(USkeletalMeshComponent*, FMatrix*, int32_t)>(base + offset::bone_matrix)(this, &matrix, index);
		return { matrix.WPlane.X,matrix.WPlane.Y, matrix.WPlane.Z };
	}
};

struct AGun_C : UObject
{
	static UObject* Static_Class()
	{
		static_var(UObject*, object, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Gun.Gun_C"), false));
		return object;
	}

	bool HasMuzzle()
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.AresEquippable.HasMuzzle"), false));

		struct
		{
			bool Out;
		}Parameters;
		Function->ProcessEvent(this, Function, &Parameters);
		return Parameters.Out;
	}

	int GetMaxCharges()
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.AresEquippable.GetMaxCharges"), false));

		struct
		{
			int Out;
		}Parameters;
		Function->ProcessEvent(this, Function, &Parameters);
		return Parameters.Out;
	}
};

struct UAresInventory
{
	AGun_C* GetCurrentEquippable()
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.AresInventory.GetCurrentEquippable"), false));

		struct
		{
			AGun_C* Out;
		}Parameters;
		Function->ProcessEvent(this, Function, &Parameters);
		return Parameters.Out;
	}
};

struct AShooterCharacter : public UObject
{
	static UObject* Static_Class()
	{
		static_var(UObject*, object, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.ShooterCharacter"), false));
		return object;
	}

	USkeletalMeshComponent* GetPawnMesh()
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.ShooterCharacter.GetPawnMesh"), false));
		
		struct
		{
			USkeletalMeshComponent* Out;
		}Parameters;
		Function->ProcessEvent(this, Function, &Parameters);
		return Parameters.Out;
	}

	UAresInventory* GetInventory()
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.ShooterCharacter.GetInventory"), false));

		struct
		{
			UAresInventory* Out;
		}Parameters;
		Function->ProcessEvent(this, Function, &Parameters);
		return Parameters.Out;
	}

	bool IsAlive()
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.ShooterCharacter.IsAlive"), false));

		struct
		{
			bool Out;
		}Parameters;
		Function->ProcessEvent(this, Function, &Parameters);
		return Parameters.Out;
	}

	FVector K2_GetActorLocation()
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.Actor.K2_GetActorLocation"), false));

		struct
		{
			FVector Out;
		}Parameters;
		Function->ProcessEvent(this, Function, &Parameters);
		return Parameters.Out;
	}

	FVector2D GetHealth()
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.ShooterCharacter.GetHealth"), false));

		struct
		{
			float Out;
		}Parameters;
		Function->ProcessEvent(this, Function, &Parameters);

		static_var(UObject*, Function_, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.ShooterCharacter.GetMaxHealth"), false));

		struct
		{
			int32_t Out;
		}Args;
		Function->ProcessEvent(this, Function_, &Args);

		return { (float)Args.Out, Parameters.Out };
	}

	FVector2D GetShield()
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.ShooterCharacter.GetShield"), false));

		struct
		{
			float Out;
		}Parameters;
		Function->ProcessEvent(this, Function, &Parameters);

		static_var(UObject*, Function_, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.ShooterCharacter.GetMaxShield"), false));

		struct
		{
			float Out;
		}Args;
		Function->ProcessEvent(this, Function_, &Args);

		return { Args.Out, Parameters.Out };
	}

	FName GetFName()
	{
		return *(uint32_t*)((uint64_t)this + 0x18);
	}

	void GetActorBounds(bool bOnlyCollidingComponents, FVector* Origin, FVector* BoxExtent, bool bIncludeFromChildActors)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.Actor.GetActorBounds"), false));

		struct
		{
			bool bOnlyCollidingComponents;
			FVector Origin;
			FVector BoxExtent;
			bool bIncludeFromChildActors;
		}Parameters;

		Parameters.bOnlyCollidingComponents = bOnlyCollidingComponents;
		Parameters.bIncludeFromChildActors = bIncludeFromChildActors;

		Function->ProcessEvent(this, Function, &Parameters);

		*Origin = Parameters.Origin;
		*BoxExtent = Parameters.BoxExtent;
	}

	UObject* GetCharacterIcon()
	{
		// ShooterGame.ShooterCharacter.GetCharacterIcon
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.ShooterCharacter.GetCharacterIcon"), false));

		struct
		{
			UObject* out;
		}Parameters;
		Function->ProcessEvent(this, Function, &Parameters);

		return Parameters.out;
	}
};

struct APlayerCameraManager
{
	FVector GetCameraLocation()
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.PlayerCameraManager.GetCameraLocation"), false));

		struct
		{
			FVector Out;
		}Parameters;

		Function->ProcessEvent(this, Function, &Parameters);
		return Parameters.Out;
	}

	FRotator GetCameraRotation()
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.PlayerCameraManager.GetCameraRotation"), false))

		struct
		{
			FRotator Out;
		}Parameters;

		Function->ProcessEvent(this, Function, &Parameters);
		return Parameters.Out;
	}
};

struct AHUD
{
	void _DrawText(FString RenderText, FVector2D ScreenPosition)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.HUD.DrawText"), false));

		struct
		{
			FString Text;
			FLinearColor TextColor;
			float ScreenX;
			float ScreenY;
			UObject* Font;
			float Scale;
			bool bScalePosition;
		} param = { RenderText, {1,1,1,1}, ScreenPosition.X, ScreenPosition.Y, nullptr, 15.f, false };

		Function->ProcessEvent(this, Function, &param);
	}
};

struct APlayerController
{
	AHUD* GetHUD()
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.PlayerController.GetHUD"), false))

			struct
		{
			AHUD* out;
		}Parameters;

		Function->ProcessEvent(this, Function, &Parameters);
		return Parameters.out;
	}

	void SimulateInputKey(FKey key, bool bDown)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.AresPlayerController.SimulateInputKey"), false))

			struct
		{
			FKey key;
			bool bPressed;
		}Parameters;

		Parameters.key = key;
		Parameters.bPressed = bDown;
		Function->ProcessEvent(this, Function, &Parameters);
	}

	FRotator GetControlRotation()
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.Controller.GetControlRotation"), false))

			struct
		{
			FRotator Out;
		}Parameters;

		Function->ProcessEvent(this, Function, &Parameters);
		return Parameters.Out;
	}

	APlayerCameraManager* GetPlayerCameraManager()
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.AresPlayerController.GetPlayerCameraManager"), false))

		struct
		{
			APlayerCameraManager* Out;
		}Parameters;

		Function->ProcessEvent(this, Function, &Parameters);
		return Parameters.Out;
	}

	void SetClientRotation(FRotator Rotation)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.Controller.ClientSetRotation"), false))

		struct
		{
			FRotator newRotation;
			bool bResetCamera;
		}Parameters;

		Parameters.newRotation = Rotation;
		Parameters.bResetCamera = false;

		Function->ProcessEvent(this, Function, &Parameters);
	}

	AShooterCharacter* GetShooterCharacter()
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.AresPlayerController.GetShooterCharacter"), false))

		struct
		{
			AShooterCharacter* Out;
		}Parameters;

		Function->ProcessEvent(this, Function, &Parameters);

		return Parameters.Out;
	}

	bool ProjectWorldToScreen(FVector WorldLocation, FVector2D* ScreenLocation)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.PlayerController.ProjectWorldLocationToScreen"), false))

		struct {
			FVector WorldLocationn;
			FVector2D ScreenLocationn;
			bool bPlayerViewportRelative;
			bool Out;
		}Parameters;

		Parameters.WorldLocationn = WorldLocation;
		Parameters.bPlayerViewportRelative = false;

		Function->ProcessEvent(this, Function, &Parameters);

		*ScreenLocation = Parameters.ScreenLocationn;

		return Parameters.Out;
	}

	bool LineOfSightTo(AShooterCharacter* Actor, FVector ViewPoint, bool bAlternateChecks)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.Controller.LineOfSightTo"), false))

		struct
		{
			AShooterCharacter* Actor;
			FVector ViewPoint;
			bool bAlternateChecks;
			bool Out;
		}Parameters;
		Parameters.Actor = Actor;
		Parameters.ViewPoint = ViewPoint;
		Parameters.bAlternateChecks = bAlternateChecks;
		Function->ProcessEvent(this, Function, &Parameters);
		return Parameters.Out;
	}

	float GetInputKeyTimeDown(FKey Key)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.PlayerController.GetInputKeyTimeDown"), false))

			struct
		{
			FKey Key;
			float Out;
		}Parameters;

		Parameters.Key = Key;
		Function->ProcessEvent(this, Function, &Parameters);
		return Parameters.Out;
	}

	bool IsInputKeyDown(FKey Key)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.PlayerController.IsInputKeyDown"), false))

		struct
		{
			FKey Key;
			bool Out;
		}Parameters;

		Parameters.Key = Key;
		Function->ProcessEvent(this, Function, &Parameters);
		return Parameters.Out;
	}

	FVector2D GetMousePosition()
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.PlayerController.GetMousePosition"), false))

		struct
		{
			float x;
			float y;
			bool out;
		}Parameters;

		Function->ProcessEvent(this, Function, &Parameters);
		return FVector2D{ Parameters.x,Parameters.y };
	}
};

struct USceneComponent
{

};

namespace ShooterGameBlueprints
{
	UObject* Static_Class()
	{
		static_var(UObject*, object, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.Default__ShooterBlueprintLibrary"), false));
		return object;
	}

	APlayerController* GetFirstLocalPlayerController(UWorld* WorldContextObject)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.ShooterBlueprintLibrary.GetFirstLocalPlayerController"), false))
		
		struct
		{
			UWorld* WorldContextObject;
			APlayerController* Out;
		}Parameters;
		Parameters.WorldContextObject = WorldContextObject;
		Function->ProcessEvent(Static_Class(), Function, &Parameters);
		return Parameters.Out;
	}

	TArray<AShooterCharacter*> FindAllShooterCharactersWithAlliance(UWorld* WorldContextObject, AShooterCharacter* LocalViewer, EAresAlliance Alliance, bool OnlyPlayerControlled, bool OnlyAlivePlayers)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.ShooterBlueprintLibrary.FindAllShooterCharactersWithAlliance"), false));
		
		struct
		{
			UWorld* WorldContextObject;
			AShooterCharacter* LocalViewer;
			EAresAlliance Alliance;
			bool OnlyPlayerControlled;
			bool OnlyAlivePlayers;
			TArray<AShooterCharacter*> Out;
		}Parameters;
		Parameters.WorldContextObject = WorldContextObject;
		Parameters.LocalViewer = LocalViewer;
		Parameters.Alliance = Alliance;
		Parameters.OnlyPlayerControlled = OnlyPlayerControlled;
		Parameters.OnlyAlivePlayers = OnlyAlivePlayers;
		Function->ProcessEvent(Static_Class(), Function, &Parameters);
		return Parameters.Out;
	}
}

namespace BaseTeamComponent
{
	UObject* Static_Class()
	{
		static_var(UObject*, object, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.Default__BaseTeamComponent"), false));
		return object;
	}

	bool IsAlly(AShooterCharacter* Enemy, AShooterCharacter* Self)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"ShooterGame.BaseTeamComponent.IsAlly"), false));
		
		struct {
			AShooterCharacter* Enemy;
			AShooterCharacter* Self;
			bool ReturnValue;
		}Parameters;
		Parameters.Enemy = Enemy;
		Parameters.Self = Self;
		Function->ProcessEvent(Static_Class(), Function, &Parameters);
		return Parameters.ReturnValue;
	}
}

namespace KismetSystemLibrary
{
	UObject* Static_Class()
	{
		static_var(UObject*, object, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.KismetSystemLibrary"), false));
		return object;
	}

	FString GetObjectName(UObject* obj)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.KismetSystemLibrary.GetObjectName"), false));

		struct
		{
			UObject* obj;
			FString out;
		}Args;
		Args.obj = obj;
		Function->ProcessEvent(Static_Class(), Function, &Args);

		return Args.out;
	}

	bool LineTraceSingle(UWorld* world, TArray<AShooterCharacter*> actors, FVector start, FVector end, FHitResult* hit)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, L"Engine.KismetSystemLibrary.LineTraceSingle", false));

		struct
		{
			UWorld* WorldContextObject;
			FVector Start;
			FVector End;
			ETraceTypeQuery TraceChannel;
			bool bTraceComplex;
			TArray<AShooterCharacter*> ActorsToIgnore;
			EDrawDebugTrace DrawDebugType;
			FHitResult OutHit;
			bool bIgnoreSelf;
			FLinearColor TraceColor;
			FLinearColor TraceHitColor;
			float DrawTime;
			bool Out;
		} Args = { world, start, end, ETraceTypeQuery::TraceTypeQuery1, true, actors, EDrawDebugTrace::None, {}, true, {}, {}, 0.f, {} };

		Function->ProcessEvent(Static_Class(), Function, &Args);
		*hit = Args.OutHit;
		return Args.DrawTime != 0;
	}
}

namespace KismetSystemStringLibrary
{
	UObject* Static_Class()
	{
		static_var(UObject*, object, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.KismetStringLibrary"), false));
		return object;
	}

	FName Conv_StringToName(FString string)
	{
		static_var(UObject*, functions, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.KismetStringLibrary.Conv_StringToName"), false))

		struct
		{
			FString string;
			FName out;
		}Args;
		Args.string = string;
		functions->ProcessEvent(Static_Class(), functions, &Args);

		return Args.out;
	}

}

namespace KismetMathLibrary
{
	UObject* Static_Class()
	{
		static_var(UObject*, object, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.KismetMathLibrary"), false));
		return object;
	}

	FRotator RInterpTo(FRotator Current, FRotator Target, float DeltaTime, float InterpSpeed)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.KismetMathLibrary.RInterpTo"), false));

		struct
		{
			FRotator Current;
			FRotator Target;
			float DeltaTime;	
			float InterpSpeed;
			FRotator out;
		}Args = { Current, Target, DeltaTime, InterpSpeed };

		Function->ProcessEvent(Static_Class(), Function, &Args);
		return Args.out;
	}

	FRotator RInterpTo_Constant(FRotator Current, FRotator Target, float DeltaTime, float InterpSpeed)
	{
		static_var(UObject*, Function, UObject::StaticFindObject(nullptr, nullptr, xor_w(L"Engine.KismetMathLibrary.RInterpTo_Constant"), false));

		struct
		{
			FRotator Current;
			FRotator Target;
			float DeltaTime;
			float InterpSpeed;
			FRotator out;
		}Args = { Current, Target, DeltaTime, InterpSpeed };

		Function->ProcessEvent(Static_Class(), Function, &Args);
		return Args.out;
	}
}

inline FKey LeftButton, RightMouseButton, Insert, Up, Down, Tab, Left, Right, LeftShift;