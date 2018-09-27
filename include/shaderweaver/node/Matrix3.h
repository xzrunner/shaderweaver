#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

#include <cpputil/StringHelper.h>

#include <SM_Matrix.h>

namespace sw
{
namespace node
{

class Matrix3 : public Node
{
public:
	Matrix3(const std::string& name, const sm::mat3& val)
		: Node("Matrix3")
		, m_val(val)
	{
		std::string real_name = name;
		if (real_name.empty()) {
			real_name = "m3";
		}

		InitVariables({
		}, {
			{ t_const | t_mat3, real_name },
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
			"%s #%s# = mat3(%f, %f, %f, %f, %f, %f, %f, %f, %f);\n",
			out.Type().ToGLSL().c_str(), out.Name().c_str(),
			m_val.x[0], m_val.x[1], m_val.x[2],
			m_val.x[3], m_val.x[4], m_val.x[5],
			m_val.x[6], m_val.x[7], m_val.x[8]
		);
	}

private:
	sm::mat3 m_val;

}; // Matrix3

}
}