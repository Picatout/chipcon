#200	#00FF	HIGH
#202	#00E0	CLS
#204	#A20E	LD I, #20E
#206	#F465	LD V4, [I]
#208	#A212	LD I, #212
#20A	#9342	PRT V3,V4
#20C	#00FD	EXIT
;-------------------------
;-  LD I, NNN addresses  -
;-------------------------
#20E	DB	#00, #00, #20, #1B
#212	DW	#6865, #6C6C, #6F20, #776F, #726C, #6421, #00FF, 
;-------------------------
;-      NOT DECODED      -
;-------------------------

