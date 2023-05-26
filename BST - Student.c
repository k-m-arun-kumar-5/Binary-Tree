/* ======================================================================
  File Name     : BST - Emp.C
  Description   : builds and prints a student list using Binary Search Tree.
  Author        : K.M. Arun Kumar alias Arunkumar Murugeswaran
  Date          :
  Remarks     1 : by default, global execution flow flag is disabled for debugging.
  Known Bugs  1 : 
  Modification
       History  :
  Others        :
====================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define NULL_DATA_PTR                            ((void *)0)
#define NULL_CHAR                               ('\0')

#define STR_MAX_NUM_CHARS             (10)
#define MIN_STUDENT_ID                (1)
#define MAX_STUDENT_ID                (1000)
#define STUDENT_NAME_MAX_CHARS        (15)
#define STUDENT_MIN_GRADE             (0.0)
#define STUDENT_MAX_GRADE             (100.0)

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

typedef char int8_t;
typedef short int int16_t;
typedef int int32_t;
typedef long int64_t;

typedef enum
{
	NO_ERROR, SUCCESS = 0, FAILURE, ERR_NULL_PTR, ERR_INVALID_DATA, ERR_BST_EMPTY, ERR_BST_FULL
} system_status_t;

typedef struct node
{
	void *dataPtr;
	struct node *left;
	struct node *right;
} NODE;

typedef struct
{
	int count;
	int (*compare)(void *arg1, void *arg2);
	NODE *root;
} BST_TREE;

typedef struct
{
    int id;
    char name[20];
    float gpa;
} STUDENT;

system_status_t system_status = NO_ERROR;

bool trace_flag = false;

BST_TREE* BST_Create(int (*compare) (void* argu1, void* argu2));
BST_TREE* BST_Destroy (BST_TREE* tree);
bool BST_Insert (BST_TREE* tree, void* dataPtr);
bool BST_Delete (BST_TREE* tree, void* dltKey);
void* BST_Retrieve (BST_TREE* tree, void* keyPtr);
void BST_Traverse (BST_TREE* tree, void (*process)(void* dataPtr));
bool BST_Empty (BST_TREE* tree);
bool BST_Full (BST_TREE* tree);
int BST_Count (BST_TREE* tree);
static NODE* _insert (BST_TREE* tree, NODE* root, NODE* newPtr);
static NODE* _delete (BST_TREE* tree, NODE* root, void* dataPtr, bool* success);
static void* _retrieve (BST_TREE* tree, void* dataPtr, NODE* root);
static void _traverse (NODE* root, void (*process) (void* dataPtr));
static void _destroy (NODE* root);

// Prototype Declarations
char getOption (void);
void addStu (BST_TREE* list);
void deleteStu (BST_TREE* list);
void findStu (BST_TREE* list);
void printList (BST_TREE* list);
void testUtilties (BST_TREE* tree);
int compareStu (void* stu1, void* stu2);
void processStu (void* dataPtr);
uint16_t Get_Validate_Input_Float(float *const float_input_num_ptr, char *const input_str_ptr, const unsigned int input_str_max_chars, const float valid_min_value, const float valid_max_value);
uint16_t Get_Validate_Input_Double(double *const double_input_num_ptr, char *const input_str_ptr, const unsigned int input_str_max_chars, const double valid_min_value, const double valid_max_value);
uint16_t Get_Validate_Input_Number(void *const int32_input_num_ptr, char *const input_str_ptr, const unsigned int input_str_max_chars, const int32_t valid_min_value, const int32_t valid_max_value);
uint16_t Get_Input_Str(char *const input_str_ptr, const unsigned int input_str_max_chars);
uint16_t Str_to_Num_Conv(void *const num_conv_from_str_ptr, const char *const num_in_str);
uint32_t Power_Of(const uint8_t base, const uint8_t power);
uint16_t Get_Input_Alpha_Char_Str(char *const input_str_ptr, const unsigned int input_str_max_chars);
char temp_str[STR_MAX_NUM_CHARS];
/*******************************************************************
 Function Name  : BST_Create
 Description    : Allocates dynamic memory for an BST tree head
                  node and returns its address to caller
 Pre            : compare is address of compare function used when two nodes need to be compared
 Post           : head allocated or error returned Return head node pointer; null if overflow
 Remarks        :
 Func ID        : 1
*******************************************************************/
BST_TREE* BST_Create (int (*compare) (void* argu1, void* argu2))
{
     BST_TREE* tree;

     tree = (BST_TREE*) calloc (1, sizeof (BST_TREE));
     if (tree)
     {
        tree->root = NULL;
        tree->count = 0;
        tree->compare = compare;
     }
	 if(trace_flag)
	 {
		 printf("\n BST Meta Head Ptr : 0X%X", tree);
	 }
     return tree;
}

