#200	#00FF	HIGH
#202	#00E0	CLS
#204	#601E	LD V0, 30
#206	#6120	LD V1, 32
#208	#A23E	LD I, #23E
#20A	#9012	PRT V0,V1
#20C	#640F	LD V4, 15
#20E	#650A	LD V5, 10
;---  JP target ---
#210	#9451	TONE V4,V5
#212	#660A	LD V6, 10
#214	#F615	LD DT, V6
#216	#2236	CALL #236
#218	#74FF	ADD V4, 255
#21A	#34FF	SE V4, 255
#21C	#1210	JP #210
;---  JP target ---
#21E	#00C1	SCD 1
#220	#660A	LD V6, 10
#222	#F615	LD DT, V6
#224	#2236	CALL #236
#226	#71FF	ADD V1, 255
#228	#3100	SE V1, 0
#22A	#121E	JP #21E
;---  JP target ---
#22C	#C47F	RND V4, #7F
#22E	#C53F	RND V5, #3F
#230	#9453	PIXI V4,V5
#232	#122C	JP #22C
;--- JP or CALL target ---
#236	#F607	LD V6, DT
#238	#3600	SE V6, 0
#23A	#1236	JP #236
#23C	#00EE	RET
;-------------------------
;-  LD I, NNN addresses  -
;-------------------------
#23E	DW	#6865, #6C6C, #6F20, #776F, #726C, #6421, #00FF, 
;-------------------------
;-      NOT DECODED      -
;-------------------------

