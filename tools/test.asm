#200	#00FF	HIGH
#202	#00E0	CLS
#204	#6D00	LD VD, 0
#206	#6E00	LD VE, 0
#208	#A237	LD I, #237
#20A	#9DE2	PRT VD,VE
#20C	#F20A	LD V2, K
#20E	#A242	LD I, #242
#210	#7E08	ADD VE, 8
#212	#9DE2	PRT VD,VE
#214	#7E08	ADD VE, 8
#216	#6402	LD V4, 2
#218	#A234	LD I, #234
#21A	#F233	LD B, V2
#21C	#6300	LD V3, 0
;---  JP target ---
#21E	#7301	ADD V3, 1
#220	#A234	LD I, #234
#222	#F31E	ADD I, V3
#224	#F065	LD V0, [I]
#226	#F029	LD F, V0
#228	#DDE5	DRW VD, VE, 5
#22A	#7D04	ADD VD, 4
#22C	#74FF	ADD V4, 255
#22E	#3400	SE V4, 0
#230	#121E	JP #21E
#232	#00FD	EXIT
;-------------------------
;-  LD I, NNN addresses  -
;-------------------------
#237	DW	#616E, #7920, #6B65, #792E, #2E2E, #006F, #6B00, #FF00, 
#242	DW	
#234	DW	#0000, #0061, 
;-------------------------
;-      NOT DECODED      -
;-------------------------

