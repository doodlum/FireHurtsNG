#pragma once

namespace FiresStorage
{

	struct STAT_info
	{
		RE::NiPoint3 A, B;
	};

	class Storage
	{
		std::unordered_map<uint32_t, STAT_info> data;
		std::set<uint32_t> data_magic, data_steams;

		void register_data_entry(uint32_t id, float scale, RE::NiPoint3 a_center, RE::NiPoint3 coll_center, RE::NiPoint3 coll_bounds)
		{
			coll_center = a_center + (coll_center - a_center) / scale;
			coll_bounds = coll_bounds / scale;
			RE::NiPoint3 boundsdiv2 = coll_bounds / 2.0f;
			data[id] = { coll_center - boundsdiv2 - a_center, coll_center + boundsdiv2 - a_center };
		}

		void register_data_entry_steam(uint32_t id, float scale, RE::NiPoint3 a_center, RE::NiPoint3 coll_center, RE::NiPoint3 coll_bounds)
		{
			register_data_entry(id, scale, a_center, coll_center, coll_bounds);
			data_steams.insert(id);
		}

		void register_data_entry_magic(uint32_t id, float scale, RE::NiPoint3 a_center, RE::NiPoint3 coll_center, RE::NiPoint3 coll_bounds)
		{
			register_data_entry(id, scale, a_center, coll_center, coll_bounds);
			data_magic.insert(id);
		}

		void add_support();

		void add_support_vanilla();

	public:
		std::pair<RE::NiPoint3, RE::NiPoint3> get_refr_bounds(RE::TESObjectREFR* a, float scale)
		{
			auto id = a->GetBaseObject()->formID;

			RE::NiPoint3 A, B;
			auto i = data.find(id);
			if (i == data.end()) {
				// default 100x100x40, h=10
				const float W_DEFAULTD_IV2 = 50.0f, H_DEFAULT_DIV2 = 40.0f, z0_DEFAULT = 10.0f;
				A = { -W_DEFAULTD_IV2, -W_DEFAULTD_IV2, -H_DEFAULT_DIV2 + z0_DEFAULT };
				B = { W_DEFAULTD_IV2, W_DEFAULTD_IV2, H_DEFAULT_DIV2 + z0_DEFAULT };
			} else {
				A = i->second.A * scale;
				B = i->second.B * scale;
			}
			return { A, B };
		}

		void init()
		{
			add_support_vanilla();
			add_support();
		}

		bool is_fire(RE::TESObjectREFR& refr)
		{
			auto base = refr.GetBaseObject();
			return base && data.find(base->formID) != data.end();
		}

		bool is_steam_refr(RE::FormID id)
		{
			return data_steams.find(id) != data_steams.end();
		}

		bool is_magic_refr(RE::FormID id)
		{
			return data_magic.find(id) != data_magic.end();
		}
	};

	std::pair<RE::NiPoint3, RE::NiPoint3> get_refr_bounds(RE::TESObjectREFR* a, float scale);

	void init_fires();
	bool is_fire(RE::TESObjectREFR& refr);
	bool is_steam_refr(RE::FormID id);
	bool is_magic_refr(RE::FormID id);
}
