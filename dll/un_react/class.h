#pragma once
#include "math.h"
#include "types.h"

#include <algorithm>

#define RAD2DEG(x) x * 180.f / (float)M_PI

struct FVector;

enum eAgent {
	Astra,
	Breach,
	Brimstone,
	Chamber,
	Cypher,
	Jett,
	Kay,
	Klljoy,
	Neon,
	Omen,
	Phoenix,
	Raze,
	Reyna,
	Sage,
	Skye,
	Sova,
	Viper,
	Yoru,
	Fade,
	Bot,
	MAX,
};

struct _state
{
	uint64_t keys[7];
};

struct FRotator 
{
	float Pitch; // 0x00(0x04)
	float Yaw; // 0x04(0x04)
	float Roll; // 0x08(0x04)

	FRotator(float pitch = 0, float yaw = 0, float roll = 0) : Pitch(pitch), Yaw(yaw), Roll(roll)
	{

	}

	float Distance(FRotator& r)
	{
		return __sqrtf((Pitch - r.Pitch) * (Pitch - r.Pitch) + (Yaw - r.Yaw) * (Yaw - r.Yaw));
	}

	FRotator operator*(float m) const
	{
		return { Pitch * m, Yaw * m, Roll * m };
	}

	FRotator operator-(const FRotator& r) const
	{
		return FRotator(Pitch - r.Pitch, Yaw - r.Yaw, Roll - r.Roll);
	}

	FRotator operator+(const FRotator& r)
	{
		return FRotator(Pitch + r.Pitch, Yaw + r.Yaw, Roll + r.Roll);
	}

	void clamp()
	{
		if (Yaw < 0.f)
			Yaw += 360.0f;
		else if (Yaw > 360.f)
			Yaw -= 360.0f;

		while (Pitch > 90.f)
			Pitch -= 180.f;

		while (Pitch < -90.f)
			Pitch = 180.f;

		if (Pitch < 0.f)
			Pitch += 360.f;


		Roll = 0.f;
	}
};

class FName {
public:
	FName() : comparison_index(uint32_t()), number(int32_t()) {};
	FName(uint32_t index) : comparison_index(index), number(int32_t()) {};

public:
	uint32_t comparison_index;
	int32_t number;
};

enum class EDrawDebugTrace : uint8_t {
	None = 0,
	ForOneFrame = 1,
	ForDuration = 2,
	Persistent = 3,
	EDrawDebugTrace_MAX = 4
};

enum class ETraceTypeQuery : uint8_t {
	TraceTypeQuery1 = 0,
	TraceTypeQuery2 = 1,
	TraceTypeQuery3 = 2,
	TraceTypeQuery4 = 3,
	TraceTypeQuery5 = 4,
	TraceTypeQuery6 = 5,
	TraceTypeQuery7 = 6,
	TraceTypeQuery8 = 7,
	TraceTypeQuery9 = 8,
	TraceTypeQuery10 = 9,
	TraceTypeQuery11 = 10,
	TraceTypeQuery12 = 11,
	TraceTypeQuery13 = 12,
	TraceTypeQuery14 = 13,
	TraceTypeQuery15 = 14,
	TraceTypeQuery16 = 15,
	TraceTypeQuery17 = 16,
	TraceTypeQuery18 = 17,
	TraceTypeQuery19 = 18,
	TraceTypeQuery20 = 19,
	TraceTypeQuery21 = 20,
	TraceTypeQuery22 = 21,
	TraceTypeQuery23 = 22,
	TraceTypeQuery24 = 23,
	TraceTypeQuery25 = 24,
	TraceTypeQuery26 = 25,
	TraceTypeQuery27 = 26,
	TraceTypeQuery28 = 27,
	TraceTypeQuery29 = 28,
	TraceTypeQuery30 = 29,
	TraceTypeQuery31 = 30,
	TraceTypeQuery32 = 31,
	TraceTypeQuery_MAX = 32,
	ETraceTypeQuery_MAX = 33
};

