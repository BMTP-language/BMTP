#ifndef _FILE_INIT_CPP_
#error "This file should be included by init.cpp"
#endif // !_FILE_INIT_CPP_

typedef pointer(*signle_opr_t)(const pointer&,const pointer&);
typedef pointer(*signle_opr_t2)(const pointer&);
#include "opr_add.h"
#include "opr_sub.h"
#include "opr_mul.h"
#include "opr_div.h"
#include "opr_pow.h"
#include "opr_eql.h"
#include "opr_neq.h"
#include "opr_les.h"
#include "opr_grt.h"
#include "opr_leq.h"
#include "opr_geq.h"
#include "opr_mod.h"
#include "opr_idv.h"
#include "opr_and.h"
#include "opr_not.h"
#include "opr_orr.h"
#include "opr_xor.h"
#include "opr_pos.h"
#include "opr_neg.h"
#include "opr_idx.h"
