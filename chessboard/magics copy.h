// #pragma once
// #include <cstdint>

// namespace tables {
// 	const uint64_t RMagic[64] = {
// 		0x756607761056301088ULL,
// 		0x486389076241424385ULL,
// 		0x72092778679042112ULL,
// 		0x2630106649158748161ULL,
// 		0x180146733941000192ULL,
// 		0x648589814731636770ULL,
// 		0x180146184134856832ULL,
// 		0x2918332697065554048ULL,
// 		0x11529355785708503092ULL,
// 		0x1267187687899136ULL,
// 		0x2490631468938694656ULL,
// 		0x293015506591416352ULL,
// 		0x288934097953489024ULL,
// 		0x3096241940480520ULL,
// 		0x2596606664451031552ULL,
// 		0x281616710647938ULL,
// 		0x9719133065330688ULL,
// 		0x1193595738806829072ULL,
// 		0x576745526890856466ULL,
// 		0x360429257568030720ULL,
// 		0x722829389795952768ULL,
// 		0x9223513324166054400ULL,
// 		0x144397214955930112ULL,
// 		0x576744426307850385ULL,
// 		0x140739637952517ULL,
// 		0x1170971088563208264ULL,
// 		0x19799803437186ULL,
// 		0x942527774937055360ULL,
// 		0x5070958365245568ULL,
// 		0x72061994232053888ULL,
// 		0x10385301842383864320ULL,
// 		0x581848436590281801ULL,
// 		0x329150075794096256ULL,
// 		0x72128031505784833ULL,
// 		0x5197155070050832448ULL,
// 		0x288300890958333970ULL,
// 		0x146649081425430530ULL,
// 		0x581105115214119936ULL,
// 		0x4828149286694814032ULL,
// 		0x2323899487900144641ULL,
// 		0x18049857767899136ULL,
// 		0x9223654062661107744ULL,
// 		0x9516248984046403600ULL,
// 		0x1153765998257045536ULL,
// 		0x577591050391027840ULL,
// 		0x5066584209096708ULL,
// 		0x1152939105383088648ULL,
// 		0x4827859488876920833ULL,
// 		0x180144604161573632ULL,
// 		0x63067988042981952ULL,
// 		0x35188671256320ULL,
// 		0x2469116105206038656ULL,
// 		0x2251817001949568ULL,
// 		0x562984581857792ULL,
// 		0x9227893237937669120ULL,
// 		0x2305843628831998464ULL,
// 		0x4647715366276964609ULL,
// 		0x18014538100186369ULL,
// 		0x2307340820539246849ULL,
// 		0x360850989000950818ULL,
// 		0x5231493951774524418ULL,
// 		0x281767035013651ULL,
// 		0x4611687186792841348ULL,
// 		0x10696191184830530ULL,
// 	};

// 	const uint64_t BMagic[64] = {
// 		0x9917227751251903040ULL,
// 		0x2307114353926610960ULL,
// 		0x2335125204253151232ULL,
// 		0x7099935816794769416ULL,
// 		0x1130435459416064ULL,
// 		0x72200668527460369ULL,
// 		0x13836258868546633738ULL,
// 		0x11400320691298305ULL,
// 		0x288310780225421568ULL,
// 		0x144612171694473348ULL,
// 		0x37163495452180496ULL,
// 		0x8935681818720ULL,
// 		0x2305988166290530304ULL,
// 		0x3459059462248091652ULL,
// 		0x1155173665466746882ULL,
// 		0x9314574473536473344ULL,
// 		0x146375853238993920ULL,
// 		0x20266336064686592ULL,
// 		0x1087641317556690952ULL,
// 		0x1154047576346992641ULL,
// 		0x594756660686751761ULL,
// 		0x18049591482261512ULL,
// 		0x140738897911808ULL,
// 		0x9800118663272763392ULL,
// 		0x369299586550874768ULL,
// 		0x1154051803924398209ULL,
// 		0x2306414859195514913ULL,
// 		0x1126724741955906ULL,
// 		0x10670162350880858112ULL,
// 		0x457257199186501649ULL,
// 		0x110345957249649152ULL,
// 		0x5188217414386523136ULL,
// 		0x73258399252365824ULL,
// 		0x365081849510826017ULL,
// 		0x1128253566878720ULL,
// 		0x9277419647610126624ULL,
// 		0x9241387535958149122ULL,
// 		0x324401040235561104ULL,
// 		0x72621153876968002ULL,
// 		0x578994379315020032ULL,
// 		0x9223937469837230208ULL,
// 		0x302023234194513920ULL,
// 		0x7036977531065344ULL,
// 		0x144116571189805184ULL,
// 		0x3677198029416300800ULL,
// 		0x292751911566753824ULL,
// 		0x4616754818640315144ULL,
// 		0x2342470636376440960ULL,
// 		0x76847071520890880ULL,
// 		0x7494838608309258752ULL,
// 		0x2378182361733988352ULL,
// 		0x9367487226022232098ULL,
// 		0x2269924327424ULL,
// 		0x634426866335808ULL,
// 		0x18032007883821056ULL,
// 		0x586040256495747604ULL,
// 		0x18155688169644036ULL,
// 		0x40816225332369609ULL,
// 		0x288935455733403656ULL,
// 		0x40533771111638032ULL,
// 		0x281476087169285ULL,
// 		0x4504184552499712ULL,
// 		0x2675147045886527752ULL,
// 		0x4616224805122081088ULL,
// 	};

