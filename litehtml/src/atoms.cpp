#include "html.h"

#include <qDebug>

namespace litehtml {


	int GT[] = {
		1, -291, 1, 0, -8, 1, 1, 0, 0, -19, -25, -26, -27, 2, -30, 0, -47, 5, -54, 6,
		-58, -70, -71, -76, -79, -84, 0, 1, 0, -106, -110, 0, 5, 0, 0, 0, 0, 1, 0, -333,
		0, -169, -180, -181, 1, 2, 6, 1, -182, 1, 2, 3, 1, 5, -185, -190, 0, 6, 0, 0,
		-205, 1, 1, 0, -207, 1, 5, -215, 2, -222, -267, 0, 0, 0, 0, 0, 0, 5, -303, -314,
		0, -316, 0, 1, -164, -323, 0, 2, 8, 5, 7, 0, 0, 0, 0, 9, -312, 0, 0, 0,
		0, 0, 3, 0, 1, 0, 0, -287, 0, 0, 8, -272, 1, -268, 3, -257, 0, -244, -243, -242,
		0, -236, -232, -225, 0, 0, 7, 0, -211, 0, 1, 0, 1, -203, -201, 2, 1, -192, 1, -187,
		8, 2, 1, 1, 0, 0, 8, -167, 1, -161, 0, 0, 0, 1, 0, 1, -138, -137, 0, 0,
		-127, 1, -125, 0, 0, -118, -111, 1, 0, -105, 0, 0, 0, 1, 0, -81, 0, 0, 0, 0,
		0, -68, -66, 0, 0, 5, 0, 0, -51, 3, 0, 0, 0, -38, 0, 0, 8, 0, 3, 0,
		2, 0, -16, 1, 5, -4, -3, 0, 0, 0, 0, -1, -18, -20, 2, 0, 6, -35, 1, 0,
		-45, 0, 0, 0, 0, -46, 0, 18, -56, 0, 0, -59, -64, 1, 0, 0, 2, 4, -77, 0,
		0, 2, -83, 0, 0, 0, 0, 0, -90, 0, 2, -100, 4, -120, 1, -128, 0, -131, -145, -150,
		-153, -157, 7, 0, 0, -174, 0, -206, 0, -208, 0, -214, -216, 12, -239, 0, -246, -281, 1, -288,
		-290, -293, -301, 3, 0, -305, 0, 0, 0, 0, 0, -306, -311, -315, 0, 7, -317, 0, 2, 0,
		0, 2, 0, 0, 0, 0, -321, 5, 0, -296, -283, -277, 1, 0, -270, 1, 0, 8, 3, -196,
		-193, 0, -168, -148, -141, 2, -126, -121, 0, 4, 1, -94, -44, 2, 0, 0, 9, -6,
	};
	int VT[] = {
		-236, -17, -244, -75, -164, -311, -226, -174, -34, -308, -232, -92, -172, -94, -225, -237, -8, -296, -119, -270,
		-213, -284, -278, -313, -242, -285, -326, -59, -109, -214, -152, -202, -281, -180, -51, -23, -292, -22, -222, -286,
		-221, -253, -29, -30, -135, -233, -206, -306, -41, -50, -95, -104, -230, -40, -256, -247, -188, -167, -178, -38,
		-142, -72, -32, -89, -99, -273, -121, -117, -37, -334, -7, -262, -151, -275, -241, -332, -251, -245, -111, -97,
		-319, -25, -274, -112, -183, -191, -132, -18, -197, -128, -227, -235, -291, -314, -42, -56, -297, -123, -302, -78,
		-147, -10, -43, -205, -204, -52, -139, -316, -127, -220, -63, -74, -309, -294, -198, -162, -80, -260, -5, -130,
		-54, -210, -196, -265, -195, -150, -186, -299, -36, -199, -113, -62, -200, -116, -243, -131, -161, -11, -318, -55,
		-90, -143, -3, -49, -184, -126, -330, -264, -53, -33, -69, -305, -208, -219, -231, -136, -239, -301, -79, -87,
		-325, -295, -129, -252, -163, -240, -4, -181, -323, -64, -187, -224, -263, -86, -228, -321, -102, -58, -137, -338,
		-14, -320, -60, -153, -283, -267, -159, -234, -329, -203, -209, -157, -101, -327, -31, -71, -212, -140, -149, -293,
		-91, -124, -148, -45, -168, -322, -215, -207, -277, -289, -93, -9, -259, -324, -21, -20, -288, -176, -298, -337,
		-182, -125, -290, -280, -67, -279, -249, -134, -108, -2, -331, -169, -336, -254, -12, -192, -145, -238, -66, -13,
		-114, -47, -48, -268, -189, -81, -156, -155, -158, -84, -106, -65, -282, -255, -27, -138, -175, -44, -46, -85,
		-19, -82, -1, -303, -300, -16, -201, -269, -26, -173, -118, -261, -98, -317, -216, -287, -218, -333, -70, -179,
		-68, -257, -166, -110, -15, -154, -122, -103, -315, -229, -258, -310, -88, -335, -223, -304, -248, -170, -276, -57,
		-24, -146, -107, -73, -144, -307, -246, -266, -105, -141, -76, -35, -193, -165, -77, -312, -185, -100, -61, -120,
		-328, -6, -194, -160, -39, -115, -272, -96, -190, -83, -133, -217, -177, -211, -28, -271, -250, -171,
	};

static const char input[] =
		"*\0-xx-border-spacing-x\0-xx-border-spacing-y\0::after\0::before\0a\0abbr\0absolute\0address\0after\0" \
		"align\0align-content\0align-items\0align-self\0alt\0area\0article\0aside\0audio\0auto\0" \
		"b\0background\0background-attachment\0background-baseurl\0background-clip\0background-color\0background-image\0background-origin\0background-position\0background-position-x\0" \
		"background-position-y\0background-repeat\0background-size\0base\0bdi\0bdo\0before\0bgcolor\0big\0block\0" \
		"blockquote\0body\0border\0border-bottom\0border-bottom-color\0border-bottom-left-radius\0border-bottom-left-radius-x\0border-bottom-left-radius-y\0border-bottom-right-radius\0border-bottom-right-radius-x\0" \
		"border-bottom-right-radius-y\0border-bottom-style\0border-bottom-width\0border-box\0border-collapse\0border-color\0border-left\0border-left-color\0border-left-style\0border-left-width\0" \
		"border-radius\0border-radius-x\0border-radius-y\0border-right\0border-right-color\0border-right-style\0border-right-width\0border-spacing\0border-style\0border-top\0" \
		"border-top-color\0border-top-left-radius\0border-top-left-radius-x\0border-top-left-radius-y\0border-top-right-radius\0border-top-right-radius-x\0border-top-right-radius-y\0border-top-style\0border-top-width\0border-width\0" \
		"bottom\0box-shadow\0box-sizing\0br\0button\0canvas\0capitalize\0caption\0cellpadding\0cellspacing\0" \
		"center\0charset\0checked\0circle\0cite\0class\0clear\0code\0col\0colgroup\0" \
		"color\0cols\0colspan\0column-count\0column-gap\0column-width\0content\0content-boxrepeat\0controls\0cursor\0" \
		"cx\0cy\0datalist\0datetime\0dd\0del\0details\0dfn\0dialog\0dir\0" \
		"direction\0disabled\0display\0div\0dl\0dt\0em\0embed\0face\0fieldset\0" \
		"figcaption\0figure\0fill\0fixed\0flex\0flex-basis\0flex-direction\0flex-flow\0flex-grow\0flex-wrap\0" \
		"float\0font\0font-face\0font-family\0font-size\0font-style\0font-variant\0font-weight\0footer\0for\0" \
		"form\0frame\0frameset\0h1\0h2\0h3\0h4\0h5\0h6\0halign\0" \
		"head\0header\0height\0hgroup\0hr\0href\0html\0i\0id\0iframe\0" \
		"img\0inherit\0inline\0inline-block\0inline-table\0input\0ins\0isindex\0justify-content\0kbd\0" \
		"keygen\0label\0lang\0layer\0left\0legend\0letter-spacing\0li\0line-height\0line-through\0" \
		"link\0list\0list-item\0list-style\0list-style-baseurl\0list-style-image\0list-style-position\0list-style-type\0listing\0lowercase\0" \
		"main\0map\0margin\0margin-bottom\0margin-left\0margin-right\0margin-top\0mark\0marquee\0max\0" \
		"max-height\0max-width\0media\0menu\0meta\0meter\0min\0min-height\0min-width\0name\0" \
		"nav\0no-repeat\0nobr\0noframes\0none\0nowrap\0object\0object-fit\0ol\0opacity\0" \
		"optgroup\0option\0outline\0outline-offset\0output\0overflow\0overflow-x\0overflow-y\0overline\0p\0" \
		"padding\0padding-bottom\0padding-box\0padding-left\0padding-right\0padding-top\0param\0placeholder\0plaintext\0pointer-events\0" \
		"position\0pre\0progress\0pseudo\0pseudo-el\0q\0r\0readonly\0rel\0relative\0" \
		"repeat-x\0repeat-y\0resize\0right\0role\0rows\0rowspan\0rp\0rt\0ruby\0" \
		"s\0samp\0script\0section\0select\0size\0small\0span\0src\0static\0" \
		"strike\0strong\0style\0sub\0summary\0sup\0svg\0tabindex\0table\0table-caption\0" \
		"table-cell\0table-column\0table-column-group\0table-footer-group\0table-header-group\0table-layout\0table-row\0table-row-group\0tbody\0td\0" \
		"template\0text-align\0text-decoration\0text-indent\0text-overflow\0text-rendering\0text-shadow\0text-transform\0textarea\0tfoot\0" \
		"th\0thead\0time\0title\0top\0tr\0transparent\0tt\0type\0u\0" \
		"ul\0underline\0unicode-bidi\0uppercase\0user_drag\0valign\0value\0var\0vertical-align\0video\0" \
		"visibility\0white-space\0width\0word-break\0word-spacing\0word-wrap\0xmp\0z-index\0zoom\0";

