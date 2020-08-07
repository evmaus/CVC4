/*********************                                                        */
/*! \file term_context.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Term context utilities.
 **/

#include "cvc4_private.h"

#ifndef CVC4__EXPR__TERM_CONTEXT_H
#define CVC4__EXPR__TERM_CONTEXT_H

#include "expr/node.h"

namespace CVC4 {

/**
 * This is an abstract class for computing "term context identifiers". A term
 * context identifier is a hash value that identifies some property of the
 * context in which a term occurs. Common examples of the implementation of
 * such a mapping are implemented in the subclasses below.
 * 
 * In the below documentation, we write t @ [p] to refer to a
 * term-context-sensitive term, where p is a list of indices corresponding to
 * its position in another term. For example, the atomic subterms of:
 *   (and P (not Q))
 * are P @ [0] and Q @ [1,0].
 */
class TermContext
{
 public:
  TermContext() {}
  virtual ~TermContext() {}
  /** Initial value */
  virtual uint32_t initialValue() const = 0;
  /**
   * Compute the term context identifier of the index^th child of t, where tval
   * is the term context identifier of t.
   */
  virtual uint32_t computeValue(TNode t, uint32_t tval, size_t index) const = 0;
};

/**
 * Computes whether we are inside a term (as opposed to being part of Boolean
 * skeleton) and whether we are inside a quantifier. For example, for:
 *   (and (= a b) (forall ((x Int)) (P x)))
 * we have the following mappings (term -> inTerm,inQuant)
 *   (= a b) @ [0] -> false, false
 *   a @ [0,1] -> true, false
 *   (P x) @ [1,1] -> false, true
 *    x @ [1,1,0] -> true, true
 * Notice that the hash of a child can be computed from the parent's hash only,
 * and hence this can be implemented without considering paths.
 */
class RtfTermContext : public TermContext
{
 public:
  RtfTermContext(){}
  /** The initial value */
  uint32_t initialValue() const override;
  /** Compute the value */
  uint32_t computeValue(TNode t, uint32_t tval, size_t index) const override;
  /** get value */
  static uint32_t getValue(bool inQuant, bool inTerm);
  /** get flags */
  static void getFlags(uint32_t val, bool& inQuant, bool& inTerm);
 private:
  /**
   * Returns true if the children of t should be considered in a "term" context.
   */
  static bool hasNestedTermChildren(TNode t);
};

/**
 * Computes the polarity of a term-context-sensitive term, which is one of
 * {true, false, none}. This corresponds to the value that can be assigned to
 * that term while preservering satisfiability of the overall formula. This
 * typically corresponds to the number of NOT the formula is beneath, although
 * special cases exist (e.g. IMPLIES).
 *
 * For example, given the formula:
 *   (and P (not (= (f x) 0)))
 * assuming the root of this formula has positive polarity, we have that:
 *   P @ [0] -> true
 *   (not (= (f x) 0)) @ [1] -> true
 *   (= (f x) 0) @ [1,0] -> false
 *   (f x) @ [1,0,0]), x @ [1,0,0,0]), 0 @ [1,0,1] -> none
 *
 * Notice that a term-context-sensitive Node is not one-to-one with Node.
 * In particular, given the formula:
 *   (and P (not P))
 * We have that the P at path [0] has polarity true and the P at path [1,0] has
 * polarity false.
 * 
 * Finally, notice that polarity does not correspond to an entailed value. Thus,
 * for the formula:
 *   (or P Q)
 * we have that
 *   P @ [0] -> true
 *   Q @ [1] -> true
 * although neither is entailed.
 * 
 * Notice that the hash of a child can be computed from the parent's hash only,
 * and hence this can be implemented without considering paths.
 */
class PolarityTermContext : public TermContext
{
 public:
  PolarityTermContext(){}
  /** Compute the value */
  uint32_t computeValue(TNode t, uint32_t tval, size_t index) const override;
  /** get value */
  static uint32_t getValue(bool hasPol, bool pol);
  /** get flags */
  static void getFlags(uint32_t val, bool& hasPol, bool& pol);
};

}  // namespace CVC4

#endif /* CVC4__EXPR__TERM_CONVERSION_PROOF_GENERATOR_H */
