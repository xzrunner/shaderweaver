#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

#include <cpputil/StringHelper.h>

#include <SM_Matrix.h>

namespace sw
{
namespace node
{

class Matrix2 : public Node
{
public:
	Matrix2(const std::string& name = "", const sm::mat2& val = sm::mat2())
		: Node("Matrix2")
		, m_val(val)
	{
		SetName(name);
	}

	void SetName(const std::string& name)
	{
		std::string real_name = name;
		if (real_name.empty()) {
			real_name = "m2";
		}

		InitVariables({
		}, {
			{ t_const | t_mat2, real_name },
		}, {
		});
	}

	void SetValue(const sm::mat2& val) { m_val = val; }

protected:
	virtual std::string GetBody() const override
	{
		auto& exports = GetExports();
		assert(exports.size() == 1);
		auto& out = GetExports()[0].var;
		return cpputil::StringHelper::Format(
			"%s #%s# = mat2(%f, %f, %f, %f);\n",
			out.GetType().ToGLSL().c_str(), out.GetName().c_str(),
			m_val.x[0], m_val.x[1], m_val.x[2], m_val.x[3]
		);
	}

private:
	sm::mat2 m_val;

	RTTR_ENABLE(Node)

}; // Matrix2

}
}