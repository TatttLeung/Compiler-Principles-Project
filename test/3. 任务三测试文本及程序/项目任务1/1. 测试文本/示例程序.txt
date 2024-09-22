//Sample program
//In MiniC language-computes factorial
//}
int x; // an integer
int fact;
double y;
int arr[100];
void test(int a, int b)
{
  a = 1;
  b = 2;
  return ;
}
void main(void)
{
  x = 2;
  y = 3;
  if ( x > 1 )  //don't compute if x <= 0 
      do 
          fact = fact * x;
      while (fact >= 1);
  else
    x = 1;
  return ; //return void
}