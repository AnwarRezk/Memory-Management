/*
Name : Anwar Rezk
ID : 4633
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#define MAX 1000

/*This Program is to simulate different page replacement algorithms*/

///Global Variables
int n,*ref,f_n,*frame,p_f=0,*prt_tab,*use_tab;
/*
n -> The reference string length
*ref -> an array to store the reference string
f_n -> the number of frames allocated to a process
*frame -> an array to store the entries
p_f -> /counter to keep track of page faults
*prt_tab -> an array to hold priority of requests
*use_tab -> an array to hold the use bits used in clock policy
*/


///This function initializes all entries of the frame array to a number
void init_frameTab(){
    for(int i=0; i<f_n; i++)
        frame[i]=-1;
}

///we need a function to find the referenced page in the frames array
int find_page(int request){
    for(int i=0;i<f_n;i++){
        if(frame[i] == request)
            return i; //return the index of the found page either to replace or keep
    }
    return -1; //return -1 if not found
}


///This function just displays the frame entries every time it's called
void print_frames(){
    for (int k=0; k<f_n; k++){
        if(frame[k]!=-1){printf("%02d ",frame[k]);}
    }
}

///FIFO policy
int fifo_policy(){
    int ind_trace = 0,current = 0 ; //to keep track of the FIFO queue without needing to shift it
    init_frameTab();
    for(int i=0;i<n;i++){ //traverse the reference string
        if(current==0){ //it the table initially empty,insert normally
            printf("\n%02d     ",ref[i]);
            frame[0]=ref[i];
            current++;
            print_frames();
        }
        else if(find_page(ref[i]) == -1){ //the page referenced is in the frames
            if(current<f_n){ //if the frame table still have space for pages
                printf("\n%02d     ",ref[i]);
                frame[current]=ref[i];
                current++;
            }
            else{
            printf("\n%02d F   ",ref[i]);
            frame[ind_trace] = ref[i];
            ind_trace=(ind_trace+1)%f_n;
            p_f++;
            }
            print_frames();
        }
        else{
            printf("\n%02d     ",ref[i]);
            print_frames();}
    }
    return p_f;
}

///LRU policy
int lru_policy(){
    int t=0,current=0;//t->counter for the number of times being referenced
    init_frameTab();
    for(int i=0;i<n;i++){
        if(current==0){ //it the table initially empty,insert normally
            printf("\n%02d     ",ref[i]);
            frame[0]=ref[i];
            prt_tab[0]=t;
            t++;current++;
            print_frames();
        }
       else if(find_page(ref[i])== -1){ //if the page doesn't exist in memory
            if(current<f_n){ //if the frame table still have space for pages
                printf("\n%02d     ",ref[i]);
                frame[current]=ref[i];
                prt_tab[current]=t;
                t++;current++;
                print_frames();
            }
            else{ //if the frame table is full, we will need to replace
                printf("\n%02d F   ",ref[i]);
                int min_index = 0,min=prt_tab[0];
                for(int i=1;i<current;i++){
                    if(prt_tab[i]<min){
                        min = prt_tab[i];
                        min_index = i;
                    }
                }
                frame[min_index]=ref[i];//the entry with the smallest time is replaced
                prt_tab[min_index]=t;
                t++;p_f++;
                print_frames();
            }
        }
      else{ //if the page already exists in memory, we just update the time
        printf("\n%02d     ",ref[i]);
        int ind_to_mod = find_page(ref[i]);
        prt_tab[ind_to_mod]=t;
        t++;print_frames();
      }
    }
    return p_f;
}

