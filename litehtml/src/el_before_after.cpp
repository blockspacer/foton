#include "html.h"
#include "el_before_after.h"
#include "el_text.h"
#include "el_space.h"
#include "el_image.h"

litehtml::el_before_after_base::el_before_after_base(const std::shared_ptr<litehtml::document>& doc, bool before) : html_tag(doc)
{
	if(before)
	{
		set_tagName( atom___before /*_t("::before")*/ );
	} else
	{
		set_tagName( atom___after /*_t("::after")*/ );
	}
}

litehtml::el_before_after_base::~el_before_after_base()
{

}

void litehtml::el_before_after_base::add_style(const litehtml::style& st)
{
	html_tag::add_style(st);

	tstring content = get_style_property(atom_content, false, _t(""));
	if(!content.empty())
	{
		int idx = value_index(content.c_str(), content_property_string);
		if(idx < 0)
		{
			tstring fnc;
			tstring::size_type i = 0;
			while(i < content.length() && i != tstring::npos)
			{
				if(content.at(i) == _t('"'))
				{
					fnc.clear();
					i++;
					tstring::size_type pos = content.find(_t('"'), i);
					tstring txt;
					if(pos == tstring::npos)
					{
						txt = content.substr(i);
						i = tstring::npos;
					} else
					{
						txt = content.substr(i, pos - i);
						i = pos + 1;
					}
					add_text(txt);
				} else if(content.at(i) == _t('('))
				{
					i++;
					litehtml::trim(fnc);
					litehtml::lcase(fnc);
					tstring::size_type pos = content.find(_t(')'), i);
					tstring params;
					if(pos == tstring::npos)
					{
						params = content.substr(i);
						i = tstring::npos;
					} else
					{
						params = content.substr(i, pos - i);
						i = pos + 1;
					}
					add_function(fnc, params);
					fnc.clear();
				} else
				{
					fnc += content.at(i);
					i++;
				}
			}
		}
	}
}

void litehtml::el_before_after_base::add_text( const tstring& txt )
{
	tstring word;
	tstring esc;
	for(tstring::size_type i = 0; i < txt.length(); i++)
	{
		if( (txt.at(i) == _t(' ')) || (txt.at(i) == _t('\t')) || (txt.at(i) == _t('\\') && !esc.empty()) )
		{
			if(esc.empty())
			{
				if(!word.empty())
				{
					element::ptr el = std::make_shared<el_text>(word.c_str(), get_document());
					appendChild(el);
					word.clear();
				}

				element::ptr el = std::make_shared<el_space>(txt.substr(i, 1).c_str(), get_document());
				appendChild(el);
			} else
			{
				word += convert_escape(esc.c_str() + 1);
				esc.clear();
				if(txt.at(i) == _t('\\'))
				{
					esc += txt.at(i);
				}
			}
		} else
		{
			if(!esc.empty() || txt.at(i) == _t('\\'))
			{
				esc += txt.at(i);
			} else
			{
				word += txt.at(i);
			}
		}
	}

	if(!esc.empty())
	{
		word += convert_escape(esc.c_str() + 1);
	}
	if(!word.empty())
	{
		element::ptr el = std::make_shared<el_text>(word.c_str(), get_document());
		appendChild(el);
		word.clear();
	}
}

void litehtml::el_before_after_base::add_function( const tstring& fnc, const tstring& params )
{
	int idx = value_index(fnc.c_str(), _t("attr;counter;url"));
	switch(idx)
	{
	// attr
	case 0:
		{
			element::ptr el_parent = parent();
			if (el_parent)
			{
				tstring p_name = params;
				trim(p_name);
				//lcase(p_name);
				atom a_name = atom_create(p_name.c_str() );
				//__asm int 3;	//<eco:

				const tchar_t* attr_value = el_parent->get_attr(a_name);
				if (attr_value)
				{
					add_text(attr_value);
				}
			}
		}
		break;
	// counter
	case 1:
		break;
	// url
	case 2:
		{
			tstring p_url = params;
			trim(p_url);
			if(!p_url.empty())
			{
				if(p_url.at(0) == _t('\'') || p_url.at(0) == _t('\"'))
				{
					p_url.erase(0, 1);
				}
			}
			if(!p_url.empty())
			{
				if(p_url.at(p_url.length() - 1) == _t('\'') || p_url.at(p_url.length() - 1) == _t('\"'))
				{
					p_url.erase(p_url.length() - 1, 1);
				}
			}
			if(!p_url.empty())
			{
				element::ptr el = std::make_shared<el_image>(get_document());
				el->set_attr(atom_src, p_url.c_str());
				el->set_attr(atom_style, _t("display:inline-block"));
				el->set_tagName( atom_img /*_t("img")*/);
				appendChild(el);
				el->parse_attributes();
			}
		}
		break;
	}
}

litehtml::tchar_t litehtml::el_before_after_base::convert_escape( const tchar_t* txt )
{
	tchar_t* sss = 0;
	return (tchar_t) t_strtol(txt, &sss, 16);
}

void litehtml::el_before_after_base::apply_stylesheet( const litehtml::css& /*stylesheet*/ )
{

}
