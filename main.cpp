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

/**
 * USAGE
 * ./program <mode> <value>
 * <mode> can either be 0 (thus <value> is a string to be displayed)
 * or 1 (<value> is a path to a file)
 */


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "PiCommander.h"


using namespace FlashMat;

#define ADDRESS1    0x41 //65
#define ADDRESS2    0x40 //64
#define ADDRESS3    0x3D //61
#define ADDRESS4    0x3E //62
#define FONT_ID     0
#define CHARSPACING 1
#define LINESPACING 1
#define COORD_X     0
#define COORD_Y     0
#define MONOSPACE   0
#define OVERLAY     0
#define TEXT_SPEED  30 // the more, the slowest

int RED_COLOR  [3] = MAKE_RGB(255, 127, 0);
int BLACK_COLOR[3] = MAKE_RGB(0, 0, 0);


int displaylen(char* s)
{
    int lengths [95] =
    {
        0x06, // 0x20 ( 32)
        0x01, // 0x21 ( 33)
        0x03, // 0x22 ( 34)
        0x06, // 0x23 ( 35)
        0x05, // 0x24 ( 36)
        0x06, // 0x25 ( 37)
        0x05, // 0x26 ( 38)
        0x01, // 0x27 ( 39)
        0x03, // 0x28 ( 40)
        0x03, // 0x29 ( 41)
        0x05, // 0x2A ( 42)
        0x05, // 0x2B ( 43)
        0x02, // 0x2C ( 44)
        0x03, // 0x2D ( 45)
        0x02, // 0x2E ( 46)
        0x05, // 0x2F ( 47)
        0x04, // 0x30 ( 48)
        0x03, // 0x31 ( 49)
        0x04, // 0x32 ( 50)
        0x04, // 0x33 ( 51)
        0x05, // 0x34 ( 52)
        0x04, // 0x35 ( 53)
        0x04, // 0x36 ( 54)
        0x04, // 0x37 ( 55)
        0x04, // 0x38 ( 56)
        0x04, // 0x39 ( 57)
        0x01, // 0x3A ( 58)
        0x02, // 0x3B ( 59)
        0x03, // 0x3C ( 60)
        0x04, // 0x3D ( 61)
        0x03, // 0x3E ( 62)
        0x05, // 0x3F ( 63)
        0x07, // 0x40 ( 64)
        0x04, // 0x41 ( 65)
        0x04, // 0x42 ( 66)
        0x04, // 0x43 ( 67)
        0x04, // 0x44 ( 68)
        0x04, // 0x45 ( 69)
        0x04, // 0x46 ( 70)
        0x04, // 0x47 ( 71)
        0x04, // 0x48 ( 72)
        0x03, // 0x49 ( 73)
        0x05, // 0x4A ( 74)
        0x04, // 0x4B ( 75)
        0x04, // 0x4C ( 76)
        0x05, // 0x4D ( 77)
        0x05, // 0x4E ( 78)
        0x05, // 0x4F ( 79)
        0x04, // 0x50 ( 80)
        0x05, // 0x51 ( 81)
        0x04, // 0x52 ( 82)
        0x04, // 0x53 ( 83)
        0x05, // 0x54 ( 84)
        0x05, // 0x55 ( 85)
        0x05, // 0x56 ( 86)
        0x07, // 0x57 ( 87)
        0x05, // 0x58 ( 88)
        0x05, // 0x59 ( 89)
        0x04, // 0x5A ( 90)
        0x03, // 0x5B ( 91)
        0x05, // 0x5C ( 92)
        0x03, // 0x5D ( 93)
        0x05, // 0x5E ( 94)
        0x06, // 0x5F ( 95)
        0x02, // 0x60 ( 96)
        0x04, // 0x61 ( 97)
        0x04, // 0x62 ( 98)
        0x03, // 0x63 ( 99)
        0x04, // 0x64 (100)
        0x04, // 0x65 (101)
        0x04, // 0x66 (102)
        0x04, // 0x67 (103)
        0x04, // 0x68 (104)
        0x01, // 0x69 (105)
        0x03, // 0x6A (106)
        0x03, // 0x6B (107)
        0x03, // 0x6C (108)
        0x05, // 0x6D (109)
        0x04, // 0x6E (110)
        0x04, // 0x6F (111)
        0x04, // 0x70 (112)
        0x04, // 0x71 (113)
        0x04, // 0x72 (114)
        0x04, // 0x73 (115)
        0x03, // 0x74 (116)
        0x04, // 0x75 (117)
        0x05, // 0x76 (118)
        0x05, // 0x77 (119)
        0x04, // 0x78 (120)
        0x04, // 0x79 (121)
        0x04, // 0x7A (122)
        0x04, // 0x7B (123)
        0x01, // 0x7C (124)
        0x04, // 0x7D (125)
        0x07, // 0x7E (126)
    };
    int strl = strlen(s);
    int res = 0;
    for(int i = 0; i < strl; i++)
        res += lengths[s[i] - 32];
    // Add charspacing.
    res += strl * CHARSPACING;
    return res;
}

