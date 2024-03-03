#include "function.h"
#include "decryption.h"
#include "hook.h"
#include "interface.h"

void post_render(uint64_t dis, UCanvas* canvas)
{
	UWorld* world = stub<UWorld*>(dis + 0x80);

	if (world && canvas)
	{
		float best_fov = config::fov == 0.f ? FLT_MAX : config::fov;
		FVector target = FVector::ZeroVector;
		auto size = canvas->GetSize();

		auto controller = ShooterGameBlueprints::GetFirstLocalPlayerController(world);
		auto shooter = controller->GetShooterCharacter();
		auto actors = ShooterGameBlueprints::FindAllShooterCharactersWithAlliance(world, shooter, EAresAlliance::Alliance_Any, false, true);
		auto camera = controller->GetPlayerCameraManager();

		if (controller && shooter)
		{
			if (camera)
			{
				for (int i = 0; i < actors.Num(); i++)
				{
					auto actor = actors[i];

					if (!actor || !actor->IsAlive())
						continue;

					if (BaseTeamComponent::IsAlly(actor, shooter))
						continue;

					auto Life = actor->GetHealth();

					if (!Life.X || !Life.Y)
						continue;

					FVector org, ext;
					actor->GetActorBounds(true, &org, &ext, false);

					if (ext == FVector::ZeroVector)
						continue;

					if (auto mesh = actor->GetPawnMesh())
					{
						const auto root = mesh->GetBoneWithRotation(0);
						const auto forehead = mesh->GetBoneWithRotation(Bone::GenericHead);

						bool visible = controller->LineOfSightTo(actor, {}, false);
						auto color = visible ? FLinearColor{ 0, 1, 0, 1 } : FLinearColor{ 1, 0, 0, 1 };

						FVector2D out;
						if (controller->ProjectWorldToScreen(root, &out))
						{
							if (config::line)
								canvas->K2_DrawLine(out, { size.X / 2.f, size.Y * 0.95f }, 1.5f, color);

							if (config::box)
								draw_box(controller, canvas, root, ext);

							if (config::health)
							{
								FLinearColor health{ min((510 * (100 - (int)(Life.Y / Life.X * 100))) / 100 / 255.f, 1.f), min((510 * (int)(Life.Y / Life.X * 100)) / 100 / 255.f, 1.f), 0, 1.f };

								canvas->K2_DrawBox(FVector2D(out.X - 15.f, out.Y + 4), 30.f, 3, { 0,0,0,0.4f });
								canvas->K2_DrawBox(FVector2D(out.X - 15.f, out.Y + 4), 2.f * (Life.Y / Life.X * 15.f), 3, health);
							}

							if (config::shield)
							{
								FVector2D Shield = actor->GetShield();

								if (Shield.X)
								{
									canvas->K2_DrawBox(FVector2D(out.X - 15.f, out.Y + 10), 30.f, 3, { 0,0,0,0.4f });
									canvas->K2_DrawBox(FVector2D(out.X - 15.f, out.Y + 10), 2.f * (Shield.Y / Shield.X * 15.f), 3, { 1,1,1,1 });
								}
							}
	
							if (config::icon)
							{
								if (auto texture = actor->GetCharacterIcon())
								{
									canvas->K2_DrawTexture(texture, out - FVector2D(10, -10), FVector2D(20, 20), FVector2D(), FVector2D(1, 1),
										FLinearColor(1.f, 1.f, 1.f, 1.f), EBlendMode::BLEND_Translucent, 0.f, FVector2D());
								}
							}
						}

						if (controller->ProjectWorldToScreen(forehead, &out) || config::fov == 0.f)
						{
							float fov = FVector2D{ out - size / 2.f }.Size();

							if (fov < best_fov)
							{
								if (visible)
								{
									best_fov = fov;
									target = forehead;
								}
							}
						}
					}
				}

				if (config::aimbot &&
					target != FVector::ZeroVector)
				{
					if (controller->IsInputKeyDown(key_list[config::key]))
					{
						if (auto inventory = shooter->GetInventory())
						{
							if (auto weapon = inventory->GetCurrentEquippable())
							{
								FVector recoil = {};
								if (weapon->HasMuzzle())
									recoil = get_error_angle((uint64_t)shooter, stub<uint64_t>((uint64_t)weapon + 0x1030));

								auto org = controller->GetControlRotation();
								auto delta = camera->GetCameraLocation() - target;
								auto _final = delta.Rotator() - FRotator(recoil.X, recoil.Y, 0);

								if (config::speed != 1.f)
								{
									if (config::type == 0)
										_final = KismetMathLibrary::RInterpTo_Constant(org, _final, 0.3f, config::speed);
									else
										_final = KismetMathLibrary::RInterpTo(org, _final, 0.3f, config::speed);
								}

								if (config::speed == 1.f)
								{
									controller->SimulateInputKey(LeftButton, true);
									controller->SimulateInputKey(LeftButton, false);
								}

								controller->SetClientRotation(_final);
							}
						}
					}
				}
			}

			ui::update(canvas, controller);

			ui::input::tick();

			if (config::draw_fov)
				ui::renderer::draw_circle(size / 2.f, (int)config::fov, 128, { 1,1,1,1 });

			ui::draw();
		}
	}

	return reinterpret_cast<decltype(&post_render)>(o_post_render)(dis, canvas);
}

