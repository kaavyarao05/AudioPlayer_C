#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include <unistd.h>
#include<fcntl.h>
#include<unistd.h>
#include<pulse/simple.h>

#define TEXTSIZE 100

struct SongStruct{
    char* path;
    struct SongStruct *next;
};
typedef struct SongStruct * Song;
Song start = NULL;

char dirpath[TEXTSIZE];

char* getext(char filename[]){
    char * ext = strrchr(filename, '.');
    if (!ext)
        return NULL;
    else
        return(ext + 1);
}
Song create(char * path){
    Song snode;
    snode = (Song)malloc(sizeof(struct SongStruct));
    snode->path=path;
    snode->next=NULL;
    printf("Added: %s\n",snode->path);
    return snode;
}
Song insertfront(char *path){
    Song temp;
    temp = create(path);
    if(start == NULL){
        return temp;
    }
    temp->next = start;
    return temp;
}
Song deletefront(){
    Song temp;
    if(start == NULL){
        printf("\nLinked list is empty");
        return NULL;
    }
    if(start->next == NULL){
        printf("\nThe Student SongStruct with path:%s is deleted ",start->path);
        free(start);
        return NULL;
    }
    temp = start;
    start = start->next;
    printf("\nThe Student SongStruct with path:%s is deleted",temp->path);
    free(temp);
    return start;
}
Song insertend(char * path){
    Song cur,temp;
    temp = create(path);
    if(start == NULL){
        start=temp;
        return temp;
    }
    cur = start;
    while(cur->next !=NULL){
        cur = cur->next;
    }
    cur->next = temp;
    return start;
}
void setDir(){
    printf("\nEnter Directory: ");
    scanf("%s",dirpath);
}
void addToPlaylist(){
    setDir();
    DIR *d;
    struct dirent *dir;
    char *path=(char*)malloc(sizeof(dirpath));
    d = opendir(dirpath);
    if (d){
        while ((dir = readdir(d)) != NULL){
            char *ext=getext(dir->d_name);
            if(strcmp(ext,"wav")==0){
                char *fpath=(char*)malloc(sizeof(dirpath)+sizeof(path));
                strcpy(fpath,dirpath);
                strcat(fpath,dir->d_name);
                start=insertend(fpath);
            }
        }
    }
    else{
        printf("\nDirectory %s is empty",dirpath);
    }
    closedir(d);
}
Song deleteend(){
    Song cur,prev;
    if(start == NULL){
        printf("\nLinked List is empty");
        return NULL;
    }
    if(start->next == NULL){
        printf("\nSong with path %s is deleted",cur->path);
        free(start);
        return NULL;
    }
    prev = NULL;
    cur = start;
    while(cur->next!=NULL){
        prev = cur;
        cur = cur->next;
    }
    printf("\nSong with path %s is deleted",cur->path);
    free(cur);
    prev->next = NULL;
    return start;
}
void display(){
    Song cur;
    int num=1;
    if(start == NULL){
        printf("\nEmpty Playlist\n");
        return;
    }
    printf("\nPlaylist: \n");
    cur = start;
    while(cur!=NULL){
        printf("\n%d. %s",num,cur->path);
        cur = cur->next;
        num++;
    }
}

void PlaySound(Song song){
    char * file_content[1000000]; //45 secs
    int myfd=open(song->path,O_RDONLY);
    read(myfd,file_content,sizeof(file_content));
    close(myfd);

    // pa_simple defined in pulse simple file
    pa_simple *simple=NULL;
    pa_sample_spec ss;
    //following data can be recieved by executing
    //file filename.wav
    ss.format=PA_SAMPLE_S16LE; //16 bit little endian
    ss.rate=44100; //hz
    ss.channels=2; //stereo
    simple=pa_simple_new(NULL,"Audio Playback",PA_STREAM_PLAYBACK,NULL,"playback",&ss,NULL,NULL,NULL);
    pa_simple_write(simple,file_content,sizeof(file_content),NULL);
    pa_simple_drain(simple,NULL);
    pa_simple_free(simple);
}

void Play(){
    Song cur;
    int num=1;
    if(start == NULL){
        printf("\nEmpty Playlist\n");
        return;
    }
    cur = start;
    while(cur!=NULL){
        PlaySound(cur);
        cur = cur->next;
        num++;
    }
}

void editmenu(){
    while(1){
        printf("\n====Edit Playlist====");
        printf("\n1. Add Song At End");
    }
}

int menu(){
    int ch,i,n;
    while(1){
        printf("\n~~~Menu~~~");
        printf("\n1: Create Playlist");
        printf("\n2: View Playlist");
        printf("\n3: Delete at end");
        printf("\n4: Play");
        printf("\n5: Exit\n");
        printf("\nEnter your choice:");
        scanf("%d",&ch);
        switch(ch){
            case 1:
                addToPlaylist();
                break;
            case 2:
                display();
                break;
            case 3:
                start = deleteend();
                break;
            case 4:
                Play();
                break;
            case 5:
                return 0;
            default: printf("\nPlease enter the valid choice");
        }
    }
}

int main(){
    return main();
}