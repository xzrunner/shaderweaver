#pragma once

#include "shaderweaver/Node.h"
#include "shaderweaver/NodeHelper.h"

namespace sw
{
namespace node
{

class Multiply : public sw::Node
{
public:
	Multiply()
		: Node("Multiply")
	{
		InitVariables({
			{ 0,  "a" },
			{ 0,  "b" },
		}, {
			{ 0, "_out" },
		}, {
		});

		m_dim_group = { 0, 1, MAX_IMPORTS_COUNT };
	}

	enum InputID
	{
		ID_A = 0,
		ID_B,
	};

    // fixme: same with add's
    void SetInputPortCount(int count)
    {
        if (count <= 2) {
            return;
        }

        std::vector<Variable> input;
        input.resize(count);
        for (int i = 0; i < count; ++i) {
            std::string name;
            name.push_back(static_cast<char>('a' + i));
            input[i] = Variable(0, name);
        }
        InitVariables(input, { { 0, "_out" }, }, {});

        m_dim_group.resize(count + 1);
        for (int i = 0; i < count; ++i) {
            m_dim_group[i] = i;
        }
        m_dim_group[count] = MAX_IMPORTS_COUNT;
    }

protected:
    // fixme: same with add's
	virtual std::string GetBody() const override
	{
        std::string str_sum;
        for (int i = 0, n = m_imports.size(); i < n; ++i)
        {
            str_sum.push_back('#');
            str_sum.push_back(static_cast<char>('a' + i));
            str_sum.push_back('#');
            if (i != n - 1) {
                str_sum += " * ";
            }
        }

        return cpputil::StringHelper::Format("#_out# = %s;\n", str_sum.c_str());
	}

	RTTR_ENABLE(Node)

}; // Multiply

}
}