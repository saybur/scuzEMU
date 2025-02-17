data 'MENU' (129, "File") {
	$"0081 0000 0000 0000 0000 FFFF FFE3 0446"            /* .Å.............F */
	$"696C 6507 4F70 656E 2E2E 2E00 4F00 0001"            /* ile.Open....O... */
	$"2D00 0000 0009 5570 6C6F 6164 2E2E 2E00"            /* -....ΔUpload.... */
	$"5500 0001 2D00 0000 0004 5175 6974 0051"            /* U...-.....Quit.Q */
	$"0000 00"                                            /* ... */
};

data 'MENU' (130, "Edit") {
	$"0082 0000 0000 0000 0000 FFFF FFFB 0445"            /* .Ç.............E */
	$"6469 7404 556E 646F 005A 0000 012D 0000"            /* dit.Undo.Z...-.. */
	$"0000 0343 7574 0058 0000 0443 6F70 7900"            /* ...Cut.X...Copy. */
	$"4300 0005 5061 7374 6500 5600 0005 436C"            /* C...Paste.V...Cl */
	$"6561 7200 0000 0000"                                /* ear..... */
};

data 'MENU' (128, "Apple") {
	$"0080 0000 0000 0000 0000 FFFF FFFB 0114"            /* .Ä.............. */
	$"1041 626F 7574 2073 6375 7A45 4D55 2E2E"            /* .About scuzEMU.. */
	$"2E00 0000 0001 2D00 0000 0000"                      /* ......-..... */
};

data 'DITL' (128, "About") {
	$"0003 0000 0000 00A4 010A 00B8 0144 0402"            /* .......§...∏.D.. */
	$"4F4B 0000 0000 000A 0014 002A 0034 A002"            /* OK.........*.4†. */
	$"0080 0000 0000 000A 004B 002A 0132 883A"            /* .Ä.......K.*.2à: */
	$"7363 757A 454D 552C 2061 2053 4353 4920"            /* scuzEMU, a SCSI  */
	$"456D 756C 6174 6F72 2055 7469 6C69 7479"            /* Emulator Utility */
	$"0D43 6F70 7972 6967 6874 2028 4329 2032"            /* ¬Copyright (C) 2 */
	$"3032 3420 7361 7962 7572 0000 0000 0037"            /* 024 saybur.....7 */
	$"0010 0097 0144 88CF 5468 6973 2070 726F"            /* ...ó.DàœThis pro */
	$"6772 616D 2063 6F6D 6573 2077 6974 6820"            /* gram comes with  */
	$"4142 534F 4C55 5445 4C59 204E 4F20 5741"            /* ABSOLUTELY NO WA */
	$"5252 414E 5459 2E20 466F 7220 6465 7461"            /* RRANTY. For deta */
	$"696C 7320 7365 6520 7468 6520 434F 5059"            /* ils see the COPY */
	$"494E 4720 6669 6C65 2E0D 0D54 6869 7320"            /* ING file.¬¬This  */
	$"6973 2066 7265 6520 736F 6674 7761 7265"            /* is free software */
	$"2C20 616E 6420 796F 7520 6172 6520 7765"            /* , and you are we */
	$"6C63 6F6D 6520 746F 2072 6564 6973 7472"            /* lcome to redistr */
	$"6962 7574 6520 6974 2075 6E64 6572 2063"            /* ibute it under c */
	$"6572 7461 696E 2063 6F6E 6469 7469 6F6E"            /* ertain condition */
	$"732E 2052 6566 6572 0D74 6F20 7468 6520"            /* s. Refer¬to the  */
	$"6162 6F76 6520 6669 6C65 2066 6F72 2064"            /* above file for d */
	$"6574 6169 6C73 2E80"                                /* etails.Ä */
};

data 'DITL' (257, "Bad System") {
	$"0001 0000 0000 003F 012A 0053 0164 0402"            /* .......?.*.S.d.. */
	$"4F4B 0000 0000 000A 004E 003B 0164 8868"            /* OK.......N.;.dàh */
	$"5379 7374 656D 2036 2E30 2E37 2061 6E64"            /* System 6.0.7 and */
	$"2065 6172 6C69 6572 206D 6179 2065 7870"            /*  earlier may exp */
	$"6572 6965 6E63 6520 6572 726F 7273 2E20"            /* erience errors.  */
	$"506C 6561 7365 2072 6570 6F72 7420 616E"            /* Please report an */
	$"7920 7072 6F62 6C65 6D73 2074 6F20 7468"            /* y problems to th */
	$"6520 6170 706C 6963 6174 696F 6E20 6465"            /* e application de */
	$"7665 6C6F 7065 722E"                                /* veloper. */
};

data 'DITL' (256, "Generic") {
	$"0001 0000 0000 0057 014C 006B 0186 0402"            /* .......W.L.k.Ü.. */
	$"4F4B 0000 0000 000A 004B 004A 0186 8804"            /* OK.......K.J.Üà. */
	$"5E30 5E31"                                          /* ^0^1 */
};

data 'DITL' (512, "Choose ID") {
	$"000E 0000 0000 00DC 00AA 00F0 00E6 0402"            /* .........™...... */
	$"4F4B 0000 0000 00DC 0061 00F0 009D 0406"            /* OK.......a...ù.. */
	$"4361 6E63 656C 0000 0000 0027 0014 0037"            /* Cancel.....'...7 */
	$"0064 0609 5343 5349 2049 4420 3000 0000"            /* .d.ΔSCSI ID 0... */
	$"0000 003F 0014 004F 0064 0609 5343 5349"            /* ...?...O.d.ΔSCSI */
	$"2049 4420 3100 0000 0000 0057 0014 0067"            /*  ID 1......W...g */
	$"0064 0609 5343 5349 2049 4420 3200 0000"            /* .d.ΔSCSI ID 2... */
	$"0000 006F 0014 007F 0064 0609 5343 5349"            /* ...o.....d.ΔSCSI */
	$"2049 4420 3300 0000 0000 0027 008C 0037"            /*  ID 3......'.å.7 */
	$"00DC 0609 5343 5349 2049 4420 3400 0000"            /* ...ΔSCSI ID 4... */
	$"0000 003F 008C 004F 00DC 0609 5343 5349"            /* ...?.å.O...ΔSCSI */
	$"2049 4420 3500 0000 0000 0057 008C 0067"            /*  ID 5......W.å.g */
	$"00DC 0609 5343 5349 2049 4420 3600 0000"            /* ...ΔSCSI ID 6... */
	$"0000 00B7 0014 00C7 0064 0605 4669 6C65"            /* ...∑...«.d..File */
	$"7353 0000 0000 00B7 008C 00C7 00DC 0606"            /* sS.....∑.å.«.... */
	$"496D 6167 6573 0000 0000 000A 0014 001A"            /* Images.......... */
	$"00AA 8815 4F70 656E 2053 4353 4920 456D"            /* .™à.Open SCSI Em */
	$"756C 6174 6F72 2E2E 2E9A 0000 0000 009D"            /* ulator...ö.....ù */
	$"0014 00AD 0078 880E 4765 7420 4C69 7374"            /* ...≠.xà.Get List */
	$"206F 662E 2E2E 0000 0000 008F 0014 0090"            /*  of........è...ê */
	$"00DC 8000 0000 0000 00D4 00A2 00F8 00EE"            /* ..Ä......‘.¢.... */
	$"8000"                                               /* Ä. */
};

data 'DITL' (258, "SCSI Error") {
	$"0001 0000 0000 0057 0124 006B 015E 0402"            /* .......W.$.k.^.. */
	$"4F4B 0000 0000 000A 004B 004A 015E 8804"            /* OK.......K.J.^à. */
	$"5E30 5E31"                                          /* ^0^1 */
};

data 'DITL' (129, "Image Change") {
	$"0001 0000 0000 0057 00F2 006B 012C 0402"            /* .......W...k.,.. */
	$"4F4B 0000 0000 000A 0054 004A 012C 884C"            /* OK.......T.J.,àL */
	$"4E65 7874 2064 6973 6B20 696D 6167 6520"            /* Next disk image  */
	$"7570 6461 7465 642E 0D0D 5265 6D6F 7665"            /* updated.¬¬Remove */
	$"2074 6865 2063 7572 7265 6E74 2064 6973"            /*  the current dis */
	$"6B20 6F6E 2074 6865 2044 6573 6B74 6F70"            /* k on the Desktop */
	$"2074 6F20 7072 6F63 6565 642E"                      /*  to proceed. */
};

