#pragma once

#include "sw/ShaderType.h"

#include <cstdint>
#include <string>
#include <vector>

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
	VT_T2D,		// sample 2d
	VT_TCUBE,	// sample cube
};

enum VarPrecision
{
	VT_PRECISION_ANY = 0,
	VT_BOOL,
	VT_INT,
	VT_FLT,
};

enum VarQualifiers
{
	VT_TEMP = 0,
	VT_CONST,
	VT_UNIF,
	VT_SHADER_IN,
	VT_SHADER_OUT,
};

enum VarRegion
{
	VT_REGION_ANY = 0,
	VT_NODE_IN,
	VT_NODE_OUT,
	VT_NODE_MID,
};

union VariableType
{
	bool operator == (const VariableType& type) const {
		return u32 == type.u32;
	}
	bool operator != (const VariableType& type) const {
		return u32 != type.u32;
	}

	std::string ToGLSL(ShaderType st = ST_ANY) const;

	void TypePromoteFrom(const VariableType& type);

	struct {
		unsigned dim       : 3;
		unsigned len       : 1;
		unsigned basis     : 3;
		unsigned interp    : 4;
		unsigned precision : 2;
		unsigned qualifier : 3;
		unsigned region    : 2;

		unsigned padding   : 14;
	};

	uint32_t u32 = 0;
};

// single

static const uint32_t t_tex2d   = VariableType{ 0, 0, 0, VT_T2D }.u32;

static const uint32_t t_const   = VariableType{ 0, 0, 0, 0, 0, VT_CONST }.u32;
static const uint32_t t_unif    = VariableType{ 0, 0, 0, 0, 0, VT_UNIF }.u32;
static const uint32_t t_s_in    = VariableType{ 0, 0, 0, 0, 0, VT_SHADER_IN }.u32;
static const uint32_t t_s_out   = VariableType{ 0, 0, 0, 0, 0, VT_SHADER_OUT }.u32;

static const uint32_t t_n_in    = VariableType{ 0, 0, 0, 0, 0, 0, VT_NODE_IN }.u32;
static const uint32_t t_n_out   = VariableType{ 0, 0, 0, 0, 0, 0, VT_NODE_OUT }.u32;
static const uint32_t t_n_mid   = VariableType{ 0, 0, 0, 0, 0, 0, VT_NODE_MID }.u32;

// complex

static const uint32_t t_int1    = VariableType{ VT_1, 0, 0, 0, VT_INT }.u32;
static const uint32_t t_flt2    = VariableType{ VT_2, 0, 0, 0, VT_FLT }.u32;
static const uint32_t t_flt4    = VariableType{ VT_4, 0, 0, 0, VT_FLT }.u32;

static const uint32_t t_uv      = VariableType{ VT_2, 0, 0, VT_TEX, VT_FLT }.u32;
static const uint32_t t_pos4    = VariableType{ VT_4, 0, 0, VT_PNT, VT_FLT }.u32;
static const uint32_t t_col3    = VariableType{ VT_3, 0, 0, VT_RGB, VT_FLT }.u32;
static const uint32_t t_col4    = VariableType{ VT_4, 0, 0, VT_RGB, VT_FLT }.u32;
static const uint32_t t_mat4    = VariableType{ VT_4, 0, 0, VT_MAT, VT_FLT }.u32;

}