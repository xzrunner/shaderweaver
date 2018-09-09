#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

#include <SM_Vector.h>

namespace sw
{
namespace node
{

class Vector3 : public Node
{
public:
	Vector3(const std::string& name, const sm::vec3& val)
		: Node("Vector3")
		, m_val(val)
	{
		if (name.empty()) {
			AddVariable(Variable(t_n_out | t_const | t_flt3, "flt3"));
		} else {
			AddVariable(Variable(t_n_out | t_const | t_flt3, name));
		}
	}

protected:
	virtual std::string GetBody() const override
	{
		auto& exports = GetExports();
		assert(exports.size() == 1);
		auto& out = GetExports()[0].var;
		return cpputil::StringHelper::Format(
			"%s (%s) = vec3(%f, %f, %f);\n",
			out.Type().ToGLSL().c_str(), out.Name().c_str(), m_val.x, m_val.y, m_val.z
		);
	}

private:
	sm::vec3 m_val;

}; // Vector3

}
}