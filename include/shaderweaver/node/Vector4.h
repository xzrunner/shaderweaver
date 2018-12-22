#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

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
		std::string real_name = name;
		if (real_name.empty()) {
			real_name = "flt4";
		}

		InitVariables({
		}, {
			{ t_const | t_flt4, real_name },
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
			"%s #%s# = vec4(%f, %f, %f, %f);\n",
			out.GetType().ToGLSL().c_str(), out.GetName().c_str(), m_val.x, m_val.y, m_val.z, m_val.w
		);
	}

private:
	sm::vec4 m_val;

}; // Vector4

}
}