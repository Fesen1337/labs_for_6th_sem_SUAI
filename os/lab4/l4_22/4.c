#include <math.h>
#include <stdio.h>

int main()
{
    int N;
    int flag;
    scanf("%d", &N);
    flag = N;
    int len = floor(10 * N/3) + 1;
    int A[1000];

    for(int i = 0; i < len; ++i) 
    {
        A[i] = 2;
    }

    int nines = 0;
    int predigit = 0;

    while(flag > 0) 
    {        
        int q = 0;

        for(int i = len; i > 0; --i) {
            int x  = 10 * A[i-1] + q*i;
            A[i-1] = x % (2*i - 1);
            q = x / (2*i - 1);
        }

        A[0] = q%10;
        q    = q/10;

        if (9 == q) {
            ++nines;
        }else if (10 == q)
        {
            if(flag > 0)
                printf("%d", predigit + 1);
            flag--;
            for (int k = 0; k < nines; ++k) 
            {
                if(flag > 0)
                    printf("%d", 0);
                flag--;
            }
            predigit, nines = 0;
        }
        else {
            //if(j != 1)
            if(flag > 0)
                printf("%d", predigit);
            predigit = q;
            flag--;
            if (0 != nines) {    
            for (int k = 0; k < nines; ++k) {
                if(flag > 0)
                    printf("%d", 9);
                flag--;
            }

            nines = 0;
            }
        }
    }
    
    printf("%d", predigit);
}