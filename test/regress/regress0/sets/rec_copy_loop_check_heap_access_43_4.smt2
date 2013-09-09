(set-option :print-success false)
(set-logic AUFLIA_SETS)
(set-info :status unsat)
(declare-sort Loc 0)
(define-sort SetLoc () (Set Loc))
(define-sort SetInt () (Set Int))
(declare-sort FldLoc 0)
(declare-sort FldInt 0)
(declare-fun null$0 () Loc)
(declare-fun read$0 (FldLoc Loc) Loc)
(declare-fun Btwn$0 (FldLoc Loc Loc Loc) Bool)
(declare-fun Alloc$0 () SetLoc)
(declare-fun Alloc_2$0 () SetLoc)
(declare-fun FP$0 () SetLoc)
(declare-fun FP_3$0 () SetLoc)
(declare-fun FP_Caller$0 () SetLoc)
(declare-fun FP_Caller_2$0 () SetLoc)
(declare-fun cp$0 () Loc)
(declare-fun cp_1$0 () Loc)
(declare-fun curr$0 () Loc)
(declare-fun lseg_domain$0 (FldLoc Loc Loc) SetLoc)
(declare-fun lseg_struct$0 (SetLoc FldLoc Loc Loc) Bool)
(declare-fun next$0 () FldLoc)
(declare-fun old_cp_2$0 () Loc)
(declare-fun sk_?X_36$0 () SetLoc)
(declare-fun sk_?X_37$0 () SetLoc)
(declare-fun sk_?X_38$0 () SetLoc)
(declare-fun tmp_1$0 () Loc)

(assert (! (forall ((?y Loc))
           (or (not (Btwn$0 next$0 null$0 ?y ?y)) (= null$0 ?y)
               (Btwn$0 next$0 null$0 (read$0 next$0 null$0) ?y)))
   :named btwn_reach_6))

(assert (! (forall ((?y Loc))
           (or (not (= (read$0 next$0 null$0) null$0))
               (not (Btwn$0 next$0 null$0 ?y ?y)) (= null$0 ?y)))
   :named btwn_cycl_6))

(assert (! (Btwn$0 next$0 null$0 (read$0 next$0 null$0) (read$0 next$0 null$0))
   :named btwn_step_6))

(assert (! (= (read$0 next$0 null$0) null$0) :named read_null_6))

(assert (! (forall ((l1 Loc))
           (or
               (and (Btwn$0 next$0 curr$0 l1 null$0)
                    (in l1 (lseg_domain$0 next$0 curr$0 null$0))
                    (not (= l1 null$0)))
               (and (or (= l1 null$0) (not (Btwn$0 next$0 curr$0 l1 null$0)))
                    (not (in l1 (lseg_domain$0 next$0 curr$0 null$0))))))
   :named lseg_footprint_14))

(assert (! (not (in tmp_1$0 Alloc$0)) :named new_42_10))

(assert (! (not (in null$0 Alloc$0))
   :named initial_footprint_of_rec_copy_loop_34_11_4))

(assert (! (not (= curr$0 null$0)) :named if_else_37_6))

(assert (! (lseg_struct$0 sk_?X_37$0 next$0 curr$0 null$0)
   :named precondition_of_rec_copy_loop_34_11_16))

(assert (! (= sk_?X_38$0 (lseg_domain$0 next$0 cp$0 null$0))
   :named precondition_of_rec_copy_loop_34_11_17))

(assert (! (= sk_?X_36$0 FP$0) :named precondition_of_rec_copy_loop_34_11_18))

(assert (! (= (as emptyset SetLoc) (intersection sk_?X_38$0 sk_?X_37$0))
   :named precondition_of_rec_copy_loop_34_11_19))

(assert (! (= old_cp_2$0 cp$0) :named assign_41_4))

(assert (! (= FP_Caller_2$0 (setminus FP_Caller$0 FP$0)) :named assign_37_2_2))

(assert (! (= Alloc_2$0 (union Alloc$0 (setenum tmp_1$0))) :named assign_42_10))

