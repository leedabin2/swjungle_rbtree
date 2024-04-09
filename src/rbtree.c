#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t*)calloc(1,sizeof(node_t));
  nil->color = RBTREE_BLACK;

  t->nil = t->root = nil;
  
  return t;
}


// 후위 순회하며 메모리 반납
void postorder_rbtree(rbtree *t, node_t *x) {

  if (x != t->nil) {
    postorder_rbtree(t,x->left);
    postorder_rbtree(t,x->right);
    free(x);
  }  
}


void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  node_t *x = t->root;
  postorder_rbtree(t,x);
  free(t->nil);
  free(t);
}


static void right_rotate(rbtree* t, node_t* node){
  node_t* x = node->left;
  node->left = x->right;
  if(x->right != t->nil){
    x->right->parent = node;
  }
  x->parent = node->parent;
  if(node->parent == t->nil){
    t->root = x;
  }else if(node->parent->left == node){
    node->parent->left = x;
  }else{
    node->parent->right = x;
  }
  x->right = node;
  node->parent = x;
}


static void left_rotate(rbtree* t, node_t* node){
  node_t* y = node->right;

  node->right = y->left;
  if(y->left != t->nil){
    y->left->parent = node;
  }
  y->parent = node->parent;
  if(node->parent == t->nil){
    t->root = y;
  }else if(node->parent->left == node){
    node->parent->left = y;
  }else{
    node->parent->right = y;
  }
  y->left = node;
  node->parent = y;
}


static void rbtree_insert_fixup(rbtree* T, node_t* node){
  // 새노드의 부모가 레드일 때
  while(node->parent->color == RBTREE_RED){
    
    // case 1 삼촌이 레드일 때 (recoloring)
    if(node->parent == node->parent->parent->left){
      node_t* uncle = node->parent->parent->right;
      if(uncle->color == RBTREE_RED){
        node->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        
        node = node->parent->parent; //노드 포인터를 조상으로 
      }else{ // case 2 오른쪽 자식이고, 삼촌이 흑색일 때
        if(node == node->parent->right ){
        node = node->parent;
        left_rotate(T, node);
      }
      // case2 node가 왼쪽이고, 삼촌이 흑색 일 때
      node->parent->color = RBTREE_BLACK;
      node->parent->parent->color = RBTREE_RED;
      right_rotate(T, node->parent->parent);
      } 
    }else{ // 조상 기준 부모가 오른쪽에 위치할 때 
      node_t* uncle = node->parent->parent->left;
      if(uncle->color == RBTREE_RED){
        node->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        node = node->parent->parent;
      }else{
        if(node->parent->left == node){
        node = node->parent;
        right_rotate(T, node);
      }
      node->parent->color = RBTREE_BLACK;
      node->parent->parent->color = RBTREE_RED;
      left_rotate(T, node->parent->parent);
      }
    }
  }
  T->root->color = RBTREE_BLACK;
}


node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  node_t *y = t->nil;
  node_t *x = t->root;

  while (x != t->nil)
  {
    y = x;
    if (key < x->key ) {
        x = x->left;
    }else if (key >= x->key)
    {
      // 중복 값 허용
      x = x->right;
    } 
  }

  // 새로운 노드 동적 할당
  node_t* newnode = (node_t* )calloc(1,sizeof(node_t));

  newnode->key = key;
  newnode->parent = y;
  newnode->left = t->nil;
  newnode->right = t->nil;
  newnode->color = RBTREE_RED;
  
  if ( y== t->nil ) {
    t->root = newnode;
  }else if (newnode->key < y->key)
  {
    y->left = newnode;
  } else {
    y->right = newnode;
  }
  
  rbtree_insert_fixup(t,newnode);

  return t->root;
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find

  if (t->root == t->nil) {
    return NULL;
  }
  node_t *x = t->root; 

  while (x != t->nil)
  {
    if (x->key == key) {
      return x;
    } else if (x->key <= key)
    {
      x = x->right;
    } else {
      x = x->left;
    } 
  }
  
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  // 가장 왼쪽으로 내려가기
  node_t *x = t->root;
  while (x->left != t->nil)
  {
    x = x->left; 
  }
  return x;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *x = t->root;
  while (x->right != t->nil)
  {
    x = x->right;
  }
  
  return x;
}

