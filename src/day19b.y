%{
extern int yylex();
int yyerror(const char* s);
%}
%start _0
%glr-parser
%%
_101: _64 _33  | _14 _121  ;
_130: _14 _96  ;
_117: _64 _14  | _14 _14  ;
_48: _78 _14  | _102 _64  ;
_107: _14 _14  | _64 _64  ;
_56: _14 _43  | _64 _104  ;
_5: _107 _14  | _106 _64  ;
_67: _14 _44  | _64 _94  ;
_100: _14 _39  | _64 _103  ;
_32: _14 _96  | _64 _16  ;
_25: _64 _107  | _14 _96  ;
_37: _64 _108  | _14 _30  ;
_42: _95 _14  | _27 _64  ;
_113: _14 _79  ;
_13: _14 _136  | _64 _116  ;
_83: _64 _62  | _14 _2  ;
_105: _14 _108  | _64 _117  ;
_28: _14 _133  | _64 _35  ;
_66: _64 _106  | _14 _96  ;
_41: _64 _85  | _14 _60  ;
_88: _16 _64  | _108 _14  ;
_49: _58 _64  | _14 _14  ;
_111: _52 _64  | _114 _14  ;
_3: _14 _124  | _64 _119  ;
_82: _14 _17  | _64 _47  ;
_116: _49 _14  | _98 _64  ;
_91: _58 _6  ;
_127: _106 _64  | _30 _14  ;
_125: _56 _14  | _100 _64  ;
_23: _64 _61  | _14 _5  ;
_31: _64 _9  | _14 _109  ;
_86: _125 _14  | _99 _64  ;
_35: _64 _49  | _14 _106  ;
_10: _64 _25  | _14 _84  ;
_85: _92 _64  | _132 _14  ;
_46: _14 _18  | _64 _82  ;
_128: _78 _14  | _79 _64  ;
_79: _64 _64  | _14 _64  ;
_29: _135 _14  | _68 _64  ;
_87: _106 _14  | _79 _64  ;
_96: _64 _14  | _64 _64  ;
_27: _134 _14  | _51 _64  ;
_109: _76 _64  | _86 _14  ;
_9: _64 _120  | _14 _74  ;
_81: _64 _131  | _14 _93  ;
_12: _118 _64  | _37 _14  ;
_11: _42 _31  | _42 _11 _31  ;
_108: _64 _14  | _14 _64  ;
_16: _64 _14  | _14 _58  ;
_63: _117 _64  | _16 _14  ;
_126: _64 _129  | _14 _15  ;
_53: _64 _79  | _14 _16  ;
_75: _105 _14  | _88 _64  ;
_93: _127 _14  | _73 _64  ;
_26: _36 _14  | _97 _64  ;
_124: _30 _58  ;
_36: _106 _14  | _117 _64  ;
_21: _25 _14  | _119 _64  ;
_92: _64 _96  | _14 _102  ;
_131: _64 _59  | _14 _66  ;
_4: _64 _108  | _14 _106  ;
_44: _32 _64  | _71 _14  ;
_45: _41 _14  | _67 _64  ;
_94: _119 _14  | _110 _64  ;
_58: _14  | _64  ;
_38: _14 _14  ;
_112: _75 _14  | _21 _64  ;
_104: _58 _79  ;
_59: _14 _16  | _64 _98  ;
_114: _14 _30  | _64 _102  ;
_65: _96 _14  | _6 _64  ;
_68: _30 _14  | _79 _64  ;
_129: _29 _14  | _12 _64  ;
_39: _14 _6  | _64 _117  ;
_78: _14 _14  | _64 _58  ;
_115: _14 _72  | _64 _24  ;
_57: _64 _122  | _14 _22  ;
_71: _107 _14  | _96 _64  ;
_30: _14 _14  | _14 _64  ;
_64: 'a' ;
_106: _14 _64  ;
_89: _28 _64  | _80 _14  ;
_20: _64 _70  | _14 _19  ;
_133: _107 _14  | _34 _64  ;
_34: _64 _64  ;
_22: _96 _58  ;
_90: _16 _14  | _96 _64  ;
_51: _14 _46  | _64 _112  ;
_69: _23 _14  | _13 _64  ;
_15: _14 _55  | _64 _3  ;
_123: _30 _14  | _108 _64  ;
_17: _117 _64  | _78 _14  ;
_40: _49 _64  | _117 _14  ;
_135: _64 _34  | _14 _108  ;
_47: _64 _102  | _14 _78  ;
_97: _14 _34  | _64 _117  ;
_2: _64 _63  | _14 _88  ;
_54: _14 _26  | _64 _77  ;
_118: _98 _14  | _96 _64  ;
_76: _69 _14  | _54 _64  ;
_102: _64 _14  ;
_121: _114 _14  | _130 _64  ;
_99: _64 _57  | _14 _10  ;
_60: _130 _64  | _65 _14  ;
_98: _58 _58  ;
_132: _64 _117  | _14 _30  ;
_74: _89 _14  | _50 _64  ;
_19: _78 _64  | _6 _14  ;
_1: _115 _64  | _20 _14  ;
_18: _53 _64  | _128 _14  ;
_72: _14 _78  | _64 _30  ;
_103: _34 _64  | _38 _14  ;
_52: _30 _14  | _49 _64  ;
_120: _83 _14  | _81 _64  ;
_70: _79 _14  | _108 _64  ;
_7: _64 _113  | _14 _40  ;
_24: _49 _14  | _16 _64  ;
_84: _30 _64  | _34 _14  ;
_110: _64 _30  | _14 _96  ;
_55: _14 _90  | _64 _37  ;
_62: _14 _4  | _64 _47  ;
_80: _64 _48  | _14 _91  ;
_95: _126 _64  | _45 _14  ;
_77: _14 _87  | _64 _35  ;
_14: 'b' ;
_134: _101 _64  | _1 _14  ;
_119: _14 _102  | _64 _102  ;
_0: _8 _11  ;
_33: _14 _92  | _64 _123  ;
_6: _58 _64  | _64 _14  ;
_50: _111 _64  | _7 _14  ;
_43: _14 _98  | _64 _38  ;
_73: _102 _14  | _6 _64  ;
_61: _96 _64  ;
_8: _42  | _42 _8  ;
_136: _30 _64  | _96 _14  ;
_122: _58 _49  ;
%%
int yyerror(const char *s) { return -1; }