(assert (! (or (Btwn$0 next$0 cp$0 null$0 null$0)
       (not (lseg_struct$0 sk_?X_38$0 next$0 cp$0 null$0)))
   :named unnamed_22))

(assert (! (forall ((l1 Loc))
           (or
               (and (Btwn$0 next$0 cp$0 l1 null$0)
                    (in l1 (lseg_domain$0 next$0 cp$0 null$0))
                    (not (= l1 null$0)))
               (and (or (= l1 null$0) (not (Btwn$0 next$0 cp$0 l1 null$0)))
                    (not (in l1 (lseg_domain$0 next$0 cp$0 null$0))))))
   :named lseg_footprint_15))

(assert (! (not (in cp_1$0 FP_3$0)) :named check_heap_access_43_4))

(assert (! (not (= tmp_1$0 null$0)) :named new_42_10_1))

(assert (! (lseg_struct$0 sk_?X_38$0 next$0 cp$0 null$0)
   :named precondition_of_rec_copy_loop_34_11_20))

(assert (! (= FP_Caller$0 (union FP$0 FP_Caller$0))
   :named precondition_of_rec_copy_loop_34_11_21))

(assert (! (= sk_?X_37$0 (lseg_domain$0 next$0 curr$0 null$0))
   :named precondition_of_rec_copy_loop_34_11_22))

(assert (! (= sk_?X_36$0 (union sk_?X_37$0 sk_?X_38$0))
   :named precondition_of_rec_copy_loop_34_11_23))

(assert (! (= Alloc$0 (union FP_Caller$0 Alloc$0))
   :named initial_footprint_of_rec_copy_loop_34_11_5))

(assert (! (= cp_1$0 tmp_1$0) :named assign_42_4))

(assert (! (= FP_3$0 (union FP$0 (setenum tmp_1$0))) :named assign_42_10_1))

(assert (! (or (Btwn$0 next$0 curr$0 null$0 null$0)
       (not (lseg_struct$0 sk_?X_37$0 next$0 curr$0 null$0)))
   :named unnamed_23))

(assert (! (forall ((?x Loc)) (Btwn$0 next$0 ?x ?x ?x)) :named btwn_refl_6))

(assert (! (forall ((?x Loc) (?y Loc)) (or (not (Btwn$0 next$0 ?x ?y ?x)) (= ?x ?y)))
   :named btwn_sndw_6))

(assert (! (forall ((?x Loc) (?y Loc) (?z Loc))
           (or (not (Btwn$0 next$0 ?x ?y ?y)) (not (Btwn$0 next$0 ?x ?z ?z))
               (Btwn$0 next$0 ?x ?y ?z) (Btwn$0 next$0 ?x ?z ?y)))
   :named btwn_ord1_6))

(assert (! (forall ((?x Loc) (?y Loc) (?z Loc))
           (or (not (Btwn$0 next$0 ?x ?y ?z))
               (and (Btwn$0 next$0 ?x ?y ?y) (Btwn$0 next$0 ?y ?z ?z))))
   :named btwn_ord2_6))

(assert (! (forall ((?x Loc) (?y Loc) (?z Loc))
           (or (not (Btwn$0 next$0 ?x ?y ?y)) (not (Btwn$0 next$0 ?y ?z ?z))
               (Btwn$0 next$0 ?x ?z ?z)))
   :named btwn_trn1_6))

(assert (! (forall ((?u Loc) (?x Loc) (?y Loc) (?z Loc))
           (or (not (Btwn$0 next$0 ?x ?y ?z)) (not (Btwn$0 next$0 ?y ?u ?z))
               (and (Btwn$0 next$0 ?x ?y ?u) (Btwn$0 next$0 ?x ?u ?z))))
   :named btwn_trn2_6))

(assert (! (forall ((?u Loc) (?x Loc) (?y Loc) (?z Loc))
           (or (not (Btwn$0 next$0 ?x ?y ?z)) (not (Btwn$0 next$0 ?x ?u ?y))
               (and (Btwn$0 next$0 ?x ?u ?z) (Btwn$0 next$0 ?u ?y ?z))))
   :named btwn_trn3_6))

(check-sat)
(exit)
