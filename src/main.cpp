extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
#ifndef DEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= Version::PROJECT;
	*path += ".log"sv;
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef DEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::info);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);

	logger::info(FMT_STRING("{} v{}"), Version::PROJECT, Version::NAME);

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = Version::PROJECT.data();
	a_info->version = Version::MAJOR;

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_1_5_39) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
		return false;
	}

	return true;
}

#include <UselessFenixUtils.h>
#include "Geom.h"
#include "FireStorage.h"
#include "Settings.h"

class DataStorage
{
	static constexpr auto ModName = "FiresHurtRE.esp"sv;

public:
	static auto get_f314FH_kywd_All()
	{
		static auto ans = RE::TESDataHandler::GetSingleton()->LookupForm<RE::BGSKeyword>(0x803, ModName);
		return ans;
	}
	static auto get_f314FH_spel_Fire()
	{
		static auto ans = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(0x808, ModName);
		return ans;
	}
	static auto get_f314FH_spel_Magic()
	{
		static auto ans = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(0x809, ModName);
		return ans;
	}
	static auto get_f314FH_spel_Steam()
	{
		static auto ans = RE::TESDataHandler::GetSingleton()->LookupForm<RE::SpellItem>(0x80a, ModName);
		return ans;
	}
};

bool is_cooking(RE::Actor* a)
{
	return a->GetSitSleepState() != RE::SIT_SLEEP_STATE::kNormal ||
	       a->IsPlayerRef() && RE::UI::GetSingleton()->IsMenuOpen("Crafting Menu");
}

bool is_onfire(RE::Actor* a)
{
	return FenixUtils::TESObjectREFR__HasEffectKeyword(a, DataStorage::get_f314FH_kywd_All());
}

template <glm::vec4 Color = Colors::RED>
void draw_bounds(const Rect& bounds, float update_period)
{
	auto verts = get_bound_vertexes(bounds);
	draw_line<Color>(verts[0], verts[1], 5.0f, static_cast<int>(update_period) * 1000);
	draw_line<Color>(verts[0], verts[2], 5.0f, static_cast<int>(update_period) * 1000);
	draw_line<Color>(verts[3], verts[1], 5.0f, static_cast<int>(update_period) * 1000);
	draw_line<Color>(verts[3], verts[2], 5.0f, static_cast<int>(update_period) * 1000);

	draw_line<Color>(verts[4], verts[5], 5.0f, static_cast<int>(update_period) * 1000);
	draw_line<Color>(verts[4], verts[6], 5.0f, static_cast<int>(update_period) * 1000);
	draw_line<Color>(verts[7], verts[5], 5.0f, static_cast<int>(update_period) * 1000);
	draw_line<Color>(verts[7], verts[6], 5.0f, static_cast<int>(update_period) * 1000);

	draw_line<Color>(verts[6], verts[2], 5.0f, static_cast<int>(update_period) * 1000);
	draw_line<Color>(verts[4], verts[0], 5.0f, static_cast<int>(update_period) * 1000);
	draw_line<Color>(verts[5], verts[1], 5.0f, static_cast<int>(update_period) * 1000);
	draw_line<Color>(verts[7], verts[3], 5.0f, static_cast<int>(update_period) * 1000);
}

template <glm::vec4 Color = Colors::RED>
void draw([[maybe_unused]] RE::Actor* a, [[maybe_unused]] RE::TESObjectREFR* refr, [[maybe_unused]] float update_period)
{
#ifdef DEBUG
	draw_line<Color>(refr->GetPosition(), a->GetPosition(), 5.0f, static_cast<int>(update_period) * 1000);
	draw_bounds<Color>(get_npc_bounds(a), update_period);
	draw_bounds<Color>(get_refr_bounds(refr), update_period);
#endif  // DEBUG
}

class Timeouts
{
public:
	static float cooking() {
		return 10.0f;
	}
	static float noFireNear()
	{
		return 5.0f;
	}
	static float inFire()
	{
		return 1.0f;
	}
	static float fireNear(float dist2) {
		return sqrtf(dist2) / 1000.0f;
	};
};

enum class FireTypes
{
	None,
	Fire,
	Steam,
	Magic
};