data 'DITL' (130, "No Images Returned") {
	$"0001 0000 0000 0057 00FC 006B 0136 0402"            /* .......W...k.6.. */
	$"4F4B 0000 0000 000A 0054 004A 0136 887C"            /* OK.......T.J.6à| */
	$"4465 7669 6365 2072 6573 706F 6E64 6564"            /* Device responded */
	$"2077 6974 6820 616E 2065 6D70 7479 2069"            /*  with an empty i */
	$"6D61 6765 206C 6973 742E 2056 6572 6966"            /* mage list. Verif */
	$"7920 616E 2069 6D61 6765 2066 6F6C 6465"            /* y an image folde */
	$"7220 6C69 6B65 2022 4344 5E30 2220 6578"            /* r like "CD^0" ex */
	$"6973 7473 206F 6E20 7468 6520 6D65 6D6F"            /* ists on the memo */
	$"7279 2063 6172 6420 616E 6420 6861 7320"            /* ry card and has  */
	$"6669 6C65 7320 696E 2069 742E"                      /* files in it. */
};

data 'DITL' (131, "No Files Returned") {
	$"0001 0000 0000 0057 00FC 006B 0136 0402"            /* .......W...k.6.. */
	$"4F4B 0000 0000 000A 0054 004A 0136 887B"            /* OK.......T.J.6à{ */
	$"4465 7669 6365 2072 6573 706F 6E64 6564"            /* Device responded */
	$"2077 6974 6820 616E 2065 6D70 7479 2066"            /*  with an empty f */
	$"696C 6520 6C69 7374 2E20 5665 7269 6679"            /* ile list. Verify */
	$"2061 2064 6972 6563 746F 7279 2063 616C"            /*  a directory cal */
	$"6C65 6420 2273 6861 7265 6422 2065 7869"            /* led "shared" exi */
	$"7374 7320 6F6E 2074 6865 206D 656D 6F72"            /* sts on the memor */
	$"7920 6361 7264 2061 6E64 2068 6173 2066"            /* y card and has f */
	$"696C 6573 2069 6E20 6974 2E00"                      /* iles in it.. */
};

data 'DITL' (132, "Duplicate Files") {
	$"0002 0000 0000 0057 00FC 006B 0136 0402"            /* .......W...k.6.. */
	$"4E6F 0000 0000 0057 00B5 006B 00EF 0403"            /* No.....W.µ.k.... */
	$"5965 7320 0000 0000 000A 0054 004A 0136"            /* Yes .......T.J.6 */
	$"886C 4F6E 6520 6F72 206D 6F72 6520 6669"            /* àlOne or more fi */
	$"6C65 7320 696E 2074 6865 2074 6172 6765"            /* les in the targe */
	$"7420 6469 7265 6374 6F72 7920 6861 7665"            /* t directory have */
	$"206D 6174 6368 696E 6720 6669 6C65 206E"            /*  matching file n */
	$"616D 6573 2E20 576F 756C 6420 796F 7520"            /* ames. Would you  */
	$"6C69 6B65 2074 6F20 6F76 6572 7772 6974"            /* like to overwrit */
	$"6520 7468 6F73 6520 6669 6C65 733F"                 /* e those files? */
};

data 'DITL' (259, "File Error") {
	$"0001 0000 0000 0057 0124 006B 015E 0402"            /* .......W.$.k.^.. */
	$"4F4B 0000 0000 000A 004B 004A 015E 8804"            /* OK.......K.J.^à. */
	$"5E30 5E31"                                          /* ^0^1 */
};

data 'DITL' (1000, "Memory Fail") {
	$"0001 0000 0000 0047 010B 005B 0145 0402"            /* .......G...[.E.. */
	$"4F4B 0000 0000 000A 004B 003A 0145 8861"            /* OK.......K.:.Eàa */
	$"4F75 7420 6F66 206D 656D 6F72 7921 2043"            /* Out of memory! C */
	$"6C6F 7365 2074 6865 2070 726F 6772 616D"            /* lose the program */
	$"2061 6E64 2069 6E63 7265 6173 6520 7468"            /*  and increase th */
	$"6520 616D 6F75 6E74 206F 6620 6176 6169"            /* e amount of avai */
	$"6C61 626C 6520 5241 4D20 696E 2074 6865"            /* lable RAM in the */
	$"2047 6574 2049 6E66 6F20 7363 7265 656E"            /*  Get Info screen */
	$"2E00"                                               /* .. */
};

data 'DITL' (133, "Duplicate Upload") {
	$"0002 0000 0000 0057 00FC 006B 0136 0402"            /* .......W...k.6.. */
	$"4E6F 0000 0000 0057 00B5 006B 00EF 0403"            /* No.....W.µ.k.... */
	$"5965 7320 0000 0000 000A 0054 004A 0136"            /* Yes .......T.J.6 */
	$"887D 4120 6669 6C65 2077 6974 6820 7468"            /* à}A file with th */
	$"6973 206E 616D 6520 616C 7265 6164 7920"            /* is name already  */
	$"6578 6973 7473 206F 6E20 7468 6520 6465"            /* exists on the de */
	$"7669 6365 2E20 5570 6C6F 6164 696E 6720"            /* vice. Uploading  */
	$"696E 2074 6869 7320 636F 6E64 6974 696F"            /* in this conditio */
	$"6E20 7769 6C6C 206C 696B 656C 7920 636F"            /* n will likely co */
	$"7272 7570 7420 7468 6520 6669 6C65 2E20"            /* rrupt the file.  */
	$"5072 6F63 6565 6420 616E 7977 6179 3F00"            /* Proceed anyway?. */
};

data 'DITL' (134, "Unrecognized Emulator") {
	$"0002 0000 0000 0047 00FC 005B 0136 0403"            /* .......G...[.6.. */
	$"5965 7300 0000 0000 0047 00B5 005B 00EF"            /* Yes......G.µ.[.. */
	$"0402 4E6F 0000 0000 000A 0054 003A 0136"            /* ..No.......T.:.6 */
	$"8855 5468 6520 6465 7669 6365 2064 6964"            /* àUThe device did */
	$"206E 6F74 2072 6573 706F 6E64 206C 696B"            /*  not respond lik */
	$"6520 6120 7479 7069 6361 6C20 656D 756C"            /* e a typical emul */
	$"6174 6F72 2E20 576F 756C 6420 796F 7520"            /* ator. Would you  */
	$"6C69 6B65 2074 6F20 636F 6E6E 6563 7420"            /* like to connect  */
	$"616E 7977 6179 3F00"                                /* anyway?. */
};

data 'ALRT' (128, "About") {
	$"0030 0020 00F2 016E 0080 4444"                      /* .0. ...n.ÄDD */
};

data 'ALRT' (257, "Bad System") {
	$"0028 0028 0085 0196 0101 5555"                      /* .(.(.Ö.ñ..UU */
};

data 'ALRT' (258, "SCSI Error") {
	$"0028 0028 009D 0190 0102 5555"                      /* .(.(.ù.ê..UU */
};

data 'ALRT' (259, "File Error") {
	$"0028 0028 009D 0190 0103 5555"                      /* .(.(.ù.ê..UU */
};

data 'ALRT' (130, "No Images Returned") {
	$"0028 0028 009D 0168 0082 5555"                      /* .(.(.ù.h.ÇUU */
};

data 'ALRT' (131, "No Files Returned") {
	$"0028 0028 009D 0168 0083 5555"                      /* .(.(.ù.h.ÉUU */
};

data 'ALRT' (132, "Duplicate Files") {
	$"0028 0028 009D 0168 0084 5555"                      /* .(.(.ù.h.ÑUU */
};

data 'ALRT' (1000, "Memory Fail") {
	$"0028 0028 008D 0177 03E8 5555"                      /* .(.(.ç.w..UU */
};

data 'ALRT' (129, "Image Change") {
	$"0028 0028 009D 0161 0081 5555"                      /* .(.(.ù.a.ÅUU */
};

data 'ALRT' (256, "Generic") {
	$"0028 0028 009D 01B8 0100 5555"                      /* .(.(.ù.∏..UU */
};

data 'ALRT' (133, "Duplicate Upload") {
	$"0028 0028 009D 0168 0085 5555"                      /* .(.(.ù.h.ÖUU */
};

data 'ALRT' (134, "Unrecognized Emulator") {
	$"0028 0028 008D 0168 0086 5555"                      /* .(.(.ç.h.ÜUU */
};

