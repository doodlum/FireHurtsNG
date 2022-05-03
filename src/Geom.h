#pragma once

using RE::NiPoint3;
struct Rect
{
	static void mull(NiPoint3& a, const NiPoint3& b)
	{
		a.x *= b.x, a.y *= b.y, a.z *= b.z;
	}
	NiPoint3 M, B, alpha;  // halfed bounds
	std::vector<NiPoint3> get_vertexes() const
	{
		std::vector<NiPoint3> ans = { { 1, 1, 1 }, { 1, 1, -1 }, { 1, -1, 1 },
			{ 1, -1, -1 }, { -1, 1, 1 }, { -1, 1, -1 }, { -1, -1, 1 }, { -1, -1, -1 } };
		for (auto& i : ans) {
			mull(i, B);
			i += M;
		}
		return ans;
	}
	std::vector<NiPoint3> get_edges()
	{
		std::vector<NiPoint3> ans = { { 0, 0, 1 }, { 0, 0, -1 }, { 0, 1, 0 },
			{ 0, -1, 0 }, { 1, 0, 0 }, { -1, 0, 0 } };
		for (auto& i : ans) {
			mull(i, B);
			i += M;
		}
		return ans;
	}
};

Rect get_refr_bounds(RE::TESObjectREFR* a);
Rect get_npc_bounds(RE::TESObjectREFR* p);
std::vector<RE::NiPoint3> get_bound_vertexes(const Rect& a);
bool is_collides(RE::TESObjectREFR* a, RE::TESObjectREFR* refr);
