#include "linked_list.h"
#include <assert.h>

void List_init(ListHead *head)
{
    head->first = 0;
    head->last = 0;
    head->size = 0;
}

ListItem *List_find(ListHead *head, ListItem *item)
{
    ListItem *aux = head->first;
    while (aux)
    {
        if (aux == item)
            return aux;
        aux = aux->next;
    }
    return 0;
}

ListItem *List_insert(ListHead *head, ListItem *previous, ListItem *item)
{
    if (item->prev || item->next)
        return 0;

    ListItem *next = previous ? previous->next : head->first;
    if (previous)
    {
        item->prev = previous;
        previous->next = item;
    }
    if (next)
    {
        item->next = next;
        next->prev = item;
    }
    if (!previous)
    {
        head->first = item;
    }
    if (!next)
    {
        head->last = item;
    }
    head->size++;
    return item;
}

ListItem *List_detach(ListHead *head, ListItem *item)
{
    assert(List_find(head, item) || "Element is not in the list");

    ListItem *prev = item->prev;
    ListItem *next = item->next;

    if (prev)
        prev->next = next;
    if (next)
        next->prev = prev;
    if (item == head->first)
        head->first = next;
    if (item == head->last)
        head->last = prev;

    head->size--;
    item->next = 0;
    item->prev = 0;
    return item;
}

ListItem *List_popFront(ListHead *head)
{
    return List_detach(head, head->first);
}

ListItem *List_popBack(ListHead *head)
{
    return List_detach(head, head->last);
}

ListItem *List_pushFront(ListHead *head, ListItem *item)
{
    return List_insert(head, 0, item);
}

ListItem *List_pushBack(ListHead *head, ListItem *item)
{
    return List_insert(head, head->last, item);
}