data 'ICON' (128) {
	$"003F FC00 00C0 0300 0330 10C0 0466 6220"            /* .?...¿...0.¿.fb  */
	$"0ADC CC10 1293 B808 22BF 6004 23E4 E004"            /* ..Ã..ì∏."ø`.#... */
	$"424C 3C02 4612 4602 8821 8201 8840 8201"            /* BL<.F.F.à!Ç.à@Ç. */
	$"9040 B201 9046 B201 9046 C401 9021 3BC1"            /* ê@≤.êF≤.êFƒ.ê!;¡ */
	$"901E 0021 9000 0011 9000 C009 901C 38C5"            /* ê..!ê...ê.¿Δê.8≈ */
	$"8826 260D 8821 11C5 4410 883A 4210 4402"            /* à&&¬à!.≈D.à:B.D. */
	$"2208 2404 2104 1804 1082 0008 0841 0010"            /* ".$.!....Ç...A.. */
	$"0440 8020 0320 40C0 00E0 4300 003F FC00"            /* .@Ä . @¿..C..?.. */
};

data 'ICON' (129) {
	$"0003 0000 0003 0000 000F C000 000F C000"            /* ..........¿...¿. */
	$"003C F000 003C F000 00F0 3C00 00F0 3C00"            /* .<...<....<...<. */
	$"03C0 0F00 03C0 0F00 0F0F C3C0 0F0F C3C0"            /* .¿...¿....√¿..√¿ */
	$"3C30 0000 3C30 0000 F03F CFFC F03F CFFC"            /* <0..<0...?œ..?œ. */
	$"3C30 0000 3C30 0000 0F0F C3C0 0F0F C3C0"            /* <0..<0....√¿..√¿ */
	$"03C0 0F00 03C0 0F00 00F0 3C00 00F0 3C00"            /* .¿...¿....<...<. */
	$"003C F000 003C F000 000F C000 000F C000"            /* .<...<....¿...¿. */
	$"0003 0000 0003 0000 0000 0000 0000 0000"            /* ................ */
};

data 'ICON' (130) {
	$"FFFF 0000 FFFF 0000 C000 C000 C000 C000"            /* ........¿.¿.¿.¿. */
	$"CFC0 3000 CFC0 3000 C000 3FF0 C000 3FF0"            /* œ¿0.œ¿0.¿.?.¿.?. */
	$"CFFC 300C CFFC 300C C000 3CC3 C000 3CC3"            /* œ.0.œ.0.¿.<√¿.<√ */
	$"CFFF 3333 CFFF 3333 C000 3CC3 C000 3CC3"            /* œ.33œ.33¿.<√¿.<√ */
	$"CFFF 3333 CFFF 3333 C000 3CC3 C000 3CC3"            /* œ.33œ.33¿.<√¿.<√ */
	$"CFFF 3333 CFFF 3333 C000 3CC3 C000 3CC3"            /* œ.33œ.33¿.<√¿.<√ */
	$"FFFF F333 FFFF F333 000C CCC3 000C CCC3"            /* ...3...3..Ã√..Ã√ */
	$"000C 0003 000C 0003 000F FFFF 000F FFFF"            /* ................ */
};

data 'ICON' (131) {
	$"03FF C000 03FF C000 0C00 3000 0C00 3000"            /* ..¿...¿...0...0. */
	$"30C0 0C00 30C0 0C00 C300 0300 C300 0300"            /* 0¿..0¿..√...√... */
	$"CC3C 0300 CC3C 0300 C0C3 0300 C0C3 0300"            /* Ã<..Ã<..¿√..¿√.. */
	$"C0C3 03FC C0C3 03FC C03C 3333 C03C 3333"            /* ¿√..¿√..¿<33¿<33 */
	$"C000 C333 C000 C333 3003 0C33 3003 0C33"            /* ¿.√3¿.√30..30..3 */
	$"0C00 3FC3 0C00 3FC3 03FF C003 03FF C003"            /* ..?√..?√..¿...¿. */
	$"000C 0003 000C 0003 000C 3FC3 000C 3FC3"            /* ..........?√..?√ */
	$"000C 0003 000C 0003 0003 FFFC 0003 FFFC"            /* ................ */
};

data 'DLOG' (512, "Choose ID") {
	$"0028 0014 0122 0104 0001 0000 0000 0000"            /* .(...".......... */
	$"0000 0200 1253 656C 6563 7420 456D 756C"            /* .....Select Emul */
	$"6174 6F72 2049 44"                                  /* ator ID */
};

data 'WIND' (128, "Main") {
	$"0032 0010 0120 0114 0008 0000 0100 0000"            /* .2... .......... */
	$"0000 0773 6375 7A45 4D55"                           /* ...scuzEMU */
};

data 'WIND' (129, "Progress") {
	$"003C 0014 008A 0140 0005 0000 0000 0000"            /* .<...ä.@........ */
	$"0000 0854 7261 6E73 6665 72"                        /* ...Transfer */
};

data 'STR#' (258, "SCSI Errors") {
	$"0007 4743 6F75 6C64 206E 6F74 2067 6574"            /* ..GCould not get */
	$"2063 6F6E 7472 6F6C 206F 6620 7468 6520"            /*  control of the  */
	$"5343 5349 2062 7573 2E20 4973 2061 6E6F"            /* SCSI bus. Is ano */
	$"7468 6572 2061 7070 6C69 6361 7469 6F6E"            /* ther application */
	$"2075 7369 6E67 2069 743F 1F4E 6F20 6465"            /*  using it?.No de */
	$"7669 6365 2072 6573 706F 6E64 6564 2074"            /* vice responded t */
	$"6F20 7265 7175 6573 742E 6E54 6865 2064"            /* o request.nThe d */
	$"6576 6963 6520 7265 6A65 6374 6564 2074"            /* evice rejected t */
	$"6865 2072 6571 7565 7374 2E20 5665 7269"            /* he request. Veri */
	$"6679 2069 7420 6973 2061 2076 616C 6964"            /* fy it is a valid */
	$"2065 6D75 6C61 746F 7220 616E 6420 6D61"            /*  emulator and ma */
	$"6B65 2073 7572 6520 7468 6520 746F 6F6C"            /* ke sure the tool */
	$"626F 7820 636F 6E74 726F 6C73 2061 7265"            /* box controls are */
	$"2065 6E61 626C 6564 2E85 556E 6162 6C65"            /*  enabled.ÖUnable */
	$"2074 6F20 7265 6164 2072 6573 706F 6E73"            /*  to read respons */
	$"6520 6672 6F6D 2064 6576 6963 652E 204D"            /* e from device. M */
	$"616B 6520 7375 7265 2079 6F75 2061 7265"            /* ake sure you are */
	$"2075 7369 6E67 2074 6865 206D 6F73 7420"            /*  using the most  */
	$"7570 2D74 6F2D 6461 7465 2076 6572 7369"            /* up-to-date versi */
	$"6F6E 206F 6620 7468 6520 6465 7669 6365"            /* on of the device */
	$"2066 6972 6D77 6172 6520 616E 6420 7468"            /*  firmware and th */
	$"6973 2061 7070 6C69 6361 7469 6F6E 2E4F"            /* is application.O */
	$"436F 6D6D 756E 6963 6174 696F 6E20 6572"            /* Communication er */
	$"726F 7220 6475 7269 6E67 2065 6E64 206F"            /* ror during end o */
	$"6620 6F70 6572 6174 696F 6E2E 2043 6F6E"            /* f operation. Con */
	$"7369 6465 7220 7265 7374 6172 7469 6E67"            /* sider restarting */
	$"2079 6F75 7220 636F 6D70 7574 6572 2E89"            /*  your computer.â */
	$"436F 6D6D 616E 6420 636F 6D70 6C65 7465"            /* Command complete */
	$"6420 7769 7468 2061 6E20 6572 726F 7220"            /* d with an error  */
	$"6D65 7373 6167 652E 204D 616B 6520 7375"            /* message. Make su */
	$"7265 2079 6F75 2061 7265 2075 7369 6E67"            /* re you are using */
	$"2074 6865 206D 6F73 7420 7570 2D74 6F2D"            /*  the most up-to- */
	$"6461 7465 2076 6572 7369 6F6E 206F 6620"            /* date version of  */
	$"7468 6520 6465 7669 6365 2066 6972 6D77"            /* the device firmw */
	$"6172 6520 616E 6420 7468 6973 2061 7070"            /* are and this app */
	$"6C69 6361 7469 6F6E 2E63 556E 6162 6C65"            /* lication.cUnable */
	$"2074 6F20 616C 6C6F 6361 7465 2065 6E6F"            /*  to allocate eno */
	$"7567 6820 6D65 6D6F 7279 2E20 496E 6372"            /* ugh memory. Incr */
	$"6561 7365 2074 6865 2061 6D6F 756E 7420"            /* ease the amount  */
	$"6F66 2052 414D 2079 6F75 2068 6176 6520"            /* of RAM you have  */
	$"6173 7369 676E 6564 2074 6F20 7468 6973"            /* assigned to this */
	$"2061 7070 6C69 6361 7469 6F6E 2E"                   /*  application. */
};