void parser(char * text)
{
    // TODO Use a dynamic array.
    char text2[10000];
    int length = strlen(text);
    int cont = 0;
    for(int i = 0; i < length; i++, cont++)
    {
        if(text[i] == '\n')
            text2[cont] = ' ';
        else
            if((int)text[i] < 0 || (int)text[i] > 127)
            {
                switch((int)text[i])
                {
                case 195:
                {
                    switch((int)text[i + 1])
                    {
                    case 168: //è
                        text2[cont++] = 'e';
                        text2[cont] = (char)39;
                        break;
                    case 160: //à
                        text2[cont++] = 'a';
                        text2[cont] = (char)39;
                        break;
                    case 178: //ò
                        text2[cont++] = 'o';
                        text2[cont] = (char)39;
                        break;
                    case 172: //ì
                        text2[cont++] = 'i';
                        text2[cont] = (char)39;
                        break;
                    case 185: //ù
                        text2[cont++] = 'u';
                        text2[cont] = (char)39;
                        break;
                    }
                    i++;
                    break;
                }
                case 226:
                {
                    if((int)text[i + 2] == 166 && (int)text[i + 1] == 128)
                    {
                        text2[cont++] = '.';
                        text2[cont++] = '.';
                        text2[cont] = '.';
                        i += 2;
                    }
                    break;
                }
                default:
                    text2[cont] = '*';
                    break;
                }
            }
            // Remove links.
            // TODO Who wrote this shit? Switch to strncmp()!!!
            else
                if((text[i] == 'h' && text[i + 1] == 't' && text[i + 2] == 't'
                        && text[i + 3] == 'p' && text[i + 4] == ':' && text[i + 5] == '/'
                        && text[i + 6] == '/') || (text[i] == 'h' && text[i + 1] == 't'
                                                   && text[i + 2] == 't' && text[i + 3] == 'p' && text[i + 4] == 's'
                                                   && text[i + 5] == ':' && text[i + 6] == '/' && text[i + 7] == '/'))
                {
                    while(text[i] != ' ' && text[i] != '\0')
                        i++;
                }
                else
                    text2[cont] = text[i];
    }
    text2[cont] = '\0';
    int i = 0;
    for(int j = 0; j < cont; j++)
    {
        if(text2[j] != '\0')
        {
            text[i] = text2[j];
            i++;
        }
    }
    text[i] = '\0';
}

