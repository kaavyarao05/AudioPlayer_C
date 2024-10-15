#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>

//g++ LinkedListPlayer.c -lwinmm

#include "songlist.c"
/*
songlist.c is a c file in the same folder as LinkedListPlayer.c
songlist.c contains constants of type LPCSTR
*/


typedef struct SongStruct{
    struct SongStruct * prev;
    LPCTSTR path;
    struct SongStruct * next;
}Song;

void playSound(Song song){
    PlaySound(TEXT(song.path),NULL,SND_SYNC);
    //SND_SYNC ensures that the function returns after the song finishes playing
}

int main(){
    Song s1;
    s1.path=PATH1; //from songlist.c
    playSound(s1);
    playSound(s1);
    playSound(s1);
    return 0;
}