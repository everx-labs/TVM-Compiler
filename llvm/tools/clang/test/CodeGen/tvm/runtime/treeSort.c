// XFAIL: *
//#define NATIVE
// RUN: %clang -w -O3 -S -c -target tvm %s -o - | tvm-testrun --no-trace --entry test | FileCheck %S/shared.h
long ag[] = {1,3,2,4,9,8,7,5,6,11,10,12};

struct node {
   long data; 
   struct node *leftChild;
   struct node *rightChild;
};

struct node *root = 0;

void insert(long data, struct node* tempNode) {
   struct node *current;
   struct node *parent;

   tempNode->data = data;
   tempNode->leftChild = 0;
   tempNode->rightChild = 0;

   if(root == 0) {
      root = tempNode;
   } else {
      current = root;
      parent = 0;

      while(1) { 
         parent = current;
         
         if(data < parent->data) {
            current = current->leftChild;                
            
            if(current == 0) {
               parent->leftChild = tempNode;
               return;
            }
         }
         else {
            current = current->rightChild;

            if(current == 0) {
               parent->rightChild = tempNode;
               return;
            }
         }
      }            
   }
}

void inorder_traversal(struct node* root, long *arr, long *pos) {
   if(root != 0) {
      inorder_traversal(root->leftChild, arr, pos);
      arr[*pos] = root->data;
      *pos += 1;
      inorder_traversal(root->rightChild, arr, pos);
   }
}

void treeSort(long *array, long len) {
   long i;
   root = 0;
   struct node item[len];
   for(i = 0; i < len; i++){
      insert(array[i], &item[i]);
   }
   i=0;
   inorder_traversal(root, array, &i);
}

long test() {
    long a[] = {1,3,2,4,9,8,7,5,6,11,10,12};
    treeSort( a,12);
    treeSort(ag,12);
    for(long i = 0; i < 12; i++)
        if((i > 0 && a[i] < a[i-1]) || a[i] != ag[i]) {
#ifndef NATIVE
            // CHECK-NOT: custom error
            __builtin_tvm_throw(13);
#endif
            return -1;
        }

    return 0;
}
#ifdef NATIVE
#include <stdio.h>

int main () {
    long res = test();
    printf("Finishing with exit code %d\n", res);
    return (int)res;
}
#endif