int main(int argc, char* argv[])
{
    assert(argc > 1 && argc <= 3);  // assert we've only 2 args
    int fd2 = wiringPiI2CSetup(ADDRESS2);
    int fd1 = wiringPiI2CSetup(ADDRESS1);
    int fd3 = wiringPiI2CSetup(ADDRESS3);
    int fd4 = wiringPiI2CSetup(ADDRESS4);
    assert(fd1 >= 0 && fd2 >= 0);
    assert(fd3 >= 0 && fd4 >= 0);
    // Inform the cells about their absolute position.
    sendCellPosition(fd1, 0, 0);
    sendCellPosition(fd2, MATRIX_COLS, 0);
    sendCellPosition(fd3, 2 * MATRIX_COLS, 0);
    sendCellPosition(fd4, 3 * MATRIX_COLS, 0);
    if(argc == 1)  // if there is no argument, send a black fill
    {
        sendFill(fd1, BLACK_COLOR);
        sendFill(fd2, BLACK_COLOR);
        sendFill(fd3, BLACK_COLOR);
        sendFill(fd4, BLACK_COLOR);
        sendSwap(fd1, 0x00);
        sendSwap(fd2, 0x00);
        sendSwap(fd3, 0x00);
        sendSwap(fd4, 0x00);
    }
    else // display the argument
    {
        /** We display the text in this way:
            - Send text string (with sendText)
            - Send text parameters, such as string color, background color, font, spacing, etc... (with sendTextPars)
            - Send text position, the top and the left position (with sendTextPosition)
            - Actually draw the text in the back buffer (with sendDrawText)
            - Send swap to flip buffers (with sendSwap)

            With a scrolling text, the first two commands can be sent only once and will be kept in memory by FlashMat matrixs;
            whereas the last three needs to be issued every time we want to scroll the text.

            The commands here are duplicated because we simulate a fake BROADCAST: in fact, we just send
            every command to every slave attached at the I2C channel. Due to how the FlashMat system is built,
            every single cell will draw and display its own part correctly.
          */
        while(true)
        {
            /*
             * We have a "total_blank" string with all the characters to display
             * (taken from Twitter thanks to tweepy),
             * and a "partial" with max 30 char (max for I2C bus).
             * "partial" is a scrollable window that take everytime
             * 30 char from "total_blank" and we send it to FlashMat.
             */
            sendTextPars(fd1, RED_COLOR, OVERLAY, BLACK_COLOR, FONT_ID, MONOSPACE,
                         CHARSPACING, LINESPACING);
            sendTextPars(fd2, RED_COLOR, OVERLAY, BLACK_COLOR, FONT_ID, MONOSPACE,
                         CHARSPACING, LINESPACING);
            sendTextPars(fd3, RED_COLOR, OVERLAY, BLACK_COLOR, FONT_ID, MONOSPACE,
                         CHARSPACING, LINESPACING);
            sendTextPars(fd4, RED_COLOR, OVERLAY, BLACK_COLOR, FONT_ID, MONOSPACE,
                         CHARSPACING, LINESPACING);
            // TODO What is this?
            // TODO Use dynamic arrays.
            char total_blank[10000], partial[31], chars[4];
            for(int i = 0; i < 20; i++)
            {
                total_blank[i] = ' ';
            }
            total_blank[20] = '\0';
            int modalita = atoi(argv[1]);
            // TODO Use a dynamic array.
            char testo[10000];
            size_t char_letti;
            switch(modalita)
            {
            case 0:
                strcat(total_blank, argv[2]);
                break;
            case 1:
                // TODO Why stdio.h? <fstream> is better.
                FILE *input;
                input = fopen(argv[2], "rb");
                while(fgets(testo, sizeof(testo), input))
                {
                    parser(testo);
                    strcat(total_blank, testo);
                }
                fclose(input);
                break;
            }
            int total_length = strlen(total_blank);
            int i = 0, predictedChars;
            while(total_length > i)
            {
                strncpy(partial, total_blank + i, 30);
                partial[30] = '\0';
                sendText(fd1, partial);
                sendText(fd2, partial);
                sendText(fd3, partial);
                sendText(fd4, partial);
                strncpy(chars, partial, 1);
                chars[1] = '\0';
                predictedChars = displaylen(chars);
                for(int j = 0; j > -predictedChars; j--)
                {
                    sendTextPosition(fd1, j, COORD_Y);
                    sendTextPosition(fd2, j, COORD_Y);
                    sendTextPosition(fd3, j, COORD_Y);
                    sendTextPosition(fd4, j, COORD_Y);
                    sendDrawText(fd1);
                    sendDrawText(fd2);
                    sendDrawText(fd3);
                    sendDrawText(fd4);
                    sendSwap(fd1, 0x00);
                    sendSwap(fd2, 0x00);
                    sendSwap(fd3, 0x00);
                    sendSwap(fd4, 0x00);
                    delay(TEXT_SPEED);
                }
                i++;
            }
        }
    }
    return 0;
}
