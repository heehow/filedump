#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <ncurses.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>

unsigned char	ascii_of_ebcdic[] = { /* 256 entries */
/*-----------------------------------------------------------------------------------------------------* 
 *		 00	 01	 02	 03	 04	 05	 06	 07	 08	 09	 0a	 0b	 0c	 0d	 0e	 0f
 *-----------------------------------------------------------------------------------------------------*/
/* 00 */ 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/* 10 */ 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/* 20 */ 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/* 30 */ 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/* 40 */ ' ' , 0x20, ',' , 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, '.' , '<' , '(' , '+' , '|' ,
/* 50 */ '&' , 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, '!' , '$' , '*' , ')' , ';' , '^' ,
/* 60 */ '-' , '/' , 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, ',' , '%' , '_' , '>' , '?' ,
/* 70 */ '[' , 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, '`' , ':' , '#' , '@' , '\'', '=' , '"' ,
/*-----------------------------------------------------------------------------------------------------* 
 *		 80	 81	 82	 83	 84	 85	 86	 87	 88	 89	 8a	 8b	 8c	 8d	 8e	 8f
 *-----------------------------------------------------------------------------------------------------*/
/* 80 */ ']' , 'a' , 'b' , 'c' , 'd' , 'e' , 'f' , 'g' , 'h' , 'i' , 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/* 90 */ 0x20, 'j' , 'k' , 'l' , 'm' , 'n' , 'o' , 'p' , 'q' , 'r' , 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/* a0 */ 0x20, '~' , 's' , 't' , 'u' , 'v' , 'w' , 'x' , 'y' , 'z' , 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/* b0 */ 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/* c0 */ '{' , 'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 'G' , 'H' , 'I' , 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/* d0 */ '}' , 'J' , 'K' , 'L' , 'M' , 'N' , 'O' , 'P' , 'Q' , 'R' , 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/* e0 */ '\\', 0x20, 'S' , 'T' , 'U' , 'V' , 'W' , 'X' , 'Y' , 'Z' , 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/* f0 */ '0' , '1' , '2' , '3' , '4' , '5' , '6' , '7' , '8' , '9' , 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
/*-----------------------------------------------------------------------------------------------------*
 *		 f0	 f1	 f2	 f3	 f4	 f5	 f6	 f7	 f8	 f9	 fa	 fb	 fc	 fd	 fe	 ff
 *-----------------------------------------------------------------------------------------------------*/
};

#ifndef S_ISLNK
#define S_ISLNK(mode)	(((mode)&0xF000) == S_IFLNK)
#endif

#ifndef S_ISSOCK
#define S_ISSOCK(mode)	(((mode)&0xF000) == S_IFSOCK)
#endif

#define	 MENUNO		  7
#define	 ENDLINE		 22
#define	 STARTLINE	  4
#define	 STARTCOL		11
#define	 STARTASCII	 60
#define	 SCREENSIZE	 16
#define	 COLSIZE		 16
#define	 MYPAGESIZE	 256
#define	 MYPATHMAX	  256
/*--------------------------------------------------------------* 
#define	 MAXALLOC		1024
#define	 MAXALLOC		1048576
#define	 MAXALLOC		2097152
*--------------------------------------------------------------*/ 
#define	 MAXALLOC		2097152

void				vDisplay_Screen ();
void				vDraw_Main_Menu ();
void				vMain_Loop ();
void				vSub_Loop ();
void				vCal_PGUP_Key ();
void				vCal_PGDN_Key ();
void				vFind_Str_And_Pos_Cur ();
void vAdd_Reverse_String (int y, int x, const char *fmt, ...);
/*--------------------------------------------------------------* 
void				vAdd_UnderLine_String ();
*--------------------------------------------------------------*/ 
void				vDraw_Help_Txt ();
void				vAdjust_Last_Position ();
void vLog_Print ( const char *fmt, ... );
void				vExit ();

int				 iKey_Up ();
int				 iKey_Down ();
int				 iKey_Left ();
int				 iKey_Right ();
int				 iRead_LOGCTL ();
int				 iRead_STKCTL ();

int				 iGet_Number_String ();
int				 iGet_Hexa_String ();
int				 iHexString_TO_Integer ();
int				 iHexChar_TO_Integer ();
int				 iFind_Pos2 ();
int				 iFile_Open ();
int				 iFile_Reread ();

char			  *cpaHelpTxt[] =
{
	"+-------------------------------------------------+",
	"| T,t,1    : Top             B,b,G  : Bottom      |",
	"| 0        : 1st Column           $ : Last Column |",
	"| a        : ASCII mode           e : EBCDIC mode |",
	"| k,UP     : 1 line up       j,DOWN : 1 line down |",
	"| ^B,PGUP  : 1 page up      ^F,PGDN : 1 page down |",
	"| h, LEFT  : move backward                        |",
	"| l, RIGHT : move forward                         |",
	"| g ...    : Goto new location (decimal)          |",
	"| x ...    : Goto new location (hexadecimal)      |",
	"| s,/ s,x,d,c pattern : search content            |",
	"| n        : search next                          |",
	"| ^L       : resume screen                        |",
	"| q        : Quit Program                         |",
	"| ?        : this Help screen                     |",
	"+-------------------------------------------------+",
	NULL
};

unsigned char	caString[80], caSearchStr[80];

char		*cpErrNumber, *cpJobName, *cpSearchStr = NULL, caErrStr[256];

long		iTokenCount, iSearchPos, iWhichTable, i256PageMax,
		iLineMax, iFileSize, iPrevPage, iPageCur, iPageMax, iCurPos;
static long	iAsciiOrEbcdic, iMaxCurYPos, iFilePos, iGab, iMaxPos, iMinPos;
static long	iCurX, iCurY, iCurLine, iLinePos, iCurMenuNo, iDisplayFlag = 1,
		iAdjustFlag = 0, iEditMode = 0, iPrevXPos = -1, iPrevYPos = -1,
		iPrevXi = -1, iPrevLine = -1, iPrevMenuNo = -1, iLinkSize;
