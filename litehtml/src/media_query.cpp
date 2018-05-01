#include "html.h"
#include "media_query.h"
#include "document.h"

namespace litehtml {
	media_query::media_query()
	{
		m_media_type	= media_type_all;
		m_not			= false;
	}

	media_query::media_query( const media_query& val )
	{
		m_not			= val.m_not;
		m_expressions	= val.m_expressions;
		m_media_type	= val.m_media_type;
	}

	media_query::ptr media_query::create_from_string( const tstring& str, const std::shared_ptr<document>& doc )
	{
		media_query::ptr query = std::make_shared<media_query>();
		string_vector tokens;
		split_string( str, tokens, _t( " \t\r\n" ), _t( "" ), _t( "(" ) );

		for( string_vector::iterator tok = tokens.begin(); tok != tokens.end(); tok++ ) {
			if( ( *tok ) == _t( "not" ) ) {
				query->m_not = true;
			}
			else if( tok->at( 0 ) == _t( '(' ) ) {
				tok->erase( 0, 1 );

				if( tok->at( tok->length() - 1 ) == _t( ')' ) ) {
					tok->erase( tok->length() - 1, 1 );
				}

				media_query_expression expr;
				string_vector expr_tokens;
				split_string( ( *tok ), expr_tokens, _t( ":" ) );

				if( !expr_tokens.empty() ) {
					trim( expr_tokens[0] );
					expr.feature = ( media_feature )get_media_feature( expr_tokens[0].c_str(), -1, media_feature_none );

					if( expr.feature != media_feature_none ) {
						if( expr_tokens.size() == 1 ) {
							expr.check_as_bool = true;
						}
						else {
							trim( expr_tokens[1] );
							expr.check_as_bool = false;

							if( expr.feature == media_feature_orientation ) {
								expr.val = get_media_orientation( expr_tokens[1].c_str(), -1, media_orientation_landscape );
							}
							else {
								tstring::size_type slash_pos = expr_tokens[1].find( _t( '/' ) );

								if( slash_pos != tstring::npos ) {
									tstring val1 = expr_tokens[1].substr( 0, slash_pos );
									tstring val2 = expr_tokens[1].substr( slash_pos + 1 );
									trim( val1 );
									trim( val2 );
									expr.val = t_atoi( val1.c_str() );
									expr.val2 = t_atoi( val2.c_str() );
								}
								else {
									css_length length;
									length.fromString( expr_tokens[1] );

									if( length.units() == css_units_dpcm ) {
										expr.val = ( int )( length.val() * 2.54 );
									}
									else if( length.units() == css_units_dpi ) {
										expr.val = ( int )( length.val() * 2.54 );
									}
									else {
										if( doc ) {
											doc->cvt_units( length, doc->container()->get_default_font_size() );
										}

										expr.val = ( int ) length.val();
									}
								}
							}
						}

						query->m_expressions.push_back( expr );
					}
				}
			}
			else {
				query->m_media_type = get_media_type( ( *tok ), -1, media_type_all );
			}
		}

		return query;
	}

	bool media_query::check( const media_features& features ) const
	{
		bool res = false;

		if( m_media_type == media_type_all || m_media_type == features.type ) {
			res = true;

			for( media_query_expression::vector::const_iterator expr = m_expressions.begin(); expr != m_expressions.end() && res; expr++ ) {
				if( !expr->check( features ) ) {
					res = false;
				}
			}
		}

		if( m_not ) {
			res = !res;
		}

		return res;
	}

	//////////////////////////////////////////////////////////////////////////

	media_query_list::ptr media_query_list::create_from_string( const tstring& str, const std::shared_ptr<document>& doc )
	{
		media_query_list::ptr list = std::make_shared<media_query_list>();
		string_vector tokens;
		split_string( str, tokens, _t( "," ) );

		for( string_vector::iterator tok = tokens.begin(); tok != tokens.end(); tok++ ) {
			trim( *tok );
			lcase( *tok );
			media_query::ptr query = media_query::create_from_string( *tok, doc );

			if( query ) {
				list->m_queries.push_back( query );
			}
		}

		if( list->m_queries.empty() ) {
			list = 0;
		}

		return list;
	}

	bool media_query_list::apply_media_features( const media_features& features )
	{
		bool apply = false;

		for( media_query::vector::iterator iter = m_queries.begin(); iter != m_queries.end() && !apply; iter++ ) {
			if( ( *iter )->check( features ) ) {
				apply = true;
			}
		}

		bool ret = ( apply != m_is_used );
		m_is_used = apply;
		return ret;
	}

