(set-logic ALL)
(set-option :sygus-inference true)
(set-option :sygus-sym-break false)
(set-option :sygus-sym-break-lazy false)
(set-option :sygus-sym-break-rlv false)
(set-info :status sat)
(declare-fun s () String)
(assert (distinct s ""))
(check-sat)