int iDigitLen, iHexaLen;

FILE		*fpSrc;

#ifdef DEBUG
FILE		*fpDebug, *fpDump;
#endif

char		*cpFileName, *cpTmpPtr, *cpRealPath;
unsigned char  *cpFileContent = NULL;
char		caSelectStmt[256];
const char     *cpZero = "00000000000000000000000000000";

int iGet_String (unsigned char *buf, int len)
{
	int				 c, n;

	refresh ();
	for (n = 0; n < len - 1;)
	{
		c = getch ();
		if (c == '\t' || c == 0x0a || c == 0x0d || c == 0x1b)
			break;
		if (c == '\b')
		{
			if (n == 0)
				return -1;
			addch ('\b');
			addch (' ');
			addch ('\b');
			refresh ();
			n--;
			continue;
		}
		buf[n++] = c;
		addch (c);
		refresh ();
	}
	if (n == 0)
		return -1;

	if (iAsciiOrEbcdic)
	{
		int				 i, k;
		unsigned char	tmpbuf[1024];

		k = ascebc (buf, n, tmpbuf, n + 10);

		if (tmpbuf[0] == 0x0E)
			memcpy (buf, tmpbuf + 1, n);
		else
			memcpy (buf, tmpbuf, n);
	}

	buf[n] = '\0';
	return n;
}

void 
vDraw_Main_Menu ()
{
	int				 i;

	vAdd_Reverse_String (0, 0, "%80s", " ");
	vAdd_Reverse_String (0, 30, " Binary File Dump ");
	mvprintw (1, 0, "FILE= %s", cpFileName);
	mvprintw (2, 0, "SIZE=%ld", (unsigned long)iFileSize);
	mvprintw (2, 36, "%d Bytes Page=[%ld/%ld]				",
							MYPAGESIZE,
							(int)((iCurLine * COLSIZE + iCurX)/MYPAGESIZE) + 1,
							i256PageMax);

	mvprintw (STARTLINE - 1, 0, "-----------------------------------------------------------");
	mvprintw (STARTLINE - 1, STARTASCII, "------ ASCII -------");

	mvprintw (ENDLINE - 1, 0, "%s", cpTmpPtr);

	if (cpRealPath)
	{
		/*--------------------------------------------------------------* 
		mvprintw (2, 36, "link destination is %s", cpTmpPtr);
		*--------------------------------------------------------------*/ 
		mvprintw (ENDLINE - 1, 0, "link dest [%s] %s", cpRealPath, cpTmpPtr);
	}
	mvprintw (ENDLINE - 2, 0, "-----------------------------------------------------------"
				 "---------------------");
	refresh ();
}

void 
vMain_Loop ()
{
	int				 i, c;

	vDraw_Main_Menu ();

	vDisplay_Screen (iLinePos);

	vSub_Loop ();
}

int				 iLenNo;

