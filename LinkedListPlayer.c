#include<dirent.h> //Opening directory and files
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<unistd.h>
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
    Song newnode;
    newnode = (Song)malloc(sizeof(struct SongStruct));
    newnode->path=path;
    newnode->next=NULL;
    printf("Added: %s\n",newnode->path);
    return newnode;
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
        printf("\nEmpty Playlist");
        return NULL;
    }
    if(start->next == NULL){
        printf("\nThe Song with path:%s is deleted ",start->path);
        free(start);
        return NULL;
    }
    temp = start;
    start = start->next;
    printf("\nThe Song with path:%s is deleted",temp->path);
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
    DIR *d; //dirent.h
    struct dirent *dir; //dirent.h
    char *path=(char*)malloc(sizeof(dirpath));
    d=opendir(dirpath);
    if (d){ //folder has stuff
        while ((dir = readdir(d)) != NULL){
            char *ext=getext(dir->d_name);
            if(strcmp(ext,"wav")==0){
                char *fpath=(char*)malloc(sizeof(dirpath)+sizeof(path)); //stores enough characters to hold path of file
                strcpy(fpath,dirpath);
                /*
                if we directly used strcat(dirpath,dir->d_name),

                dirpath=songs
                path=songs/song1.wav

                dirpath then becomes songs/song1.wav
                */

                /*
                fpath=songs
                dirpath=songs

                fpath=songs/song2.wav
                */
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
    pa_simple *simple=NULL; //stores song currently playing
    pa_sample_spec ss; //stores data about song

    //following data can be recieved by executing
    //file filename.wav
    ss.format=PA_SAMPLE_S16LE; //16 bit little endian //const defined in pulse header file
    ss.rate=44100; //hz
    ss.channels=2; //stereo
    simple=pa_simple_new(NULL,"Audio Playback",PA_STREAM_PLAYBACK,NULL,"playback",&ss,NULL,NULL,NULL);
    pa_simple_write(simple,file_content,sizeof(file_content),NULL);
    pa_simple_drain(simple,NULL); //plays song
    pa_simple_free(simple); //frees memory
}
void Play(){
    Song cur;
    if(start == NULL){
        printf("\nEmpty Playlist\n");
        return;
    }
    cur = start;
    while(cur!=NULL){
        PlaySound(cur);
        cur = cur->next;
    }
    printf("\nPlaylist Ended");
}
void edit(){
    int op;
    char * path;
    while(1){
        printf("\n====EDIT====");
        printf("\n1. Add at End\n2. Add at Start\n3. Delete at End\n4. Delete at Start\n5. View Playlist6. Back");
        printf("\nEnter: ");
        scanf("%d",&op);
        switch (op)
        {
        case 1:
            printf("\nEnter Path: ");
            scanf("%s",path);
            start=insertend(path);
            break;
        case 2:
            printf("\nEnter Path: ");
            scanf("%s",path);
            start=insertfront(path);
            break;
        case 3:
            start=deleteend();
            break;
        case 4:
            start=deletefront();
            break;
        default:
            return;
        }
    }
}
void menu(){
    int ch,i,n;
    while(1){
        printf("\n====Menu====");
        printf("\n1: Create Playlist\n2: View Playlist\n3: Edit Playlist");
        printf("\n4: Play\n5: Exit\n");
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
                edit();
                break;
            case 4:
                Play();
                break;
            case 5:
                return;
            default: printf("\nPlease enter the valid choice");
        }
    }
}
int main(){
    menu();
    return 0;
}