data 'STR#' (259, "File Errors") {
	$"001E 1E55 6E65 7870 6563 7465 6420 6669"            /* ...Unexpected fi */
	$"6C65 206D 616E 6167 6572 2065 7272 6F72"            /* le manager error */
	$"2112 4469 7265 6374 6F72 7920 6973 2066"            /* !.Directory is f */
	$"756C 6C21 0D44 6973 6B20 6973 2066 756C"            /* ull!¬Disk is ful */
	$"6C21 144E 6F20 766F 6C75 6D65 2077 6173"            /* l!.No volume was */
	$"2066 6F75 6E64 2E2C 556E 7370 6563 6966"            /*  found.,Unspecif */
	$"6965 6420 6669 6C65 2049 2F4F 2065 7272"            /* ied file I/O err */
	$"6F72 2064 7572 696E 6720 6F70 6572 6174"            /* or during operat */
	$"696F 6E2E 1646 696C 6520 6E61 6D65 2077"            /* ion..File name w */
	$"6173 2069 6E76 616C 6964 2112 4669 6C65"            /* as invalid!.File */
	$"2077 6173 206E 6F74 206F 7065 6E21 1845"            /*  was not open!.E */
	$"6E64 206F 6620 6669 6C65 2065 6E63 6F75"            /* nd of file encou */
	$"6E74 6572 6564 2E2A 5472 6965 6420 746F"            /* ntered.*Tried to */
	$"2070 6F73 6974 696F 6E20 6265 666F 7265"            /*  position before */
	$"2074 6865 2066 696C 6520 7374 6172 7465"            /*  the file starte */
	$"642E 0F4D 656D 6F72 7920 6973 2066 756C"            /* d..Memory is ful */
	$"6C21 3954 6F6F 206D 616E 7920 6669 6C65"            /* l!9Too many file */
	$"7320 6172 6520 6F70 656E 2E20 5472 7920"            /* s are open. Try  */
	$"636C 6F73 696E 6720 6669 6C65 7320 616E"            /* closing files an */
	$"6420 7472 7920 6167 6169 6E2E 1346 696C"            /* d try again..Fil */
	$"6520 7761 7320 6E6F 7420 666F 756E 642E"            /* e was not found. */
	$"1844 6973 6B20 6973 2077 7269 7465 2070"            /* .Disk is write p */
	$"726F 7465 6374 6564 210F 4669 6C65 2069"            /* rotected!.File i */
	$"7320 6C6F 636B 6564 2111 566F 6C75 6D65"            /* s locked!.Volume */
	$"2069 7320 6C6F 636B 6564 211C 4669 6C65"            /*  is locked!.File */
	$"2069 7320 6275 7379 2C20 6361 6E6E 6F74"            /*  is busy, cannot */
	$"2064 656C 6574 652E 2744 7570 6C69 6361"            /*  delete.'Duplica */
	$"7465 2066 696C 6520 6578 6973 7473 2C20"            /* te file exists,  */
	$"6E6F 7420 6F76 6572 7772 6974 696E 672E"            /* not overwriting. */
	$"3C46 696C 6520 6973 2061 6C72 6561 6479"            /* <File is already */
	$"206F 7065 6E2C 2063 616E 6E6F 7420 7265"            /*  open, cannot re */
	$"2D6F 7065 6E20 7769 7468 2077 7269 7465"            /* -open with write */
	$"2070 6572 6D69 7373 696F 6E73 2E1F 4572"            /*  permissions..Er */
	$"726F 7220 696E 2075 7365 7220 7061 7261"            /* ror in user para */
	$"6D65 7465 7273 2067 6976 656E 2E1C 5265"            /* meters given..Re */
	$"6665 7265 6E63 6520 6E75 6D62 6572 2069"            /* ference number i */
	$"7320 696E 7661 6C69 642E 1943 616E 6E6F"            /* s invalid..Canno */
	$"7420 6765 7420 6669 6C65 2070 6F73 6974"            /* t get file posit */
	$"696F 6E2E 2856 6F6C 756D 6520 6973 2065"            /* ion.(Volume is e */
	$"6A65 6374 6564 2C20 6361 6E6E 6F74 206F"            /* jected, cannot o */
	$"7065 7261 7465 206F 6E20 6974 2E2C 4361"            /* perate on it.,Ca */
	$"6E6E 6F74 206F 7065 6E20 6475 6520 746F"            /* nnot open due to */
	$"2069 6E73 7566 6669 6369 656E 7420 7065"            /*  insufficient pe */
	$"726D 6973 7369 6F6E 732E 1944 7269 7665"            /* rmissions..Drive */
	$"2069 7320 616C 7265 6164 7920 6D6F 756E"            /*  is already moun */
	$"7465 642E 284E 6F20 6472 6976 6520 6D61"            /* ted.(No drive ma */
	$"7463 6865 6420 7468 6520 6472 6976 6520"            /* tched the drive  */
	$"6E75 6D62 6572 2067 6976 656E 2E0F 4E6F"            /* number given..No */
	$"7420 6120 4D61 6320 6469 736B 2138 566F"            /* t a Mac disk!8Vo */
	$"6C75 6D65 2069 7320 616E 2065 7874 6572"            /* lume is an exter */
	$"6E61 6C20 6669 6C65 2073 7973 7465 6D2C"            /* nal file system, */
	$"2063 616E 6E6F 7420 6F70 6572 6174 6520"            /*  cannot operate  */
	$"6F6E 2069 742E 2B46 696C 6520 7379 7374"            /* on it.+File syst */
	$"656D 2069 6E74 6572 6E61 6C20 6572 726F"            /* em internal erro */
	$"7220 6475 7269 6E67 2072 656E 616D 696E"            /* r during renamin */
	$"672E 2556 6F6C 756D 6520 6D61 7374 6572"            /* g.%Volume master */
	$"2064 6972 6563 746F 7279 2062 6C6F 636B"            /*  directory block */
	$"2069 7320 6261 6421 334E 6F74 2065 6E6F"            /*  is bad!3Not eno */
	$"7567 6820 7065 726D 6973 7369 6F6E 7320"            /* ugh permissions  */
	$"746F 2077 7269 7465 2074 6F20 7468 6520"            /* to write to the  */
	$"6465 7374 696E 6174 696F 6E2E"                      /* destination. */
};

data 'STR#' (128, "Window") {
	$"0009 2A53 656C 6563 7420 6669 6C65 2873"            /* .Δ*Select file(s */
	$"2920 2620 646F 7562 6C65 2D63 6C69 636B"            /* ) & double-click */
	$"2074 6F20 646F 776E 6C6F 6164 2E1F 446F"            /*  to download..Do */
	$"7562 6C65 2D63 6C69 636B 2061 6E20 696D"            /* uble-click an im */
	$"6167 6520 746F 206D 6F75 6E74 2E0E 446F"            /* age to mount..Do */
	$"776E 6C6F 6164 696E 672E 2E2E 1C49 7465"            /* wnloading....Ite */
	$"6D73 2072 656D 6169 6E69 6E67 2074 6F20"            /* ms remaining to  */
	$"646F 776E 6C6F 6164 3A1A 4974 656D 7320"            /* download:.Items  */
	$"7265 6D61 696E 696E 6720 746F 2075 706C"            /* remaining to upl */
	$"6F61 643A 0546 696C 653A 0C44 6576 6963"            /* oad:.File:.Devic */
	$"653A 2049 4420 580B 4D6F 6465 3A20 4669"            /* e: ID X.Mode: Fi */
	$"6C65 730C 4D6F 6465 3A20 496D 6167 6573"            /* les.Mode: Images */
};

