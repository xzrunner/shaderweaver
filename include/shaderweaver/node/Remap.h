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
            { t_flt1, "min_old" },
            { t_flt1, "max_old" },
            { t_flt1, "min_new" },
            { t_flt1, "max_new" },
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
		// (_in - min_old) / (max_old - min_old) == (x - min_new) / (max_new - min_new);
		// x = (_in - min_old) / (max_old - min_old) * (max_new - min_new) + min_new;
		auto& val = GetImports()[ID_IN];
		switch (val.var.GetType().dim)
		{
		case VT_1:
			return R"(
#t# = (#max_new# - #min_new#) / (#max_old# - #min_old#);
#_out# = #min_new# + (#_in# - #min_old#) * #t#;
)" + 1;
			break;
		case VT_2:
			return R"(
#t# = (#max_new# - #min_new#) / (#max_old# - #min_old#);
#_out#.x = #min_new# + (#_in#.x - #min_old#) * #t#;
#_out#.y = #min_new# + (#_in#.y - #min_old#) * #t#;
)" + 1;
			break;
		case VT_3:
			return R"(
#t# = (#max_new# - #min_new#) / (#max_old# - #min_old#);
#_out#.x = #min_new# + (#_in#.x - #min_old#) * #t#;
#_out#.y = #min_new# + (#_in#.y - #min_old#) * #t#;
#_out#.z = #min_new# + (#_in#.z - #min_old#) * #t#;
)" + 1;
			break;
		case VT_4:
			return R"(
#t# = (#max_new# - #min_new#) / (#max_old# - #min_old#);
#_out#.x = #min_new# + (#_in#.x - #min_old#) * #t#;
#_out#.y = #min_new# + (#_in#.y - #min_old#) * #t#;
#_out#.z = #min_new# + (#_in#.z - #min_old#) * #t#;
#_out#.w = #min_new# + (#_in#.w - #min_old#) * #t#;
)" + 1;
			break;
		default:
			return "";
		}
	}

	RTTR_ENABLE(Node)

}; // Remap

}
}