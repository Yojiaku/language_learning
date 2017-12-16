#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*****************************************************************************
*                                                                            *
*  Define a structure for circular list elements.                            *
*                                                                            *
*****************************************************************************/

typedef struct CListElmt_ {

	void               *data;
	struct CListElmt_  *next;

} CListElmt;

/*****************************************************************************
*                                                                            *
*  Define a structure for circular lists.                                    *
*                                                                            *
*****************************************************************************/

typedef struct CList_ {

	int                size;

	int                (*match)(const void *key1, const void *key2);
	void               (*destroy)(void *data);

	CListElmt          *head;

} CList;

/*****************************************************************************
*                                                                            *
*  --------------------------- Public Interface ---------------------------  *
*                                                                            *
*****************************************************************************/

void clist_init(CList *list, void (*destroy)(void *data));

void clist_destroy(CList *list);

int clist_ins_next(CList *list, CListElmt *element, const void *data);

int clist_rem_next(CList *list, CListElmt *element, void **data);

#define clist_size(list) ((list)->size)

#define clist_head(list) ((list)->head)

#define clist_data(element) ((element)->data)

#define clist_next(element) ((element)->next)

/*****************************************************************************
*                                                                            *
*  ------------------------------- clist.c --------------------------------  *
*                                                                            *
*****************************************************************************/


/*****************************************************************************
*                                                                            *
*  ------------------------------ clist_init ------------------------------  *
*                                                                            *
*****************************************************************************/

void clist_init(CList *list, void (*destroy)(void *data)) {

/*****************************************************************************
*                                                                            *
*  Initialize the list.                                                      *
*                                                                            *
*****************************************************************************/

	list->size = 0;
	list->destroy = destroy;
	list->head = NULL;

	return;

}

/*****************************************************************************
*                                                                            *
*  ---------------------------- clist_destroy -----------------------------  *
*                                                                            *
*****************************************************************************/

void clist_destroy(CList *list) {

	void               *data;

	/*****************************************************************************
	*                                                                            *
	*  Remove each element.                                                      *
	*                                                                            *
	*****************************************************************************/

	while (clist_size(list) > 0) {

	   if (clist_rem_next(list, list->head, (void **)&data) == 0 && list->destroy
	      != NULL) {

	      /***********************************************************************
	      *                                                                      *
	      *  Call a user-defined function to free dynamically allocated data.    *
	      *                                                                      *
	      ***********************************************************************/

	      list->destroy(data);

	   }

	}

	/*****************************************************************************
	*                                                                            *
	*  No operations are allowed now, but clear the structure as a precaution.   *
	*                                                                            *
	*****************************************************************************/

	memset(list, 0, sizeof(CList));

	return;

}

/*****************************************************************************
*                                                                            *
*  ---------------------------- clist_ins_next ----------------------------  *
*                                                                            *
*****************************************************************************/

int clist_ins_next(CList *list, CListElmt *element, const void *data) {

	CListElmt          *new_element;

	/*****************************************************************************
	*                                                                            *
	*  Allocate storage for the element.                                         *
	*                                                                            *
	*****************************************************************************/

	if ((new_element = (CListElmt *)malloc(sizeof(CListElmt))) == NULL)
	   return -1;

	/*****************************************************************************
	*                                                                            *
	*  Insert the element into the list.                                         *
	*                                                                            *
	*****************************************************************************/

	new_element->data = (void *)data;

	if (clist_size(list) == 0) {

	   /**************************************************************************
	   *                                                                         *
	   *  Handle insertion when the list is empty.                               *
	   *                                                                         *
	   **************************************************************************/

	   new_element->next = new_element;
	   list->head = new_element;

	   }

	else {

	   /**************************************************************************
	   *                                                                         *
	   *  Handle insertion when the list is not empty.                           *
	   *                                                                         *
	   **************************************************************************/

	   new_element->next = element->next;
	   element->next = new_element;

	}

	/*****************************************************************************
	*                                                                            *
	*  Adjust the size of the list to account for the inserted element.          *
	*                                                                            *
	*****************************************************************************/

	list->size++;

	return 0;

}

/*****************************************************************************
*                                                                            *
*  ---------------------------- clist_rem_next ----------------------------  *
*                                                                            *
*****************************************************************************/