data 'STR#' (256, "Generic Alerts") {
	$"0009 204E 6F20 6669 6C65 206D 6174 6368"            /* .Δ No file match */
	$"6564 2074 6865 2067 6976 656E 2069 6E64"            /* ed the given ind */
	$"6578 2E35 436F 756C 6420 6E6F 7420 6669"            /* ex.5Could not fi */
	$"6E64 2073 656C 6563 7465 6420 696D 6167"            /* nd selected imag */
	$"6520 696E 2074 6865 2066 756C 6C20 696D"            /* e in the full im */
	$"6167 6520 6C69 7374 2EA1 5468 6520 6E75"            /* age list.°The nu */
	$"6D62 6572 206F 6620 696D 6167 6573 2069"            /* mber of images i */
	$"6E20 7468 6520 6C69 7374 2064 6964 6E27"            /* n the list didn' */
	$"7420 6D61 7463 6820 7768 6174 2077 6173"            /* t match what was */
	$"2070 726F 7669 6465 6420 746F 2074 6865"            /*  provided to the */
	$"2074 7261 6E73 6665 7220 726F 7574 696E"            /*  transfer routin */
	$"652E 2054 6869 7320 6973 2070 726F 6261"            /* e. This is proba */
	$"626C 7920 6120 7072 6F67 7261 6D6D 696E"            /* bly a programmin */
	$"6720 6572 726F 722C 2070 6C65 6173 6520"            /* g error, please  */
	$"7265 706F 7274 2069 7420 746F 2074 6865"            /* report it to the */
	$"2064 6576 656C 6F70 6572 2E85 436F 756C"            /*  developer.ÖCoul */
	$"6420 6E6F 7420 6669 6E64 2061 6E20 6F70"            /* d not find an op */
	$"6572 6174 696E 6720 7379 7374 656D 2064"            /* erating system d */
	$"7269 7665 7220 666F 7220 7468 6973 2064"            /* river for this d */
	$"6576 6963 652E 2043 6865 636B 2079 6F75"            /* evice. Check you */
	$"7220 6578 7465 6E73 696F 6E73 2061 6E64"            /* r extensions and */
	$"2076 6572 6966 7920 7468 6520 656D 756C"            /*  verify the emul */
	$"6174 6F72 2069 7320 6265 696E 6720 7365"            /* ator is being se */
	$"656E 2062 7920 7468 6520 7379 7374 656D"            /* en by the system */
	$"2E44 5468 6572 6520 6172 6520 6669 6C65"            /* .DThere are file */
	$"7320 6F70 656E 206F 6E20 7468 6520 6375"            /* s open on the cu */
	$"7272 656E 7420 696D 6167 652E 2043 6C6F"            /* rrent image. Clo */
	$"7365 2074 6865 6D20 616E 6420 7472 7920"            /* se them and try  */
	$"6167 6169 6E2E 6443 6F75 6C64 206E 6F74"            /* again.dCould not */
	$"2075 6E6D 6F75 6E74 2074 6865 2063 7572"            /*  unmount the cur */
	$"7265 6E74 2069 6D61 6765 2E20 5472 7920"            /* rent image. Try  */
	$"7468 6520 6F70 6572 6174 696F 6E20 6167"            /* the operation ag */
	$"6169 6E2C 2061 6E64 2069 6620 6974 2066"            /* ain, and if it f */
	$"6169 6C73 2072 6573 7461 7274 2079 6F75"            /* ails restart you */
	$"7220 636F 6D70 7574 6572 2E5D 4465 7669"            /* r computer.]Devi */
	$"6365 2064 7269 7665 7220 7265 706F 7274"            /* ce driver report */
	$"6564 2061 6E20 6572 726F 7220 6475 7269"            /* ed an error duri */
	$"6E67 2065 6A65 6374 696F 6E2E 2054 7279"            /* ng ejection. Try */
	$"2064 7261 6767 696E 6720 7468 6520 696D"            /*  dragging the im */
	$"6167 6520 746F 2074 6865 2054 7261 7368"            /* age to the Trash */
	$"2069 6E73 7465 6164 2E5D 5468 6520 6669"            /*  instead.]The fi */
	$"6C65 206E 616D 6520 6973 2074 6F6F 206C"            /* le name is too l */
	$"6F6E 6720 746F 2066 6974 206F 6E20 7468"            /* ong to fit on th */
	$"6520 6465 7669 6365 2E20 5265 6E61 6D65"            /* e device. Rename */
	$"2069 7420 746F 2073 6F6D 6574 6869 6E67"            /*  it to something */
	$"2073 686F 7274 6572 2061 6E64 2074 7279"            /*  shorter and try */
	$"2061 6761 696E 2E6B 5468 6520 6669 6C65"            /*  again.kThe file */
	$"206E 616D 6520 636F 6E74 6169 6E73 206F"            /*  name contains o */
	$"6E65 206F 7220 6D6F 7265 2063 6861 7261"            /* ne or more chara */
	$"6374 6572 7320 7468 6174 2063 616E 6E6F"            /* cters that canno */
	$"7420 6265 2073 746F 7265 6420 6F6E 2074"            /* t be stored on t */
	$"6865 2064 6576 6963 652E 2052 656E 616D"            /* he device. Renam */
	$"6520 6974 2061 6E64 2074 7279 2061 6761"            /* e it and try aga */
	$"696E 2E"                                            /* in. */
};

data 'ICN#' (128) {
	$"003F FC00 00C0 0300 0330 10C0 0466 6220"            /* .?...¿...0.¿.fb  */
	$"0ADC CC10 1293 B808 22BF 6004 23E4 E004"            /* ..Ã..ì∏."ø`.#... */
	$"424C 3C02 4612 4602 8821 8201 8840 8201"            /* BL<.F.F.à!Ç.à@Ç. */
	$"9040 B201 9046 B201 9046 C401 9021 3BC1"            /* ê@≤.êF≤.êFƒ.ê!;¡ */
	$"901E 0021 9000 0011 9000 C009 901C 38C5"            /* ê..!ê...ê.¿Δê.8≈ */
	$"8826 260D 8821 11C5 4410 883A 4210 4402"            /* à&&¬à!.≈D.à:B.D. */
	$"2208 2404 2104 1804 1082 0008 0841 0010"            /* ".$.!....Ç...A.. */
	$"0440 8020 0320 40C0 00E0 4300 003F FC00"            /* .@Ä . @¿..C..?.. */
	$"003F FC00 00FF FF00 03FF FFC0 07FF FFE0"            /* .?.........¿.... */
	$"0FFF FFF0 1FFF FFF8 3FFF FFFC 3FFF FFFC"            /* ........?...?... */
	$"7FFF FFFE 7FFF FFFE FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF 7FFF FFFE 7FFF FFFE"            /* ................ */
	$"3FFF FFFC 3FFF FFFC 1FFF FFF8 0FFF FFF0"            /* ?...?........... */
	$"07FF FFE0 03FF FFC0 00FF FF00 003F FC00"            /* .......¿.....?.. */
};

data 'icl4' (128) {
	$"0000 0000 00FF FFFF FFFF FF00 0000 0000"            /* ................ */
	$"0000 0000 FFDD DDDD DDDD DDFF 0000 0000"            /* ................ */
	$"0000 00FF DDFF DDDD DDDF DDDD FF00 0000"            /* ................ */
	$"0000 0FFD DFFD DFFD DFFD DDFD DDF0 0000"            /* ................ */
	$"0000 FDFD FFDF FFDD FFDD FFDD DDDF 0000"            /* ................ */
	$"000F DDFD FDDF DDFF FDFF FDDD DDDD F000"            /* ................ */
	$"00FD DDFD FDFF FFFF DFFD DDDD DDDD DF00"            /* ................ */
	$"00FD DDFF FFFE EFEE FFFD DDDD DDDD DF00"            /* ................ */
	$"0FDD DDFE EFEE FFEE EEFF FFDD DDDD DDF0"            /* ................ */
	$"0FDD DFFE EEEF 22FE EF22 2FFD DDDD DDF0"            /* ......".."/..... */
	$"FDDD FEEE EEF2 222F F222 22FD DDDD DDDF"            /* ......"/.""..... */
	$"FDDD FEEE EF22 2222 F233 32FD DDDD DDDF"            /* .....""".32..... */
	$"FDDF EEEE EF22 3332 F2FF 32FD DDDD DDDF"            /* ....."32..2..... */
	$"FDDF EEEE EF22 3FF2 F2FF 32FD DDDD DDDF"            /* ....."?...2..... */
	$"FDDF EEEE EF22 3FF2 FF22 2FDD DDDD DDDF"            /* ....."?.."/..... */
	$"FDDF EEEE EEF2 222F CCFF FCFF FFDD DDDF"            /* ......"/Ã....... */
	$"FDDF EEEE EEEF FFFC CCCC CCCC CCFD DDDF"            /* ........ÃÃÃÃÃ... */
	$"FDDF EEEE EEEC CCCC CCCC CCCC CCCF DDDF"            /* ......ÃÃÃÃÃÃÃœ.. */
	$"FDDF EEEE EEEE CCCC FFCC CCCC CCCC FDDF"            /* ......ÃÃ.ÃÃÃÃÃ.. */
	$"FDDF EEEE EEEF FFCC CCFF FCCC FFCC CFDF"            /* .......ÃÃ..Ã.Ãœ. */
	$"FDDD FEEE EEFD DFFC CCFD DFFC CCCC FFDF"            /* ........Ã...ÃÃ.. */
	$"FDDD FEEE EEFD DDDF CCCF DDDF FFCC CFDF"            /* ........Ãœ...Ãœ. */
	$"0FDD DFEE EEEF DDDD FCCC FDDD DDFF FDF0"            /* .........Ã...... */
	$"0FDD DDFE EEEF DDDD DFCC CFDD DDDD DDF0"            /* .........Ãœ..... */
	$"00FD DDFE EEEE FDDD DDFC CFDD DDDD DF00"            /* ..........œ..... */
	$"00FD DDDF EEEE EFDD DDDF FDDD DDDD DF00"            /* ................ */
	$"000F DDDD FEEE EEFD DDDD DDDD DDDD F000"            /* ................ */
	$"0000 FDDD DFEE EEEF DDDD DDDD DDDF 0000"            /* ................ */
	$"0000 0FDD DFEE EEEE FDDD DDDD DDF0 0000"            /* ................ */
	$"0000 00FF DDFE EEEE EFDD DDDD FF00 0000"            /* ................ */
	$"0000 0000 FFFE EEEE EFDD DDFF 0000 0000"            /* ................ */
	$"0000 0000 00FF FFFF FFFF FF00 0000 0000"            /* ................ */
};

