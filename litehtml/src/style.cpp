#include "html.h"
#include "style.h"
#include <functional>
#include <algorithm>
#ifndef WINCE
#include <locale>
#endif

namespace litehtml {

	/*
	attr_map style::m_valid_values =
	{
		{ atom_white_space, white_space_strings }
	};
	*/

	style::style()
	{
	}

	style::style( const style& val )
	{
		m_properties = val.m_properties;
	}

	style::~style()
	{
	}

	void style::parse( const tchar_t* txt, const tchar_t* baseurl )
	{
		while( *txt ) {
			const char*	p = skip_sp( txt );
			const char* q = p;

			while( *q && *q!=';' ) {
				q++;
			}

			parse_property( xstring(p, q-p), baseurl);

			if( *q==';' )
				q++;

			txt = q;
		}
	}

	void style::parse_property( const xstring& txt, const tchar_t* baseurl )
	{
		const tchar_t*	p = txt.c_str();
		const tchar_t*	q = p;
		while( *q && *q!=':') {
			q++;
		}

		if( *q!=':' ) {
			//todo warning bad style definition
			return;
		}

		// trim
		const tchar_t* r = q-1;
		while( r>=p && *r==' ' || *r=='\t' ) {
			r--;
		}

		int	name = get_atom( p, r-p );

		q = skip_sp( q+1 );
		r = q+t_strlen(q)-1;
		while( r>=q && *r==' ' || *r=='\t' ) {
			r--;
		}

		if( name && r>q ) {

			p = q;
			while( p<r && *p!='!' ) {
				p++;
			}

			if( *p=='!' && t_strncasecmp( skip_sp(p+1),'important')==0 ) {
				add_property( name, xstring(q,p-q).c_str(), baseurl, true );
			}
			else {
				add_property( name, xstring(q,r-q).c_str(), baseurl, false );
			}
		}
	}

	void style::combine( const style& src )
	{
		for(props_map::const_iterator i = src.m_properties.begin(); i != src.m_properties.end(); i++)
		{
			add_parsed_property( i->first, i->second.m_value.c_str(), i->second.m_important);
		}
	}

