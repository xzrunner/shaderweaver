#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

#include <cpputil/StringHelper.h>

#include <SM_Vector.h>

namespace sw
{
namespace node
{

class Boolean : public Node
{
public:
	Boolean(const std::string& name = "", bool val = true)
		: Node("Boolean")
		, m_val(val)
	{
		SetName(name);
	}

	void SetName(const std::string& name)
	{
		std::string real_name = name;
		if (real_name.empty()) {
			real_name = "boolean";
		}

		InitVariables({
		}, {
			{ t_const | t_bool, real_name },
		}, {
		});
	}

	void SetValue(bool val) { m_val = val; }

protected:
	virtual std::string GetBody() const override
	{
		auto& exports = GetExports();
		assert(exports.size() == 1);
		auto& out = GetExports()[0].var;
		return cpputil::StringHelper::Format(
			"%s #%s# = %s;\n",
			out.GetType().ToGLSL().c_str(), out.GetName().c_str(), m_val ? "true" : "false"
		);
	}

private:
	bool m_val;

	RTTR_ENABLE(Node)

}; // Boolean

}
}