data 'ics#' (128) {
	$"1FF8 2004 4002 8001 8C39 9245 A183 C083"            /* .. .@.Ä.å9íE°É¿É */
	$"C0B3 C6B3 C6C5 A139 9E01 4002 2004 1FF8"            /* ¿≥Δ≥Δ≈°9û.@. ... */
	$"1FF8 3FFC 7FFE FFFF FFFF FFFF FFFF FFFF"            /* ..?............. */
	$"FFFF FFFF FFFF FFFF FFFF 7FFE 3FFC 1FF8"            /* ............?... */
};

data 'ics8' (128) {
	$"0000 00FF FFFF FFFF FFFF FFFF FF00 0000"            /* ................ */
	$"0000 FF7F 7F7F 7F7F 7F7F 7F7F 7FFF 0000"            /* ................ */
	$"00FF 7F7F 7F7F 7F7F 7F7F 7F7F 7F7F FF00"            /* ................ */
	$"FF7F 7F7F 7F7F 7F7F 7F7F 7F7F 7F7F 7FFF"            /* ................ */
	$"FF7F 7F7F FFFF 7F7F 7F7F FFFF FF7F 7FFF"            /* ................ */
	$"FF7F 7FFF 1616 FF7F 7FFF 1616 16FF 7FFF"            /* ................ */
	$"FF7F FF16 1616 16FF FF16 1616 1616 FFFF"            /* ................ */
	$"FFFF 1616 1616 1616 FF16 D8D8 D816 FFFF"            /* ..........ÿÿÿ... */
	$"FFFF 1616 D8D8 D816 FF16 FFFF D816 FFFF"            /* ....ÿÿÿ.....ÿ... */
	$"FFFF 1616 D8FF FF16 FF16 FFFF D816 FFFF"            /* ....ÿ.......ÿ... */
	$"FFFF 1616 D8FF FF16 FFFF 1616 16FF 7FFF"            /* ....ÿ........... */
	$"FF7F FF16 1616 16FF 7F7F FFFF FF7F 7FFF"            /* ................ */
	$"FF7F 7FFF FFFF FF7F 7F7F 7F7F 7F7F 7FFF"            /* ................ */
	$"00FF 7F7F 7F7F 7F7F 7F7F 7F7F 7F7F FF00"            /* ................ */
	$"0000 FF7F 7F7F 7F7F 7F7F 7F7F 7FFF 0000"            /* ................ */
	$"0000 00FF FFFF FFFF FFFF FFFF FF00 0000"            /* ................ */
};

data 'ics4' (128) {
	$"000F FFFF FFFF F000 00FD DDDD DDDD DF00"            /* ................ */
	$"0FDD DDDD DDDD DDF0 FDDD DDDD DDDD DDDF"            /* ................ */
	$"FDDD FFDD DDFF FDDF FDDF 22FD DF22 2FDF"            /* ..........".."/. */
	$"FDF2 222F F222 22FF FF22 2222 F233 32FF"            /* .."/.""..""".32. */
	$"FF22 3332 F2FF 32FF FF22 3FF2 F2FF 32FF"            /* ."32..2.."?...2. */
	$"FF22 3FF2 FF22 2FDF FDF2 222F DDFF FDDF"            /* ."?.."/..."/.... */
	$"FDDF FFFD DDDD DDDF 0FDD DDDD DDDD DDF0"            /* ................ */
	$"00FD DDDD DDDD DF00 000F FFFF FFFF F000"            /* ................ */
};

data 'BNDL' (128) {
	$"7363 757A 0000 0001 4652 4546 0000 0000"            /* scuz....FREF.... */
	$"0080 4943 4E23 0000 0000 0080"                      /* .ÄICN#.....Ä */
};

data 'scuz' (0, "Owner resource") {
	$"0B73 6375 7A45 4D55 2030 2E35"                      /* .scuzEMU 0.5 */
};

data 'FREF' (128) {
	$"4150 504C 0000 00"                                  /* APPL... */
};

data 'CNTL' (128, "Progress Stop") {
	$"0034 00E6 0048 0122 0000 0100 0000 0000"            /* .4...H."........ */
	$"0000 0000 0000 0453 746F 70"                        /* .......Stop */
};

data 'icl8' (128) {
	$"0000 0000 0000 0000 0000 FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 FFFF 7F7F 7F7F 7F7F"            /* ................ */
	$"7F7F 7F7F 7F7F FFFF 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 FFFF 7F7F FFFF 7F7F 7F7F"            /* ................ */
	$"7F7F 7FFF 7F7F 7F7F FFFF 0000 0000 0000"            /* ................ */
	$"0000 0000 00FF FF7F 7FFF FF7F 7FFF FF7F"            /* ................ */
	$"7FFF FF7F 7F7F FF7F 7F7F FF00 0000 0000"            /* ................ */
	$"0000 0000 FF7F FF7F FFFF 7FFF FFFF 7F7F"            /* ................ */
	$"FFFF 7F7F FFFF 7F7F 7F7F 7FFF 0000 0000"            /* ................ */
	$"0000 00FF 7F7F FF7F FF7F 7FFF 7F7F FFFF"            /* ................ */
	$"FF7F FFFF FF7F 7F7F 7F7F 7F7F FF00 0000"            /* ................ */
	$"0000 FF7F 7F7F FF7F FF7F FFFF FFFF FFFF"            /* ................ */
	$"7FFF FF7F 7F7F 7F7F 7F7F 7F7F 7FFF 0000"            /* ................ */
	$"0000 FF7F 7F7F FFFF FFFF FFFB FBFF FBFB"            /* ................ */
	$"FFFF FF7F 7F7F 7F7F 7F7F 7F7F 7FFF 0000"            /* ................ */
	$"00FF 7F7F 7F7F FFFB FBFF FBFB FFFF FBFB"            /* ................ */
	$"FBFB FFFF FFFF 7F7F 7F7F 7F7F 7F7F FF00"            /* ................ */
	$"00FF 7F7F 7FFF FFFB FBFB FBFF 1616 FFFB"            /* ................ */
	$"FBFF 1616 16FF FF7F 7F7F 7F7F 7F7F FF00"            /* ................ */
	$"FF7F 7F7F FFFB FBFB FBFB FF16 1616 16FF"            /* ................ */
	$"FF16 1616 1616 FF7F 7F7F 7F7F 7F7F 7FFF"            /* ................ */
	$"FF7F 7F7F FFFB FBFB FBFF 1616 1616 1616"            /* ................ */
	$"FF16 D8D8 D816 FF7F 7F7F 7F7F 7F7F 7FFF"            /* ..ÿÿÿ........... */
	$"FF7F 7FFF FBFB FBFB FBFF 1616 D8D8 D816"            /* ............ÿÿÿ. */
	$"FF16 FFFF D816 FF7F 7F7F 7F7F 7F7F 7FFF"            /* ....ÿ........... */
	$"FF7F 7FFF FBFB FBFB FBFF 1616 D8FF FF16"            /* ............ÿ... */
	$"FF16 FFFF D816 FF7F 7F7F 7F7F 7F7F 7FFF"            /* ....ÿ........... */
	$"FF7F 7FFF FBFB FBFB FBFF 1616 D8FF FF16"            /* ............ÿ... */
	$"FFFF 1616 16FF 7F7F 7F7F 7F7F 7F7F 7FFF"            /* ................ */
	$"FF7F 7FFF FBFB FBFB FBFB FF16 1616 16FF"            /* ................ */
	$"F8F8 FFFF FFF8 FFFF FFFF 7F7F 7F7F 7FFF"            /* ................ */
	$"FF7F 7FFF FBFB FBFB FBFB FBFF FFFF FFF8"            /* ................ */
	$"F8F8 F8F8 F8F8 F8F8 F8F8 FF7F 7F7F 7FFF"            /* ................ */
	$"FF7F 7FFF FBFB FBFB FBFB FBF8 F8F8 F8F8"            /* ................ */
	$"F8F8 F8F8 F8F8 F8F8 F8F8 F8FF 7F7F 7FFF"            /* ................ */
	$"FF7F 7FFF FBFB FBFB FBFB FBFB F8F8 F8F8"            /* ................ */
	$"FFFF F8F8 F8F8 F8F8 F8F8 F8F8 FF7F 7FFF"            /* ................ */
	$"FF7F 7FFF FBFB FBFB FBFB FBFF FFFF F8F8"            /* ................ */
	$"F8F8 FFFF FFF8 F8F8 FFFF F8F8 F8FF 7FFF"            /* ................ */
	$"FF7F 7F7F FFFB FBFB FBFB FF7F 7FFF FFF8"            /* ................ */
	$"F8F8 FF7F 7FFF FFF8 F8F8 F8F8 FFFF 7FFF"            /* ................ */
	$"FF7F 7F7F FFFB FBFB FBFB FF7F 7F7F 7FFF"            /* ................ */
	$"F8F8 F8FF 7F7F 7FFF FFFF F8F8 F8FF 7FFF"            /* ................ */
	$"00FF 7F7F 7FFF FBFB FBFB FBFF 7F7F 7F7F"            /* ................ */
	$"FFF8 F8F8 FF7F 7F7F 7F7F FFFF FF7F FF00"            /* ................ */
	$"00FF 7F7F 7F7F FFFB FBFB FBFF 7F7F 7F7F"            /* ................ */
	$"7FFF F8F8 F8FF 7F7F 7F7F 7F7F 7F7F FF00"            /* ................ */
	$"0000 FF7F 7F7F FFFB FBFB FBFB FF7F 7F7F"            /* ................ */
	$"7F7F FFF8 F8FF 7F7F 7F7F 7F7F 7FFF 0000"            /* ................ */
	$"0000 FF7F 7F7F 7FFF FBFB FBFB FBFF 7F7F"            /* ................ */
	$"7F7F 7FFF FF7F 7F7F 7F7F 7F7F 7FFF 0000"            /* ................ */
	$"0000 00FF 7F7F 7F7F FFFB FBFB FBFB FF7F"            /* ................ */
	$"7F7F 7F7F 7F7F 7F7F 7F7F 7F7F FF00 0000"            /* ................ */
	$"0000 0000 FF7F 7F7F 7FFF FBFB FBFB FBFF"            /* ................ */
	$"7F7F 7F7F 7F7F 7F7F 7F7F 7FFF 0000 0000"            /* ................ */
	$"0000 0000 00FF 7F7F 7FFF FBFB FBFB FBFB"            /* ................ */
	$"FF7F 7F7F 7F7F 7F7F 7F7F FF00 0000 0000"            /* ................ */
	$"0000 0000 0000 FFFF 7F7F FFFB FBFB FBFB"            /* ................ */
	$"FBFF 7F7F 7F7F 7F7F FFFF 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 FFFF FFFB FBFB FBFB"            /* ................ */
	$"FBFF 7F7F 7F7F FFFF 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF 0000 0000 0000 0000 0000"            /* ................ */
};

