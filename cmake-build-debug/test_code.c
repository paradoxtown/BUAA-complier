int sum(int a, int b){
    return (a + b);
}

int multi(int a, int b){
    return (a * b);
}

int fib(int n){
    if (n == 1){
        return (1);
    }
    if (n == 2){
        return (1);
    }
    return (fib(n - 1) + fib(n - 2));
}

void print(char * str){
    printf("Unexpection imformation: ", str);
}

void main(){
    int n, tmp, result;
    char str[20], ch;
    printf("Please input a integer: ");
    scanf(n);
    tmp = 0;

    while(tmp != n){
        result = fib(tmp + 1);
        printf("The nth fib number is", result);
        tmp = tmp + 1;
    }
    printf("Please input a string: ");
    scanf(str);

    tmp = 0;
    while (tmp < 20){
        if (str[tmp] < 'a'){
            if (str[tmp] > 'Z'){
                tmp = 20;
                print(str);
            }
        }
        if (str[tmp] > 'z'){
            tmp = 20;
            print(str);
        }
        if (str[tmp] < 'A'){
            tmp = 20;
            print(str);
        }
    }

    scanf(ch)
    switch(ch){
        case 'a': 'A';
        case 'b': 'B';
        case 'c': 'C';
        case 'd': 'D';
        default: ;
        return;
    }
}