void 
vSub_Loop ()
{
	int	i, j, k, c, d, m, n, len, no;
	static int		ach, xch;
	char digittmp[64], hexatmp[64];
	int dlen, xlen;
	long ipos;

	while (1)
	{

		iPrevPage = iPageCur;
		iPageCur = (int)((iCurLine * COLSIZE + iCurX) / MAXALLOC);
#ifdef DEBUG
		vLog_Print("First in vSub_Loop : iPrevPage : %d, iPageCur [%d], iLinePos : %d\n", iPrevPage, iPageCur, iLinePos);
#endif
		if((iCurLine * COLSIZE + iCurX) < (iPrevPage * MAXALLOC))
		{
			if(iFile_Reread(1, iPageCur, cpFileName) < 0)
				vExit(-1);
		}
		if((iCurLine * COLSIZE + iCurX) >= ((iPrevPage+1) * MAXALLOC))
		{
			if(iFile_Reread(2, iPageCur, cpFileName) < 0)
				vExit(-1);
		}

		/* 이전 커서위치의 반전을 없앤다. */
		if (!iAdjustFlag && iPrevXPos >= 0 && (iPrevYPos - STARTLINE) <= iMaxCurYPos)
		{
			mvprintw (iPrevYPos, iPrevXPos, "%.02X", xch);
			mvprintw (iPrevYPos, STARTASCII + 2 + iPrevXi, "%c", isprint (ach) ? ach : '.');
		}

		if (iDisplayFlag)
		{
			mvprintw (2, 36, "%d Bytes Page=[%d/%d]				",
									MYPAGESIZE,
									(int)((iCurLine * COLSIZE + iCurX)/MYPAGESIZE) + 1,
									i256PageMax);

			iLinePos = (((iCurLine - iCurY) < 0) ? 0 : (iCurLine - iCurY));
 
#ifdef DEBUG
			vLog_Print("before vDisplay_Screen:%d\n", iLinePos);
#endif
			vDisplay_Screen (iLinePos);
#ifdef DEBUG
			vLog_Print("after  vDisplay_Screen:%d\n", iLinePos);
#endif
		}

		/* 현재의 커서위치를 반전시킨다. */

		iGab = ((iPageCur == 0) ? (0) : (MAXALLOC));
		xch = cpFileContent[iCurLine * COLSIZE + iCurX - (iPageCur * MAXALLOC) + iGab];

		ach = ((iAsciiOrEbcdic) ? (ascii_of_ebcdic[xch]) : (xch));

		vAdd_Reverse_String (STARTLINE + iCurY,
		  ((iCurX > 7) ? (STARTCOL + iCurX * 3 + 1) : (STARTCOL + iCurX * 3)),
									"%.02X", xch);
		vAdd_Reverse_String (STARTLINE + iCurY, STARTASCII + 2 + iCurX, "%c",
									isprint (ach) ? ach : '.');

		iPrevXPos = ((iCurX > 7) ? (STARTCOL + iCurX * 3 + 1) : (STARTCOL + iCurX * 3));
		iPrevYPos = STARTLINE + iCurY;
		iPrevLine = iCurLine;
		iPrevXi = iCurX;

		iCurPos = iCurLine * COLSIZE + iCurX;
		sprintf( digittmp, "%012ld", iCurPos );
		dlen = strlen( digittmp );
		sprintf( hexatmp, "%010lX", iCurPos );
		xlen = strlen( hexatmp );

		mvprintw (ENDLINE + 1, 0, "%70s", " ");
		mvprintw (ENDLINE + 1, 0, "%s-%s: ", digittmp, hexatmp);
		//mvprintw (ENDLINE + 1, 0, "%.*s%s-%.*s%s: ", iDigitLen - dlen, cpZero, digittmp, iHexaLen - xlen, cpZero, hexatmp);
		refresh ();

		iDisplayFlag = 0;
		iAdjustFlag = 0;

		c = getch ();

		mvprintw (ENDLINE + 1, 0, "%79s", " ");
		mvprintw (ENDLINE + 1, 0, "%s-%s: ", digittmp, hexatmp);
		//mvprintw (ENDLINE + 1, 0, "%.*s%s-%.*s%s: ", iDigitLen - dlen, cpZero, digittmp, iHexaLen - xlen, cpZero, hexatmp);
		refresh ();

		if (iEditMode == 0)
		{
			if (c == 'q')
				return;
			continue;
		}

		switch (c)
		{
		case 'k':
		case KEY_UP:
			iKey_Up ();
			break;
		case 'j':
		case KEY_DOWN:
			iKey_Down ();
			break;
		case 'h':
		case KEY_LEFT:
			iKey_Left ();
			break;
		case 'l':
		case KEY_RIGHT:
			iKey_Right ();
			break;
		case 'a':					 /* ascii mode */
			iDisplayFlag = 1;
			iAsciiOrEbcdic = 0;
			mvprintw (STARTLINE - 1, STARTASCII, "------ ASCII -------");
			break;
		case 'e':					 /* ebcdic mode */
			iDisplayFlag = 1;
			iAsciiOrEbcdic = 1;
			mvprintw (STARTLINE - 1, STARTASCII, "------ EBCDIC ------");
			break;
		case 0x1B:					 /* PGUP & PGDN key for TERM=xterm */
			d = getch ();
			c = getch ();
			if(d == 0x5B)
			{
				if (c == 0x35)
				{
					getch ();
					vCal_PGUP_Key (&iCurY, &iCurLine, &iDisplayFlag);
				}
				else if (c == 0x36)
				{
					getch ();
					vCal_PGDN_Key (&iCurLine, &iDisplayFlag);
					vAdjust_Last_Position ();
				}
				else if (c == 0x41)
					iKey_Up ();
				else if (c == 0x42)
					iKey_Down ();
				else if (c == 0x44)
					iKey_Left ();
				else if (c == 0x43)
					iKey_Right ();
			}
			break;
		case KEY_PPAGE:					 /* PGUP key for all terminals except 'xterm' */
			vCal_PGUP_Key (&iCurY, &iCurLine, &iDisplayFlag);
			break;
		case KEY_NPAGE:					 /* PGDN key for all terminals except 'xterm' */
			vCal_PGDN_Key (&iCurLine, &iDisplayFlag);
			vAdjust_Last_Position ();
			break;
		case 'B':
		case '':					 /* PGUP key for all terminals */
			vCal_PGUP_Key (&iCurY, &iCurLine, &iDisplayFlag);
			break;
		case 'F':					 /* PGDN key for all terminals */
		case '':					 /* PGDN key for all terminals */
			vCal_PGDN_Key (&iCurLine, &iDisplayFlag);
			vAdjust_Last_Position ();
				/*-----------------------------------------------* 
				*-----------------------------------------------*/
			break;
		case '?':
			vDraw_Help_Txt ();
			refresh ();
			getch ();
			for (i = 0; i < SCREENSIZE; i++)
				mvprintw (STARTLINE + i, 0, "%79s", " ");
			refresh ();
			iDisplayFlag = 1;
			break;
		case 0x0c: /* refresh '^L' */
			for (i = 0; i < SCREENSIZE; i++)
				mvprintw (STARTLINE + i, 0, "%79s", " ");
			refresh ();
			iDisplayFlag = 1;
			break;
		case '1':
		case 't':
		case 'T':
			iCurY = 0;
			iCurLine = 0;
			iCurX = 0;
			iDisplayFlag = 1;
			iPrevXPos = iPrevYPos = iPrevXi = iPrevLine = -1;
			break;
		case 'G':
		case 'b':
			iCurY = SCREENSIZE - 1;
			iCurLine = iLineMax - 1;
			if (iCurY > iCurLine)
				iCurY = iCurLine;
			iCurX = (iFileSize % COLSIZE) ? (iFileSize % COLSIZE) - 1 : COLSIZE - 1;
			iDisplayFlag = 1;
			iPrevXPos = iPrevYPos = iPrevXi = iPrevLine = -1;
			break;
		case '0':
			iCurX = 0;
			break;
		case '$':
			iCurX = (iFileSize % COLSIZE) ? (iFileSize % COLSIZE) - 1 : COLSIZE - 1;
			break;
		case 'g':
			mvprintw (ENDLINE + 1, 0, "%79s", " ");
			mvprintw (ENDLINE + 1, 0, "goto (decimal) : ");
			refresh ();
			if ((c = iGet_Number_String (caString, 11)) == -1)
				break;

			if (c > iFileSize - 1)
			{
				beep ();
				mvprintw (ENDLINE + 1, 40, "address %s out of range", caString);
				break;
			}
			iCurLine = (int) (c / COLSIZE);
			iCurX = c % COLSIZE;
			iCurY = iCurLine % SCREENSIZE;
			iDisplayFlag = 1;
			break;
		case 'x':
			mvprintw (ENDLINE + 1, 0, "%79s", " ");
			mvprintw (ENDLINE + 1, 0, "goto (hexa)	 : ");
			refresh ();
			c = iGet_Hexa_String (caString, 7);
#ifdef DEBUG
			fprintf(fpDebug, "c=%d, [%s]\n", c, caString);
#endif
			if (c  == -1)
	 		{
				break;
	 		}

			if (c > iFileSize - 1)
			{
				beep ();
				mvprintw (ENDLINE + 1, 40, "address 0x%s out of range", caString);
				break;
			}
			iCurLine = (int) (c / COLSIZE);
			iCurX = c % COLSIZE;
			iCurY = iCurLine % SCREENSIZE;
			iDisplayFlag = 1;
			break;
		case '/':
			mvprintw (ENDLINE + 1, 0, "%79s", " ");
			mvprintw (ENDLINE + 1, 0, "search str : ");
			c = iGet_String (caString, 41);
			if (c != -1)
			{
				iLenNo = (d == 'd') ? 1 : c;
				memcpy(caSearchStr, caString, iLenNo);
				cpSearchStr = (char *) caSearchStr;
#ifdef DEBUG
				fprintf(fpDebug, "before vFind_Str_And_Pos_Cur\n");
#endif
				vFind_Str_And_Pos_Cur (iLenNo);
#ifdef DEBUG
				fprintf(fpDebug, "after  vFind_Str_And_Pos_Cur\n");
#endif

				iDisplayFlag = 1;
			}
			break;
		case 's':
			while (1)
			{
				mvprintw (ENDLINE + 1, 0, "%79s", " ");
				/* 01234567890123456789012 */
				mvprintw (ENDLINE + 1, 0, "search (s, x, d, c) : ");
				refresh ();
				d = getch ();
				mvprintw (ENDLINE + 1, 0, "%79s", " ");
				switch (d)
				{
				case 's':
					mvprintw (ENDLINE + 1, 0, "search str : ");
					c = iGet_String (caString, 41);
					break;
				case 'x':
					mvprintw (ENDLINE + 1, 0, "search hex : ");
					c = iGet_Hexa_String (caString, 41);
					if (c != -1)
					{
						len = strlen ((char *) caString);
						for (i = 0, j = 0; i < len;)
						{
							caString[j] = iHexString_TO_Integer (caString + i, 2);
							j++;
							i += 2;
						}
						caString[j] = 0;
						c = j;
					}
					break;
				case 'd':
					mvprintw (ENDLINE + 1, 0, "search dec : ");
					c = iGet_Number_String (caString, 4);
					if (c != -1)
					{
						caString[0] = c;
						caString[1] = 0;
					}
					break;
				case 'c':
					mvprintw (ENDLINE + 1, 0, "search char : ");
					c = iGet_String (caString, 2);
					break;
				default:
					continue;
				}
				break;
			}
			if (c != -1)
			{
				iLenNo = (d == 'd') ? 1 : c;
				memcpy(caSearchStr, caString, iLenNo);
				cpSearchStr = (char *) caSearchStr;

				vFind_Str_And_Pos_Cur (iLenNo);

				iDisplayFlag = 1;
			}
			break;
		case 'n':

			vFind_Str_And_Pos_Cur (iLenNo);

			iDisplayFlag = 1;
			break;
		case 'q':
			return;
		default:
			beep ();
			break;

		}
		/*--------------------------------------------------------------* 
		if(iSearchPos > 0)
		*--------------------------------------------------------------*/ 
			iSearchPos = iCurLine * COLSIZE + iCurX + 1;
	}
}

