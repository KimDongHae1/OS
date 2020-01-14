#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define REFER 1000 // 레퍼런스 길이
#define PAGE 15 // 페이지 수
#define Frame 10 // 메모리의 프레임의 개수

int main (int argc , char *argv[])
{
    int reference [REFER]; //임의 reference 스트링 배열
    int i,result [3];
    
    srand(time(NULL));
    
    for (i = 0 ; i < REFER ; i++ ){
        reference[i] = rand()%PAGE +1; // 레퍼런스 생성 난수를 생성할 때 0부터 하지 않고 1부터 15까지로 한다. 왜냐하면 프레임의 안의 수를 0으로 하려고 하기 때문이다. 0이면 빈 페이지고 0이 아닐경우 페이지가 프레임이 페이지로 채워져있다는 의미로 하기 위함이다.
        printf(" %d" , reference[i]); // 내가 생성한 레퍼런스를 보여준다
    }
    
    printf("\n");
    
   
    result[0] = Fifo(reference, Frame, REFER); // FIFO의 함수에서 나온 pagefault의 반환 값을 result에 넣었다.
    result[1] = Optimal(reference,Frame, REFER);// Optimal의 함수에서 나온 pagefault의 반환 값을 result에 넣었다.
    result[2] = LRU(reference,REFER); // LPU의 함수에서 나온 pagefault의 반환 값을 result에 넣었다.
    printf("FIF0 페이지 폴트 수 :  %d \n",result[0]); // 전체 FIFO pagefault수 출력
    printf("OPtimal 페이지 폴트 수 : %d \n",result[1]); // 전체 OPtimal pagefault수 출력
    printf("lru 페이지 폴트 수 : %d \n",result[2]); // 전체 LPU pagefault수 출력
    
}

int forward(int *ref,int p, int c, int refsize) // optimal함수를 할 때 레퍼런스를 확인하고 그 다음에 사용할 것 과의 거리를 나타낸다
{
    int i, k =0;
    for (i = p ; i <refsize ; i++){
        if(ref[i]!=c)//
            k ++; // 없을 수록 +1씩 채운다. 높은 숫자일 경우 가장 늦게 사용된다는 뜻이다.
        else
            break; // 바로 for문을 나온다.
    }
    return k; // 더해진 값
}

int found (int *mem , int ref) // 메모리에 존재하는 지를 확인하는 함수이다.
{
    int i;
    for (i = 0; i < Frame ; i++)
        if(mem[i] == ref) // Frame을 다 확인해서 존재하면 i 값을 리턴한다
            return i; // 있으면 그 있던 프레임을 장소를 반환해준다.
    
    return -1 ; // 존재하지 않으면 페이지 폴트 발생
}
int Fifo (int *ref, int framesize , int refsize)//ref는 랜덤 레퍼런스가 들어가고 , msize는 프레임 사이즈 , rsize는 레퍼런스 사이즈이다.
{
    int memory[Frame]={0}; // 메모리 생성
    int i , n_fault = 0; // n_fault는 page fault의 수를 의미힌다
    int cur =0; //cur은 프레임의 위치를 가리킨다.
    
    for (i = 0 ; i < refsize ; i ++) // 선입 선출이므로 먼저 들어온 순서대로 i =0 부터 999까지의 수를 차례대로 검사한다.
    {
        if(found(memory,ref[i]) == -1) // 메모리에 존재하지 않는 페이지라면 페이지 폴트 발생
        {
            memory[cur] = ref[i]; // 존재하지 않으니까 레퍼런스 값을 cur가 가르치는 곳에 넣는다.
            cur = (cur+1)%Frame; // 그 다음에 사용할 프레임을 나타낸다
            n_fault++; // 페이지 폴트가 발생할 때 마다 수를 1씩 증가해서 총 횟수를 정한다.
            printf("FIFO 알고리즘에서 페이지 폴트가 일어나는 레퍼런스 순서 : %ld \n" ,i ); // 이 경우 i는 천번을 도는데 페이지 폴트가 일어나는 경우의 i만 포함하고 있다.

        }
    }
    return n_fault;
}

int Optimal(int *ref, int framesize , int refsize) // ref는 랜덤 레퍼런스가 들어가고 , msize는 프레임 사이즈 , rsize는 레퍼런스 사이즈이다.
{
    int memory[Frame] = {0}; //메모리생성
    int i, j, cur = 0;
    int n_fault= 0; // 총 일어나는 페이지 폴트 수
    int outpage , in; // outpage는 그 다음에 얼마나 사용 안되는 지를 판단한다., in은 넣을 수 있는 프레임 자리를 말한다.
    
    for(i=0; i<refsize ; i++) //레퍼런스 전체를 확인하다.
    {
        outpage =0;
        if (cur < framesize){
            if(found(memory,ref[i]) == -1) // 메모리의 어느 프레임에도 아무 페이지가 없다.
            {
                memory[cur] = ref[i];
                n_fault++;
               
                cur++;
            }
        }else
            if(found(memory,ref[i]) != -1)
                continue;
            for(j=0 ; j<Frame ; j++) // page가 얼마나 오랜시간 동안 접근이 있는지 확인
            {
            if(outpage < forward(ref,i, memory[j], refsize)) //
            {
                outpage = forward(ref,i, memory[j], refsize);
                in = j;
            }
                
        }
        printf("optimal 알고리즘에서 페이지 폴트가 일어나는 레퍼런스 순서 : %ld \n" , i );
        n_fault++;
        memory[in] = ref[i]; // 레퍼런스 값을 메모리에 넣는다.
        
    }
    return n_fault;
}
int LRU (int *ref ,int rsize)
{
    int memory[Frame] ={0};
    int i,j;
    int n_fault = 0;
    int cur = 0; // cur는 자기가 가리키는 프레임의 페이지가 무었인지를 나타낼 때 사용된다.
    int cursize =0; // 얼마나 오랫동안 페이지가 사용안되었는지 사용하는 척도
    for (i=0 ; i< rsize; i++)
    {
        if(found(memory,ref[i]) == -1) //페이지 폴트가 일어난다.
        {
                if(Frame==cursize){ // 같을 경우
                    for(j=1 ; j<Frame ; j++)
                        memory[j-1] = memory[j];
                    memory[Frame-1] = ref[i];
                    cursize = 0;
                }else { // 다른 페이지가 프레임에 들어올 경우
                    memory[cur] = ref[i];
                    cursize ++;
                }
            printf("LRU 알고리즘에서 페이지 폴트가 일어나는 레퍼런스 순서 : %ld \n" ,i );
            n_fault++;
    }
 }
    return n_fault;
}


