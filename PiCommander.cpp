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

#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>

#include "PiCommander.h"


namespace FlashMat
{

    int sendFill(int fd, int color[3])
    {
        int command = PKT_FILL;
        return wiringPiI2CWriteBlock(fd, command, color, 3);
    }

    int sendSwap(int fd, int type)
    {
        int command = PKT_SWAP;
        int tipo[1] = { type };
        return wiringPiI2CWriteBlock(fd, command, tipo, 1);
    }

    int sendTextPars(int fd, int color[3], bool overlay, int bgColor[3],
                     int fontId, bool monospace, int charSpacing,
                     int lineSpacing)
    {
        int command = PKT_TEXT_PARS;
        int args[11];
        int i = 0;
        for(; i < 3; i++)
            args[i] = color[i];
        args[3] = overlay;
        i = 0;
        for(; i < 3; i++)
            args[i + 4] = bgColor[i];
        args[7] = fontId;
        args[8]  = monospace;
        args[9]  = charSpacing;
        args[10] = lineSpacing;
        return wiringPiI2CWriteBlock(fd, command, args, 11);
    }

    int sendTextPosition(int fd, int x, int y)
    {
        int command = PKT_TEXT_POSITION;
        int args[4] = {0};
        args[0] = x & 0xFF;
        args[1] = (x >> 8) & 0xFF;
        args[2] = y;
        args[3] = 0;
        return wiringPiI2CWriteBlock(fd, command, args, 4);
    }

    int sendText(int fd, char *text)
    {
        int args[TEXT_PACKET_MAX_SIZE + 1];
        int command = PKT_TEXT;
        int chunkLen = strlen(text);
        for(int i = 0; i < (chunkLen / TEXT_PACKET_MAX_SIZE) + 1; i++)
        {
            args[0] = i;
            int k = 0;
            for(int j = 0; j < TEXT_PACKET_MAX_SIZE
                    && text[i * TEXT_PACKET_MAX_SIZE + j] != '\0'; j++, k++)
                args[k + 1] = (int)text[i * TEXT_PACKET_MAX_SIZE + j];
            int res  = wiringPiI2CWriteBlock(fd, command, args, k + 1);
        }
    }

    int sendDrawText(int fd)
    {
        int command = PKT_DRAW_TEXT;
        return wiringPiI2CWriteBlock(fd, command, NULL, 0);
    }

    int sendCellPosition(int fd, int x, int y)
    {
        int command = PKT_CELL_POSITION;
        int args[4] = {0};
        args[0] = x & 0xFF;
        args[1] = (x >> 8) & 0xFF;
        args[2] = y & 0xFF;
        args[3] = (y >> 8) & 0xFF;
        return wiringPiI2CWriteBlock(fd, command, args, 4);
    }

}