void 
vDisplay_Screen (line)
	  long				 line;
{
	long				 i, j, k, n, mm;
	unsigned char	c;

	move (STARTLINE, 0);

	for (i = 0; i < SCREENSIZE; i++)
	{
		if ((line + i + 1) > iLineMax)
		{
			mvprintw (STARTLINE + i, 0, "%79s", " ");
		}
		else
		{
			if(((int)(iCurLine + i + 1)/MAXALLOC) < iPageCur)
				mm = iPageCur - 1;
			else if(((int)(iCurLine + i + 1)/MAXALLOC) > iPageCur)
				mm = iPageCur;
			else
				mm = iPageCur;

			mvprintw (STARTLINE + i, 0, "%79s", " ");
			if ((line + i) * COLSIZE != iFileSize)
				mvprintw (STARTLINE + i, 0, "%010lX: ", (line + i) * 16);
			k = STARTCOL;
			if ((line + i + 1) == iLineMax)
			{
				;
			}
			for (j = 0; j < COLSIZE; j++)
			{
				if (((line + i) * COLSIZE + j) > (iFileSize - 1))
				{
					;
				}
				else
				{

					n = cpFileContent[(line + i) * COLSIZE + j - (mm * MAXALLOC)];
					mvprintw (STARTLINE + i, k + j * 3, "%.02X", n);
					c = ((iAsciiOrEbcdic) ? (ascii_of_ebcdic[n]) : (n));
					mvprintw (STARTLINE + i, k + (STARTASCII - STARTCOL) + j + 2 + ((j > 7) ? -1 : 0), "%c",
								 isprint (c) ? c : '.');
					if (j == 7)
						k++;
				}
			}
			iMaxCurYPos = i;
		}
	}
}

void 
vCal_PGUP_Key (iy, iline, iflag)
	  int				*iy;
	  int				*iline;
	  int				*iflag;
{
	if ((*iline - SCREENSIZE + 1) > 0)
	{
		*iflag = 1;
		*iline -= SCREENSIZE;
		if (*iline < SCREENSIZE)
			*iline = *iy;
	}
	else
		beep ();
}

void 
vCal_PGDN_Key (iline, iflag)
	  int				*iline, *iflag;
{
	if ((*iline + SCREENSIZE) < iLineMax)
	{
		*iflag = 1;
		*iline += SCREENSIZE;
	}
	else
		beep ();
}

