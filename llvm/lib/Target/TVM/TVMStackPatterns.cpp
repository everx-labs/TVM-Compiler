//==-- TVMStackPatterns.cpp - optimized stack exchange patterns --*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "TVMStackPatterns.h"

namespace llvm {

namespace StackPatterns {

namespace {

// Helper for indices correction.
//  Next operation may use src operand, moved by previous operation.
struct XchgPairCorrector {
  XchgPairCorrector(unsigned Deep, unsigned Top) : Deep(Deep), Top(Top) {}
  XchgPairCorrector &operator()(unsigned &Idx) {
    if (Idx == Deep)
      Idx = Top;
    else if (Idx == Top)
      Idx = Deep;
    return *this;
  }
  unsigned Deep;
  unsigned Top;
};

bool arg0(unsigned idx) { return idx == 1; }
bool arg1(unsigned idx) { return idx == 0; }
bool noArg(unsigned idx) { return idx >= 2; }

bool is_xx(unsigned a, unsigned b, unsigned c) {
  return noArg(a) && noArg(b) && noArg(c);
}
bool is_xc(unsigned a, unsigned b, unsigned c) {
  return arg1(c) && noArg(a) && noArg(b);
}
bool is_ax(unsigned a, unsigned b, unsigned c) {
  return arg0(a) && noArg(b) && noArg(c);
}
bool is_ab(unsigned a, unsigned b, unsigned c) {
  return arg0(a) && arg1(b) && noArg(c);
}
bool is_ac(unsigned a, unsigned b, unsigned c) {
  return arg0(a) && arg1(c) && noArg(b);
}
bool is_bx(unsigned a, unsigned b, unsigned c) {
  return arg0(b) && noArg(a) && noArg(c);
}
bool is_ba(unsigned a, unsigned b, unsigned c) {
  return arg0(b) && arg1(a) && noArg(c);
}
bool is_bc(unsigned a, unsigned b, unsigned c) {
  return arg0(b) && arg1(c) && noArg(a);
}
bool is_cx(unsigned a, unsigned b, unsigned c) {
  return arg0(c) && noArg(a) && noArg(b);
}
bool is_ca(unsigned a, unsigned b, unsigned c) {
  return arg0(c) && arg1(a) && noArg(b);
}
bool is_cb(unsigned a, unsigned b, unsigned c) {
  return arg0(c) && arg1(b) && noArg(a);
}

} // namespace

void pattern_x(StackFixup &Rv, unsigned i, unsigned Dst0) {
  if (i == Dst0)
    return;
  if (Dst0 == 0)
    Rv(StackFixup::xchgTop(i));
  else if (i == 0)
    Rv(StackFixup::xchgTop(Dst0));
  else
    Rv(StackFixup::xchg(i, Dst0));
}

void pattern2_pp(StackFixup &Rv, unsigned i, unsigned j) {
  // No problems for two pushes
  // { a | x | x | b || - } => { a | x | x | b || a | b | - }
  Rv(StackFixup::push2(i, j));
}

void pattern2_xp(StackFixup &Rv, unsigned i, unsigned j) {
  if (i == 0) { // no exchange, just push
    Rv(StackFixup::pushI(j));
  } else {
    // First exchange may move argument for second push
    // if second push src is s(0) or the same as xhg src
    XchgPairCorrector Correct(0, i);
    Correct(j);
    Rv(StackFixup::xcpu(i, j));
  }
}

void pattern2_px(StackFixup &Rv, unsigned i, unsigned j) {
  // push + swap + exchange (puxc)
  if (j == 0) {
    // If xchg value is already in place s(0), we need to do push + swap
    // { a | x | x || p(a) | b | - }
    Rv(StackFixup::pushI(i));
    Rv(StackFixup::swap());
  } else { // normal case
    Rv(StackFixup::puxc(i, j));
  }
}

void pattern2_xx(StackFixup &Rv, unsigned i, unsigned j) {
  if (i == 1 && j == 0)
    return;
  // If one of arguments already in place, xchg other
  if (i == 1)
    return pattern_x(Rv, j, 0);
  if (j == 0)
    return pattern_x(Rv, i, 1);

  // We have problem if second argument sourced from first
  // { a | x | x || b | x | - }  => { x | x | x || a | b | - }
  // XCHG2 4, 1; (4<->1, 1<->0) will generate { b | x | x || x | a | - }
  // We need to correct it to XCHG2 4, 4 here (4<->1, 4<->0)
  if (j == 1 && i != 0) {
    Rv(StackFixup::xchg2(i, i));
  } else if (j == 1 && i == 0) { // just swap
    Rv(StackFixup::swap());
  } else { // normal case
    Rv(StackFixup::xchg2(i, j));
  }
}

void pattern2_xx(StackFixup &Rv, unsigned i, unsigned j, unsigned Dst0,
                 unsigned Dst1) {
  assert(Dst0 != Dst1 && i != j);

  if (i == Dst0 && j == Dst1)
    return;
  if (i == Dst0)
    return pattern_x(Rv, j, Dst1);
  if (j == Dst1)
    return pattern_x(Rv, i, Dst0);

  if (Dst0 == 1 && Dst1 == 0)
    return pattern2_xx(Rv, i, j);
  if (Dst0 == 0 && Dst1 == 1)
    return pattern2_xx(Rv, j, i);

  pattern_x(Rv, i, Dst0);
  XchgPairCorrector CorrectI(i, Dst0);
  CorrectI(j);
  pattern_x(Rv, j, Dst1);
}

void pattern3_xxx(StackFixup &Rv, unsigned i, unsigned j, unsigned k) {
  // xchg i <-> 2; xchg j <-> 1; xchg k <-> 0
  if (i < 3 && j < 3 && k < 3) {
    // All vals in [2:0] range, it is just some reorder of 3 values
    if (i == 2 && j == 1 && k == 0)      // { |abc } => { |abc }
      return;                            // No changes required
    else if (i == 2 && j == 0 && k == 1) // { |acb } => { |abc }
      Rv(StackFixup::swap());
    else if (i == 1 && j == 2 && k == 0) // { |bac } => { |abc }
      Rv(StackFixup::xchg(2, 1));
    else if (i == 0 && j == 2 && k == 1) // { |bca } => { |abc }
      Rv(StackFixup::makeRollRev(2));
    else if (i == 1 && j == 0 && k == 2) // { |cab } => { |abc }
      Rv(StackFixup::rot());
    else if (i == 0 && j == 1 && k == 2) // { |cba } => { |abc }
      Rv(StackFixup::reverse(3, 0));
    else
      llvm_unreachable("pattern3_xxx (i, j, k) inconsistence");
  } else {
    if (i == 2) // { bc|axx } => { abc }
      return pattern2_xx(Rv, j, k);

    // "processing" first xchg i <-> 2
    XchgPairCorrector CorrectI(i, 2);
    CorrectI(j)(k);
    // "processing" second xchg j <-> 1
    XchgPairCorrector CorrectJ(j, 1);
    CorrectJ(k);
    Rv(StackFixup::xchg3(i, j, k));
  }
}

void pattern3_xxp(StackFixup &Rv, unsigned i, unsigned j, unsigned k) {
  // xchg i <-> 1; xchg j <-> 0; push k
  // { abc|xx } => { xbc|ax } => { xxc|ab } => { xxc|abc }
  // { xbc|xa } => { xbc|ax } => { xxc|ab } => { xxc|abc } - OK
  // { axc|xb } => { xxc|ab } => { xxc|ab } => { xxc|abc } - OK
  // { abx|xc } => { xbx|ac } => { xcx|ab } => { xcx|abc } k=j
  // { xbc|ax } => { xbc|ax } => { xxc|ab } => { xxc|abc } - OK
  // { xxc|ab } => { xxc|ab } => { xxc|ab } => { xxc|abc } - OK
  // { xbx|ac } => { xbx|ac } => { xcx|ab } => { xcx|abc } k=j
  // { axc|bx } => { bxc|ax } => { xxc|ab } => { xxc|abc } j=i
  // { xxc|ba } => { xxc|ab } => { xxc|ab } => { xxc|abc } j=0
  // { axx|bc } => { bxx|ac } => { cxx|ab } => { cxx|abc } j=i, k=i
  // { abx|cx } => { cbx|ax } => { cxx|ab } => { cxx|abc } k=i
  // { xbx|ca } => { xbx|ac } => { xcx|ab } => { xcx|abc } k=j
  // { axx|cb } => { cxx|ab } => { cxx|ab } => { cxx|abc } k=i

  if (is_ab(i, j, k)) { // { xxc|ab }
    Rv(StackFixup::pushI(k));
    return;
  }
  if (is_ax(i, j, k)) // { xbc|ax }
    return pattern2_xp(Rv, j, k);

  XchgPairCorrector CorrectI(i, 1);
  CorrectI(j)(k);
  XchgPairCorrector CorrectJ(j, 0);
  CorrectJ(k);

  Rv(StackFixup::xc2pu(i, j, k));
}

void pattern3_xpx(StackFixup &Rv, unsigned i, unsigned j, unsigned k) {
  // xchg i <-> 1; push j & swap; xchg k+1 <-> 0
  // xcpuxc: { bx|ac } => { bx|ac } => { bx|acb } => { bx|abc } => error k+1=0
  if (i == 1 && k == 0) { // { bx|ac } => { bx|acb } => { bx|abc }
    Rv(StackFixup::pushI(j));
    Rv(StackFixup::swap());
    return;
  }
  // { xc|ab } => { xc|abb } => { xc|abb } => { xb|abc }
  // { bc|ax } => { bc|axb } => { bc|abx } => { bx|abc }
  if (i == 1)
    return pattern2_px(Rv, j, k);

  // { ac|bx } => { bc|ax } => { bc|axb } => { bc|abx } => { bx|abc } - OK
  // { cx|ba } => { cx|ab } => { cx|abb } => { cx|abb } => { bx|abc } - OK
  // { ax|cb } => { cx|ab } => { cx|abb } => { cx|abb } => { bx|abc } - OK
  // { ab|cx } => { cb|ax } => { cb|axb } => { cb|abx } => { xb|abc } - OK
  // { bc|xa } => { bc|ax } => { bc|axb } => { bc|abx } => { bx|abc } - OK
  // { ac|xb } => { xc|ab } => { xc|abb } => { xc|abb } => { xb|abc } - OK

  XchgPairCorrector CorrectI(i, 1);
  CorrectI(j)(k);
  // If k == 0 (after correction), we can't optimize using xcpuxc:
  // { ba|xc } => { bx|ac } => { bx|abc } => error (k+1=0,k=?)
  // { bx|ca } => { bx|ac } => { bx|abc } => error (k+1=0,k=?)
  // { ax|bc } => { bx|ac } => { bx|abc } => error (k+1=0,k=?)
  if (k == 0) {
    Rv(StackFixup::xchg(i, 1));
    Rv(StackFixup::pushI(j));
    Rv(StackFixup::swap());
  } else {
    Rv(StackFixup::xcpuxc(i, j, k));
  }
}

void pattern3_xpp(StackFixup &Rv, unsigned i, unsigned j, unsigned k) {
  // xchg i <-> 0; push j; push k + 1
  if (i == 0)
    return pattern2_pp(Rv, j, k);
  XchgPairCorrector CorrectI(i, 0);
  CorrectI(j)(k);
  Rv(StackFixup::xcpu2(i, j, k));
}

void impl_pxx(StackFixup &Rv, unsigned i, unsigned j, unsigned k) {
  Rv(StackFixup::puxc2(i, j, k));
}

void pattern3_pxx(StackFixup &Rv, unsigned i, unsigned j, unsigned k) {
  // push i; xchg 0<->2; xchg j+1<->1; xchg k+1<->0
  // { abc|xx } => { abc|xxa } => { abc|axx } => { axc|abx } => { axx|abc }
  if (is_xx(i, j, k))
    return impl_pxx(Rv, i, j, k);

  // { abx|xc } => { abx|xca } => { abx|acx } => { acx|abx } => { axx|abc } k=j
  // { xbx|ac } => { xbx|aca } => { xbx|aca } => { xcx|aba } => { xax|abc } k=j
  if (is_xc(i, j, k) || is_ac(i, j, k))
    return impl_pxx(Rv, i, j, k = j);

  // { axc|bx } => { axc|bxa } => { axc|axb } => error j+1=0,j=?
  if (is_bx(i, j, k)) {
    Rv(StackFixup::xcpu(k, i));     // { axx|bc }, { axx|bca }
    Rv(StackFixup::makeRollRev(2)); // { axx|abc }
    return;
  }
  // { xxc|ba } => { xxc|baa } => { xxc|aab } => error j+1=0,j=?
  if (is_ba(i, j, k)) {
    Rv(StackFixup::xcpu(k, k));     // { xxa|bc }, { axx|bca }
    Rv(StackFixup::makeRollRev(2)); // { axx|abc }
    return;
  }
  // { axx|bc } => { axx|bca } => { axx|acb } => error j+1=0,j=?
  if (is_bc(i, j, k)) {
    Rv(StackFixup::pushI(i));       // { axx|bca }
    Rv(StackFixup::makeRollRev(2)); // { axx|abc }
    return;
  }

  // { abx|cx } => { abx|cxa } => { abx|axc } => { axx|abc } => error k+1=0,k=?
  if (is_cx(i, j, k)) {
    Rv(StackFixup::xcpu(j, i)); // { axx|cb }, { axx|cba }
    Rv(StackFixup::xchgTop(2)); // { axx|abc }
    return;
  }
  // { xbx|ca } => { xbx|caa } => { xbx|aac } => { xax|abc } => error k+1=0,k=?
  if (is_ca(i, j, k)) {
    Rv(StackFixup::xcpu(j, j)); // { xax|cb }, { xax|cba }
    Rv(StackFixup::xchgTop(2)); // { axx|abc }
    return;
  }
  // { axx|cb } => { axx|cba } => { axx|abc } => { axx|abc } => error k+1=0,k=?
  if (is_cb(i, j, k)) {
    Rv(StackFixup::pushI(i));   // { axx|cba }
    Rv(StackFixup::xchgTop(2)); // { axx|abc }
    return;
  }

  // { xbc|xa } => { xbc|xaa } => { xbc|aax } => { xac|abx } => { xax|abc } OK
  // { axc|xb } => { axc|xba } => { axc|abx } => { axc|abx } => { axx|abc } OK
  // { xbc|ax } => { xbc|axa } => { xbc|axa } => { xxc|aba } => { xxa|abc } OK
  // { xxc|ab } => { xxc|aba } => { xxc|aba } => { xxc|aba } => { xxa|abc } OK

  return impl_pxx(Rv, i, j, k);
}

void impl_pxp(StackFixup &Rv, unsigned i, unsigned j, unsigned k) {
  Rv(StackFixup::puxcpu(i, j, k));
}

void pattern3_pxp(StackFixup &Rv, unsigned i, unsigned j, unsigned k) {
  // push i; swap; xchg j+1<->0; push k+1
  // { abc|x } => { abc|xa } => { abc|ax } => { axc|ab } => { axc|abc }

  // { axc|b } => { axc|ba } => { axc|ab } => error j+1=0,j=?
  if (j == 0) {
    pattern2_pp(Rv, i, k);      // { axc|bac }
    Rv(StackFixup::xchg(2, 1)); // { axc|abc }
    return;
  }
  // { abx|c } => { abx|ca } => { abx|ac } => { acx|ab } => { acx|abc } k=j
  if (k == 0)
    return impl_pxp(Rv, i, j, k = j);

  // { xbc|a } => { xbc|aa } => { xbc|aa } => { xac|ab } => { xac|abc } - OK
  return impl_pxp(Rv, i, j, k);
}

void pattern3_ppx(StackFixup &Rv, unsigned i, unsigned j, unsigned k) {
  // push i; swap; push j + 1; swap; xchg k+2<->0
  // { abc|x } => { abc|xa } => { abc|ax } => { abc|axb } => { abc|abx }
  //  => { abx|abc }

  // { abx|c } => { abx|ca } => { abx|ac } => { abx|acb } => { abx|abc }
  //  => error k+2=0,k=?
  if (k == 0) {                  // { abx|c }
    pattern2_pp(Rv, i, j);       // { abx|ca }; { abx|cab }
    Rv(StackFixup::makeRoll(2)); // { abx|abc }
    return;
  }

  // { axc|b } => { axc|ba } => { axc|ab } => error j+1=0,j=?
  if (j == 0 && k != 1) {                  // { axc|b }
    pattern3_xpp(Rv, k, i, k);   // { axb|c } => { axb|ca } => { axb|cab }
    Rv(StackFixup::makeRoll(2)); // { axb|abc }
    return;
  }
  // { ab|cx }
  if (k == 1) {
    pattern2_pp(Rv, i, j);       // { ab|cxab }
    Rv(StackFixup::makeRoll(3)); // { ab|cxab }
    return;
  }

  // { xbc|a } => { xbc|aa } => { xbx|aa } => { xbc|aab } => { xbc|aba }
  //  => { xba|abc } - OK
  Rv(StackFixup::pu2xc(i, j, k));
}

void pattern3_ppp(StackFixup &Rv, unsigned i, unsigned j, unsigned k) {
  // No problems for three pushes
  // { abc| } => { abc|abc }
  Rv(StackFixup::push3(i, j, k));
}

} // namespace StackPatterns

} // namespace llvm