/*******************************************************************
 Function Name  : BST_Insert
 Description    : inserts new data into the tree.
 Pre            : tree is pointer to BST tree structure
 Post           : data inserted or memory overflow and Return Success (true) or Overflow (false)
 Remarks        :
 Func ID        : 2
*******************************************************************/
bool BST_Insert(BST_TREE* tree, void* dataPtr)
{
    // Local Definitions
    NODE* newPtr;

    newPtr = (NODE*)calloc(1, sizeof(NODE));
    if (!newPtr)
       return false;
    newPtr->right = NULL;
    newPtr->left = NULL;
    newPtr->dataPtr = dataPtr;
    if (tree->count == 0)
	{
       tree->root = newPtr;
	   if(trace_flag)
		    printf("\n Insert in Empty Tree - tree->root : 0X%X",  tree->root);
	}
    else
      _insert(tree, tree->root, newPtr);
    (tree->count)++;
    return true;
}

/*******************************************************************
 Function Name  : _insert
 Description    : recursion to insert the new data into a leaf node in the BST tree.
 Pre            : Application has called BST_Insert, which passes root and data pointer
 Post           : Data have been inserted and Return pointer to [potentially] new root
 Remarks        :
 Func ID        : 3
*******************************************************************/
NODE* _insert (BST_TREE* tree, NODE* root, NODE* newPtr)
{
   if (!root)
      // if NULL tree
      return newPtr;
   // Locate null subtree for insertion
   if (tree->compare(newPtr->dataPtr, root->dataPtr) < 0)
   {
      root->left = _insert(tree, root->left, newPtr);
	  if(trace_flag)
	  {
		  printf("\n TRACE[03.01]: root : 0X%X, root->left: 0X%X, NewPtr: 0X%X", root, root->left, newPtr);
	  }
      return root;
   } // new < node
   else
    // new data >= root data
   {
       root->right = _insert(tree, root->right, newPtr);
	   if(trace_flag)
	   {
	         printf("\n TRACE[03.02]: root : 0X%X, root->right: 0X%X, NewPtr: 0X%X", root, root->right, newPtr);
	   }
       return root;
   } // else new data == root data
   return root;
}

/*******************************************************************
 Function Name  : BST_Delete
 Description    : deletes a node from the tree and rebalances it if necessary.
 Pre            : tree initialized--null tree is OK dltKey is pointer to data structure
                   containing key to be deleted
 Post           : node deleted and its space recycled -or- An error code is returned. Return Success (true) or Not found (false)
 Remarks        :
 Func ID        : 4
*******************************************************************/
bool BST_Delete(BST_TREE* tree, void* dltKey)
{
    bool success;
    NODE* newRoot;

    newRoot = _delete (tree, tree->root, dltKey, &success);
    if (success)
    {
       tree->root = newRoot;
	   if(trace_flag)
		   printf("\n TRACE[04.01]: tree->root : 0X%X", tree->root);
       (tree->count)--;
       if (tree->count == 0)
	   {
          // Tree now empty
          tree->root = NULL;
	   }
     } // if
     return success;
}

