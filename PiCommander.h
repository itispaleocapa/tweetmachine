/**
 * TweetMachine project - https://github.com/lucach/tweetmachine
 * Copyright © 2014 Demetrio Carrara <carrarademetrio@gmail.com>
 * Copyright © 2014 Luca Chiodini <luca@chiodini.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PICOMMANDER_H_
#define PICOMMANDER_H_

#include "fmatdef.h"
#include "Packets.h"

#ifdef __cplusplus
    extern "C" {
#endif

namespace FlashMat {
int sendFill(int fd, int color[3]);
int sendSwap(int fd, int type);

int sendTextPars(int fd, int color[3], bool overlay, int bgColor[3],
    int fontId, bool monospace, int charSpacing, int lineSpacing);
int sendTextPosition(int fd, int x, int y);
int sendText(int fd, char *text);
int sendDrawText(int fd);
int sendCellPosition(int fd, int x, int y);
}
#ifdef __cplusplus
    }
#endif

#endif