int 
iGet_Number_String (cpBuff, iMaxLen)
	  unsigned char  *cpBuff;
	  int				 iMaxLen;
{
	int				 i;
	int				 c;

	refresh ();
	for (i = 0; i < iMaxLen - 1;)
	{
		c = getch ();
		if (isdigit (c) || c == ' ')
		{
			cpBuff[i++] = c == ' ' ? '0' : c;
			addch (c);
			refresh ();
			continue;
		}
		if (c == '\t' || c == 0x0a || c == 0x0d || c == 0x1b)
			break;
		if (c == '\b')
		{
			if (i == 0)
				return -1;
			addch ('\b');
			addch (' ');
			addch ('\b');
			refresh ();
			i--;
			continue;
		}
	}
	if (i == 0)
		return -1;
	cpBuff[i] = 0;
	return atoi ((char *) cpBuff);
}

int 
iGet_Hexa_String (cpBuff, iMaxLen)
	  unsigned char  *cpBuff;
	  int				 iMaxLen;
{
	int				 iLen;
	int				 c;
	unsigned char	caTmp[80];

	refresh ();
	for (iLen = 0; iLen < iMaxLen - 1;)
	{
		c = getch ();
		if (isxdigit (c))
		{
			cpBuff[iLen++] = toupper (c);
			addch (c);
			refresh ();
			continue;
		}
		if (c == '\t' || c == 0x0a || c == 0x0d || c == 0x1b)
			break;
		if (c == '\b')
		{
			if (iLen == 0)
				return -1;
			addch ('\b');
			addch (' ');
			addch ('\b');
			refresh ();
			iLen--;
			continue;
		}
	}
	if (iLen == 0)
		return -1;
	cpBuff[iLen] = 0;
	if (iLen % 2)
	{
		memcpy (caTmp, cpBuff, iLen + 1);
		cpBuff[0] = '0';
		memcpy (cpBuff + 1, caTmp, iLen + 1);
	}

	/*--------------------------------------------------------------* 
	return ((iLen % 2) ? ((int) (iLen / 2) + 1) : (int) (iLen / 2));
	*--------------------------------------------------------------*/ 
	return iHexString_TO_Integer(cpBuff, strlen((char *)cpBuff));
}

int 
iFile_Reread (flag, page, filename)
	  long				 flag, page;
	  char			  *filename;
{
	unsigned long size, pagetmp, n;


#ifdef DEBUG
	fprintf(fpDebug, "iFile_Reread, babo(%d), iPageMax(%d) curpage(%d)\n", flag, iPageMax, page);
#endif
	if(iPageMax <= 3)
	{
		iPageCur = 0;
		iGab = 0;
		return 0;
	}
#ifdef DEBUG
	fprintf(fpDebug, "iFile_Reread, real open\n", flag, iPageMax, page);
#endif
	free(cpFileContent);

	/*--------------------------------------------------------------* 
	if(flag == 4)
		iPageCur = page + 1;
	*--------------------------------------------------------------*/ 

	pagetmp = page;
	if(page > 0)
		pagetmp--;

	fseek (fpSrc, 0L+(pagetmp * MAXALLOC), SEEK_SET);
	iMinPos = pagetmp * MAXALLOC;

	if(page == (iPageMax - 1))
	{
		if(iPageMax > 1)
			size = iFileSize - (page * MAXALLOC) + MAXALLOC;
		else
			size = iFileSize;
	}
	else if(page == (iPageMax - 2))
	{
		if(iPageMax > 2)
			size = iFileSize - (page * MAXALLOC) + MAXALLOC;
		else
			size = iFileSize;
	}
	else
	{
		size = MAXALLOC * 3;
		if(page == 0 && iPageMax == 3)
			size = iFileSize;
	}
	iMaxPos = (pagetmp * MAXALLOC) + size;

	cpFileContent = malloc ( size );
	if(cpFileContent == NULL)
	{
		sprintf(caErrStr, "\n*(02) memory malloc error : %s\n\n", strerror (errno));
		return -1;
	}

#ifdef DEBUG
	fprintf( fpDebug, "size=%ld\n", size );
#endif

	if ((n = fread (cpFileContent, 1, size, fpSrc)) != size)
	{
		sprintf (caErrStr, "\n(02) file(%s) read error. required bytes(%ld) and read bytes(%ld) : %s\n\n",
									filename, size, n, strerror (errno));
		sprintf(caErrStr+strlen(caErrStr), "iPageMax : %d, page : %d\n", iPageMax, page);
		return -1;
	}

	return 0;
}

