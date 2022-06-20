#include "FireStorage.h"
#include <filesystem>

namespace FiresStorage
{
	void Storage::add_support_vanilla()
	{
		auto hex = get_mod_index("Skyrim.esm"sv);
		register_data_entry(hex | 0xD61B6, { 6.06f, -13.00f, 43.19f }, { 702.14f, 686.65f, 64.38f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x1092E2, { -0.28f, 1.36f, 4.25f }, { 58.18f, 53.87f, 21.72f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xE4E22, { -48.00f, -8.00f, -2.00f }, { 128.00f, 96.00f, 64.00f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xE7C9A, { 8.96f, -6.59f, 19.64f }, { 94.41f, 104.59f, 73.27f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xC2BF1, { 4.41f, 3.80f, 38.51f }, { 129.18f, 111.38f, 111.01f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xAA71C, { 0.65f, -2.67f, 6.99f }, { 42.94f, 44.57f, 30.25f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xE4E24, { -9.00f, 0.50f, -2.50f }, { 92.00f, 65.00f, 3.00f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x101A51, { 3.80f, -3.02f, 23.80f }, { 108.79f, 97.53f, 81.60f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x35F4A, { 0.00f, 0.00f, 21.78f }, { 114.00f, 98.00f, 59.57f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x90CA4, { 2.95f, 1.34f, 38.16f }, { 118.75f, 111.29f, 110.33f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xEF2D6, { -0.65f, 1.26f, 6.04f }, { 52.53f, 52.38f, 35.27f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x16D4B, { 2.50f, -1.50f, 67.50f }, { 45.00f, 305.00f, 179.00f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x951D8, { 2.37f, 1.14f, 20.25f }, { 90.89f, 116.61f, 74.50f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xCBB2F, { 8.16f, -7.54f, 23.67f }, { 102.46f, 106.96f, 81.34f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xFFF43, { -0.10f, 0.10f, 571.32f }, { 48.18f, 51.82f, 1518.64f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xCD823, { -2.00f, -0.02f, 1.27f }, { 53.55f, 57.54f, 24.71f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x106112, { 0.52f, -3.15f, 5.43f }, { 62.68f, 54.64f, 30.59f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x3103C, { 8.50f, -1.50f, 70.50f }, { 83.00f, 81.00f, 173.00f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xFB9B0, { 9.19f, -7.62f, 31.44f }, { 90.04f, 95.34f, 96.88f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xDB682, { -1.76f, 5.68f, 24.00f }, { 98.57f, 104.18f, 81.99f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xBF9E1, { -6.04f, 175.48f, 13.00f }, { 122.21f, 88.01f, 22.56f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x3103F, { 4.00f, -1.50f, 70.50f }, { 84.00f, 81.00f, 173.00f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x13B42, { 0.00f, 0.00f, 4.00f }, { 60.00f, 60.00f, 8.00f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xF597F, { -15.79f, 1.62f, 210.36f }, { 427.37f, 389.90f, 548.71f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x108D7A, { 2.27f, -1.52f, -30.50f }, { 114.99f, 173.99f, 403.00f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x10D820, { 6.46f, 5.25f, 23.63f }, { 104.70f, 87.80f, 81.27f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xF6304, { 3.08f, -11.61f, 174.58f }, { 417.02f, 391.91f, 477.16f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xCD824, { 0.49f, 0.53f, 9.26f }, { 42.73f, 42.70f, 18.51f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x3BD2E, { -0.48f, -3.25f, 6.72f }, { 48.95f, 54.44f, 19.44f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x4318B, { -3.12f, 0.50f, 16.56f }, { 63.96f, 60.11f, 43.64f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xBBCF1, { -13.57f, 279.17f, 13.90f }, { 299.74f, 213.21f, 30.08f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x33DA4, { 3.02f, -3.54f, 7.07f }, { 70.65f, 55.49f, 32.57f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x35F47, { -1.88f, -1.02f, 27.88f }, { 95.70f, 118.41f, 89.76f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xD61BE, { -7.75f, -3.24f, 232.00f }, { 509.34f, 521.75f, 442.00f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xCAE0B, { -15.44f, 167.83f, 22.51f }, { 101.61f, 117.39f, 24.25f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xC6918, { -1.81f, 5.86f, 25.92f }, { 96.67f, 103.53f, 85.84f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xB6C08, { -1.32f, -7.49f, 36.08f }, { 87.35f, 121.65f, 106.16f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x30B39, { 5.00f, -1.50f, 110.00f }, { 84.00f, 81.00f, 248.00f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x51577, { 3.59f, -16.30f, 195.54f }, { 354.00f, 357.89f, 519.09f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x10ACC0, { 18.32f, -25.57f, 206.25f }, { 354.71f, 304.91f, 416.50f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xA9230, { -2.52f, 0.41f, 2.08f }, { 64.96f, 59.77f, 38.16f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x33DA9, { -2.10f, -1.84f, 5.41f }, { 48.31f, 53.07f, 16.53f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x35F49, { -2.46f, -4.85f, 36.89f }, { 90.96f, 113.08f, 67.30f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xEF77F, { 13.03f, -5.74f, 232.00f }, { 571.43f, 592.28f, 442.00f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x13B40, { 0.00f, 0.00f, 4.00f }, { 128.00f, 128.00f, 8.00f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xE4E23, { -21.00f, 0.50f, -3.00f }, { 92.00f, 65.00f, 2.00f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x4C5CF, { 7.07f, -3.69f, 202.13f }, { 418.99f, 384.54f, 532.26f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0xCBB23, { -5.04f, -7.50f, 26.40f }, { 114.13f, 98.69f, 86.80f }, { 0.0000f, 0.0000f, 0.0000f });
		
		set_magic_forms(hex, { 0xFFF43, 0x108D7A });
		set_steam_forms(hex, { 0x16D4B, 0x30B39, 0xE4E22, 0xE4E24, 0xE4E23, 0x3103F, 0x3103C });
	}

	void Storage::add_support_campfire() {
		auto hex = get_mod_index("Campfire.esm"sv);
		if (hex == -1)
			return;
		
		register_data_entry(hex | 0x33E67, { -8.21f, 2.23f, 48.34f }, { 101.98f, 89.54f, 117.80f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x6ABB2, { -3.30f, -2.41f, 22.84f }, { 105.07f, 83.25f, 79.68f }, { 0.0000f, 0.0000f, 314.1635f });
		register_data_entry(hex | 0x5C8D8, { -0.11f, -0.01f, 13.31f }, { 47.86f, 43.97f, 26.24f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x40013, { -1.72f, -0.18f, 18.70f }, { 53.24f, 58.36f, 40.02f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x33E69, { -0.65f, -7.32f, 19.34f }, { 92.83f, 99.29f, 72.68f }, { 0.0000f, 0.0000f, 34.3777f });
		register_data_entry(hex | 0x328A6, { 2.14f, -4.46f, 12.14f }, { 62.12f, 59.05f, 27.46f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x328A8, { 9.38f, 3.46f, 13.42f }, { 40.20f, 43.09f, 25.05f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x5C8D6, { -2.53f, -4.01f, 10.42f }, { 18.41f, 38.02f, 16.62f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x32333, { -1.96f, -1.68f, 6.65f }, { 51.61f, 32.79f, 20.89f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x328B9, { -3.92f, -3.67f, 15.55f }, { 92.24f, 96.44f, 47.10f }, { 0.0000f, 0.0000f, 302.7047f });
		register_data_entry(hex | 0x56204, { 0.00f, 0.00f, 4.00f }, { 128.00f, 128.00f, 8.00f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x32334, { 3.05f, -0.98f, 9.26f }, { 29.20f, 31.95f, 14.41f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x4F5EB, { 1.35f, 4.95f, 577.54f }, { 47.30f, 42.52f, 1531.08f }, { 0.0000f, 0.0000f, 0.0000f });
		
		set_magic_forms(hex, { 0x4F5EB });
	}

	void Storage::add_support_dragonborn() {
		auto hex = get_mod_index("Dragonborn.esm"sv);
		if (hex == -1)
			return;

		register_data_entry(hex | 0x18616, { -1.54f, -1.31f, 0.79f }, { 38.70f, 43.24f, 35.59f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x1D737, { -2.35f, -80.89f, -53.05f }, { 392.80f, 393.46f, 101.81f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x289E8, { -7.69f, 0.32f, 2.81f }, { 47.79f, 51.09f, 39.63f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x1D738, { 11.94f, -20.56f, -1.66f }, { 310.78f, 323.74f, 180.69f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x2C0B0, { -6.84f, -1.99f, 3.05f }, { 55.43f, 46.72f, 40.09f }, { 0.0000f, 0.0000f, 0.0000f });
		register_data_entry(hex | 0x18619, { -0.54f, -0.07f, 1.68f }, { 43.39f, 58.95f, 37.36f }, { 0.0000f, 0.0000f, 0.0000f });
	}

	Storage fires_storage;

	local_bounds_t get_refr_bounds(RE::TESObjectREFR* a, float scale)
	{
		return fires_storage.get_refr_bounds(a, scale);
	}

	void rotateSkyrim_M_Player(RE::NiMatrix3& ans, RE::TESObjectREFR* p)
	{
		auto rot = RE::NiPoint3(0,0, p->GetAngleZ());
		RE::NiMatrix3 m;
		m.EulerAnglesToAxesZXY(rot);
		ans = m * ans;
	}

	local_bounds_t get_refr_bounds(RE::TESObjectREFR* a)
	{
		auto ans = get_refr_bounds(a, a->GetScale());

		if (auto bone = a->Get3D()) {
			ans.Normals = bone->world.rotate * ans.Normals;
			ans.Base = bone->world.rotate * ans.Base;
		}

		return ans;
	}

	local_bounds_t get_npc_bounds(RE::TESObjectREFR* p)
	{
		constexpr float DOWN = 5.0f;
		const RE::NiMatrix3 PLAYER_BOUNDS{ { 25.0f, 0, 0 }, { 0, 17.0f, 0 }, { 0, 0, 60.0f + DOWN } };
		auto scale = p->GetScale();
		local_bounds_t ans;
		ans.Base = RE::NiPoint3{ 0, 0, PLAYER_BOUNDS.entry[2][2] - DOWN } * scale;
		ans.Normals = PLAYER_BOUNDS * scale;
		rotateSkyrim_M_Player(ans.Normals, p);
		return ans;
	}

	RE::NiPoint3 get_bounds_center(RE::TESObjectREFR* refr) {
		auto bounds = get_refr_bounds(refr);
		bounds.Base += refr->GetPosition();
		return bounds.Base;
	}

	void init_fires()
	{
		fires_storage.init();
	}

	bool is_fire(RE::TESObjectREFR& refr)
	{
		return fires_storage.is_fire(refr);
	}

	bool is_steam_refr(RE::FormID id)
	{
		return fires_storage.is_steam_refr(id);
	}

	bool is_magic_refr(RE::FormID id)
	{
		return fires_storage.is_magic_refr(id);
	}
}
