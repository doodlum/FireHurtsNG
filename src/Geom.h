#pragma once

#include "FireStorage.h"

struct global_bounds_t
{
	RE::NiMatrix3 Normals;  // cols
	RE::NiPoint3 Base;      // offset from fig center

	global_bounds_t(const local_bounds_t& local, const RE::NiPoint3& pos) :
		Normals(local.Normals), Base(local.Base + pos) {}

	global_bounds_t() = delete;
	global_bounds_t(const global_bounds_t&) = delete;
	global_bounds_t& operator=(const global_bounds_t&) = delete;
};

global_bounds_t get_npc_bounds(RE::TESObjectREFR* p);
global_bounds_t get_refr_bounds(RE::TESObjectREFR* p);

using RE::NiPoint3;

bool is_collides(RE::TESObjectREFR* a, RE::TESObjectREFR* refr);
