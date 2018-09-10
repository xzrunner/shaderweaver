#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

#include <SM_Vector.h>

namespace sw
{
namespace node
{

class Vector4 : public sw::Node
{
public:
	Vector4(const std::string& name, const sm::vec4& val)
		: Node("Vector4")
		, m_val(val)
	{
		if (name.empty()) {
			AddVariable(Variable(t_n_out | t_const | t_flt4, "flt4"));
		} else {
			AddVariable(Variable(t_n_out | t_const | t_flt4, name));
		}
	}

protected:
	virtual std::string GetBody() const override
	{
		auto& exports = GetExports();
		assert(exports.size() == 1);
		auto& out = GetExports()[0].var;
		return cpputil::StringHelper::Format(
			"%s (%s) = vec4(%f, %f, %f, %f);\n",
			out.Type().ToGLSL().c_str(), out.Name().c_str(), m_val.x, m_val.y, m_val.z, m_val.w
		);
	}

private:
	sm::vec4 m_val;

}; // Vector4

}
}