// 	// A hash map for the pop_1st_bit function below.
// 	const int BitTable[64] = {
// 		63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
// 		51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
// 		26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
// 		58, 20, 37, 17, 36, 8
// 	};

// 	// Equivalent to first_set_bit(), but also removes the bit in question.
// 	// Takes in a uint64, then uses bit hacks to create a key which
// 	// references the hash map above.
// 	int pop_1st_bit(uint64_t *bb) {
// 		uint64_t b = *bb ^ (*bb - 1);
// 		unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
// 		*bb &= (*bb - 1);
// 		return BitTable[(fold * 0x783a9b23) >> 26];
// 	}

// 	// Returns an array of blockers given a square and a mask of occupancy bits.
// 	uint64_t index_to_uint64(int index, int bits, uint64_t m) {
// 		int i, j;
// 		uint64_t result = 0ULL;
// 		for(i = 0; i < bits; i++) {
// 			j = pop_1st_bit(&m);
// 			if(index & (1 << i)) result |= (1ULL << j);
// 		}
// 		return result;
// 	}

// 	int transform(uint64_t board, int magic, int bits) {
// 		return (board * magic) >> (64 - bits);
// 	}

// 	// Generates the relavent occupancy bit masks for a particular
// 	// square with a rook at that position.
// 	uint64_t rmask(int sq) {
// 		uint64_t result = 0ULL;
// 		int rk = sq/8, fl = sq%8, r, f;
// 		for(r = rk+1; r <= 6; r++) result |= (1ULL << (fl + r*8));
// 		for(r = rk-1; r >= 1; r--) result |= (1ULL << (fl + r*8));
// 		for(f = fl+1; f <= 6; f++) result |= (1ULL << (f + rk*8));
// 		for(f = fl-1; f >= 1; f--) result |= (1ULL << (f + rk*8));
// 		return result;
// 	}

// 	// Generates the relavent occupancy bit masks for a particular
// 	// square with a bishop at that position.
// 	uint64_t bmask(int sq) {
// 		uint64_t result = 0ULL;
// 		int rk = sq/8, fl = sq%8, r, f;
// 		for(r=rk+1, f=fl+1; r<=6 && f<=6; r++, f++) result |= (1ULL << (f + r*8));
// 		for(r=rk+1, f=fl-1; r<=6 && f>=1; r++, f--) result |= (1ULL << (f + r*8));
// 		for(r=rk-1, f=fl+1; r>=1 && f<=6; r--, f++) result |= (1ULL << (f + r*8));
// 		for(r=rk-1, f=fl-1; r>=1 && f>=1; r--, f--) result |= (1ULL << (f + r*8));
// 		return result;
// 	}

