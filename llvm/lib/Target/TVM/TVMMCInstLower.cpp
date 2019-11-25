//===-- TVMMCInstLower.cpp - Convert TVM MachineInstr to an MCInst --------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains code to lower TVM MachineInstrs to their corresponding
// MCInst records.
//
//===----------------------------------------------------------------------===//

#include "TVMMCInstLower.h"
#include "InstPrinter/TVMInstPrinter.h"
#include "TVMMCExpr.h"
#include "TVMTargetMachine.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Mangler.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

void TVMMCInstLower::lower(const MachineInstr *MI, MCInst &OutMI) {
  OutMI.setOpcode(MI->getOpcode());

  for (unsigned i = 0, e = MI->getNumOperands(); i != e; ++i) {
    const MachineOperand &MO = MI->getOperand(i);

    MCOperand MCOp;
    switch (MO.getType()) {
    default:
      MI->print(errs());
      llvm_unreachable("unknown operand type");
    case MachineOperand::MO_Register:
      // Ignore all implicit register operands.
      if (MO.isImplicit())
        continue;
      MCOp = MCOperand::createReg(MO.getReg());
      break;
    case MachineOperand::MO_Immediate:
      MCOp = MCOperand::createImm(MO.getImm());
      break;
    case MachineOperand::MO_CImmediate:
      if (MO.getCImm()->getValue().getMinSignedBits() <= 64) {
        MCOp = MCOperand::createImm(MO.getCImm()->getSExtValue());
        break;
      } else {
        // To avoid a memory leak, initial size of the SmallString should be
        // chosen enough for the entire string. Otherwise, its internal memory
        // will be reallocated into the generic heap but not into the Ctx
        // arena and thus never deallocated.
        auto Str = new (Ctx) SmallString<80>();
        if (MI->getOpcode() == TVM::CONST_U257_S)
          MO.getCImm()->getValue().toString(*Str, 16, false, true);
        else
          MO.getCImm()->getValue().toString(*Str, 16, true, true);
        MCOp = MCOperand::createExpr(TVMImmStringMCExpr::create(*Str, Ctx));
      }
      break;
    case MachineOperand::MO_MachineBasicBlock: {
      auto *MBB = MO.getMBB();
      do { // we need to iterate all fallthrough blocks
        for (auto &I : *MBB) {
          auto Inst = std::make_shared<MCInst>();
          ContinuationInstructionStorage.push_back(Inst);

          lower(&I, *Inst);
          OutMI.addOperand(MCOperand::createInst(Inst.get()));

          MCInstrsMap[Inst.get()] = &I;
        }
      } while ((MBB = MBB->getFallThrough()));
    }
      continue;
    case MachineOperand::MO_GlobalAddress: {
      assert(MO.getTargetFlags() == 0 &&
             "TVM does not use target flags on GlobalAddresses");

      const MCExpr *Expr = nullptr;

      switch (MI->getOpcode()) {
      case TVM::PRINTSTR_S:
      case TVM::LOGSTR_S: {
        const MachineOperand &Op = MI->getOperand(0);
        const GlobalValue *GV = Op.getGlobal();

        assert(GV && "LOGSTR_S/PRINTSTR_S operand must be GlobalValue");

        StringRef DataString;
        static constexpr size_t MaxStringLength = 15;

        if (const auto *GlobalVar = dyn_cast<GlobalVariable>(GV)) {
          const auto *Data =
              dyn_cast<ConstantArray>(GlobalVar->getInitializer());
          assert(Data &&
                 "Only constant static strings are supported for "
                 "LOGSTR_S/PRINTSTR_S for now");

          auto Str = new (Ctx) std::array<char, MaxStringLength>();
          unsigned Length = std::min((unsigned)MaxStringLength,
              Data->getNumOperands() - 1);
          for (unsigned i = 0; i != Length; ++i) {
            auto C = dyn_cast<ConstantInt>(Data->getOperand(i));
            assert(C->getValue().isIntN(8) && "Invalid character");
            (*Str)[i] = (char)C->getZExtValue();
          }
          DataString = StringRef(Str->data(), Length);
        } else if (const auto *Fn = dyn_cast<Function>(GV)) {
          DataString = Fn->getName();
        }

        if (DataString.size() > MaxStringLength)
          DataString = DataString.substr(0, MaxStringLength);

        Expr = TVMImmStringMCExpr::create(DataString, Ctx);

        break;
      }
      default:
        Expr = MCSymbolRefExpr::create(Printer.getSymbol(MO.getGlobal()), Ctx);

        if (int64_t Offset = MO.getOffset())
          Expr = MCBinaryExpr::createAdd(
              Expr, MCConstantExpr::create(Offset, Ctx), Ctx);
      }

      assert(Expr && "Expr must be initialized");
      MCOp = MCOperand::createExpr(Expr);
      break;
    }
    case MachineOperand::MO_ExternalSymbol: {
      MCSymbol *Sym = Ctx.getOrCreateSymbol(StringRef(MO.getSymbolName()));
      Sym->setExternal(true);
      const MCSymbolRefExpr *Expr = MCSymbolRefExpr::create(Sym, Ctx);
      MCOp = MCOperand::createExpr(Expr);
      break;
    }
    case MachineOperand::MO_RegisterMask:
      continue;
    }

    OutMI.addOperand(MCOp);
  }
}