/*******************************************************************
 Function Name  : _delete
 Description    : Deletes node from the tree and rebalances tree if necessary.
 Pre            : tree initialized--null tree is OK. dataPtr contains key of node to be deleted
 Post           : node is deleted and its space recycled. -or- if key not found, tree is unchanged
                  success is true if deleted; false if not Return pointer to root
 Remarks        :
 Func ID        : 5
*******************************************************************/
NODE* _delete (BST_TREE* tree, NODE* root, void* dataPtr, bool* success)
{
    // Local Definitions
     NODE* dltPtr;
     NODE* exchPtr;
     NODE* newRoot;
     void* holdPtr;

     if (!root)
     {
        *success = false;
        return NULL;
     } // if
     if (tree->compare(dataPtr, root->dataPtr) < 0)
	 {
         root->left = _delete (tree, root->left, dataPtr, success);
		 if(trace_flag)
			 printf("\n TRACE[05.01]: < 0 in root : 0X%X, root->left: 0X%X", root, root->left);
	 }
     else if (tree->compare(dataPtr, root->dataPtr) > 0)
	 {
        root->right = _delete (tree, root->right, dataPtr, success);
		 if(trace_flag)
			 printf("\n TRACE[05.02]: > 0 in root: 0X%X, root->right: 0X%X", root, root->right);
	 }
     else
       {
		    // Delete node found--test for leaf node
           dltPtr = root;
		   if(trace_flag)
			   printf("\n TRACE[05.04]: delete node found - dltPtr: 0X%X", dltPtr);
           if (!root->left)
           {
			   // No left subtree
			   if(trace_flag)
			        printf("\n TRACE[05.05]: no left subtree - delete data: 0X%X", root->dataPtr);
               free (root->dataPtr); // data memory
               newRoot = root->right;
			    if(trace_flag)
			        printf("\n TRACE[05.06]: root: 0X%X, root->right: 0X%X, Delete Node: 0X%X", root, newRoot, dltPtr );
               free (dltPtr); // BST Node
               *success = true;
               return newRoot; // base case
           }   // if true
           else
		   {
			   if (!root->right)
                {
			  // Only left subtree
                    newRoot = root->left;
					if(trace_flag)
			             printf("\n TRACE[05.07]: only left subtree - root: 0X%X, root->left: 0X%X, dltPtr: 0X%X", root, root->left, dltPtr);
                    free (dltPtr);
                    *success = true;
                    return newRoot; // base case
              } // if
              else
                 // Delete Node has two subtrees
              {
				  if(trace_flag)
			             printf("\n TRACE[05.08]: delete node has two subtrees - root: 0X%X, root->left: 0X%X", root, root->left );
                  exchPtr = root->left;
                 // Find largest node on left subtree
                  while (exchPtr->right)
                       exchPtr = exchPtr->right;
                   // Exchange Data
				  if(trace_flag)
			             printf("\n TRACE[05.09]: largest data on left subtree: 0X%X", exchPtr);
                  holdPtr = root->dataPtr;
                  root->dataPtr = exchPtr->dataPtr;
                  exchPtr->dataPtr = holdPtr;
				  if(trace_flag)
			             printf("\n TRACE[05.10]: exchange data : 0X%X <-> 0X%X, root: 0X%X, root->left:0X%X",root->dataPtr, holdPtr, root, root->left);
                  root->left = _delete (tree, root->left, exchPtr->dataPtr, success);
               } // else
         }
	 }		// node found
    return root;
}

/*******************************************************************
 Function Name  : BST_Retrieve
 Description    : Retrieve node searches tree for the node containing
                  the requested key and returns pointer to its data.
 Pre            : Tree has been created (may be null) data is pointer to data structure
                   containing key to be located
 Post           :  Tree searched and data pointer returned Return Address of matching node returned.
                   If not found, NULL returned
 Remarks        :
 Func ID        : 6
*******************************************************************/
void* BST_Retrieve (BST_TREE* tree, void* keyPtr)
{

if (tree->root)
    return _retrieve (tree, keyPtr, tree->root);
else
    return NULL;
}

/*******************************************************************
 Function Name  : _retrieve
 Description    : Searches tree for node containing requested key
                  and returns its data to the calling function.
 Pre            : _retrieve passes tree, dataPtr, root, dataPtr is pointer to data structure
                   containing key to be located
 Post           :  tree searched; data pointer returned Return Address of data in matching node.
                   If not found, NULL returned
 Remarks        :
 Func ID        : 7
*******************************************************************/
void* _retrieve (BST_TREE* tree, void* dataPtr, NODE* root)
{
    if (root)
	{
         if (tree->compare(dataPtr, root->dataPtr) < 0)
		 {
			 if(trace_flag)
			      printf("\n TRACE[07.01]: < 0 dataPtr: 0X%X, root: 0X%X, root->left: 0X%X", dataPtr, root, root->left );
            return _retrieve(tree, dataPtr, root->left);
		 }
         else if (tree->compare(dataPtr, root->dataPtr) > 0)
		 {
			if(trace_flag)
			      printf("\n TRACE[07.02]: > 0 dataPtr: 0X%X, root: 0X%X, root->right: 0X%X", dataPtr, root, root->right );
            return _retrieve(tree, dataPtr, root->right);
		 }
          else
		  {
			   // Found equal key
			  if(trace_flag)
			      printf("\n TRACE[07.03]: = 0 root: 0X%X, dataPtr: 0X%X", root, root->dataPtr);
            return root->dataPtr;
		  }
    } // if root
    else
     // Data not in tree
      return NULL;
}

