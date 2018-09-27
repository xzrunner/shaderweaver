#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class Split : public sw::Node
{
public:
	Split()
		: Node("Split")
	{
		InitVariables({
			{ 0,       "_in" },
		}, {
			{ t_flt1,  "r" },
			{ t_flt1,  "g" },
			{ t_flt1,  "b" },
			{ t_flt1,  "a" },
		}, {
		});
	}

	enum OutputID
	{
		ID_R = 0,
		ID_G,
		ID_B,
		ID_A,
	};

protected:
	virtual std::string GetBody() const override
	{
		std::string ret;
		if (!m_exports[ID_R].conns.empty()) {
			ret += "#r# = #_in#.r;\n";
		}
		if (!m_exports[ID_G].conns.empty()) {
			ret += "#g# = #_in#.g;\n";
		}
		if (!m_exports[ID_B].conns.empty()) {
			ret += "#b# = #_in#.b;\n";
		}
		if (!m_exports[ID_A].conns.empty()) {
			ret += "#a# = #_in#.a;\n";
		}
		return ret;
	}

}; // Split

}
}