	uint32_t	RH[] = {
		0x50c5d35,0x3fa6a53e,0x3fa6a53f,0x1a021dcd,0x3ce37cd8,0x50c5d7e,0x38de710c,0x1b081786,0x621e09cf,0x1f545b8d, \
		0x360ea6cc,0x386398a4,0x2e365e5f,0x515d67c7,0x419c2c76,0x5bb60646,0x4b5df355,0xf2b4763,0x3fc40859,0x4cc737c4, \
		0x50c5d7d,0x5999a081,0x6892511d,0x70982cb0,0x72c39624,0x1cc58c7f,0x3eaf3bd5,0x31bfdc34,0x161a0f87,0x4824eb00, \
		0x4824eb01,0x91ba837,0x15eb1451,0x4cf9b596,0x1b99b1f0,0x1b99b1f6,0x440dfc98,0xc7f658d,0x2699c28d,0x6e8347b0, \
		0x123ab90c,0x6dfe676d,0x79acc563,0x42565c2d,0x7c9c27eb,0x30336510,0x5a5df5df,0x5a5df5de,0x33b890d3,0x7a5dba34, \
		0x7a5dba35,0x77aaeed1,0x5244908e,0x580317,0x64ec4701,0x1f0a1231,0x1f07c15f,0x352dc8d,0x1dce8ec7,0x5541b384, \
		0x2ee58ab6,0x66ace705,0x66ace704,0x64d46c62,0x1e9e759c,0x93b7f66,0x33f92839,0x4a4e5447,0x1b4e332b,0x21e905, \
		0x178ae963,0x4bdbd328,0x531a7437,0x531a7436,0x393b378b,0x6dd849cc,0x6dd849cd,0x7db75139,0x15fc8f6,0x202fb278, \
		0x6864ba60,0x49721765,0x460ea43,0x6f772bb5,0x3a42c455,0x3c6bb669,0x7acef57f,0x67afb8b5,0x4adcab68,0x8c95528, \
		0x76822486,0x6bb7696f,0x2a6aa4d8,0xe545add,0x67ca2d8c,0x5e8533db,0x688fd0b0,0x53c5915a,0x4097ad2d,0x1ecc0daa, \
		0x301748da,0x5bc59da4,0x6aed9d81,0x76411875,0x436a7ddc,0x2719f4c4,0x1d9253d8,0x6b5f86b5,0x1c7eaa23,0x4f507a4f, \
		0x6e772a4c,0x6e772a4d,0x42064895,0x5bb409c6,0x6d7728c5,0x16952ce0,0x2734ea4d,0x1995312b,0x49a80b09,0x1a9532aa, \
		0xffcd08,0x41a03855,0x35fe47dd,0x1a9532ae,0x6d7728cd,0x6d7728d5,0x6c772763,0x2319d828,0x20c00292,0x57a05c5d, \
		0x5a1edd61,0x1d46dc55,0x35ac9c22,0x299bc2f9,0x2ab95688,0x57f88aad,0x115c6b58,0x27418fe9,0x5ae09254,0x165df50d, \
		0x4edcd7a9,0x33b11622,0x61e81028,0x7bb2230f,0x5c152a40,0x23ecbfa6,0x12f85836,0x3216547,0x1a1dab94,0x408f5e0e, \
		0x2fb11067,0x6e2f4e8c,0x1d13f84,0x69772264,0x69772267,0x69772266,0x69772261,0x69772260,0x69772263,0x5ba0e07a, \
		0x4d887e07,0x3a199652,0x4c47d5c0,0x160b434,0x69772227,0x3170f966,0x5d6233d6,0x50c5d76,0x687720a6,0x3645c761, \
		0x2288731a,0x513749fa,0x5c488032,0x4b96d332,0xcd1c719,0x561edfff,0x1f886eb7,0x7b3bf3e3,0x280137db,0x4b8436be, \
		0x3f4d41ec,0x61867a7d,0x28346585,0x71482af0,0x3eb432,0x5ecb940a,0x4db06ca5,0x65771c60,0x1ffbfe09,0x307b34a3, \
		0x820aba1,0xd20b3dd,0x1c3e93ef,0x27c730a1,0x19423a50,0x62469475,0x4cbfdea7,0x344dc69e,0x780a378b,0x7174a998, \
		0x3c76e6ba,0x3e7e1245,0x4a2ae8d3,0x6ea69dbd,0x69f9304f,0x6904fe12,0x7fe019b5,0x2776c5ae,0x244300cf,0x3e7e124d, \
		0x9273d87,0xd882a2e,0x3b4d62db,0x1b6c25f0,0x316c4846,0xf75bf94,0x467e1ea3,0x274603a5,0x55a45c38,0x2f8b3bf4, \
		0x167b94d0,0x2a2acb74,0x28a33500,0x63c2a2b2,0x2ca33bdb,0x4543a450,0x361de6b4,0x65b62076,0x6277173c,0x212b5824, \
		0x26fb0055,0x2b9d6e4e,0x6e6ce7a9,0x4d7f6d37,0x498d6ac6,0x56628157,0x2fb85570,0x2fb85571,0x62be06bd,0x50c5d6f, \
		0x440b8034,0x57989a4,0x273acdcc,0x780c617e,0x1d0beb1d,0x2770e7de,0x4a620ba4,0x5c4fce4c,0x3451334e,0x12a70bd2, \
		0x5e19d274,0x3d7754b8,0x126568e0,0x4937e75b,0x2f528adf,0x50c5d6e,0x50c5d6d,0x12d23a4f,0x3a72d29a,0x1c020035, \
		0x5a96bbd7,0x5a96bbd6,0x21efca8d,0x6816f049,0x78d05a29,0x13d084be,0x2361cdc3,0x5f7712e7,0x5f7712e3,0x769a3d55, \
		0x50c5d6c,0x30e998a6,0x23177498,0x291a199e,0x556391b5,0x23fe9f72,0x6e13411a,0x39100929,0x2b707ca1,0x540353f7, \
		0x7502dd07,0x51f39bd8,0x4a5356c0,0x26707481,0x8ab57a3,0x26707493,0x27707611,0x34d5d75e,0x7a1e796b,0xacbc652, \
		0x2ace1118,0x56026dc2,0x51573238,0x521025e3,0x2505eb1d,0xae95b3e,0x57ec9070,0x48bc2dba,0x3787cc5b,0x5d770f15, \
		0x4529f96f,0x4de7210c,0xe7f89e3,0x45adcd97,0x20e2e97,0x16ae8387,0x58bde79d,0x19ddf2fb,0x6a111f7b,0x53744fa7, \
		0x5d770f19,0x529ea245,0x1e3669da,0x5ce0e62d,0x406ccc4a,0x5d770f03,0x188403f9,0x5d770f05,0x310f785d,0x50c5d6a, \
		0x5c770db2,0x56b9047b,0x153b1671,0xc43ca2f,0xca8a5bd,0x2bf928f0,0x28ed63dc,0x2668266c,0x7e45286b,0x31cc1af6, \
		0x4e96dd39,0xffca4e9,0x182e64eb,0x4924905b,0x7f911e1d,0x41ce8b2a,0x1e639c28,0x243ce5de, \
};

