#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>

#include "songlist.c"

//g++ LinkedListPlayer.c -lwinmm

typedef struct SongStruct{
    struct SongStruct * prev;
    LPCTSTR path;
    struct SongStruct * next;
}Song;

int main(){
    Song birthdayKid;
    birthdayKid.path=path1 //from songlist.c
    PlaySound(TEXT(birthdayKid.path),NULL,SND_ASYNC);
    system("PAUSE");
    return 0;
}