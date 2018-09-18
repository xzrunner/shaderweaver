#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

#include <SM_Vector.h>

namespace sw
{
namespace node
{

class Vector2 : public Node
{
public:
	Vector2(const std::string& name, const sm::vec2& val)
		: Node("Vector2")
		, m_val(val)
	{
		if (name.empty()) {
			AddVariable(Variable(t_n_out | t_const | t_flt2, "flt2"));
		} else {
			AddVariable(Variable(t_n_out | t_const | t_flt2, name));
		}
	}

protected:
	virtual std::string GetBody() const override
	{
		auto& exports = GetExports();
		assert(exports.size() == 1);
		auto& out = GetExports()[0].var;
		return cpputil::StringHelper::Format(
			"%s (%s) = vec2(%f, %f);\n",
			out.Type().ToGLSL().c_str(), out.Name().c_str(), m_val.x, m_val.y
		);
	}

private:
	sm::vec2 m_val;

}; // Vector2

}
}