#include "html.h"
#include "stylesheet.h"
#include <algorithm>
#include "document.h"


void litehtml::css::parse_stylesheet(const tchar_t* str, const tchar_t* baseurl, const std::shared_ptr<document>& doc, const media_query_list::ptr& media)
{
	tstring text = str;

	// remove comments
	tstring::size_type c_start = text.find(_t("/*"));
	while(c_start != tstring::npos)
	{
		tstring::size_type c_end = text.find(_t("*/"), c_start + 2);
		text.erase(c_start, c_end - c_start + 2);
		c_start = text.find(_t("/*"));
	}

	tstring::size_type pos = text.find_first_not_of(_t(" \n\r\t"));
	while(pos != tstring::npos)
	{
		while(pos != tstring::npos && text[pos] == _t('@'))
		{
			tstring::size_type sPos = pos;
			pos = text.find_first_of(_t("{"), pos);
			if(pos != tstring::npos && text[pos] == _t('{'))
			{
				pos = find_close_bracket(text, pos, _t('{'), _t('}'));
			}
			if(pos != tstring::npos)
			{
				parse_atrule(text.substr(sPos, pos - sPos + 1), baseurl, doc, media);
			} else
			{
				parse_atrule(text.substr(sPos), baseurl, doc, media);
			}

			if(pos != tstring::npos)
			{
				pos = text.find_first_not_of(_t(" \n\r\t"), pos + 1);
			}
		}

		if(pos == tstring::npos)
		{
			break;
		}

		tstring::size_type style_start = text.find(_t("{"), pos);
		tstring::size_type style_end	= text.find(_t("}"), pos);
		if(style_start != tstring::npos && style_end != tstring::npos)
		{
			style::ptr st = std::make_shared<style>();
			st->add(text.substr(style_start + 1, style_end - style_start - 1).c_str(), baseurl);

			parse_selectors(text.substr(pos, style_start - pos), st, media);

			if(media && doc)
			{
				doc->add_media_list(media);
			}

			pos = style_end + 1;
		} else
		{
			pos = tstring::npos;
		}

		if(pos != tstring::npos)
		{
			pos = text.find_first_not_of(_t(" \n\r\t"), pos);
		}
	}
}

void litehtml::css::parse_css_url( const xstring& str, xstring& url )
{
	url.clear( );

	//	todo check that we get 'url...'
	const tchar_t*	p = str.c_str();
	while( *p && *p!='(' ) {
		p++;
	}

	if( *p=='(' ) {
		p++;

		// trim spaces
		while( *p==' ' || *p=='\t' )
			p++;

		// skip ' or "
		if( *p=='\'' || *p=='"' )
			p++;

		//	find )
		const tchar_t*	q = p+1;
		while( *q && *q!=')' ) {
			q++;
		}

		if( *q==')' ) {

			// trim spaces
			while( q>q && *q==' ' || *q=='\t' )
				q--;

			// skip ' or "
			if( q>p && (*q=='\'' || *q=='"') )
				q--;

			url.set( p, q-p );
		}
	}
}

bool litehtml::css::parse_selectors( const tstring& txt, const litehtml::style::ptr& styles, const media_query_list::ptr& media )
{
	tstring selector = txt;
	trim(selector);
	string_vector tokens;
	split_string(selector, tokens, _t(","));

	bool added_something = false;

	for(string_vector::iterator tok = tokens.begin(); tok != tokens.end(); tok++)
	{
		css_selector::ptr selector = std::make_shared<css_selector>(media);
		selector->m_style = styles;
		trim(*tok);
		if(selector->parse(*tok))
		{
			selector->calc_specificity();
			add_selector(selector);
			added_something = true;
		}
	}

	return added_something;
}

void litehtml::css::sort_selectors()
{
	std::sort(m_selectors.begin(), m_selectors.end(),
		 [](const css_selector::ptr& v1, const css_selector::ptr& v2)
		 {
			 return (*v1) < (*v2);
		 }
	);
}

void litehtml::css::parse_atrule(const tstring& text, const tchar_t* baseurl, const std::shared_ptr<document>& doc, const media_query_list::ptr& media)
{
	if(text.substr(0, 7) == _t("@import"))
	{
		int sPos = 7;
		tstring iStr;
		iStr = text.substr(sPos);
		if(iStr[iStr.length() - 1] == _t(';'))
		{
			iStr.erase(iStr.length() - 1);
		}
		trim(iStr);
		string_vector tokens;
		split_string(iStr, tokens, _t(" "), _t(""), _t("(\""));
		if(!tokens.empty())
		{
			// todo: do not use token.front.c_str() but token.front
			xstring url;
			parse_css_url( tokens.front().c_str(), url);

			if(url.empty()) {
				url = tokens.front().c_str();
			}

			tokens.erase(tokens.begin());
			if(doc)
			{
				document_container* doc_cont = doc->container();
				if(doc_cont)
				{
					xstring css_text;
					xstring css_baseurl;
					if(baseurl) {
						css_baseurl = baseurl;
					}

					doc_cont->import_css(css_text, url, css_baseurl);
					if(!css_text.empty())
					{
						media_query_list::ptr new_media = media;
						if(!tokens.empty())
						{
							tstring media_str;
							for(string_vector::iterator iter = tokens.begin(); iter != tokens.end(); iter++)
							{
								if(iter != tokens.begin())
								{
									media_str += _t(" ");
								}
								media_str += (*iter);
							}
							new_media = media_query_list::create_from_string(media_str, doc);
							if(!new_media)
							{
								new_media = media;
							}
						}
						parse_stylesheet(css_text.c_str(), css_baseurl.c_str(), doc, new_media);
					}
				}
			}
		}
	} else if(text.substr(0, 6) == _t("@media"))
	{
		tstring::size_type b1 = text.find_first_of(_t('{'));
		tstring::size_type b2 = text.find_last_of(_t('}'));
		if(b1 != tstring::npos)
		{
			tstring media_type = text.substr(6, b1 - 6);
			trim(media_type);
			media_query_list::ptr new_media = media_query_list::create_from_string(media_type, doc);

			tstring media_style;
			if(b2 != tstring::npos)
			{
				media_style = text.substr(b1 + 1, b2 - b1 - 1);
			} else
			{
				media_style = text.substr(b1 + 1);
			}

			parse_stylesheet(media_style.c_str(), baseurl, doc, new_media);
		}
	}
}