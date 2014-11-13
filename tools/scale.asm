;---  JP target ---
#200	#6300	LD V3, 0
#202	#640A	LD V4, 10
;---  JP target ---
#204	#2226	CALL #226
#206	#9341	TONE V3,V4
#208	#600A	LD V0, 10
#20A	#F015	LD DT, V0
#20C	#2232	CALL #232
#20E	#7301	ADD V3, 1
#210	#3310	SE V3, 16
#212	#1204	JP #204
;---  JP target ---
#214	#73FF	ADD V3, 255
#216	#43FF	SNE V3, 255
#218	#1200	JP #200
#21A	#2226	CALL #226
#21C	#9341	TONE V3,V4
#21E	#600A	LD V0, 10
#220	#F015	LD DT, V0
#222	#2232	CALL #232
#224	#1214	JP #214
;--- JP or CALL target ---
#226	#00E0	CLS
#228	#6E00	LD VE, 0
#22A	#6D00	LD VD, 0
#22C	#F329	LD F, V3
#22E	#DED5	DRW VE, VD, 5
#230	#00EE	RET
;--- JP or CALL target ---
#232	#F007	LD V0, DT
#234	#3000	SE V0, 0
#236	#1232	JP #232
#238	#00EE	RET
;-------------------------
;-  LD I, NNN addresses  -
;-------------------------
;-------------------------
;-      NOT DECODED      -
;-------------------------
#23A			.DB #FF