	bool media_query_expression::check( const media_features& features ) const
	{
		switch( feature ) {
			case media_feature_width:
				if( check_as_bool ) {
					return ( features.width != 0 );
				}
				else if( features.width == val ) {
					return true;
				}

				break;

			case media_feature_min_width:
				if( features.width >= val ) {
					return true;
				}

				break;

			case media_feature_max_width:
				if( features.width <= val ) {
					return true;
				}

				break;

			case media_feature_height:
				if( check_as_bool ) {
					return ( features.height != 0 );
				}
				else if( features.height == val ) {
					return true;
				}

				break;

			case media_feature_min_height:
				if( features.height >= val ) {
					return true;
				}

				break;

			case media_feature_max_height:
				if( features.height <= val ) {
					return true;
				}

				break;

			case media_feature_device_width:
				if( check_as_bool ) {
					return ( features.device_width != 0 );
				}
				else if( features.device_width == val ) {
					return true;
				}

				break;

			case media_feature_min_device_width:
				if( features.device_width >= val ) {
					return true;
				}

				break;

			case media_feature_max_device_width:
				if( features.device_width <= val ) {
					return true;
				}

				break;

			case media_feature_device_height:
				if( check_as_bool ) {
					return ( features.device_height != 0 );
				}
				else if( features.device_height == val ) {
					return true;
				}

				break;

			case media_feature_min_device_height:
				if( features.device_height <= val ) {
					return true;
				}

				break;

			case media_feature_max_device_height:
				if( features.device_height <= val ) {
					return true;
				}

				break;

			case media_feature_orientation:
				if( features.height >= features.width ) {
					if( val == media_orientation_portrait ) {
						return true;
					}
				}
				else {
					if( val == media_orientation_landscape ) {
						return true;
					}
				}

				break;

			case media_feature_aspect_ratio:
				if( features.height && val2 ) {
					int ratio_this = round_d( ( double ) val / ( double ) val2 * 100 );
					int ratio_feat = round_d( ( double ) features.width / ( double ) features.height * 100.0 );

					if( ratio_this == ratio_feat ) {
						return true;
					}
				}

				break;

			case media_feature_min_aspect_ratio:
				if( features.height && val2 ) {
					int ratio_this = round_d( ( double ) val / ( double ) val2 * 100 );
					int ratio_feat = round_d( ( double ) features.width / ( double ) features.height * 100.0 );

					if( ratio_feat >= ratio_this ) {
						return true;
					}
				}

				break;

			case media_feature_max_aspect_ratio:
				if( features.height && val2 ) {
					int ratio_this = round_d( ( double ) val / ( double ) val2 * 100 );
					int ratio_feat = round_d( ( double ) features.width / ( double ) features.height * 100.0 );

					if( ratio_feat <= ratio_this ) {
						return true;
					}
				}

				break;

			case media_feature_device_aspect_ratio:
				if( features.device_height && val2 ) {
					int ratio_this = round_d( ( double ) val / ( double ) val2 * 100 );
					int ratio_feat = round_d( ( double ) features.device_width / ( double ) features.device_height * 100.0 );

					if( ratio_feat == ratio_this ) {
						return true;
					}
				}

				break;

			case media_feature_min_device_aspect_ratio:
				if( features.device_height && val2 ) {
					int ratio_this = round_d( ( double ) val / ( double ) val2 * 100 );
					int ratio_feat = round_d( ( double ) features.device_width / ( double ) features.device_height * 100.0 );

					if( ratio_feat >= ratio_this ) {
						return true;
					}
				}

				break;

			case media_feature_max_device_aspect_ratio:
				if( features.device_height && val2 ) {
					int ratio_this = round_d( ( double ) val / ( double ) val2 * 100 );
					int ratio_feat = round_d( ( double ) features.device_width / ( double ) features.device_height * 100.0 );

					if( ratio_feat <= ratio_this ) {
						return true;
					}
				}

				break;

			case media_feature_color:
				if( check_as_bool ) {
					return ( features.color != 0 );
				}
				else if( features.color == val ) {
					return true;
				}

				break;

			case media_feature_min_color:
				if( features.color >= val ) {
					return true;
				}

				break;

			case media_feature_max_color:
				if( features.color <= val ) {
					return true;
				}

				break;

			case media_feature_color_index:
				if( check_as_bool ) {
					return ( features.color_index != 0 );
				}
				else if( features.color_index == val ) {
					return true;
				}

				break;

			case media_feature_min_color_index:
				if( features.color_index >= val ) {
					return true;
				}

				break;

			case media_feature_max_color_index:
				if( features.color_index <= val ) {
					return true;
				}

				break;

			case media_feature_monochrome:
				if( check_as_bool ) {
					return ( features.monochrome != 0 );
				}
				else if( features.monochrome == val ) {
					return true;
				}

				break;

			case media_feature_min_monochrome:
				if( features.monochrome >= val ) {
					return true;
				}

				break;

			case media_feature_max_monochrome:
				if( features.monochrome <= val ) {
					return true;
				}

				break;

			case media_feature_resolution:
				if( features.resolution == val ) {
					return true;
				}

				break;

			case media_feature_min_resolution:
				if( features.resolution >= val ) {
					return true;
				}

				break;

			case media_feature_max_resolution:
				if( features.resolution <= val ) {
					return true;
				}

				break;

			default:
				return false;
		}

		return false;
	}
}