int 
iFile_Open (argno, filename)
	  int				 argno;
	  char			  *filename;
{
	int tmplen, readbytes, tmperrno, size, len;
	char *p, digittmp[64], hexatmp[64];
	unsigned char *str;
	struct stat stbuf;

	iEditMode = 0;
	tmplen = MYPATHMAX;

	if (argno != 3)
		cpFileName = filename;

	if (lstat (filename, &stbuf) < 0)
	{
		fprintf (stderr, "file(%s) : %s\n", filename, strerror (errno));
		return -1;
	}

	if (S_ISREG (stbuf.st_mode))		  /* regular file */
	{
		cpTmpPtr = "regular file";
		if ((fpSrc = fopen (filename, "r")) == NULL)
		{
			fprintf (stderr, "\nfile(%s) open error : %s\n\n", filename, strerror (errno));
			return -1;
		}
		/*--------------------------------------------------------------* 
		fseek (fpSrc, 0L, SEEK_END);
		iFileSize = ftell (fpSrc);
		fseek (fpSrc, 0L, SEEK_SET);
		*--------------------------------------------------------------*/ 
		iFileSize = stbuf.st_size;
		sprintf( digittmp, "%ld", iFileSize );
		sprintf( hexatmp, "%lX", iFileSize );

		len = strlen(digittmp);
		iDigitLen = (len % 2 == 0 ? len : len + 1);
		if( iDigitLen < 4 )
			iDigitLen = 4;
		len = strlen(hexatmp);
		iHexaLen = (len % 2 == 0 ? len : len + 1);
		if( iHexaLen < 4 )
			iHexaLen = 4;

		size = ((iFileSize > (MAXALLOC*3)) ? (MAXALLOC*3) : iFileSize);
		iMaxPos = size;

		fprintf(stdout, "memory alloc ... ");

		cpFileContent = (unsigned char *) malloc (size + 1);
		if (cpFileContent == NULL)
		{
			fprintf (stderr, "\n*(01) memory malloc error : %s, size=%d\n\n", strerror (errno), size);
			return -1;
		}

		fprintf(stdout, "OK !\n");

		fprintf(stdout, "file read ... ");

		if ((readbytes = fread (cpFileContent, 1, size, fpSrc)) != size)
		{
			fprintf (stderr, "\n(01) file(%s) read error : %s\n\n", filename, strerror (errno));
			return -1;
		}

		fprintf(stdout, "OK !\n");

		/* get 256 bytes page max */
		i256PageMax = (int) (iFileSize / MYPAGESIZE);
		if ((iFileSize % MYPAGESIZE) > 0)
			i256PageMax++;

		/* get MAXALLOC(5000000) page max */
		iPageMax = (int) (iFileSize / MAXALLOC);
		if ((iFileSize % MAXALLOC) > 0)
			iPageMax++;
		
		/* get line max */
		iLineMax = iFileSize / COLSIZE;
		if ((iFileSize % COLSIZE) > 0)
			iLineMax++;

#ifdef DEBUG
		fprintf(fpDebug, "iPageMax : %d, iLineMax : %d, i256PageMax : %d, size : %d, iFileSize : %d\n",
								iPageMax, iLineMax, i256PageMax, size, iFileSize);
#endif

		if (iFileSize)
			iEditMode = 1;
	}
	else if (S_ISLNK (stbuf.st_mode))
	{
		cpTmpPtr = "symbolic link";
		if (argno != 3)
		{
			cpRealPath = (char *) calloc (MYPATHMAX, sizeof (char));
			if (cpRealPath == NULL)
			{
				fprintf (stderr, "\n(03) memory calloc error : %s\n\n", strerror (errno));
				return -1;
			}
			iLinkSize = readlink (filename, cpRealPath, tmplen);
			if (iLinkSize < 0)
			{
				fprintf (stderr, "\nfile(%s) readlink error : %s\n\n", filename, strerror (errno));
				return -1;
			}

			if (cpRealPath)
			{
				fprintf (stdout, "\nfile(%s) is symbolic link to [%s]\n\n", filename, cpRealPath);
				if (iFile_Open (3, cpRealPath) < 0)
					return -1;
			}
			else
			{
				fprintf (stderr, "%s -> %s (link file does not exist)", cpTmpPtr, cpRealPath);
				return -1;
			}

			/*--------------------------------------------------------------* 
			free (cpRealPath);
			*--------------------------------------------------------------*/ 
			return 0;
		}
		else
		{
			fprintf (stderr, "\nfile(%s) : Number of symbolic links encountered during path name traversal exceeds 2\n\n", filename);
			return -1;
		}
	}
	else
	{
		if (S_ISDIR (stbuf.st_mode))
			cpTmpPtr = "directory";
		else if (S_ISCHR (stbuf.st_mode))
			cpTmpPtr = "character special";
		else if (S_ISBLK (stbuf.st_mode))
			cpTmpPtr = "block special";
		else if (S_ISFIFO (stbuf.st_mode))
			cpTmpPtr = "fifo";
		else if (S_ISSOCK (stbuf.st_mode))
			cpTmpPtr = "socket";
		else
			cpTmpPtr = "** unknown mode **";

		fprintf (stderr, "\nfilename(%s) is %s. can't open.\n\n", filename, cpTmpPtr);
		return -1;
	}

	return 0;
}

void 
vAdjust_Last_Position ()
{
	/*--------------------------------------------------------------* 
	 * 다시 디스플레이 되지 않아도 될 때 디스플레이되는 것을 막기 
	 * 위해 넣음. HONGILDONG.
	 *--------------------------------------------------------------*/ 

	if (((iCurLine + 1) == iLineMax) &&
		 (iCurX > ((iFileSize % COLSIZE) ? ((iFileSize % COLSIZE) - 1) : (COLSIZE - 1))))
	{
		iCurX = (iFileSize % COLSIZE) ? (iFileSize % COLSIZE) - 1 : COLSIZE - 1;
		iAdjustFlag = 1;
		iDisplayFlag = 1;
	}
}

void 
vExit (n)
	  int				 n;
{
	if(n <= 0)
		endwin ();

	if(n < 0)
		fprintf (stderr, "%s", caErrStr);
	else
		fprintf (stdout, "\n\nFile Dump Program Version. 2.4 (build 1519) made by HONGILDONG  (2020.03.03.)\n\n");
	if (n == 1)
		fprintf (stderr, "Usage : filedump	filename\n\n");

	exit (n);
}


int 
iHexString_TO_Integer (cpBuff, iLen)
	  char			  *cpBuff;
	  int				 iLen;
{
	int				 i, iTmp, iHex, iRet;

	iHex = 1;
	iRet = 0;
	for (i = iLen - 1; i >= 0; i--)
	{
		iTmp = iHexChar_TO_Integer (cpBuff[i]);
		if (iTmp < 0)
			break;
		iRet = iRet + iTmp * iHex;
		iHex = iHex * 16;
	}
	return iRet;
}

static char			  *cpHexCode = "0123456789ABCDEF";

int 
iHexChar_TO_Integer (ch)
	  unsigned char	ch;
{
	int				 iCmp, iLow, iMid, iHigh;

	iLow = 0;
	iHigh = 15;

	while (iLow <= iHigh)
	{
		iMid = (int) ((iLow + iHigh) / 2);
		if ((iCmp = ch - cpHexCode[iMid]) == 0)
			return iMid;
		else if (iCmp > 0)
			iLow = iMid + 1;
		else
			iHigh = iMid - 1;
	}
	beep ();
	return -1;
}

