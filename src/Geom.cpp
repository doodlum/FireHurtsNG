#include "Geom.h"
#include "FireStorage.h"

global_bounds_t get_npc_bounds(RE::TESObjectREFR* p)
{
	auto local = FiresStorage::get_npc_bounds(p);
	return global_bounds_t(local, p->GetPosition());
}

global_bounds_t get_refr_bounds(RE::TESObjectREFR* p)
{
	auto local = FiresStorage::get_refr_bounds(p);
	return global_bounds_t(local, p->GetPosition());
}

bool is_collides(const global_bounds_t& ABox, const global_bounds_t& BBox)
{
	auto D = BBox.Base - ABox.Base;

#define TEST_SEP_PLANE(PARAM_RelativeVal, PARAM_R0, PARAM_R1) \
	if ((PARAM_R0) + (PARAM_R1) < fabs(PARAM_RelativeVal))    \
		return false;

	auto an0 = RE::NiPoint3(ABox.Normals.entry[0][0], ABox.Normals.entry[1][0], ABox.Normals.entry[2][0]);
	auto an1 = RE::NiPoint3(ABox.Normals.entry[0][1], ABox.Normals.entry[1][1], ABox.Normals.entry[2][1]);
	auto an2 = RE::NiPoint3(ABox.Normals.entry[0][2], ABox.Normals.entry[1][2], ABox.Normals.entry[2][2]);
	auto bn0 = RE::NiPoint3(BBox.Normals.entry[0][0], BBox.Normals.entry[1][0], BBox.Normals.entry[2][0]);
	auto bn1 = RE::NiPoint3(BBox.Normals.entry[0][1], BBox.Normals.entry[1][1], BBox.Normals.entry[2][1]);
	auto bn2 = RE::NiPoint3(BBox.Normals.entry[0][2], BBox.Normals.entry[1][2], BBox.Normals.entry[2][2]);

	float a0 = an0.Unitize();
	float a1 = an1.Unitize();
	float a2 = an2.Unitize();
	float b0 = bn0.Unitize();
	float b1 = bn1.Unitize();
	float b2 = bn2.Unitize();

	float A0D = an0.Dot(D);
	float A1D = an1.Dot(D);
	float A2D = an2.Dot(D);
	float B0D = bn0.Dot(D);
	float B1D = bn1.Dot(D);
	float B2D = bn2.Dot(D);

	float c00 = an0.Dot(bn0);  // b0_a0
	float c01 = an0.Dot(bn1);  // b1_a0
	float c02 = an0.Dot(bn2);  // b2_a0

	float c10 = an1.Dot(bn0);  // b0_a1
	float c11 = an1.Dot(bn1);  // b1_a1
	float c12 = an1.Dot(bn2);  // b2_a1

	float c20 = an2.Dot(bn0);  // b0_a2
	float c21 = an2.Dot(bn1);  // b1_a2
	float c22 = an2.Dot(bn2);  // b2_a2

	TEST_SEP_PLANE(A0D, a0, b0 * fabs(c00) + b1 * fabs(c01) + b2 * fabs(c02));
	TEST_SEP_PLANE(A1D, a1, b0 * fabs(c10) + b1 * fabs(c11) + b2 * fabs(c12));
	TEST_SEP_PLANE(A2D, a2, b0 * fabs(c20) + b1 * fabs(c21) + b2 * fabs(c22));
	
	TEST_SEP_PLANE(B0D, a0 * fabs(c00) + a1 * fabs(c10) + a2 * fabs(c20), b0);
	TEST_SEP_PLANE(B1D, a0 * fabs(c01) + a1 * fabs(c11) + a2 * fabs(c21), b1);
	TEST_SEP_PLANE(B2D, a0 * fabs(c02) + a1 * fabs(c12) + a2 * fabs(c22), b2);

#undef TEST_SEP_PLANE

#define TEST_SEP_AXIS(PARAM_DirA, PARAM_DirB, PARAM_RelativeVal, PARAM_R0, PARAM_R1) \
	if (PARAM_DirA.Cross(PARAM_DirB).SqrLength() > 0.0000001f) {                     \
		if ((PARAM_R0) + (PARAM_R1)-fabs(PARAM_RelativeVal) < 0)                     \
			return false;                                                            \
	}

	///           DirA DirB           RelVal                      R0                           R1
	TEST_SEP_AXIS(an0, bn0, c10 * A2D - c20 * A1D, a1 * fabs(c20) + a2 * fabs(c10), b1 * fabs(c02) + b2 * fabs(c01))
	TEST_SEP_AXIS(an0, bn1, c11 * A2D - c21 * A1D, a1 * fabs(c21) + a2 * fabs(c11), b0 * fabs(c02) + b2 * fabs(c00))
	TEST_SEP_AXIS(an0, bn2, c12 * A2D - c22 * A1D, a1 * fabs(c22) + a2 * fabs(c12), b0 * fabs(c01) + b1 * fabs(c00))
	TEST_SEP_AXIS(an1, bn0, c20 * A0D - c00 * A2D, a0 * fabs(c20) + a2 * fabs(c00), b1 * fabs(c12) + b2 * fabs(c11))
	TEST_SEP_AXIS(an1, bn1, c21 * A0D - c01 * A2D, a0 * fabs(c21) + a2 * fabs(c01), b0 * fabs(c12) + b2 * fabs(c10))
	TEST_SEP_AXIS(an1, bn2, c22 * A0D - c02 * A2D, a0 * fabs(c22) + a2 * fabs(c02), b0 * fabs(c11) + b1 * fabs(c10))
	TEST_SEP_AXIS(an2, bn0, c00 * A1D - c10 * A0D, a0 * fabs(c10) + a1 * fabs(c00), b1 * fabs(c22) + b2 * fabs(c21))
	TEST_SEP_AXIS(an2, bn1, c01 * A1D - c11 * A0D, a0 * fabs(c11) + a1 * fabs(c01), b0 * fabs(c22) + b2 * fabs(c20))
	TEST_SEP_AXIS(an2, bn2, c02 * A1D - c12 * A0D, a0 * fabs(c12) + a1 * fabs(c02), b0 * fabs(c21) + b1 * fabs(c20))

#undef TEST_SEP_AXIS

	return true;
}

bool is_collides(RE::TESObjectREFR* a, RE::TESObjectREFR* refr)
{
	return is_collides(get_npc_bounds(a), get_refr_bounds(refr));
}