	void style::add_property( atom name, const tchar_t* val, const tchar_t* baseurl, bool important )
	{
		if(!name || !val)
		{
			return;
		}

		// Add baseurl for background image
		if(	name==atom_background_image )
		{
			add_parsed_property(name, val, important);
			if(baseurl)
			{
				add_parsed_property(atom_background_baseurl, baseurl, important);
			}
		}
		// Parse border spacing properties
		else if( name==atom_border_spacing )
		{
			string_vector tokens;
			split_string(val, tokens, _t(" "));
			if(tokens.size() == 1)
			{
				add_property(atom__xx_border_spacing_x, tokens[0].c_str(), baseurl, important);
				add_property(atom__xx_border_spacing_y, tokens[0].c_str(), baseurl, important);
			}
			else if(tokens.size() == 2)
			{
				add_property(atom__xx_border_spacing_x, tokens[0].c_str(), baseurl, important);
				add_property(atom__xx_border_spacing_y, tokens[1].c_str(), baseurl, important);
			}
		}
		// Parse borders shorthand properties
		else if( name == atom_border )
		{
			string_vector tokens;
			split_string(val, tokens, _t(" "), _t(""), _t("("));
			int idx;
			tstring str;
			for(string_vector::const_iterator tok = tokens.begin(); tok != tokens.end(); tok++)
			{
				idx = get_border_style( tok->c_str(), tok->length(), -1 );
				if(idx >= 0) {
					add_property(atom_border_left_style, tok->c_str(), baseurl, important);
					add_property(atom_border_right_style, tok->c_str(), baseurl, important);
					add_property(atom_border_top_style, tok->c_str(), baseurl, important);
					add_property(atom_border_bottom_style, tok->c_str(), baseurl, important);
				}
				else {
					if (t_isdigit((*tok)[0]) || (*tok)[0] == _t('.') ||
						get_border_width( tok->c_str(), tok->length(), -1 )>=0 )
					{
						add_property(atom_border_left_width, tok->c_str(), baseurl, important);
						add_property(atom_border_right_width, tok->c_str(), baseurl, important);
						add_property(atom_border_top_width, tok->c_str(), baseurl, important);
						add_property(atom_border_bottom_width, tok->c_str(), baseurl, important);
					}
					else
					{
						add_property(atom_border_left_color, tok->c_str(), baseurl, important);
						add_property(atom_border_right_color, tok->c_str(), baseurl, important);
						add_property(atom_border_top_color, tok->c_str(), baseurl, important);
						add_property(atom_border_bottom_color, tok->c_str(), baseurl, important);
					}
				}
			}
		}
		else if( name==atom_border_left	||
				 name==atom_border_right ||
				 name==atom_border_top ||
				 name==atom_border_bottom )
		{
			string_vector tokens;
			split_string(val, tokens, _t(" "), _t(""), _t("("));
			int idx;
			tstring str;
			for(string_vector::const_iterator tok = tokens.begin(); tok != tokens.end(); tok++)
			{
				idx = get_border_style(tok->c_str(), tok->length(), -1);
				if(idx >= 0)
				{
					str = name;
					str += _t("-style");
					add_property( atom_create(str.c_str()), tok->c_str(), baseurl, important);
				}
				else
				{
					if(web_color::is_color(tok->c_str()))
					{
						str = name;
						str += _t("-color");
						add_property(atom_create(str.c_str()), tok->c_str(), baseurl, important);
					}
					else
					{
						str = name;
						str += _t("-width");
						add_property(atom_create(str.c_str()), tok->c_str(), baseurl, important);
					}
				}
			}
		}
		// Parse border radius shorthand properties
		 else if(name==atom_border_bottom_left_radius)
		{
			string_vector tokens;
			split_string(val, tokens, _t(" "));
			if(tokens.size() >= 2)
			{
				add_property(atom_border_bottom_left_radius_x, tokens[0].c_str(), baseurl, important);
				add_property(atom_border_bottom_left_radius_y, tokens[1].c_str(), baseurl, important);
			}
			else if(tokens.size() == 1)
			{
				add_property(atom_border_bottom_left_radius_x, tokens[0].c_str(), baseurl, important);
				add_property(atom_border_bottom_left_radius_y, tokens[0].c_str(), baseurl, important);
			}

		}
		else if(name==atom_border_bottom_right_radius)
		{
			string_vector tokens;
			split_string(val, tokens, _t(" "));
			if(tokens.size() >= 2)
			{
				add_property(atom_border_bottom_right_radius_x, tokens[0].c_str(), baseurl, important);
				add_property(atom_border_bottom_right_radius_y, tokens[1].c_str(), baseurl, important);
			}
			else if(tokens.size() == 1)
			{
				add_property(atom_border_bottom_right_radius_x, tokens[0].c_str(), baseurl, important);
				add_property(atom_border_bottom_right_radius_y, tokens[0].c_str(), baseurl, important);
			}

		}
		else if(name==atom_border_top_right_radius)
		{
			string_vector tokens;
			split_string(val, tokens, _t(" "));
			if(tokens.size() >= 2)
			{
				add_property(atom_border_top_right_radius_x, tokens[0].c_str(), baseurl, important);
				add_property(atom_border_top_right_radius_y, tokens[1].c_str(), baseurl, important);
			}
			else if(tokens.size() == 1)
			{
				add_property(atom_border_top_right_radius_x, tokens[0].c_str(), baseurl, important);
				add_property(atom_border_top_right_radius_y, tokens[0].c_str(), baseurl, important);
			}

		}
		else if(name==atom_border_top_left_radius)
		{
			string_vector tokens;
			split_string(val, tokens, _t(" "));
			if(tokens.size() >= 2)
			{
				add_property(atom_border_top_left_radius_x, tokens[0].c_str(), baseurl, important);
				add_property(atom_border_top_left_radius_y, tokens[1].c_str(), baseurl, important);
			}
			else if(tokens.size() == 1)
			{
				add_property(atom_border_top_left_radius_x, tokens[0].c_str(), baseurl, important);
				add_property(atom_border_top_left_radius_y, tokens[0].c_str(), baseurl, important);
			}

		}
		// Parse border-radius shorthand properties
		else if(name==atom_border_radius)
		{
			string_vector tokens;
			split_string(val, tokens, _t("/"));
			if(tokens.size() == 1)
			{
				add_property(atom_border_radius_x, tokens[0].c_str(), baseurl, important);
				add_property(atom_border_radius_y, tokens[0].c_str(), baseurl, important);
			}
			else if(tokens.size() >= 2)
			{
				add_property(atom_border_radius_x, tokens[0].c_str(), baseurl, important);
				add_property(atom_border_radius_y, tokens[1].c_str(), baseurl, important);
			}
		}
		else if(name==atom_border_radius_x)
		{
			string_vector tokens;
			split_string(val, tokens, _t(" "));
			if(tokens.size() == 1)
			{
				add_property(atom_border_top_left_radius_x,			tokens[0].c_str(), baseurl, important);
				add_property(atom_border_top_right_radius_x,		tokens[0].c_str(), baseurl, important);
				add_property(atom_border_bottom_right_radius_x,		tokens[0].c_str(), baseurl, important);
				add_property(atom_border_bottom_left_radius_x,		tokens[0].c_str(), baseurl, important);
			}
			else if(tokens.size() == 2)
			{
				add_property(atom_border_top_left_radius_x,			tokens[0].c_str(), baseurl, important);
				add_property(atom_border_top_right_radius_x,		tokens[1].c_str(), baseurl, important);
				add_property(atom_border_bottom_right_radius_x,		tokens[0].c_str(), baseurl, important);
				add_property(atom_border_bottom_left_radius_x,		tokens[1].c_str(), baseurl, important);
			}
			else if(tokens.size() == 3)
			{
				add_property(atom_border_top_left_radius_x,			tokens[0].c_str(), baseurl, important);
				add_property(atom_border_top_right_radius_x,		tokens[1].c_str(), baseurl, important);
				add_property(atom_border_bottom_right_radius_x,		tokens[2].c_str(), baseurl, important);
				add_property(atom_border_bottom_left_radius_x,		tokens[1].c_str(), baseurl, important);
			}
			else if(tokens.size() == 4)
			{
				add_property(atom_border_top_left_radius_x,			tokens[0].c_str(), baseurl, important);
				add_property(atom_border_top_right_radius_x,		tokens[1].c_str(), baseurl, important);
				add_property(atom_border_bottom_right_radius_x,		tokens[2].c_str(), baseurl, important);
				add_property(atom_border_bottom_left_radius_x,		tokens[3].c_str(), baseurl, important);
			}
		}
		else if(name==atom_border_radius_y)
		{
			string_vector tokens;
			split_string(val, tokens, _t(" "));
			if(tokens.size() == 1)
			{
				add_property(atom_border_top_left_radius_y,			tokens[0].c_str(), baseurl, important);
				add_property(atom_border_top_right_radius_y,		tokens[0].c_str(), baseurl, important);
				add_property(atom_border_bottom_right_radius_y,		tokens[0].c_str(), baseurl, important);
				add_property(atom_border_bottom_left_radius_y,		tokens[0].c_str(), baseurl, important);
			}
			else if(tokens.size() == 2)
			{
				add_property(atom_border_top_left_radius_y,			tokens[0].c_str(), baseurl, important);
				add_property(atom_border_top_right_radius_y,		tokens[1].c_str(), baseurl, important);
				add_property(atom_border_bottom_right_radius_y,		tokens[0].c_str(), baseurl, important);
				add_property(atom_border_bottom_left_radius_y,		tokens[1].c_str(), baseurl, important);
			}
			else if(tokens.size() == 3)
			{
				add_property(atom_border_top_left_radius_y,			tokens[0].c_str(), baseurl, important);
				add_property(atom_border_top_right_radius_y,		tokens[1].c_str(), baseurl, important);
				add_property(atom_border_bottom_right_radius_y,		tokens[2].c_str(), baseurl, important);
				add_property(atom_border_bottom_left_radius_y,		tokens[1].c_str(), baseurl, important);
			}
			else if(tokens.size() == 4)
			{
				add_property(atom_border_top_left_radius_y,			tokens[0].c_str(), baseurl, important);
				add_property(atom_border_top_right_radius_y,		tokens[1].c_str(), baseurl, important);
				add_property(atom_border_bottom_right_radius_y,		tokens[2].c_str(), baseurl, important);
				add_property(atom_border_bottom_left_radius_y,		tokens[3].c_str(), baseurl, important);
			}
		}


		// Parse list-style shorthand properties
		if(name==atom_list_style)
		{
			add_parsed_property(atom_list_style_type,			_t("disc"),		important);
			add_parsed_property(atom_list_style_position,		_t("outside"),	important);
	//<eco:		add_parsed_property(atom_list_style_image,			_t(""),			important);
	//<eco:		add_parsed_property(atom_list_style_baseurl,		_t(""),			important);

			string_vector tokens;
			split_string(val, tokens, _t(" "), _t(""), _t("("));
			for(string_vector::iterator tok = tokens.begin(); tok != tokens.end(); tok++)
			{
				int idx = get_list_style_type(tok->c_str(), -1, -1 );
				if(idx >= 0) {
					add_parsed_property(atom_list_style_type, *tok, important);
				}
				else
				{
					idx = get_list_style_position(tok->c_str(), -1, -1);
					if(idx >= 0) {
						add_parsed_property(atom_list_style_position, *tok, important);
					}
					else if(!t_strncmp(val, _t("url"), 3)) {
						//todo: check
						add_parsed_property(atom_list_style_image, *tok, important);
						if(baseurl)
						{
							add_parsed_property(atom_list_style_baseurl, baseurl, important);
						}
					}
				}
			}
		}
		// Add baseurl for background image
		else if( name==atom_list_style_image )
		{
			add_parsed_property(name, val, important);
			if(baseurl)
			{
				add_parsed_property(atom_list_style_baseurl, baseurl, important);
			}
		}
		// Parse background shorthand properties
		else if(name==atom_background)
		{
			parse_short_background(val, baseurl, important);
		}
		// Parse margin shorthand properties
		else if( name==atom_margin )
		{
			string_vector tokens;
			split_string(val, tokens, _t(" "));
			if(tokens.size() >= 4)
			{
				add_parsed_property(atom_margin_top,		tokens[0], important);
				add_parsed_property(atom_margin_right,		tokens[1], important);
				add_parsed_property(atom_margin_bottom,		tokens[2], important);
				add_parsed_property(atom_margin_left,		tokens[3], important);
			}
			else if(tokens.size() == 3)
			{
				add_parsed_property(atom_margin_top,		tokens[0], important);
				add_parsed_property(atom_margin_right,		tokens[1], important);
				add_parsed_property(atom_margin_left,		tokens[1], important);
				add_parsed_property(atom_margin_bottom,		tokens[2], important);
			}
			else if(tokens.size() == 2)
			{
				add_parsed_property(atom_margin_top,		tokens[0], important);
				add_parsed_property(atom_margin_bottom,		tokens[0], important);
				add_parsed_property(atom_margin_right,		tokens[1], important);
				add_parsed_property(atom_margin_left,		tokens[1], important);
			}
			else if(tokens.size() == 1)
			{
				add_parsed_property(atom_margin_top,		tokens[0], important);
				add_parsed_property(atom_margin_bottom,		tokens[0], important);
				add_parsed_property(atom_margin_right,		tokens[0], important);
				add_parsed_property(atom_margin_left,		tokens[0], important);
			}
		}
		// Parse padding shorthand properties
		else if( name==atom_padding )
		{
			string_vector tokens;
			split_string(val, tokens, _t(" "));
			if(tokens.size() >= 4)
			{
				add_parsed_property(atom_padding_top,		tokens[0], important);
				add_parsed_property(atom_padding_right,		tokens[1], important);
				add_parsed_property(atom_padding_bottom,	tokens[2], important);
				add_parsed_property(atom_padding_left,		tokens[3], important);
			}
			else if(tokens.size() == 3)
			{
				add_parsed_property(atom_padding_top,		tokens[0], important);
				add_parsed_property(atom_padding_right,		tokens[1], important);
				add_parsed_property(atom_padding_left,		tokens[1], important);
				add_parsed_property(atom_padding_bottom,	tokens[2], important);
			}
			else if(tokens.size() == 2)
			{
				add_parsed_property(atom_padding_top,		tokens[0], important);
				add_parsed_property(atom_padding_bottom,	tokens[0], important);
				add_parsed_property(atom_padding_right,		tokens[1], important);
				add_parsed_property(atom_padding_left,		tokens[1], important);
			}
			else if(tokens.size() == 1)
			{
				add_parsed_property(atom_padding_top,		tokens[0], important);
				add_parsed_property(atom_padding_bottom,	tokens[0], important);
				add_parsed_property(atom_padding_right,		tokens[0], important);
				add_parsed_property(atom_padding_left,		tokens[0], important);
			}
		}
		// Parse border-* shorthand properties
		else if( name==atom_border_left ) {

			string_vector tokens;
			split_string(val, tokens, _t(" "), _t(""), _t("("));

			if(tokens.size() >= 3)
			{
				add_parsed_property(atom_border_left_width,	tokens[0], important);
				add_parsed_property(atom_border_left_style,	tokens[1], important);
				add_parsed_property(atom_border_left_color,	tokens[2], important);
			}
			else if(tokens.size() == 2)
			{
				if(iswdigit(tokens[0][0]) || get_border_width(val, -1, -1 ) >= 0)
				{
					add_parsed_property(atom_border_left_width,	tokens[0], important);
					add_parsed_property(atom_border_left_style,	tokens[1], important);
				}
				else
				{
					add_parsed_property(atom_border_left_style,	tokens[0], important);
					add_parsed_property(atom_border_left_color,	tokens[1], important);
				}
			}
		}
		else if( name==atom_border_right ) {
			string_vector tokens;
			split_string(val, tokens, _t(" "), _t(""), _t("("));

			if(tokens.size() >= 3)
			{
				add_parsed_property(atom_border_right_width,	tokens[0], important);
				add_parsed_property(atom_border_right_style,	tokens[1], important);
				add_parsed_property(atom_border_right_color,	tokens[2], important);
			}
			else if(tokens.size() == 2)
			{
				if(iswdigit(tokens[0][0]) || get_border_width(val, -1, -1 ) >= 0)
				{
					add_parsed_property(atom_border_right_width,	tokens[0], important);
					add_parsed_property(atom_border_right_style,	tokens[1], important);
				}
				else
				{
					add_parsed_property(atom_border_right_style,	tokens[0], important);
					add_parsed_property(atom_border_right_color,	tokens[1], important);
				}
			}
		}
		else if( name==atom_border_top ) {
			string_vector tokens;
			split_string(val, tokens, _t(" "), _t(""), _t("("));
			if(tokens.size() >= 3)
			{
				add_parsed_property(atom_border_top_width,	tokens[0], important);
				add_parsed_property(atom_border_top_style,	tokens[1], important);
				add_parsed_property(atom_border_top_color,	tokens[2], important);
			}
			else if(tokens.size() == 2)
			{
				if(iswdigit(tokens[0][0]) || get_border_width(val, -1, -1) >= 0)
				{
					add_parsed_property(atom_border_top_width,	tokens[0], important);
					add_parsed_property(atom_border_top_style,	tokens[1], important);
				}
				else
				{
					add_parsed_property(atom_border_top_style,	tokens[0], important);
					add_parsed_property(atom_border_top_color,	tokens[1], important);
				}
			}
		}
		else if( name==atom_border_bottom ) {
			string_vector tokens;
			split_string(val, tokens, _t(" "), _t(""), _t("("));

			if(tokens.size() >= 3) {
				add_parsed_property(atom_border_bottom_width,	tokens[0], important);
				add_parsed_property(atom_border_bottom_style,	tokens[1], important);
				add_parsed_property(atom_border_bottom_color,	tokens[2], important);
			}
			else if(tokens.size() == 2) {
				if(iswdigit(tokens[0][0]) || get_border_width(val, -1, -1) >= 0) {
					add_parsed_property(atom_border_bottom_width,	tokens[0], important);
					add_parsed_property(atom_border_bottom_style,	tokens[1], important);
				}
				else {
					add_parsed_property(atom_border_bottom_style,	tokens[0], important);
					add_parsed_property(atom_border_bottom_color,	tokens[1], important);
				}
			}
		}
		// Parse border-width/style/color shorthand properties
		else if( name==atom_border_width )
		{
			string_vector tokens;
			split_string(val, tokens, _t(" "));

			if(tokens.size() >= 4) {
				add_parsed_property(atom_border_top_width,		tokens[0], important);
				add_parsed_property(atom_border_right_width,	tokens[1], important);
				add_parsed_property(atom_border_bottom_width,	tokens[2], important);
				add_parsed_property(atom_border_left_width,		tokens[3], important);
			}
			else if(tokens.size() == 3) {
				add_parsed_property(atom_border_top_width,		tokens[0], important);
				add_parsed_property(atom_border_right_width,	tokens[1], important);
				add_parsed_property(atom_border_left_width,		tokens[1], important);
				add_parsed_property(atom_border_bottom_width,	tokens[2], important);
			}
			else if(tokens.size() == 2) {
				add_parsed_property(atom_border_top_width,		tokens[0], important);
				add_parsed_property(atom_border_bottom_width,	tokens[0], important);
				add_parsed_property(atom_border_right_width,	tokens[1], important);
				add_parsed_property(atom_border_left_width,		tokens[1], important);
			}
			else if(tokens.size() == 1) {
				add_parsed_property(atom_border_top_width,		tokens[0], important);
				add_parsed_property(atom_border_bottom_width,	tokens[0], important);
				add_parsed_property(atom_border_right_width,	tokens[0], important);
				add_parsed_property(atom_border_left_width,		tokens[0], important);
			}
		}
		else if( name==atom_border_style ) {
			string_vector tokens;
			split_string(val, tokens, _t(" "));

			if(tokens.size() >= 4) {
				add_parsed_property(atom_border_top_style,		tokens[0], important);
				add_parsed_property(atom_border_right_style,	tokens[1], important);
				add_parsed_property(atom_border_bottom_style,	tokens[2], important);
				add_parsed_property(atom_border_left_style,		tokens[3], important);
			}
			else if(tokens.size() == 3) {
				add_parsed_property(atom_border_top_style,		tokens[0], important);
				add_parsed_property(atom_border_right_style,	tokens[1], important);
				add_parsed_property(atom_border_left_style,		tokens[1], important);
				add_parsed_property(atom_border_bottom_style,	tokens[2], important);
			}
			else if(tokens.size() == 2) {
				add_parsed_property(atom_border_top_style,		tokens[0], important);
				add_parsed_property(atom_border_bottom_style,	tokens[0], important);
				add_parsed_property(atom_border_right_style,	tokens[1], important);
				add_parsed_property(atom_border_left_style,		tokens[1], important);
			}
			else if(tokens.size() == 1) {
				add_parsed_property(atom_border_top_style,		tokens[0], important);
				add_parsed_property(atom_border_bottom_style,	tokens[0], important);
				add_parsed_property(atom_border_right_style,	tokens[0], important);
				add_parsed_property(atom_border_left_style,		tokens[0], important);
			}
		}
		else if( name==atom_border_color ) {

			string_vector tokens;
			split_string(val, tokens, _t(" "));

			if(tokens.size() >= 4) {
				add_parsed_property(atom_border_top_color,		tokens[0], important);
				add_parsed_property(atom_border_right_color,	tokens[1], important);
				add_parsed_property(atom_border_bottom_color,	tokens[2], important);
				add_parsed_property(atom_border_left_color,		tokens[3], important);
			}
			else if(tokens.size() == 3) {
				add_parsed_property(atom_border_top_color,		tokens[0], important);
				add_parsed_property(atom_border_right_color,	tokens[1], important);
				add_parsed_property(atom_border_left_color,		tokens[1], important);
				add_parsed_property(atom_border_bottom_color,	tokens[2], important);
			}
			else if(tokens.size() == 2) {
				add_parsed_property(atom_border_top_color,		tokens[0], important);
				add_parsed_property(atom_border_bottom_color,	tokens[0], important);
				add_parsed_property(atom_border_right_color,	tokens[1], important);
				add_parsed_property(atom_border_left_color,		tokens[1], important);
			}
			else if(tokens.size() == 1) {
				add_parsed_property(atom_border_top_color,		tokens[0], important);
				add_parsed_property(atom_border_bottom_color,	tokens[0], important);
				add_parsed_property(atom_border_right_color,	tokens[0], important);
				add_parsed_property(atom_border_left_color,		tokens[0], important);
			}
		}
		// Parse font shorthand properties
		else if( name==atom_font ) {
			parse_short_font(val, important);
		}
		else {
			add_parsed_property(name, val, important);
		}
	}