/*******************************************************************
 Function Name  : BST_Traverse
 Description    : Process tree using inorder traversal.
 Pre            : Tree has been created (may be null) process “visits” nodes during traversal
 Post           : Nodes processed in LNR (inorder) sequence
 Remarks        :
 Func ID        : 8
*******************************************************************/
void BST_Traverse (BST_TREE* tree, void (*process) (void* dataPtr))
{
	// Statements
    _traverse (tree->root, process);
     return;
}
/*******************************************************************
 Function Name  : _traverse
 Description    : Inorder tree traversal. To process a node, we use
                  the function passed when traversal was called.
 Pre            : Tree has been created (may be null)
 Post           : All nodes processed
 Remarks        :
 Func ID        : 9
*******************************************************************/
void _traverse (NODE* root, void (*process) (void* dataPtr))
{
     if (root)
     {
		 if(trace_flag)
			  printf("\n TRACE[09.01]: root: 0X%X, root->left: 0X%X", root, root->left);
        _traverse (root->left, process);
		 if(trace_flag)
			  printf("\n TRACE[09.02]: root: 0X%X, root->dataPtr: 0X%X", root, root->dataPtr);
		 process (root->dataPtr);
		 if(trace_flag)
		     printf("\n TRACE[09.03]: root: 0X%X, root->right: 0X%X", root, root->right);
        _traverse (root->right, process);
     } // if
     return;
}

/*******************************************************************
 Function Name  : BST_Empty
 Description    : Returns true if tree is empty; false if any data.
 Pre            : Tree has been created. (May be null)
 Post           : Returns True if tree empty, false if any data
 Remarks        :
 Func ID        : 10
*******************************************************************/
bool BST_Empty(BST_TREE* tree)
{
    // Statements
    return (tree->count == 0);
}

/*******************************************************************
 Function Name  : BST_Full
 Description    : If there is no room for another node, returns true.
 Pre            : Tree has been created. (May be null)
 Post           : true if no room for another insert
 Remarks        :
 Func ID        : 11
*******************************************************************/
bool BST_Full(BST_TREE* tree)
{
    NODE* newPtr;

    newPtr = (NODE*)calloc(1, sizeof (*(tree->root)));
    if (newPtr)
    {
       free (newPtr);
       return false;
    } // if
    return true;
}
/*******************************************************************
 Function Name  : BST_Count
 Description    : Returns number of nodes in tree.
 Pre            : Tree has been created. (May be null)
 Post           : Returns tree count
 Remarks        :
 Func ID        : 12
*******************************************************************/
int BST_Count (BST_TREE* tree)
{
    return (tree->count);
}

/*******************************************************************
 Function Name  : BST_Destroy
 Description    : Deletes all data in tree and recycles memory.
                  The nodes are deleted by calling a recursive function to traverse the tree in inorder sequence.
 Pre            : tree is a pointer to a valid tree
 Post           : All data and head structure deleted. Return null head pointer
 Remarks        :
 Func ID        : 13
*******************************************************************/
BST_TREE* BST_Destroy (BST_TREE* tree)
{
    if (tree)
       _destroy (tree->root);
     // All nodes deleted. Free structure
     free (tree);
     return NULL;
}

/*******************************************************************
 Function Name  : _destroy
 Description    : Deletes all data in tree and recycles memory. It also recycles memory for the key and data nodes.
                  The nodes are deleted by calling a recursive function to traverse the tree in inorder sequence.
 Pre            : root is pointer to valid tree/subtree
 Post           : All data and head structure deleted
 Remarks        :
 Func ID        : 14
*******************************************************************/
void _destroy (NODE* root)
{
    if(root)
    {
		if(trace_flag)
			  printf("\n TRACE[14.01]: root: 0X%X, root->left: 0X%X", root, root->left);
         _destroy (root->left);
		 if(trace_flag)
			  printf("\n TRACE[14.02]: root: 0X%X, delete: 0X%X", root, root->dataPtr);
         free (root->dataPtr);
		 if(trace_flag)
			  printf("\n TRACE[14.03]: root: 0X%X, root->right: 0X%X", root, root->right);
         _destroy (root->right);
         free(root);
    }
    return;
}
/*******************************************************************
 Function Name  : main
 Description    :
 Pre            :
 Post           :
 Remarks        :
 Func ID        : 15
*******************************************************************/
int main (void)
{
   BST_TREE* list;
   char option = ' ';
   printf("\n Begin Student List");
   list = BST_Create (compareStu);
   while ((option = getOption ()) != 'Q')
   {
	    switch (option)
       {
            case 'A':
			    addStu (list);
            break;
            case 'D':
			    deleteStu (list);
            break;
            case 'F':
			   findStu (list);
            break;
            case 'P':
			   printList (list);
            break;
            case 'U':
    			testUtilties (list);
            break;
       }
    }
    list = BST_Destroy (list);
    printf("\nEnd Student List\n");
    return 0;
}

