#include<cstdint>
#include<cstdio>
#include<vector>
using namespace std;
typedef vector<uint32_t> seed;

const uint32_t TINYMT32_MASK = 0x7FFFFFFF;
const int TINYMT32_SH0 = 1;
const int TINYMT32_SH1 = 10;
const int TINYMT32_SH8 = 8;
const uint32_t mat1 = 0x8f7011ee;
const uint32_t mat2 = 0xfc78ff1f;
const uint32_t tmat = 0x3793fdff;

void nextState(seed &state){
  uint32_t y = state[3];
  uint32_t x = (state[0] & TINYMT32_MASK) ^ state[1] ^ state[2];
  x ^= (x << TINYMT32_SH0);
  y ^= (y >> TINYMT32_SH0) ^ x;
  state[0] = state[1];
  state[1] = state[2];
  state[2] = x ^ (y << TINYMT32_SH1);
  state[3] = y;
  if( (y & 0x1) == 0x1 ){
    state[1] ^= mat1;
    state[2] ^= mat2;
  }
  return;
}

uint32_t temper(const seed &state){
  uint32_t t0 = state[3];
  uint32_t t1 = state[0] + (state[2] >> TINYMT32_SH8);
  t0 ^= t1;
  if( (t1 & 0x1) == 0x1 ){
    t0 ^= tmat;
  }
  return t0;
}

struct condition{
  bool Sex;
  bool DestinyKnot;
  bool Ball;
  bool Internatioal;
  bool ShinyCharm;
};

pair<int,seed> countNum(seed state,condition cond){
  int used = 0;

  nextState(state); used++; // 1

  if(cond.Sex){
    nextState(state); // sex
    used++;
  }
  nextState(state); used++; // nature
  nextState(state); used++; // ability
  int num = ( cond.DestinyKnot ? 5 : 3 );
  bool check[6]={};
  while( num > 0 ){
    int mod = temper(state)%6;
    nextState(state); used++;
    if( !check[mod] ){
      num--;
      check[mod] = true;
      nextState(state); used++; // father or mother      
    }
  }
  for( int i = 0 ; i < 6 ; ++i ){
      nextState(state); // IV  
      used++;  
  }
  nextState(state); used++;  // for Spinda

  int shinyNum=0;
  if( cond.ShinyCharm ) shinyNum += 2;
  if( cond.Internatioal ) shinyNum += 6;
  for( int i = 0 ; i < shinyNum ; ++i ){
      nextState(state); // PID
      used++;
  }
  if( cond.Ball ){
      nextState(state); // Ball    
      used++;
  }

  nextState(state); used++; // 2
  return make_pair(used,state);
}

int dist(seed from,seed to){
  int ret = 0;
  while( from != to ){
    nextState(from);
    ret++;
  }
  return ret;
}

int main(){
  seed init(4),target(4);
  printf("input Initial seed as 01234567,8ABCDEF,FEDCBA8,76543210\n");
  if( EOF == scanf("%x,%x,%x,%x",&init[3],&init[2],&init[1],&init[0]) ){
    printf("Bad input\n");
    return 1;
  }

  printf("input Target seed as 01234567,8ABCDEF,FEDCBA8,76543210\n");
  if( EOF == scanf("%x,%x,%x,%x",&target[3],&target[2],&target[1],&target[0]) ){
    printf("Bad input\n");
    return 1;
  }
    
  printf("dist = %d\n",dist(init,target));
  seed state = init;
  condition cond = { true , true , true , true , true };
  pair<int,seed> result = countNum(state,cond);
  state = result.second;
  printf("%08x,%08x,%08x,%08x",state[3],state[2],state[1],state[0]);
  printf(" (%d)\n",result.first);
  return 0;
}