	/*
	void style::parse_short_border( const tstring& prefix, const tstring& val, bool important )
	{
		string_vector tokens;
		split_string(val, tokens, _t(" "), _t(""), _t("("));
		if(tokens.size() >= 3)
		{
			add_parsed_property(atom_create(prefix + _t("-width")),	tokens[0], important);
			add_parsed_property(atom_create(prefix + _t("-style")),	tokens[1], important);
			add_parsed_property(atom_create(prefix + _t("-color")),	tokens[2], important);
		}
		else if(tokens.size() == 2)
		{
			if(iswdigit(tokens[0][0]) || value_index(val.c_str(), border_width_strings) >= 0)
			{
				add_parsed_property(atom_create(prefix + _t("-width")),	tokens[0], important);
				add_parsed_property(atom_create(prefix + _t("-style")),	tokens[1], important);
			}
			else
			{
				add_parsed_property(atom_create(prefix + _t("-style")),	tokens[0], important);
				add_parsed_property(atom_create(prefix + _t("-color")),	tokens[1], important);
			}
		}
	}
	*/

	void style::parse_short_background( const tstring& val, const tchar_t* baseurl, bool important )
	{
		add_parsed_property(atom_background_color,			_t("transparent"),	important);
		add_parsed_property(atom_background_image,			_t(""),				important);
		add_parsed_property(atom_background_baseurl,		_t(""),				important);
		add_parsed_property(atom_background_repeat,			_t("repeat"),		important);
		add_parsed_property(atom_background_origin,			_t("padding-box"),	important);
		add_parsed_property(atom_background_clip,			_t("border-box"),	important);
		add_parsed_property(atom_background_attachment,		_t("scroll"),		important);

		if(val == _t("none"))
		{
			return;
		}

		string_vector tokens;
		split_string(val, tokens, _t(" "), _t(""), _t("("));
		bool origin_found = false;

		for(string_vector::iterator tok = tokens.begin(); tok != tokens.end(); tok++)
		{
			if(tok->substr(0, 3) == _t("url"))
			{
				add_parsed_property(atom_background_image, *tok, important);
				if(baseurl)
				{
					add_parsed_property( atom_background_baseurl, baseurl, important);
				}

			}
			else if( get_background_repeat(tok->c_str(), -1, -1 )>=0 )
			{
				add_parsed_property( atom_background_repeat, *tok, important);
			}
			else if( get_background_attachment(tok->c_str(), -1, -1)>=0 )
			{
				add_parsed_property(atom_background_attachment, *tok, important);
			}
			else if( get_background_box(tok->c_str(), -1, -1)>=0 )
			{
				if(!origin_found) {
					add_parsed_property(atom_background_origin, *tok, important);
					origin_found = true;
				}
				else {
					add_parsed_property(atom_background_clip,*tok, important);
				}
			}
			else if(	get_background_position(tok->c_str(), -1, -1 ) ||
						iswdigit((*tok)[0]) ||
						(*tok)[0] == _t('-')	||
						(*tok)[0] == _t('.')	||
						(*tok)[0] == _t('+'))
			{
				if(m_properties.find(atom_background_position) != m_properties.end())
				{
					//todo: optimize
					m_properties[atom_background_position].m_value = m_properties[atom_background_position].m_value + _t(" ") + *tok;
				}
				else
				{
					add_parsed_property(atom_background_position, *tok, important);
				}
			}
			else if (web_color::is_color(tok->c_str()))
			{
				add_parsed_property(atom_background_color, *tok, important);
			}
		}
	}