/*******************************************************************
 Function Name  : getOption
 Description    : Reads and validates processing option from keyboard.
 Pre            :
 Post           : valid option returned
 Remarks        :
 Func ID        : 16
*******************************************************************/
char getOption (void)
{
    char option[2];
    bool error = true;

    printf("\n ====== MENU ======\n");
    printf(" A - Add Student\n");
    printf(" D - Delete Student\n");
    printf(" F - Find Student\n");
    printf(" P - Print Class List\n");
    printf(" U - Show Utilities\n");
    printf(" Q - Quit\n");
    do
    {
        printf("\nEnter Option: ");
        if((Get_Input_Str(option, 2)) != SUCCESS)
		{
			printf("\n Invalid option. Please re-enter: ");
            error = true;
			continue;
		}
        option[0] = toupper(option[0]);
        if (option[0] == 'A' || option[0] == 'D' || option[0] == 'F' || option[0] == 'P' || option[0] == 'U' || option[0] == 'Q')
          error = false;
        else
        {
           printf("\n Invalid option. Please re-enter: ");
           error = true;
        }
    } while (error == true);
    return option[0];
}
/*******************************************************************
 Function Name  : addStu
 Description    : Adds a student to tree.
 Pre            :
 Post           : student added (abort on memory overflow)
 Remarks        :
 Func ID        : 17
*******************************************************************/
void addStu (BST_TREE* list)
{
    STUDENT *getPtr, *stuPtr;

     stuPtr = (STUDENT*) calloc (1, sizeof (STUDENT));
     if (!stuPtr)
     {
         printf("\n ERR: Memory Overflow in add");
    	 exit(101);
     }
     printf("Enter student id: ");
	 if((Get_Validate_Input_Number(&(stuPtr->id), temp_str, STR_MAX_NUM_CHARS, MIN_STUDENT_ID, MAX_STUDENT_ID)) != SUCCESS)
     {
		printf("\n ERR: Invalid student ID");
		free(stuPtr);
		return;
     }
     getPtr = (STUDENT*)BST_Retrieve (list, &stuPtr->id);
	 if(getPtr != NULL && getPtr->id == (stuPtr->id))
	 {
		 printf("\n ERR[17.01]: Student ID: %d already exist with name: %s, gpa: %f", getPtr->id, getPtr->name, getPtr->gpa);
		 free(stuPtr);
		 return;
	 }
     printf("Enter student name: ");
	 if((Get_Input_Alpha_Char_Str(stuPtr->name, STUDENT_NAME_MAX_CHARS)) != SUCCESS)
	 {
        free(stuPtr);
		return;
	 }
     printf("Enter student gpa: ");
     if((Get_Validate_Input_Float(&(stuPtr->gpa),temp_str, STR_MAX_NUM_CHARS, STUDENT_MIN_GRADE, STUDENT_MAX_GRADE)) != SUCCESS)
     {
		printf("\n ERR: Invalid grade ID");
		free(stuPtr);
		return;
     }
     BST_Insert (list, stuPtr);
}
/*******************************************************************
 Function Name  : deleteStu
 Description    : Deletes a student from the tree.
 Pre            :
 Post           : student deleted or error message printed
 Remarks        :
 Func ID        : 18
*******************************************************************/
void deleteStu (BST_TREE* list)
{
   int id;
   int* idPtr = &id;

   printf("Enter student id: ");
   if((Get_Validate_Input_Number(idPtr, temp_str, STR_MAX_NUM_CHARS, MIN_STUDENT_ID, MAX_STUDENT_ID)) != SUCCESS)
   {
		printf("\n ERR: Invalid student ID");
		return;
   }
   if (!BST_Delete (list, idPtr))
      printf("\n ERROR: No Student: %d", *idPtr);
}
/*******************************************************************
 Function Name  : findStu
 Description    : Finds a student and prints name and gpa.
 Pre            : student id
 Post           : student data printed or error message
 Remarks        :
 Func ID        : 19
*******************************************************************/
void findStu (BST_TREE* list)
{
    int id;
    STUDENT* stuPtr;

    printf("\n Enter student id: ");
    if((Get_Validate_Input_Number(&id, temp_str, STR_MAX_NUM_CHARS, MIN_STUDENT_ID, MAX_STUDENT_ID)) != SUCCESS)
    {
		printf("\n ERR: Invalid student ID");
		return;
    }
    stuPtr = (STUDENT*)BST_Retrieve (list, &id);
    if (stuPtr)
    {
        printf("\n Student id: %04d", id);
        printf("\n Student name: %s", stuPtr->name);
        printf("\n Student gpa: %4.1f", stuPtr->gpa);
    }
    else
       printf("Student %d not in list\n", id);
}
/*******************************************************************
 Function Name  : printList
 Description    : Prints a list of students.
 Pre            : list has been created (may be null)
 Post           : students printed
 Remarks        :
 Func ID        : 20
*******************************************************************/
void printList (BST_TREE* list)
{
    printf("\nStudent List:");
	printf("\n======================");
    BST_Traverse (list, processStu);
	printf("\n =====================");
    printf("\nEnd of Student List\n");
    return;
}
/*******************************************************************
 Function Name  : testUtilties
 Description    : tests the ADT utilities by calling
                  each in turn and printing the results.
 Pre            : tree has been created
 Post           : results printed
 Remarks        :
 Func ID        : 21
*******************************************************************/
void testUtilties (BST_TREE* tree)
{
     printf("\n Tree contains %3d nodes: ", BST_Count(tree));
     if (BST_Empty(tree))
        printf("\n The tree IS empty");
     else
        printf("\n The tree IS NOT empty");
     if (BST_Full(tree))
        printf("\n The tree IS full\a");
     else
        printf("\n The tree IS NOT full");
     return;
}
/*******************************************************************
 Function Name  : compareStu
 Description    : Compare two student id's and return low, equal, high.
 Pre            : stu1 and stu2 are valid pointers to students
 Post           : return low (-1), equal (0), or high (+1)
 Remarks        :
 Func ID        : 22
*******************************************************************/
int compareStu (void* stu1, void* stu2)
{
    STUDENT s1;
    STUDENT s2;

    s1 = *(STUDENT*)stu1;
    s2 = *(STUDENT*)stu2;
    if ( s1.id < s2.id)
       return -1;
    if ( s1.id == s2.id)
       return 0;
    return +1;
}
/*******************************************************************
 Function Name  : processStu
 Description    : Print one student's data.
 Pre            : stu is a pointer to a student
 Post           : data printed and line advanced
 Remarks        :
 Func ID        : 23
*******************************************************************/
void processStu (void* stuPtr)
{
    STUDENT aStu;

     aStu = *(STUDENT*)stuPtr;
     printf("\n%04d %-20s %4.1f", aStu.id, aStu.name, aStu.gpa);
     return;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Str_to_Num_Conv

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           : digits are extracted from left to right format from digit in num_in_str

Func ID        : 02.04

BUGS           :
-*------------------------------------------------------------*/
uint16_t Str_to_Num_Conv( void *const num_conv_from_str_ptr, const char *const num_in_str)
{
	 int32_t num = 0, *num_conv_from_str;
	 uint32_t place;
	 int16_t cur_unit;
	 uint8_t num_chars = 0, base = 10, pos = 0, start_num_pos = 0 ;

	 if(num_conv_from_str_ptr == NULL_DATA_PTR || num_in_str == NULL_DATA_PTR )
	 {
		 #ifdef TRACE_ERROR
		   printf("ERR: data are null ptr \n");
		#endif
		return FAILURE;
	 }
	 num_conv_from_str = (int32_t *)num_conv_from_str_ptr;
	 *num_conv_from_str = 0;
	 if(num_in_str[0] >= '0' && num_in_str[0] <= '9')
	 {
		  start_num_pos = 0;
	 }
	 else if(num_in_str[0] == '-')
	 {
		 start_num_pos = 1;
	 }
	 else
	 {
    	 #ifdef TRACE_ERROR
		    printf("ERR: invalid char: %c \n", num_in_str[0]);
		 #endif
         return FAILURE;
	 }
	 num_chars = strlen(num_in_str + start_num_pos);
	 if(num_chars == 0)
	 {
		 #ifdef TRACE_ERROR
		    printf("ERR: data empty \n");
		 #endif
         return FAILURE;
	 }
	 pos = start_num_pos;
     for( place = Power_Of(base, num_chars - 1); place >= 1; place /= base, ++pos )
     {
     	 cur_unit = num_in_str[pos] - '0';
    	 if(cur_unit < 0 ||  cur_unit > 9 )
    	 {
	    	 #ifdef TRACE_ERROR
		       printf("ERR: invalid char at data[%d] = %c \n", pos, num_in_str[pos]);
		     #endif
             return FAILURE;
	     }
         num += (cur_unit * place);
     }
	 if(num_in_str[0] == '-')
	 {
		 *num_conv_from_str = -num;
	 }
	 else
	 {
	     *num_conv_from_str = num;
	 }
	 return SUCCESS;
}
/*------------------------------------------------------------*
FUNCTION NAME  : Power_Of

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 02.10

Bugs           :
-*------------------------------------------------------------*/
uint32_t Power_Of(const uint8_t base, const uint8_t power )
{
    uint32_t power_val = 1;
    uint8_t i = 0;

    if(power == 0)
    {
       return power_val;
    }
    for(i = 1; i <= power; ++i)
    {
      power_val *= base;
    }
    return power_val;
}
/*------------------------------------------------------------*
FUNCTION NAME  : Validate_Number_Input

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 01.02

BUGS           :
-*------------------------------------------------------------*/
uint16_t Get_Validate_Input_Number(void *const input_num_ptr, char *const input_str_ptr, const unsigned int input_str_max_chars, const int32_t valid_min_value, const int32_t valid_max_value)
{
	int32_t temp_int, *int32_input_num_ptr;

	if(int32_input_num_ptr == NULL_DATA_PTR)
	{
		return FAILURE;
	}
	if(valid_min_value > valid_max_value)
	{
	   return FAILURE;
	}
	int32_input_num_ptr = (int32_t *)input_num_ptr;
	*int32_input_num_ptr = 0;
	if((Get_Input_Str(input_str_ptr, input_str_max_chars)) != SUCCESS)
		return FAILURE;
	if((Str_to_Num_Conv(&temp_int, input_str_ptr)) != SUCCESS)
	{
		memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
		return FAILURE;
	}
	memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
	if(temp_int < valid_min_value || temp_int > valid_max_value)
	{
		 #ifdef TRACE_ERROR
		      printf("ERR: input data - %d, out of range [%d,%d] \n", temp_int, valid_min_value, valid_max_value);
		 #endif
		 return FAILURE;
	}
	*int32_input_num_ptr = temp_int;
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Get_Input_Str

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 01.02

BUGS           :
-*------------------------------------------------------------*/
uint16_t Get_Input_Str(char *const input_str_ptr, const unsigned int input_str_max_chars)
{
    unsigned int input_str_num_chars = 0;
	char rcvd_char;
	
    if(input_str_ptr == NULL_DATA_PTR || input_str_max_chars <= 1)
	{
		return FAILURE;
	}
    memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
	while (1)
	{
		rcvd_char = (char) getchar();
		//scanf("%c", &rcvd_char);
		switch(rcvd_char)
        {
			case '\b':
              if(input_str_num_chars > 0)
			  {
			      input_str_ptr[input_str_num_chars] = NULL_CHAR;
				  --input_str_num_chars;
			  }
			break;
            case '\n':
			   if(input_str_num_chars != 0)
			   {
			      input_str_ptr[input_str_num_chars] = NULL_CHAR;
			      return SUCCESS;
			   }
            break;
            default:
     			if(input_str_num_chars + 1 < input_str_max_chars )
    		    {
		    	   input_str_ptr[input_str_num_chars] = rcvd_char;
                   ++input_str_num_chars;
	    		}
		    	else
			    {
			       printf("ERR: Input data num chars exceeds max chars : %u \n", input_str_max_chars - 1);
			       memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
				   fflush(stdin);
				   return FAILURE;
			    }
		}
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Validate_Number_Input

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 01.02

BUGS           :
-*------------------------------------------------------------*/
uint16_t Get_Validate_Input_Double(double *const double_input_num_ptr, char *const input_str_ptr, const unsigned int input_str_max_chars, const double valid_min_value, const double valid_max_value)
{
	char float_str[30], *endptr;
    double temp_float;

    if(double_input_num_ptr == NULL_DATA_PTR || input_str_max_chars <= 1 || valid_min_value > valid_max_value)
	{
		return FAILURE;
	}	
	if((Get_Input_Str(input_str_ptr, input_str_max_chars)) != SUCCESS)
		return FAILURE;
	*double_input_num_ptr = 0.0;
	temp_float = strtod(input_str_ptr, &endptr);
	memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
	if(temp_float == 0)
	{
		if (errno == ERANGE)
            printf("ERR: The value provided was out of range\n");
		return FAILURE;
	}
	if(strcmp(endptr , ""))
	{
		printf("ERR: float data has invalid char(s) \n");
		return FAILURE;
	}
	if(temp_float < valid_min_value || temp_float > valid_max_value)
	{
		 printf("ERR: input data - %lf, out of range [%lf,%lf] \n", temp_float, valid_min_value, valid_max_value);
		 return FAILURE;
	}
	*double_input_num_ptr = temp_float;
	return SUCCESS;

}

/*------------------------------------------------------------*
FUNCTION NAME  : Validate_Number_Input

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 01.02

BUGS           :
-*------------------------------------------------------------*/
uint16_t Get_Validate_Input_Float(float *const float_input_num_ptr, char *const input_str_ptr, const unsigned int input_str_max_chars, const float valid_min_value, const float valid_max_value)
{
	char float_str[30], *endptr;
    float temp_float;

    if(float_input_num_ptr == NULL_DATA_PTR || input_str_max_chars <= 1 || valid_min_value > valid_max_value)
	{
		return FAILURE;
	}
	*float_input_num_ptr = 0.0;
	if((Get_Input_Str(input_str_ptr, input_str_max_chars)) != SUCCESS)
		return FAILURE;
	temp_float = strtof(input_str_ptr, &endptr);
	memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
	if(temp_float == 0)
	{
		if (errno == ERANGE)
            printf("ERR: The value provided was out of range\n");
		return FAILURE;
	}
	if(strcmp(endptr , ""))
	{
		printf("ERR: float data has invalid char(s) \n");
		return FAILURE;
	}
	if(temp_float < valid_min_value || temp_float > valid_max_value)
	{
		 printf("ERR: input data - %f, out of range [%f,%f] \n", temp_float, valid_min_value, valid_max_value);
		 return FAILURE;
	}
	*float_input_num_ptr = temp_float;
	return SUCCESS;

}

/*------------------------------------------------------------*
FUNCTION NAME  : 

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 01.02

BUGS           :
-*------------------------------------------------------------*/
uint16_t Get_Input_Alpha_Char_Str(char *const input_str_ptr, const unsigned int input_str_max_chars)
{
    unsigned int input_str_num_chars = 0, i =0;
	char rcvd_char;

    if(input_str_ptr == NULL_DATA_PTR || input_str_max_chars <= 1)
	{
		return FAILURE;
	}
    memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
	while (1)
	{
		rcvd_char = (char) getchar();
		//scanf("%c", &rcvd_char);
		switch(rcvd_char)
        {
			case '\b':
              if(input_str_num_chars > 0)
			  {
			      input_str_ptr[input_str_num_chars] = NULL_CHAR;
				  --input_str_num_chars;
			  }
			break;
            case '\n':
			   if(input_str_num_chars != 0)
			   {
			      input_str_ptr[input_str_num_chars] = NULL_CHAR;
			      return SUCCESS;
			   }
            break;
            default:
     			if(input_str_num_chars + 1 < input_str_max_chars )
    		    {
					if((rcvd_char >= 'A' && rcvd_char <= 'Z') || (rcvd_char >= 'a' && rcvd_char <= 'z') || rcvd_char == ' ')
					{
                       input_str_ptr[input_str_num_chars] = rcvd_char;
                       ++input_str_num_chars;
					}
					else
					{
						  printf("ERR: Input non alphabet char \n");
			              memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
				          fflush(stdin);
				          return FAILURE;
					}
		    	}
		    	else
			    {
			       printf("ERR: Input data num chars exceeds max chars : %u \n", input_str_max_chars - 1);
			       memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
				   fflush(stdin);
				   return FAILURE;
			    }
		}
	}
	return SUCCESS;
}

