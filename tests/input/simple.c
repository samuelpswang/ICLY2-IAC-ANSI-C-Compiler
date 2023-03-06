int ref(){
    int a = 5;
    return a;
}


int main(){
    int a = 5;int b = 6;
    int c = ((a+b)/(a-b)+b);  
    int d = (a >> 5)*(6);
    int e = a < b;
    int e = a>b;
    int e = (a!=b);
    int f = (a == b);
    int g = a|b;
    int h = -(a^b);
    int i = a&&b;
    int j = a||b;
    a = b;
    a = ref();

    if(a <b ){
        a = 5;
    }

    else{
        a = 6;
    }

    while(a<6){
        int a = 5;
        a = 5;
    }
}

