#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class ColorspaceConversion : public sw::Node
{
public:
	enum ColorType
	{
		COL_RGB,
		COL_LINEAR,
		COL_HSV,
	};

public:
	ColorspaceConversion(ColorType from, ColorType to)
		: Node("ColorspaceConversion")
		, m_from(from)
		, m_to(to)
	{
		std::vector<Variable> middle;
		if (m_from == COL_RGB && m_to == COL_LINEAR)
		{
			middle.push_back({ t_flt3, "linearRGBLo" });
			middle.push_back({ t_flt3, "linearRGBHi" });
		}
		else if (m_from == COL_RGB && m_to == COL_HSV)
		{
			middle.push_back({ t_flt4, "K" });
			middle.push_back({ t_flt4, "P" });
			middle.push_back({ t_flt4, "Q" });
			middle.push_back({ t_flt1, "D" });
			middle.push_back({ t_flt1, "E" });
		}
		else if (m_from == COL_LINEAR && m_to == COL_RGB)
		{
			middle.push_back({ t_flt3, "sRGBLo" });
			middle.push_back({ t_flt3, "sRGBHi" });
		}
		else if (m_from == COL_LINEAR && m_to == COL_HSV)
		{
			middle.push_back({ t_flt3, "sRGBLo" });
			middle.push_back({ t_flt3, "sRGBHi" });
			middle.push_back({ t_flt3, "Linear" });
			middle.push_back({ t_flt4, "K" });
			middle.push_back({ t_flt4, "P" });
			middle.push_back({ t_flt4, "Q" });
			middle.push_back({ t_flt1, "D" });
			middle.push_back({ t_flt1, "E" });
		}
		else if (m_from == COL_HSV && m_to == COL_RGB)
		{
			middle.push_back({ t_flt4, "K" });
			middle.push_back({ t_flt3, "P" });
		}
		else if (m_from == COL_HSV && m_to == COL_LINEAR)
		{
			middle.push_back({ t_flt4, "K" });
			middle.push_back({ t_flt3, "P" });
			middle.push_back({ t_flt3, "RGB" });
			middle.push_back({ t_flt3, "linearRGBLo" });
			middle.push_back({ t_flt3, "linearRGBHi" });
		}

		InitVariables({
			{ t_col3, "_in" },
		}, {
			{ t_col3, "_out" },
		}, middle);
	}

protected:
	virtual std::string GetBody() const override
	{
		if (m_from == m_to)
		{
			return "#_out# = #_in#;";
		}
		else if (m_from == COL_RGB && m_to == COL_LINEAR)
		{
			return R"(
#linearRGBLo# = #_in# / 12.92;
#linearRGBHi# = pow(max(abs((#_in# + 0.055) / 1.055), 1.192092896e-07), vec3(2.4, 2.4, 2.4));
#_out#.x = #_in#.x <= 0.04045 ? #linearRGBLo#.x : #linearRGBHi#.x;
#_out#.y = #_in#.y <= 0.04045 ? #linearRGBLo#.y : #linearRGBHi#.y;
#_out#.z = #_in#.z <= 0.04045 ? #linearRGBLo#.z : #linearRGBHi#.z;
)" + 1;
		}
		else if (m_from == COL_RGB && m_to == COL_HSV)
		{
			return R"(
#K# = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
#P# = mix(vec4(#_in#.bg, #K#.wz), vec4(#_in#.gb, #K#.xy), step(#_in#.b, #_in#.g));
#Q# = mix(vec4(#P#.xyw, #_in#.r), vec4(#_in#.r, #P#.yzx), step(#P#.x, #_in#.r));
#D# = #Q#.x - min(#Q#.w, #Q#.y);
#E# = 1e-10;
#_out# = vec3(abs(#Q#.z + (#Q#.w - #Q#.y)/(6.0 * #D# + #E#)), #D# / (#Q#.x + #E#), #Q#.x);
)" + 1;
		}
		else if (m_from == COL_LINEAR && m_to == COL_RGB)
		{
			return R"(
#sRGBLo# = #_in# * 12.92;
#sRGBHi# = (pow(max(abs(#_in#), 1.192092896e-07), vec3(1.0 / 2.4, 1.0 / 2.4, 1.0 / 2.4)) * 1.055) - 0.055;
#_out#.x = #_in#.x <= 0.0031308 ? #sRGBLo#.x : #sRGBHi#.x;
#_out#.y = #_in#.y <= 0.0031308 ? #sRGBLo#.y : #sRGBHi#.y;
#_out#.z = #_in#.z <= 0.0031308 ? #sRGBLo#.z : #sRGBHi#.z;
)" + 1;
		}
		else if (m_from == COL_LINEAR && m_to == COL_HSV)
		{
			return R"(
#sRGBLo# = #_in# * 12.92;
#sRGBHi# = (pow(max(abs(#_in#), 1.192092896e-07), vec3(1.0 / 2.4, 1.0 / 2.4, 1.0 / 2.4)) * 1.055) - 0.055;
#Linear#.x = #_in#.x <= 0.0031308 ? #sRGBLo#.x : #sRGBHi#.x;
#Linear#.y = #_in#.y <= 0.0031308 ? #sRGBLo#.y : #sRGBHi#.y;
#Linear#.z = #_in#.z <= 0.0031308 ? #sRGBLo#.z : #sRGBHi#.z;
#K# = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
#P# = mix(vec4(#Linear#.bg, #K#.wz), vec4(#Linear#.gb, #K#.xy), step(#Linear#.b, #Linear#.g));
#Q# = mix(vec4(#P#.xyw, #Linear#.r), vec4(#Linear#.r, #P#.yzx), step(#P#.x, #Linear#.r));
#D# = #Q#.x - min(#Q#.w, #Q#.y);
#E# = 1e-10;
#_out# = vec3(abs(#Q#.z + (#Q#.w - #Q#.y)/(6.0 * #D# + #E#)), #D# / (#Q#.x + #E#), #Q#.x);
)" + 1;
		}
		else if (m_from == COL_HSV && m_to == COL_RGB)
		{
			return R"(
#K# = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
#P# = abs(fract(#_in#.xxx + #K#.xyz) * 6.0 - #K#.www);
#_out# = #_in#.z * mix(#K#.xxx, clamp(#P# - #K#.xxx, 0.0, 1.0), #_in#.y);
)" + 1;
		}
		else if (m_from == COL_HSV && m_to == COL_LINEAR)
		{
			return R"(
#K# = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
#P# = abs(fract(#_in#.xxx + #K#.xyz) * 6.0 - #K#.www);
#RGB# = #_in#.z * mix(#K#.xxx, clamp(#P# - #K#.xxx, 0.0, 1.0), #_in#.y);
#linearRGBLo# = #RGB# / 12.92;
#linearRGBHi# = pow(max(abs((#RGB# + 0.055) / 1.055), 1.192092896e-07), vec3(2.4, 2.4, 2.4));
#_out#.x = #RGB#.x <= 0.04045 ? #linearRGBLo#.x : #linearRGBHi#.x;
#_out#.y = #RGB#.y <= 0.04045 ? #linearRGBLo#.y : #linearRGBHi#.y;
#_out#.z = #RGB#.z <= 0.04045 ? #linearRGBLo#.z : #linearRGBHi#.z;
)" + 1;
		}
		else
		{
			return "";
		}
	}

private:
	ColorType m_from, m_to;

}; // ColorspaceConversion

}
}