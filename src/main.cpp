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
#include <array>

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

auto get_bound_vertexes(const global_bounds_t& bounds)
{
	RE::NiPoint3 n1{ bounds.Normals.entry[0][0], bounds.Normals.entry[1][0], bounds.Normals.entry[2][0] };
	RE::NiPoint3 n2{ bounds.Normals.entry[0][1], bounds.Normals.entry[1][1], bounds.Normals.entry[2][1] };
	RE::NiPoint3 n3{ bounds.Normals.entry[0][2], bounds.Normals.entry[1][2], bounds.Normals.entry[2][2] };
	return std::array<RE::NiPoint3, 8>{
		bounds.Base + n1 + n2 + n3,
		bounds.Base - n1 + n2 + n3,
		bounds.Base - n1 - n2 + n3,
		bounds.Base + n1 - n2 + n3,
		bounds.Base + n1 + n2 - n3,
		bounds.Base - n1 + n2 - n3,
		bounds.Base - n1 - n2 - n3,
		bounds.Base + n1 - n2 - n3
	};
}

template <glm::vec4 Color = Colors::RED>
void draw_bounds(const global_bounds_t& bounds, float update_period)
{
	auto verts = get_bound_vertexes(bounds);
	const int dur = static_cast<int>(update_period * 1000);
	const float wide = 5.0f;

	auto draw_ = [=](int u, int v) {
		draw_line<Color>(verts[u], verts[v], wide, dur);
	};

	draw_(0, 1);
	draw_(0, 3);
	draw_(2, 1);
	draw_(2, 3);

	draw_(4, 5);
	draw_(4, 7);
	draw_(6, 5);
	draw_(6, 7);

	draw_(0, 4);
	draw_(1, 5);
	draw_(2, 6);
	draw_(3, 7);
}

template <glm::vec4 Color = Colors::RED>
void draw([[maybe_unused]] RE::Actor* a, [[maybe_unused]] RE::TESObjectREFR* refr, [[maybe_unused]] float update_period)
{
#ifdef DEBUG
	//draw_line<Color>(FiresStorage::get_bounds_center(refr), a->GetPosition(), 5.0f, static_cast<int>(update_period) * 1000);
	draw_line<Color>(refr->GetPosition(), a->GetPosition(), 5.0f, static_cast<int>(update_period) * 1000);
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
				float curdist = a->GetPosition().GetSquaredDistance(FiresStorage::get_bounds_center(&_refr));
				if (curdist < mindist2) {
					mindist2 = curdist;
					refr = &_refr;
				}
			}
			return true;
		});

#ifdef DEBUG
		draw_bounds(get_npc_bounds(a), 0);
#endif  // DEBUG

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

static void SKSEMessageHandler(SKSE::MessagingInterface::Message* message)
{
	switch (message->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		FiresStorage::init_fires();
		Settings::load();
		PlayerCharacterHook::Hook();

		break;
	}
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	auto g_messaging = reinterpret_cast<SKSE::MessagingInterface*>(a_skse->QueryInterface(SKSE::LoadInterface::kMessaging));
	if (!g_messaging) {
		logger::critical("Failed to load messaging interface! This error is fatal, plugin will not load.");
		return false;
	}

	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(1 << 10);

	g_messaging->RegisterListener("SKSE", SKSEMessageHandler);

	return true;
}
