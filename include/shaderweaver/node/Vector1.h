#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

#include <cpputil/StringHelper.h>

#include <SM_Vector.h>

namespace sw
{
namespace node
{

class Vector1 : public Node
{
public:
	Vector1(const std::string& name, float val)
		: Node("Vector1")
		, m_val(val)
	{
		if (name.empty()) {
			AddVariable(Variable(t_n_out | t_const | t_flt1, "flt1"));
		} else {
			AddVariable(Variable(t_n_out | t_const | t_flt1, name));
		}
	}

protected:
	virtual std::string GetBody() const override
	{
		auto& exports = GetExports();
		assert(exports.size() == 1);
		auto& out = GetExports()[0].var;
		return cpputil::StringHelper::Format(
			"%s (%s) = %f;\n",
			out.Type().ToGLSL().c_str(), out.Name().c_str(), m_val
		);
	}

private:
	float m_val;

}; // Vector1

}
}