// 삭제하려는 노드에 대입할 동작을 수행하는 함수
static void rb_transplant(rbtree *t,node_t *u, node_t *v) {

  // 삭제하려는 노드의 자녀의 부모가 t->nil (삭제하려는 노드가 루트노드인 경우?)
  if (u->parent == t->nil) {
    t->root = v; // 자식이 루트 노드가 됨
  } else if (u == u->parent->left) { // 삭제하려는 노드가 부모의 왼쪽 자식이
    u->parent->left = v; // 삭제하려는 부모의 왼쪽에 대체를 가져
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent; // 삭제하려는 노드의 부모가 대체제의 부모가 됨

}

node_t *find_successor(rbtree *t, node_t *z) {
  // 왼쪽 자식이 nil이면 종료(비어 있지 않은 경우 가장 작은 값을 가진 노드를 찾음)
  while (z->left != t->nil)
  {
    z = z->left;
  }
  
  return z;
}

void rb_erase_fixup(rbtree *t,node_t *x) {
  node_t *w;
  
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    // 이중 흑색 노드가 부모의 왼쪽 자식인 경우
    if (x == x->parent->left) {
      // case 1 : 이중 흑색 노드 x의 형제가 빨강인 경우
      w = x->parent->right;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;

        // 부모 노드를 기준으로 좌회전
        left_rotate(t,x->parent);
         w = x->parent->right; // 새로운 형제 노드로 변경
      }

      // case 2 : 이중 흑색 노드 형제가 검은색이고, 형제 양쪽 자식이 모두 black
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) { 
        w->color = RBTREE_RED;
        x = x->parent; 
      } else {
        if (w->right->color == RBTREE_BLACK) {
       // case 3 : 이중 흑색 노드 형제가 검은색이고, 형제의 왼쪽 자식이 빨강, 오른쪽 자식이 검정인 경우
        w->left->color = RBTREE_BLACK;
        w->color = RBTREE_RED;

        // 형제 노드를 기준으로 우회전
        right_rotate(t,w);
        w = x->parent->right;
        } 
        // case 4 : 이중 흑색 노드의 형제가 검정이고 형제 오른쪽 자식이 빨강인 경
        // 부모 노드 색을 형제에게
        w->color = x->parent->color;
        // 부모와 형제 노드의 오른쪽 자식을 검정으로
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        // 부모 노드 기준으로 좌회전 
        left_rotate(t,x->parent);
        x = t->root; 
      }
    
    }
 
    // 이중 흑색 노드의 부모가 오른쪽 자식인 경우
    else
    {
      // case 1 : 이중 흑색 노드 x의 형제가 빨강인 경우
      w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;

        // 부모 노드를 기준으로 좌회전
        right_rotate(t,x->parent);
         w = x->parent->left; // 새로운 형제 노드로 변경
      }

      // case 2 : 이중 흑색 노드 형제가 검은색이고, 형제 양쪽 자식이 모두 black
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent; 
      } else
      { // case 3 : 이중 흑색 노드 형제가 검은색이고, 형제의 왼쪽 자식이 빨강, 오른쪽 자식이 검정인 경우
        if ( w->left->color == RBTREE_BLACK) {
          w->color = RBTREE_RED;
          w->right->color = RBTREE_BLACK;

          left_rotate(t,w);
          w = x->parent->left;
        }
         // case 4 : 이중 흑색 노드의 형제가 검정이고 형제 오른쪽 자식이 빨강인 경
         // 부모 노드 색을 형제에게
        w->color = x->parent->color;
        // 부모와 형제 노드의 오른쪽 자식을 검정으로
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;

        right_rotate(t,x->parent);
        x = t->root; 

      }  
     
    }
  }
  x->color = RBTREE_BLACK;
  
}

// 삭제
int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase

  node_t *y = z; // 대체노드 탐색 후 대체할 노드를 가리키는 주소

  color_t y_original_color = y->color; // 나중에 삭제하는 노드의 색깔
  node_t *x;  // y의 자식

  // 삭제하려는 노드의 자녀가 없거나 하나면 삭제되는 색  == 삭제되는 노드의 색
  
  // 자녀가 유효한 경우에만 (nil이 아닌 경우에만)  
  if (z->left == t->nil) { // 노드의 자식이 오른쪽만 있으면
    x = z->right; // 노드의 자식을 z자리에 심는다. 
    rb_transplant(t,z,z->right); // 오른쪽 자식을 z로 옮긴다.
  } else if (z->right == t->nil) { 
    x = z->left;
    rb_transplant(t,z,z->left);
  } else { // 삭제하려는 노드의 양쪽 자식이 있으면
    y = find_successor(t,z->right); // y는 z의 직후 원소로 설정 (y는 z의 위치로 이동)
    y_original_color = y->color; //y색깔이 변동할 수 있으므로 저장
    x = y->right; // y의 오른쪽 자식 노드를 x로 지정

    // 1. y의 부모가 z일 때
    if (y->parent == z) {
      x->parent = y; // x의 부모를 y로 , x가 nil일 경우 nil의 부모는 설정되지 않아서 따로 지정
    } else { // 2. y의 부모가 z가 아닐 때
      rb_transplant(t,y,y->right); // y와 y->right를 바꿈
      y->right = z->right; // x를 z 오른 쪽 에 연
      y->right->parent = y; // z와 y를 바꿈
    }
    rb_transplant(t,z,y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  if (y_original_color == RBTREE_BLACK) {
    rb_erase_fixup(t,x);
  }

  free(z); // z가 자식들이 없고, 레드면 그냥 삭제

  return 0;
}

// 중위 순회
void inorder_rbtree(const rbtree *t, node_t *x, key_t *arr,int *idx,const size_t n) {

  if (x != t->nil && *idx < n) {
      inorder_rbtree(t, x->left, arr, idx, n);
      arr[(*idx)++] = x->key;
      inorder_rbtree(t, x->right, arr, idx, n);
  }

}

// 오름차순
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {

  node_t *x = t->root;
  if (x == t->nil) {
    return 0;
  }

  int idx = 0;
  inorder_rbtree(t,x,arr,&idx,n);

  // TODO: implement to_array
  return 0;
}

