#include "DList.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdbool.h>


// Initialize DNode pointers to NULL.
//
// Pre:  pN points to a presumably raw DNode object
// Post: pN->prev and pN->next are NULL
//
void DNode_Init(DNode* const pN)
{
	pN->prev = NULL; // sets Nodes previous to NULL
	pN->next = NULL; // sets Nodes next to NULL
}

// Initialize DList to empty state.
//
// Pre:  pL points to a presumably raw DList object
// Post: *pL has been set to an empty state (see header comments)
//
void DList_Init(DList* const pL)
{
	pL->fGuard.next = &(pL->rGuard); // fGuard next set to rGuard
	pL->fGuard.prev = NULL; // fGuard prev set to NULL
	pL->rGuard.prev = &(pL->fGuard); // rGuard prev set to fGuard
	pL->rGuard.next = NULL; // rGuard next set to NULL
}

// Return whether DList is empty.
//
// Pre:  pL points to a proper DList object
// Returns true if *pL is empty, false otherwise
//
bool DList_Empty(const DList* const pL)
{
	return ((pL->fGuard.next == &(pL->rGuard)) && (pL->rGuard.prev == &(pL->fGuard)));
	// checks if fGuards next is rGuard and rGuards prev is fGuard
}

// Insert *pNode as predecessor of *pBefore.
//
// Pre:  pBefore points to an interior node or the rear guard of a
//         proper DList object
//       pNode points to a proper DNode object
// Post: *pNode has been inserted in front of *pBefore
//
void DList_PushBefore(DNode* const pBefore, DNode* const pNode)
{
	pNode->prev = pBefore->prev; // pNode prev is linked to pBefore prev
	pBefore->prev->next = pNode; // pBefores prev's next is linked to pNode
	pBefore->prev = pNode; // pBefore prev is linked to pNode
	pNode->next = pBefore; // pNode next is linked to pBefore
}

// Insert *pNode as first interior element of *pL.
//
// Pre:  pL points to a proper DList object
//       pNode points to a proper DNode object
// Post: *pNode has been inserted at the front of *pL
//
void DList_PushFront(DList* const pL, DNode* const pNode)
{
	pNode->prev = &(pL->fGuard); // pNode prev is linked to fGuard (front)
	pNode->next = pL->fGuard.next; // pNode next is linked to fGuards next
	pL->fGuard.next->prev = pNode; // pBefore is linked to pNode
	pL->fGuard.next = pNode; // fGuards next is linked to pNode
}

// Insert *pNode as last interior element of *pL.
//
// Pre:  pL points to a proper DList object
//       pNode points to a proper DNode object
// Post: *pNode has been inserted at the rear of *pL
//
void DList_PushRear(DList* const pL, DNode* const pNode)
{
	pNode->next = &(pL->rGuard); // pNode next is linked to rGuard (rear)
	pNode->prev = pL->rGuard.prev; // pNode prev is linked to rGuards prev
	pL->rGuard.prev->next = pNode; // pNext is linked to pNode
	pL->rGuard.prev = pNode; //rGuards prev is linked to pNode
}

// Remove interior node preceding *pBefore and return it.
//
// Pre:  pBefore points to an interior node or rear guard of a
//         proper DList object
// Post: the interior DNode that preceded *pBefore has been removed
// Returns pointer to the DNode that was removed, NULL otherwise
//
DNode* DList_PopBefore(DNode* const pBefore)
{
	DNode *node = pBefore->prev; // creates new interior node
	if (pBefore->prev->prev == NULL) //if empty list
	{
		return NULL;
	}
	pBefore->prev->prev->next = pBefore; // sets pNodes prevs next to pBefore
	pBefore->prev = pBefore->prev->prev; // sets pBefores prev to pNodes prev
	return node; // returns interior node
}

// Remove first interior element of *pL and return it.
//
// Pre:  pL points to a proper DList object
// Post: the interior DNode that was at the front of *pL has been removed
// Returns pointer to the DNode that was removed, NULL if *pL was empty
//
DNode* DList_PopFront(DList* const pL)
{
	DNode *node = pL->fGuard.next; // creates new first interior element
	if(DList_Empty(pL)) // if empty list
	{
		return NULL;
	}
	pL->fGuard.next->next->prev = &(pL->fGuard); // sets fronts nexts prev to fGuard
	pL->fGuard.next = pL->fGuard.next->next; // sets fGuards next to fronts next
	return node; // returns first interior element
}

// Remove last interior element of *pL and return it.
//
// Pre:  pL points to a proper DList object
// Post: the interior DNode that was at the rear of *pL has been removed
// Returns pointer to the DNode that was removed, NULL otherwise
//
DNode* DList_PopRear(DList* const pL)
{
	DNode *node = pL->rGuard.prev; // creates new last interior element
	if(DList_Empty(pL)) // if empty list
	{
		return NULL;
	}
	pL->rGuard.prev->prev->next = &(pL->rGuard); // sets rears prevs next to rGuard
	pL->rGuard.prev = pL->rGuard.prev->prev; // sets rGuards prev to rears prev 
	return node; // returns the last interior element 
}

// Return pointer to the first interior element, if any; does not remove
// the element.
//
// Pre:  pL points to a proper DList object
// Returns pointer first interior DNode in *pL, NULL if *pL is empty
//
const DNode* DList_Front(const DList* const pL)
{
	DNode *node = pL->fGuard.next; // creates new fist interior element
	if(DList_Empty(pL)) // if empty list
	{
		return NULL;
	}
	return node; // returns the first interior element
}

// Return pointer to the last interior element, if any; useful for client-
// side traversal code.
//
// Pre:  pL points to a proper DList object
// Returns pointer last interior DNode in *pL, NULL if *pL is empty
//
const DNode* DList_Back(const DList* const pL)
{
	DNode *node = pL->rGuard.prev; // creates new last interior element
	if(DList_Empty(pL)) //if empty list
	{
		return NULL;
	}
	return node; // returns the last interior element 
}

//	On my honor:
//
//	-I have not discussed the C language code in my program with
//	anyone other than my instructor or the teaching assistants 
//	assigned to this course.
//
//	-I have not used C language code obtained from another student, 
//	or any other unauthorized source, either modified or unmodified.  
//
//	-If any C language code or documentation used in my program 
//	was obtained from another source, such as a text book or course
//	notes, that has been clearly noted with a proper citation in
//	the comments of my program.
//
//	Kavin Aravind -aravindk
