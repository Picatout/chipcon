#200	#00FF	HIGH
#202	#00E0	CLS
#204	#6900	LD V9, 0
#206	#6801	LD V8, 1
#208	#6E38	LD VE, 56
#20A	#6D00	LD VD, 0
#20C	#6C00	LD VC, 0
#20E	#6B00	LD VB, 0
#210	#6A64	LD VA, 100
#212	#2308	CALL #308
#214	#22D4	CALL #2D4
#216	#2346	CALL #346
;---  JP target ---
#218	#4A00	SNE VA, 0
#21A	#124C	JP #24C
#21C	#6004	LD V0, 4
#21E	#E0A1	SKNP V0
#220	#1230	JP #230
#222	#6006	LD V0, 6
#224	#E0A1	SKNP V0
#226	#1238	JP #238
#228	#6002	LD V0, 2
#22A	#E0A1	SKNP V0
#22C	#1240	JP #240
#22E	#124C	JP #24C
;--- JP or CALL target ---
#230	#2346	CALL #346
#232	#7CFF	ADD VC, 255
#234	#7AFF	ADD VA, 255
#236	#124A	JP #24A
;--- JP or CALL target ---
#238	#2346	CALL #346
#23A	#7C01	ADD VC, 1
#23C	#7AFF	ADD VA, 255
#23E	#124A	JP #24A
;--- JP or CALL target ---
#240	#2346	CALL #346
#242	#7BFE	ADD VB, 254
#244	#3A01	SE VA, 1
#246	#7AFF	ADD VA, 255
#248	#7AFF	ADD VA, 255
;--- JP or CALL target ---
#24A	#2346	CALL #346
;--- JP or CALL target ---
#24C	#2346	CALL #346
#24E	#22D4	CALL #2D4
#250	#8EC4	ADD VE, VC
#252	#607F	LD V0, 127
#254	#8E02	AND VE, V0
#256	#8DB4	ADD VD, VB
#258	#7B01	ADD VB, 1
#25A	#22D4	CALL #2D4
#25C	#4F01	SNE VF, 1
#25E	#128C	JP #28C
#260	#2346	CALL #346
#262	#4A00	SNE VA, 0
#264	#126C	JP #26C
#266	#600A	LD V0, 10
#268	#2282	CALL #282
#26A	#1218	JP #218
;--- JP or CALL target ---
#26C	#2346	CALL #346
#26E	#600A	LD V0, 10
#270	#6105	LD V1, 5
#272	#9011	TONE V0,V1
#274	#6005	LD V0, 5
#276	#2282	CALL #282
#278	#2346	CALL #346
#27A	#6005	LD V0, 5
#27C	#9011	TONE V0,V1
#27E	#2282	CALL #282
#280	#1218	JP #218
;--- JP or CALL target ---
#282	#F015	LD DT, V0
;---  JP target ---
#284	#F007	LD V0, DT
#286	#3000	SE V0, 0
#288	#1284	JP #284
#28A	#00EE	RET
;--- JP or CALL target ---
#28C	#2346	CALL #346
#28E	#6002	LD V0, 2
#290	#80B5	SUB V0, VB
#292	#3F01	SE VF, 1
#294	#12BA	JP #2BA
#296	#6002	LD V0, 2
#298	#80C5	SUB V0, VC
#29A	#3F01	SE VF, 1
#29C	#12BA	JP #2BA
#29E	#80E0	LD V0, VE
#2A0	#8075	SUB V0, V7
#2A2	#3F01	SE VF, 1
#2A4	#12BA	JP #2BA
#2A6	#8070	LD V0, V7
#2A8	#7010	ADD V0, 16
#2AA	#80E7	SUBN V0, VE
#2AC	#3F00	SE VF, 0
#2AE	#12BA	JP #2BA
#2B0	#A399	LD I, #399
#2B2	#6100	LD V1, 0
#2B4	#6210	LD V2, 16
#2B6	#9122	PRT V1,V2
#2B8	#12C2	JP #2C2
;--- JP or CALL target ---
#2BA	#A389	LD I, #389
#2BC	#6100	LD V1, 0
#2BE	#6210	LD V2, 16
#2C0	#9122	PRT V1,V2
;--- JP or CALL target ---
#2C2	#22DA	CALL #2DA
#2C4	#603C	LD V0, 60
#2C6	#2282	CALL #282
#2C8	#F00A	LD V0, K
;---  JP target ---
#2CA	#E0A1	SKNP V0
#2CC	#12CA	JP #2CA
;---  JP target ---
#2CE	#12CE	JP #2CE
;--- JP or CALL target ---
#2D4	#A364	LD I, #364
#2D6	#DED0	DRW VE, VD, 0
#2D8	#00EE	RET
;--- JP or CALL target ---
#2DA	#6100	LD V1, 0
#2DC	#620A	LD V2, 10
#2DE	#A3A9	LD I, #3A9
#2E0	#F933	LD B, V9
#2E2	#22F2	CALL #2F2
#2E4	#A384	LD I, #384
#2E6	#D125	DRW V1, V2, 5
#2E8	#7104	ADD V1, 4
#2EA	#A3A9	LD I, #3A9
#2EC	#F833	LD B, V8
#2EE	#22F2	CALL #2F2
#2F0	#00EE	RET
;--- JP or CALL target ---
#2F2	#6300	LD V3, 0
;---  JP target ---
#2F4	#A3A9	LD I, #3A9
#2F6	#F31E	ADD I, V3
#2F8	#F065	LD V0, [I]
#2FA	#F029	LD F, V0
#2FC	#D125	DRW V1, V2, 5
#2FE	#7104	ADD V1, 4
#300	#7301	ADD V3, 1
#302	#3303	SE V3, 3
#304	#12F4	JP #2F4
#306	#00EE	RET
;--- JP or CALL target ---
#308	#6000	LD V0, 0
#30A	#613F	LD V1, 63
#30C	#C7F8	RND V7, #F8
#30E	#6370	LD V3, 112
#310	#8375	SUB V3, V7
#312	#3F01	SE VF, 1
#314	#6770	LD V7, 112
#316	#6410	LD V4, 16
#318	#8474	ADD V4, V7
;---  JP target ---
#31A	#8300	LD V3, V0
#31C	#8375	SUB V3, V7
#31E	#3F01	SE VF, 1
#320	#132A	JP #32A
#322	#8300	LD V3, V0
#324	#8345	SUB V3, V4
#326	#3F01	SE VF, 1
#328	#133C	JP #33C
;--- JP or CALL target ---
#32A	#413F	SNE V1, 63
#32C	#71FF	ADD V1, 255
#32E	#4120	SNE V1, 32
#330	#7101	ADD V1, 1
#332	#C303	RND V3, #03
#334	#8306	SHR V3
#336	#3300	SE V3, 0
#338	#71FE	ADD V1, 254
#33A	#7101	ADD V1, 1
;--- JP or CALL target ---
#33C	#9013	PIXI V0,V1
#33E	#7001	ADD V0, 1
#340	#3080	SE V0, 128
#342	#131A	JP #31A
#344	#00EE	RET
;--- JP or CALL target ---
#346	#6100	LD V1, 0
#348	#6200	LD V2, 0
#34A	#6300	LD V3, 0
#34C	#A3A9	LD I, #3A9
#34E	#FA33	LD B, VA
;---  JP target ---
#350	#A3A9	LD I, #3A9
#352	#F31E	ADD I, V3
#354	#F065	LD V0, [I]
#356	#F030	LD LF, V0
#358	#D12A	DRW V1, V2, 10
#35A	#7108	ADD V1, 8
#35C	#7301	ADD V3, 1
#35E	#3303	SE V3, 3
#360	#1350	JP #350
#362	#00EE	RET
;-------------------------
;-  LD I, NNN addresses  -
;-------------------------
#364	DW	#03C0, #0660, #1FF8, #3FFC, #2004, #AFF5, #6C36, #2C34, #6FF6, #A005, #3FFC, #3FFC, #1290, #2448, #600C, #C006
#3A9	DW	#0000, #00FF, 
#399	DW	#636F, #6E67, #7261, #7475, #6C61, #7469, #6F6E, #2100, 
#389	DW	#796F, #7520, #6372, #6173, #6865, #6420, #4C45, #4D00, 
#384	DB	#20, #40, #40, #80, #80
;-------------------------
;-      NOT DECODED      -
;-------------------------

