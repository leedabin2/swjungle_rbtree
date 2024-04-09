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

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

// [중위 순회] : 확인용
static void inorder_tree_work(rbtree *t, node_t *x) {
  if (x != t->nil) {
    inorder_tree_work(t, x->left);
    printf("색깔 : %d, 키 값 : %d\n", x->color, x->key);
    inorder_tree_work(t, x->right);
  }
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
  inorder_tree_work(t,t->root);

  return t->root;
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

// 왼쪽 서브트리의 가장 오른쪽 노드 찾
node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  // 가장 오른쪽으로 내려가기
  while (t->root->left != t->nil)
  {
    
  }
  

  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

// 이식 : 삭제하려는 노드에 대입할 동작을 수행하는 함수
static void rb_transplant(rbtree *t,node_t *u, node_t *v) {

  if (u->parent == t->nil) {
    t->root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent;

}

void rb_delete_fixup(rbtree *t,node_t *x) {
  while (x != t->root && x->color == RBTREE_BLACK)
  {
  
  }
  
}


int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase

  node_t *y = z; // 대체노드 탐색 후 대체할 노드를 가리키는 주소

  node_t *y_original_color = z->color; // 나중에 삭제하는 노드의 색깔(블랙일 경우 문제가 돼서 저장)
  node_t *x;  // y의 자식

  if (z->left == t->nil) { // 노드의 자식이 오른쪽만 있으면
    x = z->right; // 노드의 자식을 z자리에 심는다. 
    rb_transplant(t,z,z->right); 
  } else if (z->right == t->nil) {
    x = z->left;
    rb_transplant(t,z,z->left);
  } else {
    y = rbtree_min(z->left);
    y_original_color = y->color;
    x = y->right;
    if (y->parent == z) {
      x->parent = y;
    } else {
      rb_transplant(t,y,y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    rb_transplant(t,z,y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  if (y_original_color == RBTREE_BLACK) {
    rb_delete_fixup(t,x);
  }
  return 0;
}

void inorder_rbtree_to_array(const rbtree *t, node_t *x, key_t *arr, int *idx, const size_t n) {
  if (x == t->nil) {
    return;
  }
  
  inorder_rbtree_to_array(t, x->left, arr, idx, n);
  if (*idx < n) {
    arr[(*idx)++] = x->key;                           // *idx는 0, 1, 2, 3...이다. 그리고 후위 연산자 ++이므로 0부터 인덱스가 시작된다.
  } else {
    return;
  }
  inorder_rbtree_to_array(t, x->right, arr, idx, n);
}

// [오름차순으로 배열에 값 저장]
// rbtree를 가르키는 t 값(주소값)은 변하면 안되므로 const
// arr는 inorder 함수를 거칠 때 마다 변할 수 있으므로 const 없이
// n은 arr의 사이즈로 변하면 안되므로 const를 붙여준다. size_t는 unsigned int로, 어차피 배열의 사이즈 n은 양수일테니 size_t로 선언
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  node_t *x = t->root;
  if (x == t->nil) {
    return 0;
  }
  int cnt = 0;
  int *idx = &cnt;
  inorder_rbtree_to_array(t, x, arr, idx, n);
  
  // TODO: implement to_array
  return 0;
}