data 'cicn' (129) {
	$"0000 0000 8008 0000 0000 0020 0020 0000"            /* ....Ä...... . .. */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0002 0001 0002 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0004 0000 0000 0020 0020"            /* ............. .  */
	$"0000 0000 0004 0000 0000 0020 0020 0000"            /* ........... . .. */
	$"0000 0003 0000 0003 0000 000F C000 000F"            /* ............¿... */
	$"C000 003F F000 003F F000 00FF FC00 00FF"            /* ¿..?...?........ */
	$"FC00 03FF FF00 03FF FF00 0FFF FFC0 0FFF"            /* .............¿.. */
	$"FFC0 3FFF FF00 3FFF FF00 FFFF FFFC FFFF"            /* .¿?...?......... */
	$"FFFC 3FFF FF00 3FFF FF00 0FFF FFC0 0FFF"            /* ..?...?......¿.. */
	$"FFC0 03FF FF00 03FF FF00 00FF FC00 00FF"            /* .¿.............. */
	$"FC00 003F F000 003F F000 000F C000 000F"            /* ...?...?....¿... */
	$"C000 0003 0000 0003 0000 0000 0000 0000"            /* ¿............... */
	$"0000 0003 0000 0003 0000 000F C000 000F"            /* ............¿... */
	$"C000 003C F000 003C F000 00F0 3C00 00F0"            /* ¿..<...<....<... */
	$"3C00 03C0 0F00 03C0 0F00 0F0F C3C0 0F0F"            /* <..¿...¿....√¿.. */
	$"C3C0 3C30 0000 3C30 0000 F03F CFFC F03F"            /* √¿<0..<0...?œ..? */
	$"CFFC 3C30 0000 3C30 0000 0F0F C3C0 0F0F"            /* œ.<0..<0....√¿.. */
	$"C3C0 03C0 0F00 03C0 0F00 00F0 3C00 00F0"            /* √¿.¿...¿....<... */
	$"3C00 003C F000 003C F000 000F C000 000F"            /* <..<...<....¿... */
	$"C000 0003 0000 0003 0000 0000 0000 0000"            /* ¿............... */
	$"0000 0000 0000 0000 0002 0000 FFFF FFFF"            /* ................ */
	$"FFFF 0001 9999 9999 FFFF 0003 0000 0000"            /* ....ôôôô........ */
	$"0000 0000 000F 0000 0000 0000 000F 0000"            /* ................ */
	$"0000 0000 00FF F000 0000 0000 00FF F000"            /* ................ */
	$"0000 0000 0FF5 FF00 0000 0000 0FF5 FF00"            /* ................ */
	$"0000 0000 FF55 5FF0 0000 0000 FF55 5FF0"            /* .....U_......U_. */
	$"0000 000F F555 55FF 0000 000F F555 55FF"            /* .....UU......UU. */
	$"0000 00FF 55FF F55F F000 00FF 55FF F55F"            /* ....U.._....U.._ */
	$"F000 0FF5 5F55 5555 0000 0FF5 5F55 5555"            /* ...._UUU...._UUU */
	$"0000 FF55 5FFF F5FF FFF0 FF55 5FFF F5FF"            /* ...U_......U_... */
	$"FFF0 0FF5 5F55 5555 0000 0FF5 5F55 5555"            /* ...._UUU...._UUU */
	$"0000 00FF 55FF F55F F000 00FF 55FF F55F"            /* ....U.._....U.._ */
	$"F000 000F F555 55FF 0000 000F F555 55FF"            /* .....UU......UU. */
	$"0000 0000 FF55 5FF0 0000 0000 FF55 5FF0"            /* .....U_......U_. */
	$"0000 0000 0FF5 FF00 0000 0000 0FF5 FF00"            /* ................ */
	$"0000 0000 00FF F000 0000 0000 00FF F000"            /* ................ */
	$"0000 0000 000F 0000 0000 0000 000F 0000"            /* ................ */
	$"0000 0000 0000 0000 0000 0000 0000 0000"            /* ................ */
	$"0000"                                               /* .. */
};

