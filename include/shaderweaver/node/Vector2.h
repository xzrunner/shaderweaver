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
		std::string real_name = name;
		if (real_name.empty()) {
			real_name = "flt2";
		}

		InitVariables({
		}, {
			{ t_const | t_flt2, real_name },
		}, {
		});
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