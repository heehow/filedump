# filedump

this program is a utility for file hexa dump.
at first, fd reads only 6MB if file is very large.
and fd reads all files if file size less than 6MB.

fd reads file which size is huge very quickly.

compilation

# linux
$ gcc -o fd fd.c -lcurses (or -lncurses)

# windows
cl /D_WIN32/ /DWIN32 fd.c pdcurses.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib winmm.lib

