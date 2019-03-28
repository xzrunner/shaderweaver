#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

#include <cpputil/StringHelper.h>
#include <SM_Vector.h>

namespace sw
{
namespace node
{

class Color : public sw::Node
{
public:
	Color()
		: Node("Color")
	{
		InitVariables({
		}, {
			{ t_flt4, "rgba" },
            { t_flt1, "r" },
            { t_flt1, "g" },
            { t_flt1, "b" },
            { t_flt1, "a" },
		}, {
		});
	}

    void SetValue(const sm::vec4& rgba) { m_rgba = rgba; }

protected:
	virtual std::string GetBody() const override
	{
        auto& exports = GetExports();
        assert(exports.size() == 1);
        auto& out = GetExports()[0].var;
        return cpputil::StringHelper::Format(
            "%s #%s# = vec4(%f, %f, %f, %f);\n",
            out.GetType().ToGLSL().c_str(), out.GetName().c_str(), m_rgba.x, m_rgba.y, m_rgba.z, m_rgba.w
        );
    }

private:
    sm::vec4 m_rgba;

	RTTR_ENABLE(Node)

}; // Color

}
}