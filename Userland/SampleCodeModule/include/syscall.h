#include <stdint.h>
#ifndef SYSCALL_H
#define SYSCALL_H

#define _writePixel 3
#define _readPixel 4
#define _getScreenWidth 5
#define _getScreenHeight 6
#define _write 7
#define _read 8
#define _clearBuffer 9
#define _readTime 10
#define _movePixelsUp 11
#define _replaceColor 12
#define _backupScreen 13
#define _restoreScreen 14
#define _fillScreen 15
#define _writeBlock 16
#define _beep 17
#define _unbeep 18

uint64_t _syscall(uint64_t, ...);

#endif
