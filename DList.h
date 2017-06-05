#ifndef DLIST_H
#define DLIST_H	
#include <stddef.h>
#include <stdbool.h>

/* DList objects have two Guard elements: fGuard just before the first
   interior element (if any) and rGuard just after the last interior
   element (if any).  

   The 'prev' link of the front guard is NULL, as is the 'next' link of the 
   rear guard.  Their other two links point toward each other (directly, or
   via the interior elements of the list).

   An empty list looks like this:

                      +------------+     +-----------+
                 NULL-| fGuard     |<--->| rGuard    |-NULL
                      +------------+     +-----------+

   A list with two elements in it looks like this:

           +------------+     +-------+     +-------+     +-----------+
      NULL-| fGuard     |<--->|   1   |<--->|   2   |<--->| rGuard    |-NULL
           +------------+     +-------+     +-------+     +-----------+

   The symmetry of this arrangement eliminates lots of special cases in list
   processing.  
   
   We say a DList object is proper if it conforms to the structural description
   above.  In particular, a proper DList object has the following properties:
   
     - fGuard.prev == NULL and rGuard.next == NULL
     - if the DList is empty, fGuard.next == &rGuard and 
       rGuard.prev == &fGuard
     - if the DList is nonempty, fGuard.next points to the first interior 
       node, and rGuard.prev points to the last interior node
     - for each interior node, its prev pointer points to the preceding node,
       and its next pointer points to the succeeding node

   (Because only one of the pointers in each guard element is used, we could 
   in fact combine them into a single header element without sacrificing this 
   simplicity.  But using two separate elements allows us to do a little bit 
   of checking on some operations, which can be valuable.) 

   This implementation of a list does not require use of dynamically allocated
   memory.  Instead, each structure that is a potential list element must embed
   a DNode member.  All of the list functions operate on these DNode objects.  

   The DList_Entry macro allows conversion from a DNode back to a structure object 
   that contains it.

   For example, suppose there is a need for a list of 'struct Widget'.
   'struct Widget' should contain a 'DNode' member, like so:

      struct Widget {
         DNode node;
         int bar;
         ...other members...
      };

   Then a list of 'struct Widget' can be be declared and initialized like so:

      DList Widget_L;

      DList_Init(&Widget_L);

   Retrieval is a typical situation where it is necessary to convert from a 
   DNode back to its enclosing structure.  Here's an example using Widget_Q:

      DNode *e = DList_Pop(Widget_L);

      struct Widget *f = DList_Entry(e, struct Widget, node);
      // now, do something with f...

   The interface for this list is inspired by the list<> template in the C++ 
   STL.  If you're familiar with list<>, you should find this easy to use.  
   However, it should be emphasized that these lists do *no* type checking and 
   can't do much other correctness checking.  If you screw up, it will bite you.

   Glossary of DList terms:

     - "interior element": An element that is not the head or tail, that is, a 
               real list element.  An empty list does not have any interior elements.

     - "front": The first interior element in a list.  Undefined in an empty list.
                Returned by DList_Front().

     - "back": The last interior element in a list.  Undefined in an empty list.
               Returned by DList_Back().

     - "end":  The element figuratively just after the last interior element of a list;
               i.e., the rear guard.  
               Well-defined even in an empty list.
*/

// DList node:
struct _DNode {

   struct _DNode *prev;     // Previous list element (toward fGuard)
   struct _DNode *next;     // Next list element (toward rGuard)
};

typedef struct _DNode DNode;

// DList object:
struct _DList {
   DNode fGuard;            // sentinel node at the front of the list
   DNode rGuard;            // sentinel node at the tail of the list
};

typedef struct _DList DList;

// DList_Entry() is a useful macro; there is a full discussion of a similar
// macro for a generic doubly-linked list implementation in the CS 2505 notes.
// Converts pointer to list element NODE into a pointer to the structure that
// NODE is embedded inside.  Supply the name of the outer structure STRUCT and 
// the member name MEMBER of the NODE.  See the big comment at the top of the
// file for an example. 

#define DList_Entry(NODE, STRUCT, MEMBER)                              \
        ((STRUCT *) ((uint8_t *) (NODE) - offsetof (STRUCT, MEMBER)))

// Initialize DNode pointers to NULL.
//
// Pre:  pN points to a presumably raw DNode object
// Post: pN->prev and pN->next are NULL
//
void DNode_Init(DNode* const pN);

// Initialize DList to empty state.
//
// Pre:  pL points to a presumably raw DList object
// Post: *pL has been set to an empty state (see header comments)
//
void DList_Init(DList* const pL);

// Return whether DList is empty.
//
// Pre:  pL points to a proper DList object
// Returns true if *pL is empty, false otherwise
//
bool DList_Empty(const DList* const pL);

// Insert *pNode as predecessor of *pBefore.
//
// Pre:  pBefore points to an interior node or the rear guard of a
//         proper DList object
//       pNode points to a proper DNode object
// Post: *pNode has been inserted in front of *pBefore
//
void DList_PushBefore(DNode* const pBefore, DNode* const pNode);

// Insert *pNode as first interior element of *pL.
//
// Pre:  pL points to a proper DList object
//       pNode points to a proper DNode object
// Post: *pNode has been inserted at the front of *pL
//
void DList_PushFront(DList* const pL, DNode* const pNode);

// Insert *pNode as last interior element of *pL.
//
// Pre:  pL points to a proper DList object
//       pNode points to a proper DNode object
// Post: *pNode has been inserted at the rear of *pL
//
void DList_PushRear(DList* const pL, DNode* const pNode);

// Remove interior node preceding *pBefore and return it.
//
// Pre:  pBefore points to an interior node or rear guard of a
//         proper DList object
// Post: the interior DNode that preceded *pBefore has been removed
// Returns pointer to the DNode that was removed, NULL otherwise
//
DNode* DList_PopBefore(DNode* const pBefore);

// Remove first interior element of *pL and return it.
//
// Pre:  pL points to a proper DList object
// Post: the interior DNode that was at the front of *pL has been removed
// Returns pointer to the DNode that was removed, NULL if *pL was empty
//
DNode* DList_PopFront(DList* const pL);

// Remove last interior element of *pL and return it.
//
// Pre:  pL points to a proper DList object
// Post: the interior DNode that was at the rear of *pL has been removed
// Returns pointer to the DNode that was removed, NULL otherwise
//
DNode* DList_PopRear(DList* const pL);

// Return pointer to the first interior element, if any; does not remove
// the element.
//
// Pre:  pL points to a proper DList object
// Returns pointer first interior DNode in *pL, NULL if *pL is empty
//
const DNode* DList_Front(const DList* const pL);

// Return pointer to the last interior element, if any; useful for client-
// side traversal code.
//
// Pre:  pL points to a proper DList object
// Returns pointer last interior DNode in *pL, NULL if *pL is empty
//
const DNode* DList_Back(const DList* const pL);

#endif
