#pragma once

#include "sw/Node.h"
#include "sw/VariableType.h"

namespace sw
{
namespace node
{

class Blend : public Node
{
public:
	Blend();

	enum ImportIdx
	{
		IN_BASE4 = 0,
		IN_BLEND4,
		IN_MODE,
	};

protected:
	virtual std::string GetHeader() const override;
	virtual std::string GetBody() const override;

}; // Blend

}
}