///CLOCK policy
int clock_policy(){
    int current=0,ind_trace=0;
    init_frameTab();
    for(int i=0;i<n;i++){
        if(current==0){ //it the table initially empty,insert normally
            printf("\n%02d     ",ref[i]);
            frame[0]=ref[i];
            use_tab[0]=1;
            current++;print_frames();
        }
       else if(find_page(ref[i])== -1){ //if the page doesn't exist in memory
            if(current<f_n){ //if the frame table still have space for pages
                printf("\n%02d     ",ref[i]);
                frame[current]=ref[i];
                use_tab[current]=1; //Assign use bit as 0,candidate for replacement
                current++;print_frames();
            }
            else{ //if the frame table is full, we will need to replace
                        printf("\n%02d F   ",ref[i]);
                        while(use_tab[ind_trace]!=0){ //loop until you find a zero used bit
                            use_tab[ind_trace]=0; // make all use bits zero
                            ind_trace=(ind_trace+1)%f_n;
                            }
                frame[ind_trace] = ref[i];//replace the first frame with 0 use bit
                use_tab[ind_trace]=1; //set the use bit of the newly added page to 1
                ind_trace=(ind_trace+1)%f_n;
                p_f++;print_frames();
        }
    }
      else{ //if the page already exists in memory, we just update the time
        printf("\n%02d     ",ref[i]);
        int ind_to_mod = find_page(ref[i]);
        use_tab[ind_to_mod]=1;
        print_frames();
      }
    }
    return p_f;
}

///OPTIMAL policy
int optimal_policy(){
    int prediction[MAX],curr_p,i,current=0; //prediction array used to store the index of future referenced slots
    init_frameTab();
    for(i=0;i<n;i++){
        if(current==0){ //it the table initially empty,insert normally
            printf("\n%02d     ",ref[i]);
            frame[0]=ref[i];
            current++;print_frames();
        }
       else if(find_page(ref[i])==-1){
            if(current<f_n){ //if the frame table still have space for pages
                printf("\n%02d     ",ref[i]);
                frame[current]=ref[i];
                current++;print_frames();}
        else{
            printf("\n%02d F   ",ref[i]);
            for(int j=0;j<f_n;j++){
                curr_p=frame[j];
                int f = 0;
                for(int k=i;k<n;k++){ //this loop is for scanning the rest of the ref string to check for later refs
                    if(curr_p==ref[k]){
                        prediction[j]=k;
                        f=1;break;
                    }
                    else{f=0;}
                }
                if(!f){prediction[j]=INT_MAX;}
            }
            int max=INT_MIN;int r;
            for(int j=0; j<f_n; j++){
                if(prediction[j]>max){
                    max=prediction[j];
                    r=j;}
            }
            frame[r]=ref[i];p_f++;
            print_frames();
           }
        }
        else{printf("\n%02d     ",ref[i]);;print_frames();}
    }
    return p_f;
}

/*=================================================================================*/

///MAIN

int main(){
    	int i,res;
        ///INPUT AREA
        scanf("%d",&f_n);
        frame = (int*)malloc(sizeof(int)*f_n);
        char *policy;
        policy = (char*)malloc(sizeof(char)*10);
	scanf("%s",policy);
        i=0;
        ref = (int*)malloc(sizeof(int)*MAX);
        while(scanf("%d",&ref[i])){
            if(ref[i]==-1){break;}
            i++;
        }
        n=i; //the length of the ref string

        ///OUTPUT AREA
        printf("Replacement Policy = %s\n",policy);
        printf("-------------------------------------\n");
        printf("Page   Content of Frames\n");
        printf("----   -----------------");
        if(strcmp("FIFO",policy)==0){
            res = fifo_policy();
        }
        else if(strcmp("LRU",policy)==0){
            prt_tab=(int*)malloc(sizeof(int)*f_n);
            res = lru_policy();
			free(prt_tab);
        }
        else if(strcmp("OPTIMAL",policy)==0){
            res = optimal_policy();
        }
        else if(strcmp("CLOCK",policy)==0){
            use_tab=(int*)malloc(sizeof(int)*f_n);
            res = clock_policy();
			free(use_tab);
        }
        else{
            res = 0;
        }
        printf("\n-------------------------------------");
        printf("\nNumber of page faults = %d\n",res);
    	free(policy);
        free(frame);
        free(ref);
    	return 0;
}
