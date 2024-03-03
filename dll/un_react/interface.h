#pragma once
#include "sdk.h"
#include "config.h"

namespace ui
{
	float widget_y = 0;
	int current_index = 0;
	int widget_index = 0;
	int max_index = 0;

	bool opened = true;

	FVector2D screen_size;
	APlayerController* controller;
	UCanvas* canvas;

	constexpr float ui_width = 290.f;
	constexpr FVector2D smallbox = { 3.f, 25.f };
	constexpr FVector2D widget_text_pos = { smallbox.X + 30.f, smallbox.Y / 2.f };
	constexpr float padding = 0.3f;

	namespace input
	{
		struct input_info
		{
		private:
			bool bpressed = false;
			int held_count = 0;

		public:
			void update(FKey key)
			{
				if (controller->IsInputKeyDown(key))
				{
					bpressed = true;
					held_count++;
				}
				else
				{
					bpressed = false;
					held_count = 0;
				}
			}

			bool pressed(bool once = false)
			{
				if (once)
					return bpressed && held_count == 1;

				return bpressed;
			}
		};

		input_info down, up, left, right, insert, lshift;

		void tick()
		{
			down.update(Down);
			up.update(Up);
			left.update(Left);
			right.update(Right);
			insert.update(Insert);
			lshift.update(LeftShift);
		}
	}

	namespace renderer
	{
		void draw_circle(FVector2D pos, int radius, int numSides, FLinearColor Color)
		{
			float Step = PI * 2.f / numSides;
			int Count = 0;
			FVector2D cur, next;
			for (float a = 0; a < PI * 2.f; a += Step) {
				float X1 = radius * __cosf(a) + pos.X;
				float Y1 = radius * __sinf(a) + pos.Y;
				float X2 = radius * __cosf(a + Step) + pos.X;
				float Y2 = radius * __sinf(a + Step) + pos.Y;
				canvas->K2_DrawLine(FVector2D{ X1, Y1 }, FVector2D{ X2, Y2 }, 1.0f, Color);
			}
		}

		void draw_box(FVector2D initial_pos, float w, float h, FLinearColor color, float meter = 1.f)
		{
			for (float i = 0.0f; i < h; i += meter)
				canvas->K2_DrawLine(FVector2D{ initial_pos.X, initial_pos.Y + i }, FVector2D{ initial_pos.X + w, initial_pos.Y + i }, meter, color);
		}

		void widget_boolean(const wchar_t* text, bool* v)
		{
			if (widget_index == current_index)
			{
				draw_box({ smallbox.X, widget_y }, ui_width - smallbox.X, smallbox.Y, { 0.035f,0.035f,0.04f, 1.f });

				if (input::right.pressed(true))
					*v = false;

				if (input::left.pressed(true))
					*v = true;
			}

			canvas->K2_DrawText(font, text, widget_text_pos + FVector2D{ 0, widget_y }, { 0.9f, 0.9f }, { 1, 1, 1, 0.8f }, 0.f, {}, {}, false, true, false, {});

			if (*v)
			{
				float text_pos = ui_width - 20.f - canvas->K2_TextSize(font, xor_w(L"On"), { 0.9f, 0.9f }).X;
				canvas->K2_DrawText(font, xor_w(L"On"), FVector2D{ 0, widget_text_pos.Y + widget_y } + FVector2D{ text_pos, 0 }, { 0.9f, 0.9f }, { 0,1,0,1 }, 0.f, {}, {}, false, true, false, {});
			}
			else
			{
				float text_pos = ui_width - 20.f - canvas->K2_TextSize(font, xor_w(L"Off"), { 0.9f, 0.9f }).X;
				canvas->K2_DrawText(font, xor_w(L"Off"), FVector2D{ 0, widget_text_pos.Y + widget_y } + FVector2D{ text_pos, 0 }, { 0.9f, 0.9f }, { 1,0,0,1 }, 0.f, {}, {}, false, true, false, {});
			}

			draw_box({ 0, widget_y }, smallbox.X, smallbox.Y, { 1,1,1,1 });

			widget_y += smallbox.Y;
			widget_index++;
		}
		
