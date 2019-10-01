int check(int v);
int modify(int a);

int calc(int a,int b){
	for(int i=0;i<b;i++){
		a = modify(a);
		if(check(a)){
			continue;
		}else{
			break;
		}
	}
	return a;
}

int main(){
    int a,b;
	calc(a,b);
}
