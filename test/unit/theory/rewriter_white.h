/*********************                                                        */
/*! \file rewriter_white.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andres Noetzli
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief [[ Add one-line brief description here ]]
 **
 ** [[ Add lengthier description here ]]
 ** \todo document this file
 **/

#include <cxxtest/TestSuite.h>

#include <vector>

#include "expr/node.h"
#include "expr/node_manager.h"
#include "expr/proof.h"
#include "expr/proof_node_manager.h"
#include "expr/term_conversion_proof_generator.h"
#include "smt/smt_engine.h"
#include "smt/smt_engine_scope.h"
#include "theory/bv/theory_bv_utils.h"
#include "theory/evaluator.h"
#include "theory/rewriter.h"
#include "theory/theory_engine.h"
#include "theory/theory_test_utils.h"
#include "util/rational.h"

using namespace CVC4;
using namespace CVC4::kind;
using namespace CVC4::smt;
using namespace CVC4::theory;

using namespace std;

class RewriterWhite : public CxxTest::TestSuite
{
  ExprManager* d_em;
  NodeManager* d_nm;
  SmtEngine* d_smt;
  SmtScope* d_scope;
  ProofNodeManager* d_pnm;
  TermConversionProofGenerator* d_tcpg;

 public:
  RewriterWhite() {}

  void setUp() override
  {
    Options opts;
    opts.setOutputLanguage(language::output::LANG_SMTLIB_V2);
    d_em = new ExprManager(opts);
    d_nm = NodeManager::fromExprManager(d_em);
    d_smt = new SmtEngine(d_em);
    d_smt->setOption("dag-thresh", "0");
    d_smt->setOption("proof-new", "true");
    d_scope = new SmtScope(d_smt);
    d_smt->finalOptionsAreSet();
    d_pnm = new ProofNodeManager(d_smt->getTheoryEngine()->d_pchecker.get());
    d_tcpg = new TermConversionProofGenerator(d_pnm);
  }

  void tearDown() override
  {
    delete d_tcpg;
    delete d_pnm;
    delete d_scope;
    delete d_smt;
    delete d_em;
  }

  void rewriteWithProof(Node t)
  {
    Node tr = Rewriter::rewriteWithProof(t, d_tcpg);
    std::shared_ptr<ProofNode> pn =
        d_tcpg->getProofFor(d_nm->mkNode(kind::EQUAL, t, tr));
    std::cout << t << " ---> " << tr << std::endl;
    pn->printDebug(std::cout);
    TS_ASSERT(pn->isClosed());
  }

  void testSimple()
  {
    TypeNode bv64Type = d_nm->mkBitVectorType(64);
    Node zero = d_nm->mkConst(BitVector(64, (unsigned int)0));
    Node x = d_nm->mkVar("x", bv64Type);
    Node y = d_nm->mkVar("y", d_nm->booleanType());

    Node t =
        d_nm->mkNode(kind::AND, d_nm->mkNode(kind::BITVECTOR_UGE, x, zero), y);
    rewriteWithProof(t);
  }

  Node boolToBv(Node b)
  {
    return d_nm->mkNode(ITE,
                        b,
                        d_nm->mkConst(BitVector(1, 1u)),
                        d_nm->mkConst(BitVector(1, 0u)));
  }

  void testRewriteToFixpoint()
  {
    TypeNode boolType = d_nm->booleanType();
    TypeNode bvType = d_nm->mkBitVectorType(1);

    Node zero = d_nm->mkConst(BitVector(1, 0u));
    Node b1 = d_nm->mkVar("b1", boolType);
    Node b2 = d_nm->mkVar("b2", boolType);
    Node b3 = d_nm->mkVar("b3", boolType);
    Node bv = d_nm->mkVar("bv", bvType);

    Node t = d_nm->mkNode(
        BITVECTOR_ITE,
        boolToBv(b1),
        d_nm->mkNode(BITVECTOR_ITE,
                     boolToBv(b2),
                     d_nm->mkNode(BITVECTOR_ITE, boolToBv(b3), zero, bv),
                     bv),
        bv);
    rewriteWithProof(t);
  }

  void testRewriteAgainFull()
  {
    TypeNode intType = d_nm->integerType();
    TypeNode strType = d_nm->stringType();

    Node x = d_nm->mkVar("x", strType);
    Node y = d_nm->mkVar("y", strType);
    Node n = d_nm->mkVar("n", intType);
    Node m = d_nm->mkVar("m", intType);
    Node emp = d_nm->mkConst(String(""));
    Node a = d_nm->mkConst(String("A"));
    Node abc = d_nm->mkConst(String("ABC"));

    Node zero = d_nm->mkConst(Rational(0));
    Node one = d_nm->mkConst(Rational(1));

    Node t = d_nm->mkNode(
        STRING_STRCTN,
        abc,
        d_nm->mkNode(
            STRING_SUBSTR, d_nm->mkNode(STRING_CONCAT, x, a), zero, one));
    rewriteWithProof(t);
  }

  void testCache()
  {
    TypeNode bv64Type = d_nm->mkBitVectorType(64);
    Node zero = d_nm->mkConst(BitVector(64, (unsigned int)0));
    Node x = d_nm->mkVar("x", bv64Type);
    Node y = d_nm->mkVar("y", d_nm->booleanType());

    Node t = d_nm->mkNode(
        kind::OR,
        d_nm->mkNode(
            kind::AND, d_nm->mkNode(kind::BITVECTOR_UGE, x, zero), y, y),
        y);

    // Make sure that the cache is warm
    Rewriter::rewrite(t);
    rewriteWithProof(t);
  }
};