class FKey {
public:
	FKey() : name() {}
	FKey(FName name) : name(name) {}

public:
	FName name;
	uint8_t details[24] = {};
};

struct FVector
{
	float X, Y, Z;

	FVector() : X(0.f), Y(0.f), Z(0.f) {}

	FVector(float x, float y, float z) : X(x), Y(y), Z(z) {}

	FVector(float InF) : X(InF), Y(InF), Z(InF) { }

	inline float Dot(const FVector& b) const { return (X * b.X) + (Y * b.Y) + (Z * b.Z); }

	inline float MagnitudeSqr() const { return Dot(*this); }

	inline float Magnitude() const { return __sqrtf(MagnitudeSqr()); }

	float Size() const { return __sqrtf(X * X + Y * Y + Z * Z); }

	float Sum() const { return X + Y + Z; }

	float Size2D() const { return __sqrtf(X * X + Y * Y); }

	float SizeSquared() const { return X * X + Y * Y + Z * Z; }

	float DistTo(const FVector& V) const { return (*this - V).Size(); }

	FRotator Rotator() {
		float hyp = __sqrtf(X * X + Y * Y + Z * Z);

		FRotator r{};
		r.Pitch = RAD2DEG(__acosf(Z / hyp));
		r.Yaw = RAD2DEG(__atan2f(Y, X)) + 180.f;
		r.Roll = 0;

		r.Pitch += 270.f;
		if (r.Pitch > 360.f) {
			r.Pitch -= 360.f;
		}

		if (r.Yaw < 0.f) {
			r.Yaw += 360.f;
		}

		return r;
	}

	FVector operator+(const FVector& other) const { return FVector(X + other.X, Y + other.Y, Z + other.Z); }

	FVector operator-(const FVector& other) const { return FVector(X - other.X, Y - other.Y, Z - other.Z); }

	FVector operator*(const FVector& V) const { return FVector(X * V.X, Y * V.Y, Z * V.Z); }

	FVector operator/(const FVector& V) const { return FVector(X / V.X, Y / V.Y, Z / V.Z); }

	bool operator==(const FVector& V) const { return X == V.X && Y == V.Y && Z == V.Z; }

	bool operator!=(const FVector& V) const { return X != V.X || Y != V.Y || Z != V.Z; }

	FVector operator-() const { return FVector(-X, -Y, -Z); }

	FVector operator+(float Bias) const { return FVector(X + Bias, Y + Bias, Z + Bias); }

	FVector operator-(float Bias) const { return FVector(X - Bias, Y - Bias, Z - Bias); }

	FVector operator*(float Scale) const { return FVector(X * Scale, Y * Scale, Z * Scale); } const

	FVector operator/(float Scale) const { const float RScale = 1.f / Scale; return FVector(X * RScale, Y * RScale, Z * RScale); }

	FVector operator=(const FVector& V) { X = V.X; Y = V.Y; Z = V.Z; return *this; }

	FVector operator+=(const FVector& V) { X += V.X; Y += V.Y; Z += V.Z; return *this; }

	FVector operator-=(const FVector& V) { X -= V.X; Y -= V.Y; Z -= V.Z; return *this; }

	FVector operator*=(const FVector& V) { X *= V.X; Y *= V.Y; Z *= V.Z; return *this; }

	FVector operator/=(const FVector& V) { X /= V.X; Y /= V.Y; Z /= V.Z; return *this; }

	FVector operator*=(float Scale) { X *= Scale; Y *= Scale; Z *= Scale; return *this; }

	FVector operator/=(float V) { const float RV = 1.f / V; X *= RV; Y *= RV; Z *= RV; return *this; }

	float operator|(const FVector& V) const { return X * V.X + Y * V.Y + Z * V.Z; }

	FVector operator^(const FVector& V) const { return FVector(Y * V.Z - Z * V.Y, Z * V.X - X * V.Z, X * V.Y - Y * V.X); }
	inline float Distance(FVector v) {
		auto delta = *this - v;
		return delta.Size();
	}
	bool IsValid()
	{
		if (X == 0 || Y == 0 || Z == 0)
			return false;
		else
			return true;
	}