int 
iFind_Pos2 (cpStr1, cpStr2, iLen1, iLen2)
	  register char  *cpStr1, *cpStr2;
	  register int	 iLen1, iLen2;
{
	register int	 i, j;

	for (i = 0; i < iLen1; i++)
	{
		for (j = 0; j < iLen2; j++)
		{
			if (cpStr1[i + j] == cpStr2[j])
			{
				if (i == (iLen1 - 1) && j < (iLen2 - 1))
					return -1;
				if (j == (iLen2 - 1))
					return i;
			}
			else
				break;
		}
	}
	return -1;
}

void 
vFind_Str_And_Pos_Cur (len)
	  int				 len;
{
	int				 line, page, searchtmp, no, flag;

	
#ifdef DEBUG
	vLog_Print("babo00 -- vFind_Str_And_Pos_Cur\n");
#endif
	flag = 0;

	if (cpSearchStr == NULL)
	{
		vAdd_Reverse_String (ENDLINE + 1, 40, "  No previous expression ");
		return;
	}

#ifdef DEBUG
		fprintf(fpDebug, "before iFind_Pos2, [%d][%d][%d]-[%d][%d][%d]\n",
								iSearchPos, iPageCur, iGab,
								iSearchPos - (iPageCur*MAXALLOC) + iGab, iMaxPos - iSearchPos, len);
#endif
	no = iFind_Pos2 (cpFileContent + iSearchPos - (iPageCur*MAXALLOC) + iGab,
									 cpSearchStr,
									 iMaxPos - iSearchPos,
									 len);
#ifdef DEBUG
		fprintf(fpDebug, "after  iFind_Pos2\n");
#endif

#ifdef DEBUG
		fprintf(fpDebug, "iFileSize :%d, iMaxPos : %d, iMinPos : %d, SearchPos : %d\n",
								iFileSize, iMaxPos, iMinPos, no);
#endif
	if (no == -1)
	{
		/*--------------------------------------------------------------* 
		 * 현재가 마지막 Page 인가 체크해 보고, 마지막이면 처음으로 
		 * 가서 다시 read 하고, 다시 search. HONGILDONG
		 *--------------------------------------------------------------*/ 

AGAINLABEL:
		if(flag == 1)
			page = -2;
		else
			page = iPageCur;
		for(;;)
		{
			mvprintw (2, 36, "%d Bytes Page=[%d/%d]				",
									MYPAGESIZE,
									(int)(iMinPos/MYPAGESIZE) + 1,
									i256PageMax);
			refresh();

			page += 2;
			if(page >= iPageMax)
			{
				/*--------------------------------------------------------------* 
				 * not found 일 경우 원래의 자리를 기억했다가 원래의 위치로 돌아
				 * 가야 한다.
				 *--------------------------------------------------------------*/ 
				if(flag == 1)
				{
					vAdd_Reverse_String (ENDLINE + 1, 40, " Not found ");
					beep ();

					if(iFile_Reread(3, iPageCur, cpFileName) < 0)
						vExit(-1);
					return;
				}
				/*--------------------------------------------------------------* 
				if(iSearchPos == 0 && flag == 0 && iPageCur == 0 && iPageMax <=3)
				*--------------------------------------------------------------*/ 
				/*--------------------------------------------------------------*
				 * search 위치가 처음 '0' 이면 끝까지 한번만 탐색하고,
				 * 못찾으면 리턴
				 *--------------------------------------------------------------*/ 

#ifdef DEBUG
				vLog_Print("babo01 --iSearchPos[%d] flag[%d] iPageCur[%d]\n", iSearchPos, flag, iPageCur);
#endif
				if(iSearchPos == 0 && flag == 0 && iPageCur == 0)
				{
					vAdd_Reverse_String (ENDLINE + 1, 40, " Not found ");
					beep ();

#ifdef DEBUG
					vLog_Print("babo02--iSearchPos[%d] flag[%d] iPageCur[%d]\n", iSearchPos, flag, iPageCur);
#endif
					if(iFile_Reread(3, iPageCur, cpFileName) < 0)
						vExit(-1);
					return;
				}
				flag = 1;
#ifdef DEBUG
				vLog_Print("babo03--iSearchPos[%d] flag[%d] iPageCur[%d]\n", iSearchPos, flag, iPageCur);
#endif
				goto AGAINLABEL;
			}

			if(iFile_Reread(4, page, cpFileName) < 0)
				vExit(-1);
			searchtmp = 0;
			no = iFind_Pos2 (cpFileContent + searchtmp, cpSearchStr, iMaxPos - iMinPos, len);
			if (no == -1)
			{
				/*--------------------------------------------------------------* 
				 * 현재가 마지막 Page 인가 체크해 보고, 마지막이면 처음으로 
				 * 가서 다시 read 하고, 다시 search. HONGILDONG
				 *--------------------------------------------------------------*/ 
				continue;
			}
			else
			{
				if(flag == 1 || iPageMax <= 3)
					vAdd_Reverse_String (ENDLINE + 1, 40, " (wrapped) ");
				iSearchPos = iMinPos + no;
#ifdef DEBUG
				fprintf(fpDebug, "find. iSearchPos : %d\n", iSearchPos);
#endif
				break;
			}
		}
	}
	else
		iSearchPos += no;


	line = iCurLine;
	iCurLine = (int) (iSearchPos / COLSIZE);
	iCurX = iSearchPos % COLSIZE;

	mvprintw (2, 36, "%d Bytes Page=[%d/%d]				",
							MYPAGESIZE,
							(int)((iCurLine * COLSIZE + iCurX)/MYPAGESIZE) + 1,
							i256PageMax);
#ifdef DEBUG
	fprintf(fpDebug, "iCurLine * COLSIZE + iCurX : %d, iPageMax : %d, iPageCur : %d, %d\n",
							iCurLine * COLSIZE + iCurX, iPageMax, iPageCur, (int)((iCurLine * COLSIZE + iCurX) / MAXALLOC));
#endif

	if (iCurLine < line)
	{
		if ((line - iCurLine) <= iCurY)
			iCurY = iCurY - (line - iCurLine);
		else
			iCurY = 0;
	}
	else if (iCurLine > line)
	{
		if ((iCurLine - line) <= (SCREENSIZE - iCurY - 1))
			iCurY = iCurY + (iCurLine - line);
		else
			iCurY = SCREENSIZE - 1;
	}
	iDisplayFlag = 1;
	iSearchPos++;
#ifdef DEBUG
	fprintf(fpDebug, "babo , end of vFind_Str_And_Pos_Cur.\n");
	fprintf(fpDebug, "iFileSize :%d, iMaxPos : %d, iMinPos : %d, SearchPos : %d\n",
							iFileSize, iMaxPos, iMinPos, no);
	/*--------------------------------------------------------------* 
	fwrite(cpFileContent, 1, 128, fpDump);
	*--------------------------------------------------------------*/ 
#endif
	iPageCur = 0;
}

