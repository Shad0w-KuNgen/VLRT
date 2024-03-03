#pragma once
#include "sdk.h"

UObject* font;
void* o_post_render = NULL;

FKey key_list[5];

PVOID create_jmp_hook(void* function)
{
	PVOID page1 = VirtualAlloc(0, 0x10ui64, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	uint8_t jmp64[] = 
	{ 
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
	};

	*(PVOID*)(jmp64 + 6) = function;

	m_memcpy(page1, jmp64, sizeof(jmp64));

	DWORD old;
	VirtualProtect(page1, 0x10, PAGE_EXECUTE_READ, &old);

	return page1;
}


FRotator CalcAngle(FVector MyPos, FVector EnPos, FRotator Angle, float Speed = 1.f)
{
	FVector Result;

	Result.X = (EnPos.X - MyPos.X);
	Result.Y = (EnPos.Y - MyPos.Y);
	Result.Z = (EnPos.Z - MyPos.Z);

	FRotator rot = Result.Rotator();
	return rot;
	FRotator delta = rot - Angle;

	return Angle + delta * Speed;
}

FVector get_error_angle(uint64_t actor, uint64_t firing_state_component) {
	auto get_spread_values_fn =
		(float* (__fastcall*)(uint64_t, float*))(base + 0x2D2A8A0);
	auto get_spread_angles_fn =
		(void(__fastcall*)(uint64_t, FVector*, float, float, int, int, uint64_t))(base + 0x34C4430);
	auto get_firing_location_and_direction_fn =
		(void(__fastcall*)(uint64_t, FVector*, FVector*))(base + 0x32D0AA0);
	auto to_vector_and_normalize_fn =
		(FVector * (__fastcall*)(FVector*, FVector*))(base + 0x3BF2570);
	auto to_angle_and_normalize_fn =
		(FVector * (__fastcall*)(FVector*, FVector*))(base + 0x3BEED60);

	static uint8_t error_values[4096];
	static uint8_t seed_data_snapshot[4096];
	static uint8_t spread_angles[4096];
	static uint8_t out_spread_angles[4096];

	if (!actor || !firing_state_component)
		return FVector(0, 0, 0);

	m_memset(error_values, 0, sizeof(error_values));
	m_memset(seed_data_snapshot, 0, sizeof(seed_data_snapshot));
	m_memset(spread_angles, 0, sizeof(spread_angles));
	m_memset(out_spread_angles, 0, sizeof(out_spread_angles));

	*(uint64_t*)(&out_spread_angles[0]) = (uint64_t)&spread_angles[0];
	*(int*)(&out_spread_angles[0] + 8) = 1;
	*(int*)(&out_spread_angles[0] + 12) = 1; //It's a TArray but I'm lazy

	uint64_t seed_data = stub<uint64_t>(firing_state_component + 0x488); // OK
	m_memcpy((void*)seed_data_snapshot, (void*)seed_data, sizeof(seed_data_snapshot)); //Make our own copy since we don't want to desync our own seed component

	uint64_t stability_component = stub<uint64_t>(firing_state_component + 0x478); // OK
	if (stability_component)
		get_spread_values_fn(stability_component, (float*)&error_values[0]);

	FVector temp1, temp2 = FVector(0, 0, 0);
	FVector previous_firing_direction, firing_direction = FVector(0, 0, 0);
	get_firing_location_and_direction_fn(actor, &temp1, &previous_firing_direction);
	to_vector_and_normalize_fn(&previous_firing_direction, &temp2);
	to_angle_and_normalize_fn(&temp2, &temp1);
	previous_firing_direction = temp1;
	temp1.X += *(float*)(&error_values[0] + 12); //Your recoil angle
	temp1.Y += *(float*)(&error_values[0] + 16);
	to_vector_and_normalize_fn(&temp1, &firing_direction);

	float error_degrees = *(float*)(&error_values[0] + 8) + *(float*)(&error_values[0] + 4);
	float error_power = *(float*)(firing_state_component + 0x458); // OK
	int error_retries = *(int*)(firing_state_component + 0x45c); // OK
	get_spread_angles_fn(((uint64_t)&seed_data_snapshot[0]) + 0xE8, &firing_direction, error_degrees, error_power, error_retries, 1, (uint64_t)&out_spread_angles[0]);

	FVector spread_vector = *(FVector*)(&spread_angles[0]);
	to_angle_and_normalize_fn(&spread_vector, &firing_direction);

	return firing_direction - previous_firing_direction;
}

void draw_box(APlayerController* controller, UCanvas* canvas, FVector root, FVector ext)
{
	FVector2D pos[2][4]{};

	for (int j = 0; j < 4; j++)
	{
		float cos = (j % 2 ? 0.9f : -0.9f) * (j >= 2 ? -1 : 1); // 1, 3
		float sin = j < 2 ? -0.9f : 0.9f;

		if (!controller->ProjectWorldToScreen(root + FVector(ext.X / cos, ext.Y / sin, 0), &pos[0][j]) ||
			!controller->ProjectWorldToScreen(root + FVector(ext.X / cos, ext.Y / sin, ext.Z * 1.7f), &pos[1][j]))
			return;
	}

	for (int i = 0; i < 4; i++)
	{
		canvas->K2_DrawLine(pos[0][i], pos[0][(i + 1) % 4], 1.f, { 1,1,1,1 }, true);
		canvas->K2_DrawLine(pos[1][i], pos[1][(i + 1) % 4], 1.f, { 1,1,1,1 }, true);
	}

	for (int i = 0; i < 4; i++)
		canvas->K2_DrawLine(pos[0][i], pos[1][i], 1.f, { 1,1,1,1 }, true);
}