#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Remap : public sw::Node
{
public:
	Remap()
		: Node("Remap")
	{
		InitVariables({
			{ 0, "_in" },
			{ t_flt2, "from" },
			{ t_flt2, "to" },
		}, {
			{ 0, "_out" },
		}, {
			{ t_flt1, "t" },
		});

		m_dim_group = { 0, MAX_IMPORTS_COUNT };
	}

	enum InputID
	{
		ID_IN = 0,
		ID_FROM,
		ID_TO,
	};

protected:
	virtual std::string GetBody() const override
	{
		// (_in - from.x) / (from.y - from.x) == (x - to.x) / (to.y - to.x);
		// x = (_in - from.x) / (from.y - from.x) * (to.y - to.x) + to.x;
		auto& val = GetImports()[ID_IN];
		switch (val.var.GetType().dim)
		{
		case VT_1:
			return R"(
#t# = (#to#.y - #to#.x) / (#from#.y - #from#.x);
#_out# = #to#.x + (#_in# - #from#.x) * #t#;
)" + 1;
			break;
		case VT_2:
			return R"(
#t# = (#to#.y - #to#.x) / (#from#.y - #from#.x);
#_out#.x = #to#.x + (#_in#.x - #from#.x) * #t#;
#_out#.y = #to#.x + (#_in#.y - #from#.x) * #t#;
)" + 1;
			break;
		case VT_3:
			return R"(
#t# = (#to#.y - #to#.x) / (#from#.y - #from#.x);
#_out#.x = #to#.x + (#_in#.x - #from#.x) * #t#;
#_out#.y = #to#.x + (#_in#.y - #from#.x) * #t#;
#_out#.z = #to#.x + (#_in#.z - #from#.x) * #t#;
)" + 1;
			break;
		case VT_4:
			return R"(
#t# = (#to#.y - #to#.x) / (#from#.y - #from#.x);
#_out#.x = #to#.x + (#_in#.x - #from#.x) * #t#;
#_out#.y = #to#.x + (#_in#.y - #from#.x) * #t#;
#_out#.z = #to#.x + (#_in#.z - #from#.x) * #t#;
#_out#.w = #to#.x + (#_in#.w - #from#.x) * #t#;
)" + 1;
			break;
		default:
			return "";
		}
	}

}; // Remap

}
}