	struct	symbol
	{
		void*		value;
		uint16_t	len;
		uint32_t	hash;
		symbol*		next;

		symbol( const tchar_t* s, uint16_t ln, uint32_t h ) {
			len = ln;
			hash = h;
			next = NULL;
			value = malloc( ln );
			memcpy( value, s, ln );
		}
	};

	struct	symbol_table
	{
		symbol**	table;
		uint32_t	modulo;
	};

	symbol_table	g_symbols = {NULL, 0 };

	void	atom_init( )
	{
		g_symbols.modulo = 797;
		g_symbols.table = (symbol**)malloc( g_symbols.modulo * sizeof(symbol*) );
		memset( g_symbols.table, 0, g_symbols.modulo * sizeof(symbol*) );
	}

	static	uint32_t	hash( uint32_t d, const tchar_t* text ) {

		if(d == 0) {
			d = 0x811c9dc5;
		}

		while( *text ) {
			// multiply by the 32 bit FNV magic prime mod 2^32
			d += (d << 1) + (d << 4) + (d << 7) + (d << 8) + (d << 24);
			// xor the bottom with the current octet
			d ^= *text;	//tolower(*text);
			text++;
		}

		return d & 0x7fffffff;
	}

	atom atom_create( const tchar_t* text, bool create_it )
	{
		if( *text==0 ) {
			return atom_null;
		}

		//	lowercase it
		char	lowercase[128];
		int		len = 0;
		while( len<countof(lowercase)-1 && *text ) {
			lowercase[len] = tolower(*text);
			len++;
			text++;
		}

		lowercase[len] = 0;

//		if( strcmp(lowercase,"section)")==0 ) {
//			__asm int 3;
//		}

		//	looking for our known elements (fast hash)
		int			a;
		uint32_t	h = hash(0,lowercase);
		int			d = GT[ h % countof(GT) ];
		if( d<0 )	{
			a = VT[ 0-d-1 ];
		}
		else {
			a = VT[hash(d,lowercase) % countof(VT)];
		}

		//	known element ?
		if( h==RH[0-1-a] ) {
			return (atom)a;
		}

		if( !create_it ) {
			return atom_null;
		}

		// no, add it to our temp table
		if( !g_symbols.table ) {
			atom_init( );
		}

		h	&= 0x7FFFFFFF;

		uint32_t	modulo = h % g_symbols.modulo;
		symbol*		sym	= g_symbols.table[modulo];

		while( sym != NULL ) {
			if (sym->hash==h && sym->len==len ) {
				if (memcmp(sym->value, lowercase, len) == 0)
					break;
			}

			sym = sym->next;
		}

		if (sym == NULL) {

			qDebug() << lowercase << ",";

			sym = new symbol( lowercase, len, h );
			sym->next = g_symbols.table[modulo];
			g_symbols.table[modulo] = sym;
		}

		return (atom)h;
	}

	tstring atom_get( const tchar_t* v ) {

		/*
		int		d = GT[ hash(0,text) % countof(G) ];
		if( d<0 )	{
			return input + VT[ 0-d-1 ];
		}

		return input + VT[hash(d,text) % countof(V)];

		//symbol*	x = (symbol*)v;
		//return	std::string((const char*)x->value,x->len);
		*/
		return "";
	}
}