	void style::parse_short_font( const tstring& val, bool important )
	{
		add_parsed_property(atom_font_style,	_t("normal"),	important);
		add_parsed_property(atom_font_variant,	_t("normal"),	important);
		add_parsed_property(atom_font_weight,	_t("normal"),	important);
		add_parsed_property(atom_font_size,		_t("medium"),	important);
		add_parsed_property(atom_line_height,	_t("normal"),	important);

		string_vector tokens;
		split_string(val, tokens, _t(" "), _t(""), _t("\""));

		int idx = 0;
		bool was_normal = false;
		bool is_family = false;
		tstring font_family;
		for(string_vector::iterator tok = tokens.begin(); tok != tokens.end(); tok++)
		{
			idx = get_font_style(tok->c_str(), -1, -1);

			if(!is_family) {
				if(idx >= 0) {
					if(idx == 0 && !was_normal) {
						add_parsed_property(atom_font_weight,		*tok, important);
						add_parsed_property(atom_font_variant,		*tok, important);
						add_parsed_property(atom_font_style,		*tok, important);
					}
					else {
						add_parsed_property(atom_font_style,		*tok, important);
					}
				}
				else {
					if( get_font_weight(tok->c_str(), tok->length(), -1)>=0 ) {
						add_parsed_property(atom_font_weight,		*tok, important);
					}
					else {
						if(get_font_variant(tok->c_str(), tok->length(), -1)>=0 ) {
							add_parsed_property(atom_font_variant,	*tok, important);
						}
						else if( iswdigit((*tok)[0]) ) {
							string_vector szlh;
							split_string(*tok, szlh, _t("/"));

							if(szlh.size() == 1) {
								add_parsed_property(atom_font_size,	szlh[0], important);
							}
							else if(szlh.size() >= 2)
							{
								add_parsed_property(atom_font_size,	szlh[0], important);
								add_parsed_property(atom_line_height,	szlh[1], important);
							}
						}
						else {
							is_family = true;
							font_family += *tok;
						}
					}
				}
			}
			else {
				font_family += *tok;
			}
		}

		add_parsed_property( atom_font_family, font_family, important );
	}

	void style::add_parsed_property( atom name, const tstring& val, bool important )
	{
		bool is_valid = true;

		//todo: why here ???
		if( name==atom_white_space ) {
			if( get_white_space( val.c_str(), val.length(), -1 )<0 ) {
				is_valid = false;
			}
		}

		if (is_valid) {
			props_map::iterator prop = m_properties.find(name);
			if (prop != m_properties.end())
			{
				if (!prop->second.m_important || (important && prop->second.m_important))
				{
					prop->second.m_value = val;
					prop->second.m_important = important;
				}
			}
			else
			{
				m_properties[name] = property_value(val.c_str(), important);
			}
		}
	}

	/* no used
	void style::remove_property( atom name, bool important )
	{
		props_map::iterator prop = m_properties.find(name);
		if(prop != m_properties.end())
		{
			if( !prop->second.m_important || (important && prop->second.m_important) )
			{
				m_properties.erase(prop);
			}
		}
	}
	*/
}
