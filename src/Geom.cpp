#include "Geom.h"
#include "FireStorage.h"

constexpr REL::ID get_scale_ID(19238);
REL::Relocation<float (RE::TESObjectREFR* a)> get_scale(get_scale_ID);

void rotate(NiPoint3& A, const NiPoint3 O, const NiPoint3 angle)
{
	float cosa = cos(angle.z);
	float sina = sin(angle.z);

	float cosb = cos(angle.x);
	float sinb = sin(angle.x);

	float cosc = cos(angle.y);
	float sinc = sin(angle.y);

	float Axx = cosa * cosb;
	float Axy = cosa * sinb * sinc - sina * cosc;
	float Axz = cosa * sinb * cosc + sina * sinc;

	float Ayx = sina * cosb;
	float Ayy = sina * sinb * sinc + cosa * cosc;
	float Ayz = sina * sinb * cosc - cosa * sinc;

	float Azx = -sinb;
	float Azy = cosb * sinc;
	float Azz = cosb * cosc;

	A -= O;

	float px = A.x;
	float py = A.y;
	float pz = A.z;

	A.x = Axx * px + Axy * py + Axz * pz;
	A.y = Ayx * px + Ayy * py + Ayz * pz;
	A.z = Azx * px + Azy * py + Azz * pz;

	A += O;
}

void rotate(Rect& r, const NiPoint3 O, const NiPoint3 angle)
{
	r.alpha += angle;
	rotate(r.M, O, angle);
}

bool edge_separate(const NiPoint3 O, const NiPoint3 M, const std::vector<NiPoint3>& verts)
{
	bool ans = true;
	for (auto& i : verts) {
		ans = ans && ((i - O) * (M - O) < 0);
	}
	return ans;
}

std::vector<RE::NiPoint3> get_bound_vertexes(const Rect& a) {
	auto a_verts = a.get_vertexes();
	for (auto& i : a_verts) {
		rotate(i, a.M, a.alpha);
	}
	return a_verts;
}

bool cannot_separate_(Rect p, Rect a)
{
	rotate(a, p.M, -p.alpha);
	rotate(p, p.M, -p.alpha);
	auto a_verts = get_bound_vertexes(a);

	auto p_edges = p.get_edges();
	for (auto& i : p_edges) {
		if (edge_separate(i, p.M, a_verts))
			return false;
	}
	return true;
}

bool cannot_separate(const Rect& A, const Rect& B)
{
	return cannot_separate_(A, B) && cannot_separate_(B, A);
}

Rect get_refr_bounds(RE::TESObjectREFR* a) {
	auto bound_info = FiresStorage::get_refr_bounds(a, get_scale(a));
	const NiPoint3& A = bound_info.first;
	const NiPoint3& B = bound_info.second;
	return { a->GetPosition() + (A + B) * 0.5f, (B - A) * 0.5f, a->GetAngle() };  // TODO: mb -z?
}

Rect get_npc_bounds(RE::TESObjectREFR* p)
{
	constexpr float DOWN_DIV2 = 5.0f;
	constexpr NiPoint3 PLAYER_BOUNDS = { 25.0f, 17.0f, 75.0f + DOWN_DIV2 };
	auto scale = get_scale(p);
	return {
		p->GetPosition() + NiPoint3{ 0, 0, PLAYER_BOUNDS.z - DOWN_DIV2 },
		PLAYER_BOUNDS * scale,
		{ 0, 0, -p->GetAngleZ() }
	};
}

bool is_collides(RE::TESObjectREFR* a, RE::TESObjectREFR* refr)
{
	return cannot_separate(get_npc_bounds(a), get_refr_bounds(refr));
}
