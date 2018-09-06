#pragma once

#include <cstdint>
#include <string>

namespace sw
{

enum VarDimenison
{
	VT_0 = 0,
	VT_1,
	VT_2,
	VT_3,
	VT_4
};

enum VarLength
{
	VT_LEN_ANY = 0,
	VT_UNIT,
};

enum VarBasis
{
	VT_BASIS_ANY = 0,
	VT_TANGENT,
	VT_OBJECT,
	VT_WORLD,
	VT_SCREEN
};

enum VarInterpretation
{
	VT_INTERP_ANY = 0,
	VT_RGB,     // color
	VT_TEX,     // texture coordinate
	VT_NOR,     // surface normal
	VT_VEC,     // direction
	VT_PNT,     // point
	VT_MAT,		// matrix
	VT_SAMPLER_2D,
	VT_SAMPLER_CUBE,
};

enum VarPrecision
{
	VT_FLOAT = 0,
	VT_INT,
	VT_BOOL
};

enum VarQualifiers
{
	VT_TEMP = 0,
	VT_CONST,
	VT_ATTRIBUTE,
	VT_UNIFORM,
	VT_VARYING
};

enum VarIO
{
	VT_IO_ANY = 0,
	VT_IN,
	VT_OUT,
};

union VariableType
{
	bool operator == (const VariableType& type) const {
		return u32 == type.u32;
	}

	std::string ToGLSL() const;

	struct {
		unsigned dim       : 3;
		unsigned len       : 1;
		unsigned basis     : 3;
		unsigned interp    : 4;
		unsigned precision : 2;
		unsigned qualifier : 3;
		unsigned io        : 2;

		unsigned padding   : 14;
	};

	uint32_t u32 = 0;
};

static const uint32_t t_vec4 = VariableType{ VT_4 }.u32;
static const uint32_t t_attr = VariableType{ 0, 0, 0, 0, 0, VT_ATTRIBUTE }.u32;
static const uint32_t t_unif = VariableType{ 0, 0, 0, 0, 0, VT_UNIFORM }.u32;
static const uint32_t t_vary = VariableType{ 0, 0, 0, 0, 0, VT_VARYING }.u32;
static const uint32_t t_in   = VariableType{ 0, 0, 0, 0, 0, 0, VT_IN, 0 }.u32;
static const uint32_t t_out  = VariableType{ 0, 0, 0, 0, 0, 0, VT_OUT }.u32;

}