// 	// Generates the corresponding attack squares for the rook at a given position.
// 	// Breaks out if there is a blocker. This will be used to fill the hash table.
// 	uint64_t ratt(int sq, uint64_t block) {
// 		uint64_t result = 0ULL;
// 		int rk = sq/8, fl = sq%8, r, f;
// 		for(r = rk+1; r <= 7; r++) {
// 			result |= (1ULL << (fl + r*8));
// 			if(block & (1ULL << (fl + r*8))) break;
// 		}
// 		for(r = rk-1; r >= 0; r--) {
// 			result |= (1ULL << (fl + r*8));
// 			if(block & (1ULL << (fl + r*8))) break;
// 		}
// 		for(f = fl+1; f <= 7; f++) {
// 			result |= (1ULL << (f + rk*8));
// 			if(block & (1ULL << (f + rk*8))) break;
// 		}
// 		for(f = fl-1; f >= 0; f--) {
// 			result |= (1ULL << (f + rk*8));
// 			if(block & (1ULL << (f + rk*8))) break;
// 		}
// 		return result;
// 	}

// 	// Generates the corresponding attack squares for a bishop at a given position.
// 	// Breaks out if there is a blocker. This will be used to fill the hash table.
// 	uint64_t batt(int sq, uint64_t block) {
// 		uint64_t result = 0ULL;
// 		int rk = sq/8, fl = sq%8, r, f;
// 		for(r = rk+1, f = fl+1; r <= 7 && f <= 7; r++, f++) {
// 			result |= (1ULL << (f + r*8));
// 			if(block & (1ULL << (f + r * 8))) break;
// 		}
// 		for(r = rk+1, f = fl-1; r <= 7 && f >= 0; r++, f--) {
// 			result |= (1ULL << (f + r*8));
// 			if(block & (1ULL << (f + r * 8))) break;
// 		}
// 		for(r = rk-1, f = fl+1; r >= 0 && f <= 7; r--, f++) {
// 			result |= (1ULL << (f + r*8));
// 			if(block & (1ULL << (f + r * 8))) break;
// 		}
// 		for(r = rk-1, f = fl-1; r >= 0 && f >= 0; r--, f--) {
// 			result |= (1ULL << (f + r*8));
// 			if(block & (1ULL << (f + r * 8))) break;
// 		}
// 		return result;
// 	}

// 	int RBits[64] = {
// 		12, 11, 11, 11, 11, 11, 11, 12,
// 		11, 10, 10, 10, 10, 10, 10, 11,
// 		11, 10, 10, 10, 10, 10, 10, 11,
// 		11, 10, 10, 10, 10, 10, 10, 11,
// 		11, 10, 10, 10, 10, 10, 10, 11,
// 		11, 10, 10, 10, 10, 10, 10, 11,
// 		11, 10, 10, 10, 10, 10, 10, 11,
// 		12, 11, 11, 11, 11, 11, 11, 12
// 	};

// 	int BBits[64] = {
// 		6, 5, 5, 5, 5, 5, 5, 6,
// 		5, 5, 5, 5, 5, 5, 5, 5,
// 		5, 5, 7, 7, 7, 7, 5, 5,
// 		5, 5, 7, 9, 9, 7, 5, 5,
// 		5, 5, 7, 9, 9, 7, 5, 5,
// 		5, 5, 7, 7, 7, 7, 5, 5,
// 		5, 5, 5, 5, 5, 5, 5, 5,
// 		6, 5, 5, 5, 5, 5, 5, 6
// 	};

// 	int *RAtkTable[64];
// 	int *BAtkTable[64];

// 	void hashtab(bool bishop) {
// 		uint64_t legal_moves[4096], occupied[4096];
// 		int i, j, k;

// 		for (i = 0; i < 64; ++i) {
// 			uint64_t mask = bishop ? bmask(i) : rmask(i);
// 			int *atk_table = bishop ? BAtkTable[i] : RAtkTable[i];
// 			int bits = bishop ? BBits[i] : RBits[i];
// 			uint64_t magic = bishop ? BMagic[i] : RMagic[i];

// 			atk_table = new int[(1 << bits)];
			
// 			for (j = 0; j < (1 << bits); ++j) {
// 				occupied[j] = index_to_uint64(j, bits, mask);
// 				legal_moves[j] = bishop ? batt(i, occupied[j]) : ratt(i, occupied[j]);
// 			}
// 			for (j = 0; j < (1 << bits); ++j) {
// 				k = transform(occupied[j], magic, bits);
// 				atk_table[j] = legal_moves[k];
// 			}
// 		}
// 	}

// 	void hashtab()
// 	{
// 		hashtab(true);
// 		hashtab(false);
// 	}
// }