		void widget_combo(const wchar_t* text, int* num, const wchar_t* list, int count)
		{
			const auto find_item = [&](int index)
				{
					if (index == 0)
					{
						return list;
					}

					int item_count = 1;
					for (int j = 0;; j++)
					{
						if (list[j] == '\0')
						{
							if (item_count == index)
							{
								return list + j + 1;
							}

							item_count++;
						}
					}

					return L"";
				};

			if (widget_index == current_index)
			{
				draw_box({ smallbox.X, widget_y }, ui_width - smallbox.X, smallbox.Y, { 0.035f,0.035f,0.04f, 1.f });

				if (input::right.pressed(true) && *num < count)
					*num += 1;

				if (input::left.pressed(true) && *num > 0)
					*num -= 1;
			}

			auto str = find_item(*num);
			float text_pos = ui_width - 20.f - canvas->K2_TextSize(font, str, { 0.9f, 0.9f }).X;
			canvas->K2_DrawText(font, str, FVector2D{ 0, widget_text_pos.Y + widget_y } + FVector2D{ text_pos, 0 }, { 0.9f, 0.9f }, { 1,1,1,1 }, 0.f, {}, {}, false, true, false, {});

			canvas->K2_DrawText(font, text, widget_text_pos + FVector2D{ 0, widget_y }, { 0.9f, 0.9f }, { 1, 1, 1, 0.8f }, 0.f, {}, {}, false, true, false, {});

			draw_box({ 0, widget_y }, smallbox.X, smallbox.Y, { 1,1,1,1 });

			widget_y += smallbox.Y;
			widget_index++;
		}

		void widget_slider(const wchar_t* text, float* num, float _min, float _max, float power = 10.f)
		{
			float cur = *num;
			if (widget_index == current_index)
			{
				draw_box({ smallbox.X, widget_y }, ui_width - smallbox.X, smallbox.Y, { 0.035f,0.035f,0.04f, 1.f });

				if (input::lshift.pressed(false))
					power *= 10.f;

				if (input::right.pressed(true))			
					*num = min(_max, cur + power);
				
				if (input::left.pressed(true))
					*num = max(_min, cur - power);
			}

			wchar_t str[64];
			sprintf(str, xor_w(L"%d.%d"), (int)cur, (int)(cur * 100.f - (int)cur * 100));

			float text_pos = ui_width - 20.f - canvas->K2_TextSize(font, str, { 0.9f, 0.9f }).X;
			canvas->K2_DrawText(font, str, FVector2D{ 0, widget_text_pos.Y + widget_y } + FVector2D{ text_pos, 0 }, { 0.9f, 0.9f }, { 1,1,1,1 }, 0.f, {}, {}, false, true, false, {});

			canvas->K2_DrawText(font, text, widget_text_pos + FVector2D{ 0, widget_y }, { 0.9f, 0.9f }, { 1, 1, 1, 0.8f }, 0.f, {}, {}, false, true, false, {});

			draw_box({ 0, widget_y }, smallbox.X, smallbox.Y, { 1,1,1,1 });

			widget_y += smallbox.Y;
			widget_index++;
		}

		void subtitle(const wchar_t* text)
		{
			canvas->K2_DrawText(font, text, FVector2D{ 0, widget_text_pos.Y + widget_y } + FVector2D{ ui_width / 2.f, 0 }, { 0.9f, 0.9f }, { 1,1,1,1 }, 0.f, {}, {}, true, true, false, {});

			widget_y += smallbox.Y + 5.f;
		}
	}

	void update(UCanvas* _canvas, APlayerController* _controller)
	{
		canvas = _canvas;
		controller = _controller;
		screen_size = canvas->GetSize();
	}

	void draw()
	{
		if (opened)
		{
			renderer::draw_box({ 0,0 }, ui_width, screen_size.Y, { 0.02f,0.02f,0.025f, 1.f }, 2.f);

			widget_index = 0;
			widget_y = 200.f;

			renderer::subtitle(xor_w(L"Visual"));

			if (current_index < 6)
			{
				renderer::widget_boolean(xor_w(L"Box"), &config::box);
				renderer::widget_boolean(xor_w(L"Line"), &config::line);
				renderer::widget_boolean(xor_w(L"Health"), &config::health);
				renderer::widget_boolean(xor_w(L"Shield"), &config::shield);
				renderer::widget_boolean(xor_w(L"Fov"), &config::draw_fov);
				renderer::widget_boolean(xor_w(L"Icon"), &config::icon);
			}
			else
				widget_index += 6;

			renderer::subtitle(xor_w(L"Aimbot"));

			if (current_index >= widget_index && current_index < widget_index + 5)
			{
				renderer::widget_boolean(xor_w(L"Aimbot"), &config::aimbot);
				renderer::widget_combo(xor_w(L"Smooth Type"), &config::type, xor_w(L"Constant\0Curve"), 1);
				renderer::widget_combo(xor_w(L"Key"), &config::key, xor_w(L"Left\0Right\0Middle\0XButton1\0XButton2"), 4);
				renderer::widget_slider(xor_w(L"Fov"), &config::fov, 0.f, 600.f);
				renderer::widget_slider(xor_w(L"Speed"), &config::speed, 0.f, 1.f, 0.01f);
			}
			else
				widget_index += 5;

			max_index = widget_index;

			if (input::up.pressed(true) && current_index > 0)
				current_index--;

			if (input::down.pressed(true) && current_index < max_index - 1)
				current_index++;
		}

		if (input::insert.pressed(true))
			opened = !opened;
	}
}