#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

#include <SM_Vector.h>

namespace sw
{
namespace node
{

class Vector3 : public Node
{
public:
	Vector3(const std::string& name = "", const sm::vec3& val = sm::vec3())
		: Node("Vector3")
		, m_val(val)
	{
		SetName(name);
	}

	void SetName(const std::string& name)
	{
		std::string real_name = name;
		if (real_name.empty()) {
			real_name = "_out";
		}

		InitVariables({
		}, {
			{ t_const | t_flt3, real_name },
		}, {
		});
	}

	void SetValue(const sm::vec3& val) { m_val = val; }

protected:
	virtual std::string GetBody() const override
	{
		auto& exports = GetExports();
		assert(exports.size() == 1);
		auto& out = GetExports()[0].var;
		return cpputil::StringHelper::Format(
			"%s #%s# = vec3(%f, %f, %f);\n",
			out.GetType().ToGLSL().c_str(), out.GetName().c_str(), m_val.x, m_val.y, m_val.z
		);
	}

private:
	sm::vec3 m_val;

	RTTR_ENABLE(Node)

}; // Vector3

}
}