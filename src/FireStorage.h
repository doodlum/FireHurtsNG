#pragma once
#include "json/json.h"

namespace FiresStorage
{
	struct local_bounds_t
	{
		RE::NiMatrix3 Normals;  // cols
		RE::NiPoint3 Base;      // offset from fig center
	};

	class Storage
	{
		std::unordered_map<uint32_t, local_bounds_t> data;
		std::set<uint32_t> data_magic, data_steams;

		static constexpr inline RE::NiPoint3 DEFAULT_BOUND_ANGLE = { 0.0f, 0.0f, 0.0f };

		static int get_mod_index(const std::string_view& name)
		{
			auto esp = RE::TESDataHandler::GetSingleton()->LookupModByName(name);
			if (!esp)
				return -1;

			return !esp->IsLight() ? esp->compileIndex << 24 : (0xFE000 | esp->smallFileCompileIndex) << 12;
		}

		void register_data_entry(uint32_t id, const RE::NiPoint3& offset, RE::NiPoint3 bounds, RE::NiPoint3 bound_rot)
		{
			bound_rot *= 3.1415926f / 180.0f;
			bounds *= 0.5f;

			RE::NiMatrix3 R, A{ { bounds.x, 0, 0 }, { 0, bounds.y, 0 }, { 0, 0, bounds.z } };
			R.EulerAnglesToAxesZXY(bound_rot);
			R = R * A;

			data[id] = { R, offset };
		}

		void set_magic_forms(int hex, const std::vector<int>& forms)
		{
			for (auto id : forms) {
				data_magic.insert(hex | id);
			}
		}

		void set_steam_forms(int hex, const std::vector<int>& forms)
		{
			for (auto id : forms) {
				data_steams.insert(hex | id);
			}
		}

		// [0.0, 0.0, 0.0]
		static RE::NiPoint3 parse_point(const Json::Value& p)
		{
			return { p[0].asFloat(), p[1].asFloat(), p[2].asFloat() };
		}

		void add_support_json(const std::string& path) {
			Json::Value json_root;
			std::ifstream ifs;
			ifs.open(path);
			ifs >> json_root;
			ifs.close();
			
			for (auto& mod_name : json_root.getMemberNames()) {
				const Json::Value mod_data = json_root[mod_name];
				auto hex = get_mod_index(mod_name);
				if (hex == -1)
					continue;

				for (int i = 0; i < (int)mod_data.size(); i++) {
					const auto item = mod_data[i];
					auto id = item["id"].asInt();

					register_data_entry(hex | id, parse_point(item["offset"]), parse_point(item["bounds"]), parse_point(item["angle"]));
					
					auto type = item["type"].asInt();
					if (type == 1) {
						data_magic.insert(hex | id);
					} else if (type == 2) {
						data_steams.insert(hex | id);
					}
				}
			}
		}

		void add_support_json()
		{
			auto jsons_path = "Data/SKSE/Plugins/FireHurtsRE/"s;

			namespace fs = std::filesystem;

			for (const auto& entry : fs::directory_iterator(jsons_path)) {
				if (entry.path().extension() == ".json")
					add_support_json(entry.path().string());
			}
		}

		void add_support_vanilla();
		void add_support_campfire();
		void add_support_dragonborn();

	public:
		local_bounds_t get_refr_bounds(RE::TESObjectREFR* a, float scale)
		{
			auto id = a->GetBaseObject() ? a->GetBaseObject()->formID : -1;

			RE::NiPoint3 Mid, ToMax, Rot;
			auto i = data.find(id);
			if (i == data.end()) {
				// default 100x100x40, h=10
				const float W_DIV2 = 50.0f, H_DIV2 = 40.0f, z0 = 10.0f;
				const RE::NiMatrix3 M = RE::NiMatrix3{ { W_DIV2, 0, 0 }, { 0, W_DIV2, 0 }, { 0, 0, H_DIV2 } };
				return { M * scale, RE::NiPoint3{ 0, 0, -z0 } };
			} else {
				auto ans = (*i).second;
				ans.Normals = ans.Normals * scale;
				ans.Base = ans.Base * scale;
				return ans;
			}
		}

		void init()
		{
			data = std::unordered_map<uint32_t, local_bounds_t>();
			data_magic = std::set<uint32_t>();
			data_steams = std::set<uint32_t>();

			add_support_vanilla();
			add_support_campfire();
			add_support_dragonborn();

			add_support_json();
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

	local_bounds_t get_refr_bounds(RE::TESObjectREFR* a);
	local_bounds_t get_npc_bounds(RE::TESObjectREFR* p);
	RE::NiPoint3 get_bounds_center(RE::TESObjectREFR* refr);

	void init_fires();
	bool is_fire(RE::TESObjectREFR& refr);
	bool is_steam_refr(RE::FormID id);
	bool is_magic_refr(RE::FormID id);
}
using local_bounds_t = FiresStorage::local_bounds_t;