void main_thread()
{
	base = *(uint64_t*)(__readgsqword(0x60) + 0x10);

	uint32_t uworld_key = *(uint32_t*)(base + offset::uworld_key);
	_state state_key = *(_state*)(base + offset::uworld_state);
	uint64_t uworld_xor = decrypt::uworld(uworld_key, (uint64_t*)&state_key);

	uint64_t uworld = stub<uint64_t>(uworld_xor);
	
	uint64_t game_instance = stub<uint64_t>(uworld + offset::game_instance);

	uint64_t local_players = stub<uint64_t>(game_instance + offset::local_players);
	uint64_t local_player = stub<uint64_t>(local_players);

	uint64_t* viewport_client = stub<uint64_t*>(local_player + offset::viewport_client);

	{
		auto engine = stub<UEngine*>(game_instance + 0x28);
		font = engine->TinyFont;

		LeftButton = KismetSystemStringLibrary::Conv_StringToName(xor_w(L"LeftMouseButton"));
		RightMouseButton = KismetSystemStringLibrary::Conv_StringToName(xor_w(L"RightMouseButton"));
		Insert = KismetSystemStringLibrary::Conv_StringToName(xor_w(L"Insert"));
		Up = KismetSystemStringLibrary::Conv_StringToName(xor_w(L"Up"));
		Down = KismetSystemStringLibrary::Conv_StringToName(xor_w(L"Down"));
		Tab = KismetSystemStringLibrary::Conv_StringToName(xor_w(L"Tab"));
		Left = KismetSystemStringLibrary::Conv_StringToName(xor_w(L"Left"));
		Right = KismetSystemStringLibrary::Conv_StringToName(xor_w(L"Right"));
		LeftShift = KismetSystemStringLibrary::Conv_StringToName(xor_w(L"LeftShift"));

		key_list[0] = LeftButton;
		key_list[1] = RightMouseButton;
		key_list[2] = KismetSystemStringLibrary::Conv_StringToName(xor_w(L"MiddleMouseButton"));
		key_list[3] = KismetSystemStringLibrary::Conv_StringToName(xor_w(L"ThumbMouseButton"));
		key_list[4] = KismetSystemStringLibrary::Conv_StringToName(xor_w(L"ThumbMouseButton2"));
	}

	hook::vmt(viewport_client, create_jmp_hook(post_render), 0x6B, &o_post_render);
}

BOOL APIENTRY entry_dll(HINSTANCE h_module, DWORD dw_reason, LPVOID lpv_reserved)
{
	if (dw_reason == DLL_PROCESS_ATTACH)
		main_thread();
	
	return TRUE;
}