FireTypes get_fire_type(RE::FormID id)
{
	if (FiresStorage::is_steam_refr(id))
		return FireTypes::Steam;
	if (FiresStorage::is_magic_refr(id))
		return FireTypes::Magic;
	return FireTypes::Fire;
}

RE::SpellItem* get_fireSpell(FireTypes type)
{
	switch (type) {
	case FireTypes::Steam:
		return DataStorage::get_f314FH_spel_Steam();
	case FireTypes::Magic:
		return DataStorage::get_f314FH_spel_Magic();
	default:
		return DataStorage::get_f314FH_spel_Fire();
	}
}

class TickerPlayer
{
	float updateafter = 0.0f;
	float infire_time = 0.0f;

	enum class FireStates
	{
		Cooking, NoFireNear, InFire, NearFire
	};

	struct State
	{
	public:
		FireStates state: 3;
		FireTypes type: 3;
	};

	auto update(RE::Actor* a)
	{
		std::pair<State, float> ans;
		ans.second = -1.0f;
		ans.first.type = FireTypes::None;

		if (is_cooking(a)) {
			ans.first.state = FireStates::Cooking;
			return ans;
		}

		float mindist2 = 1.0E15f;
		RE::TESObjectREFR* refr = 0;
		RE::TES::GetSingleton()->ForEachReference([=, &mindist2, &refr](RE::TESObjectREFR& _refr) {
			if (!_refr.IsDisabled() && FiresStorage::is_fire(_refr)) {
				float curdist = a->GetPosition().GetSquaredDistance(_refr.GetPosition());
				if (curdist < mindist2) {
					mindist2 = curdist;
					refr = &_refr;
				}
			}
			return true;
		});

		if (!refr || 10000000.0f < mindist2) {
			ans.first.state = FireStates::NoFireNear;
			return ans;
		}

		if (is_collides(a, refr)) {
			ans.first.state = FireStates::InFire;
			ans.first.type = get_fire_type(refr->GetBaseObject()->GetFormID());

#ifdef DEBUG
			draw<Colors::RED>(a, refr, get_new_updateafter(ans, a));
#endif  // DEBUG

			return ans;
		} else {
			ans.first.state = FireStates::NearFire;
			ans.second = mindist2;

#ifdef DEBUG
			draw<Colors::GRN>(a, refr, get_new_updateafter(ans, a));
#endif  // DEBUG

			return ans;
		}
	}

	float get_new_updateafter(const std::pair<State, float>& state, RE::Actor* a) 
	{
		switch (state.first.state) {
		case FireStates::Cooking:
			return Timeouts::cooking();
		case FireStates::InFire:
			return Timeouts::inFire();
		case FireStates::NoFireNear:
			return Timeouts::noFireNear();
		case FireStates::NearFire:
			return is_onfire(a) ? Timeouts::inFire() : Timeouts::fireNear(state.second);
		default:
			return 0.0f;
		}
	}

public:
	void tick(float delta)
	{
		updateafter -= delta;

		if (infire_time != 0.0f)
			infire_time += delta;

		if (updateafter < 0.0f) {
			RE::Actor* a = RE::PlayerCharacter::GetSingleton();
			auto state = update(a);
			updateafter = get_new_updateafter(state, a);
			if (state.first.state == FireStates::InFire) {
				if (infire_time == 0.0f)
					infire_time = delta;
				if (infire_time > *Settings::FireDelay)
					FenixUtils::cast_spell(a, a, get_fireSpell(state.first.type));
			} else {
				infire_time = 0.0f;
			}
		}
	}
} ticker;

class PlayerCharacterHook
{
public:
	static void Hook()
	{
		_Update = REL::Relocation<uintptr_t>(REL::ID(RE::VTABLE_PlayerCharacter[0])).write_vfunc(0xad, Update);
	}

private:
	static void Update(RE::PlayerCharacter* a, float delta) {
		_Update(a, delta);
		DebugAPI_IMPL::DebugAPI::Update();
		ticker.tick(delta);
	}

	static inline REL::Relocation<decltype(Update)> _Update;
};

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	logger::info("loaded");

	FiresStorage::init_fires();

	SKSE::Init(a_skse);
	Settings::load();

	SKSE::AllocTrampoline(1 << 10);
	PlayerCharacterHook::Hook();

	return true;
}
