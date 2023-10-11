/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-formatter.cc - RISC-V instruction printer (disassembler)
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
 */

#include "inst-decoder.h"

#include <functional>
#include <map>
#include <iostream>


std::ostream &
operator<<(std::ostream &os, const InstructionDecoder &decoder)
{
  /* TODO: write a textual representation of the decoded instruction
   * in "decoder" to the output stream "os". Do not include a newline.
   * And remove the statement below.
   */
  os << "not implemented";

  return os;
}


std::vector<std::string> getMnemonicFromOpcode(uint16_t opcode) {
	std::vector<std::string> mnemonics;
	switch (opcode) {
		case 0x00:
			mnemonics = {"l.j"};
			break;
		case 0x01:
			mnemonics = {"l.jal"};
			break;
		case 0x02:
			mnemonics = {"l.adrp"};
			break;
		case 0x03:
			mnemonics = {"l.bnf"};
			break;
		case 0x04:
			mnemonics = {"l.bf"};
			break;
		case 0x05:
			mnemonics = {"l.nop"};
			break;
		case 0x06:
			mnemonics = {"l.movhi", "l.macrc"};
			break;
		case 0x08:
			mnemonics = {"l.sys", "l.trap", "l.msync", "l.psync", "l.csync"};
			break;
		case 0x09:
			mnemonics = {"l.rfe"};
			break;
		case 0x0A:
			mnemonics = {"lv.cust1", "lv.cust2", "lv.cust3", "lv.cust4", "lv.all_eq.b", "lv.all_eq.h", "lv.all_ge.b", "lv.all_ge.h", "lv.all_gt.b", "lv.all_gt.h", "lv.all_le.b", "lv.all_le.h", "lv.all_lt.b", "lv.all_lt.h", "lv.all_ne.b", "lv.all_ne.h", "lv.any_eq.b", "lv.any_eq.h", "lv.any_ge.b", "lv.any_ge.h", "lv.any_gt.b", "lv.any_gt.h", "lv.any_le.b", "lv.any_le.h", "lv.any_lt.b", "lv.any_lt.h", "lv.any_ne.b", "lv.any_ne.h", "lv.add.b", "lv.add.h", "lv.adds.b", "lv.adds.h", "lv.addu.b", "lv.addu.h", "lv.addus.b", "lv.addus.h", "lv.and", "lv.avg.b", "lv.avg.h", "lv.cmp_eq.b", "lv.cmp_eq.h", "lv.cmp_ge.b", "lv.cmp_ge.h", "lv.cmp_gt.b", "lv.cmp_gt.h", "lv.cmp_le.b", "lv.cmp_le.h", "lv.cmp_lt.b", "lv.cmp_lt.h", "lv.cmp_ne.b", "lv.cmp_ne.h", "lv.madds.h", "lv.max.b", "lv.max.h", "lv.merge.b", "lv.merge.h", "lv.min.b", "lv.min.h", "lv.msubs.h", "lv.muls.h", "lv.nand", "lv.nor", "lv.or", "lv.pack.b", "lv.pack.h", "lv.packs.b", "lv.packs.h", "lv.packus.b", "lv.packus.h", "lv.perm.n", "lv.rl.b", "lv.rl.h", "lv.sll.b", "lv.sll.h", "lv.sll", "lv.srl.b", "lv.srl.h", "lv.sra.b", "lv.sra.h", "lv.srl", "lv.sub.b", "lv.sub.h", "lv.subs.b", "lv.subs.h", "lv.subu.b", "lv.subu.h", "lv.subus.b", "lv.subus.h", "lv.unpack.b", "lv.unpack.h", "lv.xor"};
			break;
		case 0x11:
			mnemonics = {"l.jr"};
			break;
		case 0x12:
			mnemonics = {"l.jalr"};
			break;
		case 0x13:
			mnemonics = {"l.maci"};
			break;
		case 0x1A:
			mnemonics = {"l.lf"};
			break;
		case 0x1B:
			mnemonics = {"l.lwa"};
			break;
		case 0x1C:
			mnemonics = {"l.cust1"};
			break;
		case 0x1D:
			mnemonics = {"l.cust2"};
			break;
		case 0x1E:
			mnemonics = {"l.cust3"};
			break;
		case 0x1F:
			mnemonics = {"l.cust4"};
			break;
		case 0x20:
			mnemonics = {"l.ld"};
			break;
		case 0x21:
			mnemonics = {"l.lwz"};
			break;
		case 0x22:
			mnemonics = {"l.lws"};
			break;
		case 0x23:
			mnemonics = {"l.lbz"};
			break;
		case 0x24:
			mnemonics = {"l.lbs"};
			break;
		case 0x25:
			mnemonics = {"l.lhz"};
			break;
		case 0x26:
			mnemonics = {"l.lhs"};
			break;
		case 0x27:
			mnemonics = {"l.addi"};
			break;
		case 0x28:
			mnemonics = {"l.addic"};
			break;
		case 0x29:
			mnemonics = {"l.andi"};
			break;
		case 0x2A:
			mnemonics = {"l.ori"};
			break;
		case 0x2B:
			mnemonics = {"l.xori"};
			break;
		case 0x2C:
			mnemonics = {"l.muli"};
			break;
		case 0x2D:
			mnemonics = {"l.mfspr"};
			break;
		case 0x2E:
			mnemonics = {"l.slli", "l.srli", "l.srai", "l.rori"};
			break;
		case 0x2F:
			mnemonics = {"l.sfeqi", "l.sfnei", "l.sfgtui", "l.sfgeui", "l.sfltui", "l.sfleui", "l.sfgtsi", "l.sfgesi", "l.sfltsi", "l.sflesi"};
			break;
		case 0x30:
			mnemonics = {"l.mtspr"};
			break;
		case 0x31:
			mnemonics = {"l.mac", "l.macu", "l.msb", "l.msbu"};
			break;
		case 0x32:
			mnemonics = {"lf.sfeq.s", "lf.sfne.s", "lf.sfgt.s", "lf.sfge.s", "lf.sflt.s", "lf.sfle.s", "lf.sfeq.d", "lf.sfne.d", "lf.sfgt.d", "lf.sfge.d", "lf.sflt.d", "lf.sfle.d", "lf.sfueq.s", "lf.sfune.s", "lf.sfugt.s", "lf.sfuge.s", "lf.sfult.s", "lf.sfule.s", "lf.sfun.s", "lf.stod.d", "lf.dtos.d", "lf.sfueq.d", "lf.sfune.d", "lf.sfugt.d", "lf.sfuge.d", "lf.sfult.d", "lf.sfule.d", "lf.sfun.d", "lf.cust1.s", "lf.cust1.d", "lf.itof.s", "lf.ftoi.s", "lf.itof.d", "lf.ftoi.d", "lf.add.s", "lf.sub.s", "lf.mul.s", "lf.div.s", "lf.madd.s", "lf.add.d", "lf.sub.d", "lf.mul.d", "lf.div.d", "lf.madd.d"};
			break;
		case 0x33:
			mnemonics = {"l.swa"};
			break;
		case 0x35:
			mnemonics = {"l.sw"};
			break;
		case 0x36:
			mnemonics = {"l.sb"};
			break;
		case 0x37:
			mnemonics = {"l.sh"};
			break;
		case 0x38:
			mnemonics = {"l.exths", "l.extws", "l.extbs", "l.extwz", "l.exthz", "l.extbz", "l.add", "l.addc", "l.sub", "l.and", "l.or", "l.xor", "l.cmov", "l.ff1", "l.sll", "l.srl", "l.sra", "l.ror", "l.fl1", "l.mul", "l.muld", "l.div", "l.divu", "l.mulu", "l.muldu"};
			break;
		case 0x39:
			mnemonics = {"l.sfeq", "l.sfne", "l.sfgtu", "l.sfgeu", "l.sfltu", "l.sfleu", "l.sfgts", "l.sfges", "l.sflts", "l.sfles"};
			break;
		case 0x3C:
			mnemonics = {"l.cust5"};
			break;
		case 0x3D:
			mnemonics = {"l.cust6"};
			break;
		case 0x3E:
			mnemonics = {"l.cust7"};
			break;
		case 0x3F:
			mnemonics = {"l.cust8"};
			break;
		default:
			mnemonics = {"INVALID"};
			break;
	}
	return mnemonics;
}