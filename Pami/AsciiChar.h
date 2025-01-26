//
// Librairie de gestion d'un afficheur 7 Segments avec un TM1637
//

#ifndef _ASCII_CHAR_H_
#define _ASCII_CHAR_H_

// //
// //      HH
// //     ----
// // GH |    | DH
// //     -MM-
// // GB |    | DB
// //     ---
// //      BB
// #define SEG_GH 0x20
// #define SEG_GB 0x10
// #define SEG_DH 0x02
// #define SEG_DB 0x04
// #define SEG_HH 0x01
// #define SEG_MM 0x40
// #define SEG_BB 0x08
// #define SEG_PP 0x80

const unsigned char asciiDef[] = {
  /*   */ 0,
  /* ! */ 0,
  /* " */ SEG_GH|SEG_DH,
  /* # */ 0,
  /* $ */ 0,
  /* % */ 0,
  /* & */ 0,
  /* ' */ SEG_GH,
  /* ( */ 0,
  /* ) */ 0,
  /* * */ 0,
  /* + */ 0,
  /* ' */ SEG_GH,
  /* - */ SEG_MM,
  /* . */ 0,
  /* / */ 0,
  /* O */ SEG_GH|SEG_GB|SEG_HH|SEG_BB|SEG_DH|SEG_DB,
  /* 1 */ SEG_DH|SEG_DB,
  /* 2 */ SEG_GB|SEG_HH|SEG_MM|SEG_BB|SEG_DH,
  /* 3 */ SEG_HH|SEG_MM|SEG_BB|SEG_DH|SEG_DB,
  /* 4 */ SEG_GH|SEG_MM|SEG_DH|SEG_DB,
  /* 5 */ SEG_GH|SEG_HH|SEG_MM|SEG_BB|SEG_DB,
  /* 6 */ SEG_GH|SEG_GB|SEG_HH|SEG_MM|SEG_BB|SEG_DB,
  /* 7 */ SEG_HH|SEG_DH|SEG_DB,
  /* 8 */ SEG_GH|SEG_GB|SEG_HH|SEG_MM|SEG_BB|SEG_DH|SEG_DB,
  /* 9 */ SEG_GH|SEG_HH|SEG_MM|SEG_BB|SEG_DH|SEG_DB,
  /* : */ 0,
  /* ; */ 0,
  /* < */ 0,
  /* = */ SEG_MM|SEG_BB,
  /* > */ 0,
  /* ? */ SEG_GB|SEG_HH|SEG_MM|SEG_DH,
  /* @ */ SEG_GH|SEG_GB|SEG_HH|SEG_MM|SEG_BB|SEG_DH,
  /* A */ SEG_GH|SEG_GB|SEG_HH|SEG_MM|SEG_DH|SEG_DB,
  /* B */ SEG_GH|SEG_GB|SEG_MM|SEG_BB|SEG_DB,
  /* C */ SEG_GH|SEG_GB|SEG_HH|SEG_BB,
  /* D */ SEG_GB|SEG_MM|SEG_BB|SEG_DH|SEG_DB,
  /* E */ SEG_GH|SEG_GB|SEG_HH|SEG_MM|SEG_BB,
  /* F */ SEG_GH|SEG_GB|SEG_HH|SEG_MM,
  /* G */ SEG_GH|SEG_GB|SEG_HH|SEG_BB|SEG_DB,
  /* H */ SEG_GH|SEG_GB|SEG_MM|SEG_DH|SEG_DB,
  /* I */ SEG_DH|SEG_DB,
  /* J */ SEG_GB|SEG_BB|SEG_DH|SEG_DB,
  /* K */ 0,
  /* L */ SEG_GH|SEG_GB|SEG_BB,
  /* M */ 0,
  /* N */ SEG_GB|SEG_MM|SEG_DB,
  /* O */ SEG_GH|SEG_GB|SEG_HH|SEG_BB|SEG_DH|SEG_DB,
  /* P */ SEG_GH|SEG_GB|SEG_HH|SEG_MM|SEG_DH,
  /* Q */ SEG_GH|SEG_HH|SEG_MM|SEG_DH|SEG_DB,
  /* R */ SEG_GB|SEG_MM,
  /* S */ SEG_GH|SEG_HH|SEG_MM|SEG_BB|SEG_DB,
  /* T */ SEG_GH|SEG_GB|SEG_MM|SEG_BB,
  /* U */ SEG_GB|SEG_BB|SEG_DB,
  /* V */ 0,
  /* W */ 0,
  /* X */ 0,
  /* Y */ SEG_GH|SEG_MM|SEG_BB|SEG_DH|SEG_DB,
  /* Z */ 0,
  /* [ */ SEG_GH|SEG_GB|SEG_HH|SEG_BB,
  /* \ */ 0,
  /* ] */ SEG_HH|SEG_BB|SEG_DH|SEG_DB,
  /* ^ */ 0,
  /* _ */ SEG_BB,
  /* ' */ SEG_DH,
  /* a */ SEG_GB|SEG_HH|SEG_MM|SEG_BB|SEG_DH|SEG_DB,
  /* b */ SEG_GH|SEG_GB|SEG_MM|SEG_BB|SEG_DB,
  /* c */ SEG_GB|SEG_MM|SEG_BB,
  /* d */ SEG_GB|SEG_MM|SEG_BB|SEG_DH|SEG_DB,
  /* e */ SEG_GH|SEG_GB|SEG_HH|SEG_MM|SEG_BB,
  /* f */ SEG_GH|SEG_GB|SEG_HH|SEG_MM,
  /* g */ SEG_GH|SEG_HH|SEG_MM|SEG_BB|SEG_DH|SEG_DB,
  /* h */ SEG_GH|SEG_GB|SEG_MM|SEG_DB,
  /* i */ SEG_DB,
  /* j */ SEG_BB|SEG_DH|SEG_DB,
  /* k */ 0,
  /* l */ SEG_GH|SEG_GB,
  /* m */ 0,
  /* n */ SEG_GB|SEG_MM|SEG_DB,
  /* o */ SEG_GB|SEG_MM|SEG_BB|SEG_DB,
  /* p */ SEG_GH|SEG_GB|SEG_HH|SEG_MM|SEG_DH,
  /* q */ SEG_GH|SEG_HH|SEG_MM|SEG_DH|SEG_DB,
  /* r */ SEG_GB|SEG_MM,
  /* s */ SEG_GH|SEG_HH|SEG_MM|SEG_BB|SEG_DB,
  /* t */ SEG_GH|SEG_GB|SEG_MM|SEG_BB,
  /* u */ SEG_GB|SEG_BB|SEG_DB,
  /* v */ 0,
  /* w */ 0,
  /* x */ 0,
  /* y */ SEG_GH|SEG_MM|SEG_BB|SEG_DH|SEG_DB,
  /* z */ 0,
  /* { */ 0,
  /* | */ SEG_DH|SEG_DB,
  /* } */ 0,
  /* ~ */ 0,
  /*   */ SEG_GH|SEG_GB|SEG_HH|SEG_MM|SEG_BB|SEG_DH|SEG_DB,
};

#define OFFSET_ASCII 32

#endif