int clist_rem_next(CList *list, CListElmt *element, void **data) {

	CListElmt          *old_element;

	/*****************************************************************************
	*                                                                            *
	*  Do not allow removal from an empty list.                                  *
	*                                                                            *
	*****************************************************************************/

	if (clist_size(list) == 0)
	   return -1;

	/*****************************************************************************
	*                                                                            *
	*  Remove the element from the list.                                         *
	*                                                                            *
	*****************************************************************************/

	*data = element->next->data;

	if (element->next == element) {

	   /**************************************************************************
	   *                                                                         *
	   *  Handle removing the last element.                                      *
	   *                                                                         *
	   **************************************************************************/

	   old_element = element->next;
	   list->head = NULL;

	   }

	else {

	   /**************************************************************************
	   *                                                                         *
	   *  Handle removing other than the last element.                           *
	   *                                                                         *
	   **************************************************************************/

	   old_element = element->next;
	   element->next = element->next->next;

	}

	/*****************************************************************************
	*                                                                            *
	*  Free the storage allocated by the abstract data type.                     *
	*                                                                            *
	*****************************************************************************/

	free(old_element);

	/*****************************************************************************
	*                                                                            *
	*  Adjust the size of the list to account for the removed element.           *
	*                                                                            *
	*****************************************************************************/

	list->size--;

	return 0;

}

#define MAX 100
#define TRUE 1
#define FALSE 0

/* Define a structure for information about pages. */
typedef struct Page_
{
	int number;
	int reference;
}Page;

/* Public Interface */
int replace_page(CListElmt **current, int c);
int find(int a, CList list, int size);

int find(int a, CList list, int size)
{
	int i;
	int flag;
	flag = FALSE;
	CListElmt *current;
	Page *p;
	current = clist_head(&list);
	for(i=0; i<size; i++)
	{
		p = clist_data(current);
		if(a == p->number)
		{
			flag = TRUE;
		}
		current = clist_next(current);
	}
	return flag;
}

int replace_page(CListElmt **current, int c)
{
	/* Circle through the list of pages until one is found to replace. */
	while(((Page *)(*current)->data)->reference != 0)
	{
		((Page *)(*current)->data)->reference = 0;
		*current = clist_next(*current);
	}

	((Page *)(*current)->data)->number = c;
	return ((Page *)(*current)->data)->number;
}

int main() {

	CList              list;
	CListElmt          *current;

	Page               *p;

	int                i;

	/*****************************************************************************
	*                                                                            *
	*  Initialize the circular list.                                             *
	*                                                                            *
	*****************************************************************************/

	clist_init(&list, free);

	/*****************************************************************************
	*                                                                            *
	*  Load the pages into the list.                                             *
	*                                                                            *
	*****************************************************************************/

	current = NULL;

	int size = 3;  //设置页框为3
	FILE* fp;
	int count;
	int c;
	int miss;
	// 初始化页框
	for (i = 0; i < size; i++) {

	    if ((p = (Page *)malloc(sizeof(Page))) == NULL)
	    	return 1;

        p->reference = 0;

	    p->number = -1;

	   if (clist_ins_next(&list, current, p) != 0)
	      return 1;

	   if (current == NULL)
	      current = clist_next(clist_head(&list));
	   else
	      current = clist_next(current);

	}

	fp = fopen("workload0", "r");
	printf("for the frame size is %d\n", size);

	current = clist_head(&list);
	count = 0;
	miss = 0;
	while(fscanf(fp, "%d*[^0-9]", &c)!=EOF)
	{
		if(find(c, list, size) == FALSE)
		{
			replace_page(&current, c);
			miss++;
		}
		current = clist_next(current);
		count++;
	}
	double missRate = miss*1.0/count*100;
	printf("缺失%d次\n", miss);
	printf("共有%d次计数\n", count);
	printf("缺失率%f%%\n", missRate);
	fclose(fp);

	/*for (i = 0; i < size; i++) {

	   p = clist_data(current);

	   fprintf(stdout, "p[%d].number=%d, p[%d].reference=%d\n", i, p->number, i,
	      p->reference);

	   current = clist_next(current);

	}*/

	/*****************************************************************************
	*                                                                            *
	*  Get the number of the page to replace.                                    *
	*                                                                            *
	*****************************************************************************/

	/*current = clist_head(&list);
	i = replace_page(&current);
	fprintf(stdout, "Selected %d\n", i);

	current = clist_head(&list);

	for (i = 0; i < 10; i++) {

	   p = clist_data(current);

	   fprintf(stdout, "p[%d].number=%d, p[%d].reference=%d\n", i, p->number, i,
	      p->reference);

	   current = clist_next(current);

	}*/

	/*****************************************************************************
	*                                                                            *
	*  Destroy the circular list.                                                *
	*                                                                            *
	*****************************************************************************/

	fprintf(stdout, "Destroying the list\n");
	clist_destroy(&list);

	return 0;

}
