#include <esphome.h>
#include "Nanakshahi.h"

int * Nanakshahi(esphome::time::ESPTime date) {
    static int info[3] = {};
    
    if( date.month < 3 || ( date.month == 3 && date.day_of_month < 14 ) ) {
        info[0] = date.year - 1469;
    } else {
        info[0] =  date.year - 1468;
    }
    
    int monthOffsets[12] = { 14, 14, 15, 15, 16, 16, 15, 15, 14, 14, 13, 12 };
    int nsMonth = ( date.month + 8 ) % 12;
    int nsNextMonth = ( nsMonth + 1 ) % 12;
    
    if ( date.day_of_month >= monthOffsets[ nsNextMonth ] ) {
        nsMonth = nsNextMonth;
        info[2] = date.day_of_month - monthOffsets[ nsNextMonth ] + 1;
    } else {
        int gregorianMonths[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if ((date.year & 3) == 0 && ((date.year % 25) != 0 || (date.year & 15) == 0)) {
          gregorianMonths[1] = 29;
        }
        info[2] = gregorianMonths[ ( date.month + 10 ) % 12 ] - monthOffsets[ nsMonth ] + date.day_of_month + 1;
    }
    
    info[1] = ( nsMonth + 1 ) % 12;
    
    return info;
}
