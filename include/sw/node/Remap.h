#pragma once

#include "sw/Node.h"
#include "sw/NodeHelper.h"
#include "sw/VariableType.h"

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
		AddVariable(Variable(t_n_in, "in"));
		AddVariable(Variable(t_n_in, "from"));
		AddVariable(Variable(t_n_in, "to"));
		AddVariable(Variable(t_flt1 | t_n_mid, "t"));

		AddVariable(Variable(t_n_out, "remap"));
	}

	virtual void Update() override {
		NodeHelper::TypePropote(*this);
	}

	enum ImportIdx
	{
		IN_IN = 0,
		IN_FROM,
		IN_TO,
	};

protected:
	virtual std::string GetBody() const override
	{
		// (in - from.x) / (from.y - from.x) == (x - to.x) / (to.y - to.x);
		// x = (in - from.x) / (from.y - from.x) * (to.y - to.x) + to.x;
		auto& val = GetImports()[IN_IN];
		switch (val.var.Type().dim)
		{
		case VT_1:
			return R"(
(t) = ((to).y - (to).x) / ((from).y - (from).x);
(remap) = (to).x + ((in) - (from).x) * (t);
)" + 1;
			break;
		case VT_2:
			return R"(
(t) = ((to).y - (to).x) / ((from).y - (from).x);
(remap).x = (to).x + ((in).x - (from).x) * (t);
(remap).y = (to).x + ((in).y - (from).x) * (t);
)" + 1;
			break;
		case VT_3:
			return R"(
(t) = ((to).y - (to).x) / ((from).y - (from).x);
(remap).x = (to).x + ((in).x - (from).x) * (t);
(remap).y = (to).x + ((in).y - (from).x) * (t);
(remap).z = (to).x + ((in).z - (from).x) * (t);
)" + 1;
			break;
		case VT_4:
			return R"(
(t) = ((to).y - (to).x) / ((from).y - (from).x);
(remap).x = (to).x + ((in).x - (from).x) * (t);
(remap).y = (to).x + ((in).y - (from).x) * (t);
(remap).z = (to).x + ((in).z - (from).x) * (t);
(remap).w = (to).x + ((in).w - (from).x) * (t);
)" + 1;
			break;
		default:
			return "";
		}
	}

}; // Remap

}
}