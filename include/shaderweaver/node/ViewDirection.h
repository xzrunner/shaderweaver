#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/VariableType.h"

namespace sw
{
namespace node
{

class ViewDirection : public Node
{
public:
    enum class ViewSpace
    {
        Object,
        View,
        World,
        Tangent
    };

public:
    ViewDirection()
        : Node("ViewDirection")
    {
		InitVariables({
		}, {
			{ t_flt3, "xyz" },
            { t_flt1, "x" },
            { t_flt1, "y" },
            { t_flt1, "z" },
		}, {
		});
    }

    ViewSpace GetViewSpace() const { return m_view_space; }
    void SetViewSpace(ViewSpace view_space) { m_view_space = view_space; }

    bool GetSafeNormalize() const { return m_safe_normalize; }
    void SetSafeNormalize(bool safe_normalize) { m_safe_normalize = safe_normalize; }

private:
    ViewSpace m_view_space;

    bool m_safe_normalize = false;

    RTTR_ENABLE(Node)

}; // ViewDirection

}
}