void 
vDraw_Help_Txt ()
{
	int				 i;

	for (i = 0; cpaHelpTxt[i]; i++)
		mvprintw (STARTLINE + i, STARTCOL, "%s", cpaHelpTxt[i]);

	vAdd_Reverse_String (STARTLINE, 29, "*SCREEN HELP*");
	move (ENDLINE + 1, 11);
}

void 
vAdd_Reverse_String (int y, int x, const char *fmt, ...)
{
	va_list			args;
	char			  buf[512];		  /* Potential overflow situation */

	va_start (args, fmt);
	vsprintf (buf, fmt, args);
	va_end (args);

	attron (A_REVERSE);
	mvprintw (y, x, "%s", buf);
	attroff (A_REVERSE);
	refresh ();

}

/*--------------------------------------------------------------* 
void 
vAdd_UnderLine_String (va_alist)
	  va_dcl
{
	va_list			args;
	char			  *ptr, buf[100];
	int				 x, y;

	va_start (args);

	y = va_arg (args, int);
	x = va_arg (args, int);
	ptr = va_arg (args, char *);
	vsprintf (buf, ptr, args);

	attron (A_UNDERLINE);
	mvprintw (y, x, "%s", buf);
	attroff (A_UNDERLINE);
	refresh ();

	va_end (args);
}
*--------------------------------------------------------------*/ 

void vLog_Print ( const char *fmt, ... )
{
	va_list			args;

	va_start (args, fmt);

#ifdef DEBUG
	if (fpDebug)
		vfprintf (fpDebug, fmt, args);
#endif

	va_end (args);
}

int 
iKey_Up ()
{
	if (iCurY == 0 && iCurLine == 0)
	{
		beep ();
		return -1;
	}
	else if (iCurY == 0 && iCurLine > 0)
	{
		iCurLine--;
		iDisplayFlag = 1;
	}
	else
	{
		iCurY--;
		iCurLine--;
	}

	return 0;
}

int 
iKey_Down ()
{
	if (iCurY == iMaxCurYPos && iCurLine == (iLineMax - 1))
	{
		beep ();
		return -1;
	}
	else if (iCurY == iMaxCurYPos && iCurLine < (iLineMax - 1))
	{
		iCurLine++;
		iDisplayFlag = 1;
	}
	else
	{
		iCurY++;
		iCurLine++;
	}

	vAdjust_Last_Position ();

	return 0;
}

int 
iKey_Left ()
{
	if (iCurX > 0)
		iCurX--;
	else
	{
		if (iKey_Up () == 0)
			iCurX = COLSIZE - 1;
	}

	return 0;
}

int 
iKey_Right ()
{
	if (((iCurLine + 1) == iLineMax) &&
		 (iCurX == ((iFileSize % COLSIZE) ? ((iFileSize % COLSIZE) - 1) : (COLSIZE - 1))))
		beep ();
	else if (iCurX < (COLSIZE - 1))
		iCurX++;
	else
	{
		if (iKey_Down () == 0)
			iCurX = 0;
	}

	return 0;
}

int 
main (argc, argv)
	int argc;
	char *argv[];
{
	int dflag, tflag;

	/*-----------------------------*
	 * command line argument check *
	 *-----------------------------*/
	if (argc != 2)
		vExit (1);

	/*----------------------*
	 * file for error debug *
	 *----------------------*/
#ifdef DEBUG
		/*--------------------------------------------------------------* 
		fpDump = fopen ("fd.dump", "w");
		if (fpDump == NULL)
		{
			fprintf (stderr, "can't create dump logfile. continue[Enter] or quit[^C] : ");
			getchar ();
		}
		setbuf(fpDump, NULL);
		*--------------------------------------------------------------*/ 
		fpDebug = fopen ("fd.err", "w");
		if (fpDebug == NULL)
		{
			fprintf (stderr, "can't create logfile. continue[Enter] or quit[^C] : ");
			getchar ();
		}
		setbuf(fpDebug, NULL);
#endif

	/* file open and read */

	cpFileName = argv[1];

	fprintf (stdout, "Wait for file(%s) open and read ...\n", cpFileName);

	if (iFile_Open (argc, argv[1]) < 0)
		exit (1);

	/*-----------------------*
	 * curses initialization *
	 *-----------------------*/
	initscr ();
	raw ();
	noecho ();
	scrollok(stdscr, TRUE);
	/*--------------------------------------------------------------* 
	keypad(stdscr,TRUE); ESC KEY 를 연속해서 누르면 에러 발생 - core dump
	*--------------------------------------------------------------*/

	/*-----------*
	 * main loop *
	 *-----------*/
	vMain_Loop ();


#ifdef DEBUG
	if (fpDebug)
	{
		fflush (fpDebug);
		fclose (fpDebug);
	}
#endif

	vExit (0);
}

