struct node {
   long data; 
   struct node *leftChild;
   struct node *rightChild;
};

struct node *root = 0;

void insert(long data) {
   struct node temp;
   struct node *tempNode = &temp;
   struct node *current;
   struct node *parent;

   tempNode->data = data;
   tempNode->leftChild = 0;
   tempNode->rightChild = 0;

   //if tree is empty
   if(root == 0) {
      root = tempNode;
   } else {
      current = root;
      parent = 0;

      while(1) { 
         parent = current;
         
         //go to left of the tree
         if(data < parent->data) {
            current = current->leftChild;                
            
            //insert to the left
            if(current == 0) {
               parent->leftChild = tempNode;
               return;
            }
         }  //go to right of the tree
         else {
            current = current->rightChild;

            //insert to the right
            if(current == 0) {
               parent->rightChild = tempNode;
               return;
            }
         }
      }            
   }
}

void inorder_traversal(struct node* root) {
   if(root != 0) {
      inorder_traversal(root->leftChild);
      inorder_traversal(root->rightChild);
   }
}

void treeSort(long *array, long len) {
   long i;
   for(i = 0; i < len; i++)
      insert(array[i]);
   inorder_traversal(root);
}
