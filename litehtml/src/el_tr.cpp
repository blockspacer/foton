#include "html.h"
#include "el_tr.h"

namespace litehtml {

	el_tr::el_tr( document* doc )
		: html_tag( doc )
	{
	}

	litehtml::el_tr::~el_tr()
	{
	}

	void el_tr::parse_attributes()
	{
		const tchar_t* str = get_attr( atom_align );

		if( str ) {
			m_style.add_property( atom_text_align, str, 0, false );
		}

		str = get_attr( atom_valign );

		if( str ) {
			m_style.add_property( atom_vertical_align, str, 0, false );
		}

		str = get_attr( atom_bgcolor );

		if( str ) {
			m_style.add_property( atom_background_color, str, 0, false );
		}

		html_tag::parse_attributes();
	}

	void el_tr::get_inline_boxes( position::vector& boxes )
	{
		position pos;

		for( auto& el : m_children ) {
			if( el->get_display() == style_display_table_cell ) {
				pos.x		= el->left() + el->margin_left();
				pos.y		= el->top() - m_padding.top - m_borders.top;
				pos.width	= el->right() - pos.x - el->margin_right() - el->margin_left();
				pos.height	= el->height() + m_padding.top + m_padding.bottom + m_borders.top + m_borders.bottom;
				boxes.push_back( pos );
			}
		}
	}
}
