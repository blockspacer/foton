#include "html.h"
#include "el_image.h"
#include "document.h"

namespace litehtml {
	el_image::el_image( document* doc )
		: html_tag( doc )
	{
		m_display = style_display_inline_block;
	}

	el_image::~el_image( void )
	{
	}

	void el_image::get_content_size( size& sz, int /*max_width*/ )
	{
		get_document()->container()->get_image_size( m_src.c_str(), 0, sz );
	}

	int el_image::line_height() const
	{
		return height();
	}

	bool el_image::is_replaced() const
	{
		return true;
	}

	int el_image::render( int x, int y, int max_width, bool /*second_pass*/ )
	{
		int parent_width = max_width;
		calc_outlines( parent_width );
		m_pos.move_to( x, y );
		document* doc = get_document();
		size sz;
		doc->container()->get_image_size( m_src.c_str(), 0, sz );
		m_pos.width		= sz.width;
		m_pos.height	= sz.height;

		if( m_css_height.is_predefined() && m_css_width.is_predefined() ) {
			m_pos.height	= sz.height;
			m_pos.width		= sz.width;

			// check for max-height
			if( !m_css_max_width.is_predefined() ) {
				int max_width = doc->cvt_units( m_css_max_width, m_font_size, parent_width );

				if( m_pos.width > max_width ) {
					m_pos.width = max_width;
				}

				if( sz.width ) {
					m_pos.height = ( int )( ( float ) m_pos.width * ( float ) sz.height / ( float )sz.width );
				}
				else {
					m_pos.height = sz.height;
				}
			}

			// check for max-height
			if( !m_css_max_height.is_predefined() ) {
				int max_height = doc->cvt_units( m_css_max_height, m_font_size );

				if( m_pos.height > max_height ) {
					m_pos.height = max_height;
				}

				if( sz.height ) {
					m_pos.width = ( int )( m_pos.height * ( float )sz.width / ( float )sz.height );
				}
				else {
					m_pos.width = sz.width;
				}
			}
		}
		else if( !m_css_height.is_predefined() && m_css_width.is_predefined() ) {
			if( !get_predefined_height( m_pos.height ) ) {
				m_pos.height = ( int )m_css_height.val();
			}

			// check for max-height
			if( !m_css_max_height.is_predefined() ) {
				int max_height = doc->cvt_units( m_css_max_height, m_font_size );

				if( m_pos.height > max_height ) {
					m_pos.height = max_height;
				}
			}

			if( sz.height ) {
				m_pos.width = ( int )( m_pos.height * ( float )sz.width / ( float )sz.height );
			}
			else {
				m_pos.width = sz.width;
			}
		}
		else if( m_css_height.is_predefined() && !m_css_width.is_predefined() ) {
			m_pos.width = ( int ) m_css_width.calc_percent( parent_width );

			// check for max-width
			if( !m_css_max_width.is_predefined() ) {
				int max_width = doc->cvt_units( m_css_max_width, m_font_size, parent_width );

				if( m_pos.width > max_width ) {
					m_pos.width = max_width;
				}
			}

			if( sz.width ) {
				m_pos.height = ( int )( ( float ) m_pos.width * ( float ) sz.height / ( float )sz.width );
			}
			else {
				m_pos.height = sz.height;
			}
		}
		else {
			m_pos.width		= ( int ) m_css_width.calc_percent( parent_width );
			m_pos.height	= 0;

			if( !get_predefined_height( m_pos.height ) ) {
				m_pos.height = ( int )m_css_height.val();
			}

			// check for max-height
			if( !m_css_max_height.is_predefined() ) {
				int max_height = doc->cvt_units( m_css_max_height, m_font_size );

				if( m_pos.height > max_height ) {
					m_pos.height = max_height;
				}
			}

			// check for max-height
			if( !m_css_max_width.is_predefined() ) {
				int max_width = doc->cvt_units( m_css_max_width, m_font_size, parent_width );

				if( m_pos.width > max_width ) {
					m_pos.width = max_width;
				}
			}
		}

		calc_auto_margins( parent_width );
		m_pos.x	+= content_margins_left();
		m_pos.y += content_margins_top();
		return m_pos.width + content_margins_left() + content_margins_right();
	}

	void el_image::parse_attributes()
	{
		m_src = get_attr( atom_src, _t( "" ) );
		const tchar_t* attr_height = get_attr( atom_height );

		if( attr_height ) {
			m_style.add_property( atom_height, attr_height, 0, false );
		}

		const tchar_t* attr_width = get_attr( atom_width );

		if( attr_width ) {
			m_style.add_property( atom_width, attr_width, 0, false );
		}
	}

	void el_image::draw( uint_ptr hdc, int x, int y, const position* clip )
	{
		position pos = m_pos;
		pos.x += x;
		pos.y += y;
		position el_pos = pos;
		el_pos += m_padding;
		el_pos += m_borders;

		// draw standard background here
		if( el_pos.does_intersect( clip ) ) {
			const background* bg = get_background();

			if( bg ) {
				background_paint bg_paint;
				init_background_paint( pos, bg_paint, bg );
				get_document()->container()->draw_background( hdc, bg_paint );
			}
		}

		// draw image as background
		if( pos.does_intersect( clip ) ) {
			if( pos.width > 0 && pos.height > 0 ) {
				background_paint bg;
				bg.image				= m_src;
				bg.clip_box				= pos;
				bg.origin_box			= pos;
				bg.border_box			= pos;
				bg.border_box			+= m_padding;
				bg.border_box			+= m_borders;
				bg.repeat				= background_repeat_no_repeat;
				bg.image_size.width		= pos.width;
				bg.image_size.height	= pos.height;
				bg.border_radius		= m_css_borders.radius.calc_percents( bg.border_box.width, bg.border_box.height );
				bg.position_x			= pos.x;
				bg.position_y			= pos.y;
				get_document()->container()->draw_background( hdc, bg );
			}
		}

		// draw borders
		if( el_pos.does_intersect( clip ) ) {
			position border_box = pos;
			border_box += m_padding;
			border_box += m_borders;
			borders bdr = m_css_borders;
			bdr.radius = m_css_borders.radius.calc_percents( border_box.width, border_box.height );
			get_document()->container()->draw_borders( hdc, bdr, border_box, have_parent() ? false : true );
		}
	}

	void el_image::parse_styles( bool is_reparse /*= false*/ )
	{
		html_tag::parse_styles( is_reparse );

		if( !m_src.empty() ) {
			if( !m_css_height.is_predefined() && !m_css_width.is_predefined() ) {
				get_document()->container()->load_image( m_src.c_str(), 0, true );
			}
			else {
				get_document()->container()->load_image( m_src.c_str(), 0, false );
			}
		}
	}
}
