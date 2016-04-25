
#ifndef __FONT_H__
#define __FONT_H__

#include <stdint.h>

#if __cplusplus
extern "C" {
#endif


typedef struct {
    const uint8_t *data;
    uint16_t char_width;
    uint16_t char_height;
    uint16_t start_char;
    uint16_t end_char;
} font_t;    

//#define USE_FONT_4X6
//#define USE_FONT_4X6
extern const font_t FONT_4X6;

//#define USE_FONT_5X8
extern const font_t FONT_5X8;

//#define USE_FONT_5X12
extern const font_t FONT_5X12;

#define USE_FONT_6X8
extern const font_t FONT_6X8;

//#define USE_FONT_6X10
extern const font_t FONT_6X10;

//#define USE_FONT_7X12
extern const font_t FONT_7X12;

//#define USE_FONT_8X8
extern const font_t FONT_8X8;

#define USE_FONT_8X12
extern const font_t FONT_8X12;

//#define USE_FONT_8X14
extern const font_t FONT_8X14;

//#define USE_FONT_10X16
#define USE_FONT_10X16
extern const font_t FONT_10X16;

#define USE_FONT_12X16
extern const font_t FONT_12X16;

//#define USE_FONT_12X20
extern const font_t FONT_12X20;

//#define USE_FONT_16X26
//#define USE_FONT_16X26
extern const font_t FONT_16X26;


#if __cplusplus
}
#endif
   
#endif // __FONT_H__