	static FVector ZeroVector;

	static FVector OneVector;
};

FVector FVector::ZeroVector = FVector{};

class FLinearColor
{
public:
	float R, G, B, A;

	FLinearColor(float r = 0, float g = 0, float b = 0, float a = 1) : R(r), G(g), B(b), A(a)
	{
	}
};

// Enum Engine.EBlendMode
enum class EBlendMode : uint8_t {
	BLEND_Opaque = 0,
	BLEND_Masked = 1,
	BLEND_Translucent = 2,
	BLEND_Additive = 3,
	BLEND_Modulate = 4,
	BLEND_AlphaComposite = 5,
	BLEND_AlphaHoldout = 6,
	BLEND_MAX = 7
};

struct FVector2D
{
	float X, Y;

	constexpr FVector2D() : X(0), Y(0) {}

	constexpr FVector2D(float x, float y) : X(x), Y(y) {}

	float Size() const { return __sqrtf(X * X + Y * Y); }

	FVector2D operator + (const FVector2D& other) const { return FVector2D(X + other.X, Y + other.Y); }

	FVector2D operator- (const FVector2D& other) const { return FVector2D(X - other.X, Y - other.Y); }

	FVector2D operator* (float scalar) const { return FVector2D(X * scalar, Y * scalar); }

	FVector2D operator/ (float scalar) const { return FVector2D(X / scalar, Y / scalar); }

	FVector2D& operator=  (const FVector2D& other) { X = other.X; Y = other.Y; return *this; }

	FVector2D& operator+= (const FVector2D& other) { X += other.X; Y += other.Y; return *this; }

	FVector2D& operator-= (const FVector2D& other) { X -= other.X; Y -= other.Y; return *this; }

	FVector2D& operator*= (const float other) { X *= other; Y *= other; return *this; }

	friend bool operator==(const FVector2D& one, const FVector2D& two) { return one.X == two.X && one.Y == two.Y; }

	friend bool operator!=(const FVector2D& one, const FVector2D& two) { return !(one == two); }

	friend bool operator>(const FVector2D& one, const FVector2D& two) { return one.X > two.X && one.Y > two.Y; }

	friend bool operator<(const FVector2D& one, const FVector2D& two) { return one.X < two.X && one.Y < two.Y; }
	bool IsValid()
	{
		if (X == 0 || Y == 0)
			return false;
		else
			return true;
	}
};

struct FMinimalViewInfo {
	FVector Location; // 0x00(0x0c)
	FRotator Rotation; // 0x0c(0x0c)
	float FOV; // 0x18(0x04)
};

struct alignas(16) FPlane : public FVector {
	float W;
};

enum class EAresAlliance : uint8_t {
	Alliance_Ally = 0,
	Alliance_Enemy = 1,
	Alliance_Neutral = 2,
	Alliance_Any = 3,
	Alliance_Count = 4,
	Alliance_MAX = 5
};

enum class EAresOutlineMode : uint8_t {
	None = 0,
	Outline = 1,
	Block = 2,
	Enemy = 3,
	AlwaysOutline = 4,
	AlwaysEnemy = 5,
	EAresOutlineMode_MAX = 6
};

struct FMatrix {
	struct FPlane XPlane;
	struct FPlane YPlane;
	struct FPlane ZPlane;
	struct FPlane WPlane;
};


template<class T>
struct TArray
{
	friend struct FString;

public:
	inline TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	inline TArray(T* data, int count)
	{
		Data = data;
		Count = Max = count;
	};

	inline int Num() const
	{
		return Count;
	};

	inline T& operator[](int i)
	{
		return Data[i];
	};

	inline const T& operator[](int i) const
	{
		return Data[i];
	};

	inline bool IsValidIndex(int i) const
	{
		return i < Num();
	}

	T* Data;
	int32_t Count;
	int32_t Max;
};