data 'cicn' (130) {
	$"0000 0000 8008 0000 0000 0020 0020 0000"            /* ....Ä...... . .. */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0002 0001 0002 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0004 0000 0000 0020 0020"            /* ............. .  */
	$"0000 0000 0004 0000 0000 0020 0020 0000"            /* ........... . .. */
	$"0000 FFFF 0000 FFFF 0000 FFFF C000 FFFF"            /* ............¿... */
	$"C000 FFFF F000 FFFF F000 FFFF FFF0 FFFF"            /* ¿............... */
	$"FFF0 FFFF FFFC FFFF FFFC FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF 000F FFFF 000F"            /* ................ */
	$"FFFF 000F FFFF 000F FFFF 000F FFFF 000F"            /* ................ */
	$"FFFF FFFF 0000 FFFF 0000 C000 C000 C000"            /* ..........¿.¿.¿. */
	$"C000 CFC0 3000 CFC0 3000 C000 3FF0 C000"            /* ¿.œ¿0.œ¿0.¿.?.¿. */
	$"3FF0 CFFC 300C CFFC 300C C000 3CC3 C000"            /* ?.œ.0.œ.0.¿.<√¿. */
	$"3CC3 CFFF 3333 CFFF 3333 C000 3CC3 C000"            /* <√œ.33œ.33¿.<√¿. */
	$"3CC3 CFFF 3333 CFFF 3333 C000 3CC3 C000"            /* <√œ.33œ.33¿.<√¿. */
	$"3CC3 CFFF 3333 CFFF 3333 C000 3CC3 C000"            /* <√œ.33œ.33¿.<√¿. */
	$"3CC3 FFFF F333 FFFF F333 000C CCC3 000C"            /* <√...3...3..Ã√.. */
	$"CCC3 000C 0003 000C 0003 000F FFFF 000F"            /* Ã√.............. */
	$"FFFF 0000 0000 0000 0003 0000 FFFF FFFF"            /* ................ */
	$"FFFF 0001 3333 9999 6666 0002 CCCC 9999"            /* ....33ôôff..ÃÃôô */
	$"6666 0003 0000 0000 0000 FFFF FFFF 0000"            /* ff.............. */
	$"0000 FFFF FFFF 0000 0000 FAAA AAAA F000"            /* ...........™™™.. */
	$"0000 FAAA AAAA F000 0000 FAFF FAAA AF00"            /* ...™™™.......™Ø. */
	$"0000 FAFF FAAA AF00 0000 FAAA AAAA AFFF"            /* .....™Ø....™™™Ø. */
	$"FF00 FAAA AAAA AFFF FF00 FAFF FFFA AF55"            /* ...™™™Ø.......ØU */
	$"55F0 FAFF FFFA AF55 55F0 FAAA AAAA AFF5"            /* U.....ØUU..™™™Ø. */
	$"F55F FAAA AAAA AFF5 F55F FAFF FFFF AF5F"            /* ._.™™™Ø.._....Ø_ */
	$"5F5F FAFF FFFF AF5F 5F5F FAAA AAAA AFF5"            /* __....Ø___.™™™Ø. */
	$"F55F FAAA AAAA AFF5 F55F FAFF FFFF AF5F"            /* ._.™™™Ø.._....Ø_ */
	$"5F5F FAFF FFFF AF5F 5F5F FAAA AAAA AFF5"            /* __....Ø___.™™™Ø. */
	$"F55F FAAA AAAA AFF5 F55F FAFF FFFF AF5F"            /* ._.™™™Ø.._....Ø_ */
	$"5F5F FAFF FFFF AF5F 5F5F FAAA AAAA AFF5"            /* __....Ø___.™™™Ø. */
	$"F55F FAAA AAAA AFF5 F55F FFFF FFFF FF5F"            /* ._.™™™Ø.._....._ */
	$"5F5F FFFF FFFF FF5F 5F5F 0000 00F5 F5F5"            /* __.....___...... */
	$"F55F 0000 00F5 F5F5 F55F 0000 00F5 5555"            /* ._......._....UU */
	$"555F 0000 00F5 5555 555F 0000 00FF FFFF"            /* U_....UUU_...... */
	$"FFFF 0000 00FF FFFF FFFF"                           /* .......... */
};

data 'cicn' (131) {
	$"0000 0000 8010 0000 0000 0020 0020 0000"            /* ....Ä...... . .. */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0004 0001 0004 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0004 0000 0000 0020 0020"            /* ............. .  */
	$"0000 0000 0004 0000 0000 0020 0020 0000"            /* ........... . .. */
	$"0000 03FF C000 03FF C000 0FFF F000 0FFF"            /* ....¿...¿....... */
	$"F000 3FFF FC00 3FFF FC00 FFFF FF00 FFFF"            /* ..?...?......... */
	$"FF00 FFFF FF00 FFFF FF00 FFC3 FF00 FFC3"            /* ...........√...√ */
	$"FF00 FFC3 FFFC FFC3 FFFC FFFF FFFF FFFF"            /* ...√...√........ */
	$"FFFF FFFF FFFF FFFF FFFF 3FFF FFFF 3FFF"            /* ..........?...?. */
	$"FFFF 0FFF FFFF 0FFF FFFF 03FF FFFF 03FF"            /* ................ */
	$"FFFF 000F FFFF 000F FFFF 000F FFFF 000F"            /* ................ */
	$"FFFF 000F FFFF 000F FFFF 0003 FFFC 0003"            /* ................ */
	$"FFFC 03FF C000 03FF C000 0C00 3000 0C00"            /* ....¿...¿...0... */
	$"3000 30C0 0C00 30C0 0C00 C300 0300 C300"            /* 0.0¿..0¿..√...√. */
	$"0300 CC3C 0300 CC3C 0300 C0C3 0300 C0C3"            /* ..Ã<..Ã<..¿√..¿√ */
	$"0300 C0C3 03FC C0C3 03FC C03C 3333 C03C"            /* ..¿√..¿√..¿<33¿< */
	$"3333 C000 C333 C000 C333 3003 0C33 3003"            /* 33¿.√3¿.√30..30. */
	$"0C33 0C00 3FC3 0C00 3FC3 03FF C003 03FF"            /* .3..?√..?√..¿... */
	$"C003 000C 0003 000C 0003 000C 3FC3 000C"            /* ¿...........?√.. */
	$"3FC3 000C 0003 000C 0003 0003 FFFC 0003"            /* ?√.............. */
	$"FFFC 0000 0000 0000 0005 0000 FFFF FFFF"            /* ................ */
	$"FFFF 0001 FFFF 6666 3333 0002 0000 9999"            /* ......ff33....ôô */
	$"FFFF 0003 AAAA AAAA AAAA 0004 DDDD 0000"            /* ....™™™™™™...... */
	$"0000 000F 0000 0000 0000 0000 00FF FFFF"            /* ................ */
	$"FFFF FF00 0000 0000 0000 0000 00FF FFFF"            /* ................ */
	$"FFFF FF00 0000 0000 0000 0000 FF22 2222"            /* .............""" */
	$"2222 22FF 0000 0000 0000 0000 FF22 2222"            /* """..........""" */
	$"2222 22FF 0000 0000 0000 00FF 2222 FF22"            /* """.........""." */
	$"2222 2222 FF00 0000 0000 00FF 2222 FF22"            /* """"........""." */
	$"2222 2222 FF00 0000 0000 FF22 22FF 2222"            /* """".......""."" */
	$"2222 2222 22FF 0000 0000 FF22 22FF 2222"            /* """""......""."" */
	$"2222 2222 22FF 0000 0000 FF22 FF22 22FF"            /* """""......"."". */
	$"FF22 2222 22FF 0000 0000 FF22 FF22 22FF"            /* .""""......"."". */
	$"FF22 2222 22FF 0000 0000 FF22 2222 FF00"            /* .""""......""".. */
	$"00FF 2222 22FF 0000 0000 FF22 2222 FF00"            /* .."""......""".. */
	$"00FF 2222 22FF 0000 0000 FF22 2222 FF00"            /* .."""......""".. */
	$"00FF 2222 22FF FFFF FF00 FF22 2222 FF00"            /* .."""......""".. */
	$"00FF 2222 22FF FFFF FF00 FF22 2222 22FF"            /* .."""......"""". */
	$"FF22 22FF 22FF 33FF 44FF FF22 2222 22FF"            /* ."".".3.D.."""". */
	$"FF22 22FF 22FF 33FF 44FF FF22 2222 2222"            /* ."".".3.D..""""" */
	$"2222 FF22 22FF 33FF 11FF FF22 2222 2222"            /* ""."".3....""""" */
	$"2222 FF22 22FF 33FF 11FF 00FF 2222 2222"            /* ""."".3....."""" */
	$"22FF 2222 FF33 33FF 44FF 00FF 2222 2222"            /* "."".33.D..."""" */
	$"22FF 2222 FF33 33FF 44FF 0000 FF22 2222"            /* "."".33.D....""" */
	$"2222 22FF FFFF FF44 11FF 0000 FF22 2222"            /* """....D.....""" */
	$"2222 22FF FFFF FF44 11FF 0000 00FF FFFF"            /* """....D........ */
	$"FFFF FF11 4411 4411 44FF 0000 00FF FFFF"            /* ....D.D.D....... */
	$"FFFF FF11 4411 4411 44FF 0000 0000 0000"            /* ....D.D.D....... */
	$"FF44 1144 1144 1144 11FF 0000 0000 0000"            /* .D.D.D.D........ */
	$"FF44 1144 1144 1144 11FF 0000 0000 0000"            /* .D.D.D.D........ */
	$"FF11 44FF FFFF FF11 44FF 0000 0000 0000"            /* ..D.....D....... */
	$"FF11 44FF FFFF FF11 44FF 0000 0000 0000"            /* ..D.....D....... */
	$"FF44 1144 1144 1144 11FF 0000 0000 0000"            /* .D.D.D.D........ */
	$"FF44 1144 1144 1144 11FF 0000 0000 0000"            /* .D.D.D.D........ */
	$"00FF FFFF FFFF FFFF FF00 0000 0000 0000"            /* ................ */
	$"00FF FFFF FFFF FFFF FF00"                           /* .......... */
};

