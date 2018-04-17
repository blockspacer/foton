#include "html.h"
#include "el_break.h"

namespace litehtml
{
	el_break::el_break( document* doc)
		: html_tag(doc)
	{
	}

	el_break::~el_break()
	{
	}

	bool el_break::is_break() const
	{
		return true;
	}
}