struct FString : public TArray<wchar_t>
{
private:
	auto wcslen(const wchar_t* str)
	{
		int i = 0;
		while (str[i])
			i++;

		return i;
	}

public:
	inline FString()
	{
	};

	FString(const wchar_t* other)
	{
		Max = Count = *other ? static_cast<int>(this->wcslen(other)) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(other);
		}
	};
	FString(const wchar_t* other, int count)
	{
		Data = const_cast<wchar_t*>(other);
		Max = Count = count;
	};

	inline bool IsValid() const
	{
		return Data != nullptr;
	}

	inline const wchar_t* c_str() const
	{
		return Data;
	}
};

enum Bone : int
{
	Skeleton = 0,
	Root = 1,

	MaleFemaleRThigh1 = 82,
	MaleFemaleRThigh2 = 83,
	MaleFemaleRFoot = 88,
	MaleFemaleLThigh1 = 75,
	MaleFemaleLThigh2 = 76,
	MaleFemaleLFoot = 81,
	MaleFemaleLUpperarm = 47,
	MaleFemaleLForearm1 = 42,
	MaleFemaleLForearm2 = 43,
	MaleFemaleRUpperarm = 73,
	MaleFemaleRForearm1 = 68,
	MaleFemaleRForearm2 = 69,
	MaleFemaleNeck = 21,

	FemaleRFoot = 85,
	FemaleLFoot = 78,
	FemaleLUpperarm = 45,
	FemaleLForearm1 = 41,
	FemaleLForearm2 = 42,
	FemaleRUpperarm = 48,
	FemaleRForearm1 = 67,
	FemaleRForearm2 = 69,
	FemaleNeck = 21,

	DummyRThigh1 = 77,
	DummyRThigh2 = 79,
	DummyRFoot = 83,
	DummyLThigh1 = 63,
	DummyLThigh2 = 65,
	DummyLFoot = 69,
	DummyLUpperarm = 33,
	DummyLForearm1 = 30,
	DummyLForearm2 = 32,
	DummyRUpperarm = 58,
	DummyRForearm1 = 55,
	DummyRForearm2 = 57,
	DummyNeck = 9,

	GenericChest = 6,
	GenericStomach = 4,
	GenericPelvis = 3,
	GenericHead = 8,
};

struct FSoftObjectPath {
	FName AssetPathName; // 0x00(0x0c)
	char pad_C[0x4]; // 0x0c(0x04)
	FString SubPathString; // 0x10(0x10)
};

struct FHitResult 
{
	int32_t FaceIndex; // 0x00(0x04)
	float Time; // 0x04(0x04)
	float Distance; // 0x08(0x04)
	FVector Location; // 0x0c(0x0c)
	FVector ImpactPoint; // 0x18(0x0c)
	FVector Normal; // 0x24(0x0c)
	FVector ImpactNormal; // 0x30(0x0c)
	FVector TraceStart; // 0x3c(0x0c)
	FVector TraceEnd; // 0x48(0x0c)
	float PenetrationDepth; // 0x54(0x04)
	int32_t Item; // 0x58(0x04)
	char ElementIndex; // 0x5c(0x01)
	char bBlockingHit; // 0x5d(0x01)
	char bStartPenetrating; // 0x5d(0x01)
	char pad_5D_2; // 0x5d(0x01)
	char pad_5E[0x2]; // 0x5e(0x02)
	uint64_t* PhysMaterial; // 0x60(0x08)
	uint64_t* Actor; // 0x68(0x08)
	uint64_t* Component; // 0x70(0x08)
	FName BoneName; // 0x78(0x0c)
	FName MyBoneName; // 0x84(0x0c)
};

FVector RotToFVector(FRotator in)
{
	float sp, sy, cp, cy;

	sy = __sinf(in.Yaw);
	cy = __cosf(in.Yaw);
	sp = __sinf(in.Pitch);
	cp = __cosf(in.Pitch);

	return { cp * cy, cp * sy, -sp };
}