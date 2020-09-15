/*********************                                                        */
/*! \file lazy_proof_chain.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Haniel Barbosa
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Lazy proof chain utility
 **/

#include "cvc4_private.h"

#ifndef CVC4__EXPR__LAZY_PROOF_CHAIN_H
#define CVC4__EXPR__LAZY_PROOF_CHAIN_H

#include <unordered_map>
#include <vector>

#include "context/cdhashmap.h"
#include "expr/proof_generator.h"
#include "expr/proof_node_manager.h"

namespace CVC4 {

/**
 * A (context-dependent) lazy generator for proof chains. This class is an
 * extension of ProofGenerator that additionally that maps facts to proof
 * generators in a context-dependent manner. The map is built with the addition
 * of lazy steps mapping facts to proof generators. More importantly, its
 * getProofFor method expands the proof generators registered to this class by
 * connecting, for the proof generated to one fact, assumptions to the proofs
 * generated for those assumptinos that are registered in the chain.
 */
class LazyCDProofChain : public ProofGenerator
{
 public:
  /** Constructor
   *
   * @param pnm The proof node manager for constructing ProofNode objects.
   * @param c The context that this class depends on. If none is provided,
   * this class is context-independent.
   */
  LazyCDProofChain(ProofNodeManager* pnm, context::Context* c = nullptr);
  ~LazyCDProofChain();
  /**
   * Get lazDyet have a concrete proof and that are registered in the chains
   * (i.e., are in the domain of d_gens). Starting with the proof generated for
   * fact, if any.
   *
   * For example, if d_gens consists of the following pairs
   *
   * --- (A, PG1)
   * --- (B, PG2)
   * --- (C, PG3)
   *
   * and getProofFor(A) is called, with PG1 generating a proof with assumptions
   * B and D, then B is expanded, with its assumption proof node being updated
   * to the expanded proof node, while D is not. Assuming PG2 provides a proof
   * with assumptions C and E, then C is expanded and E is not. Suppose PG3
   * gives a closed proof, thus the call getProofFor(A) produces a proof node
   *
   *  A : ( ... B : ( ... C : (...), ... ASSUME( E ) ), ... ASSUME( D ) )
   *
   * Note that the expansions are done directly on the proof nodes produced by
   * the generators.
   */
  std::shared_ptr<ProofNode> getProofFor(Node fact) override;
  /** Add step by generator
   *
   * This method stores that expected can be proven by proof generator pg if
   * it is required to do so. This mapping is maintained in the remainder of
   * the current context (according to the context c provided to this class).
   *
   * Moreover the lazy steps of this class are expected to fulfill the
   * requirement that pg.getProofFor(expected) generates a proof node closed
   * with relation to
   *  (1) a fixed set F1, ..., Fn,
   *  (2) formulas in the current domain of d_gens.
   *
   * This is so that we only add links to the chain that depend on a fixed set
   * of assumptions or in other links.
   *
   * @param expected The fact that can be proven.
   * @param pg The generator that can proof expected.
   * @param assumptions The fixed set of assumptions with relation to which the
   * chain, now augmented with expect, must be closed.
   * @param ctx The context we are in (for debugging).
   */
  void addLazyStep(Node expected,
                   ProofGenerator* pg,
                   const std::vector<Node>& assumptions,
                   const char* ctx = "LazyCDProofChain::addLazyStep");

  /** As above but does not do the closedness check. */
  void addLazyStep(Node expected, ProofGenerator* pg);

  /** Does the given fact have an explicitly provided generator? */
  bool hasGenerator(Node fact) const;

  /**
   * Get generator for fact, or nullptr if it doesnt exist.
   */
  ProofGenerator* getGeneratorFor(Node fact);

  /** identify */
  std::string identify() const override;

 private:
  /** The proof manager, used for allocating new ProofNode objects */
  ProofNodeManager* d_manager;
  /** A dummy context used by this class if none is provided */
  context::Context d_context;
  /** Maps facts that can be proven to generators */
  context::CDHashMap<Node, ProofGenerator*, NodeHashFunction> d_gens;
};

}  // namespace CVC4

#endif /* CVC4__EXPR__LAZY_PROOF_CHAIN_H */