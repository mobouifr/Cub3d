#include "cub3d.h"

void *ft_gc_malloc(t_gc *gc, size_t size)
{
    void *mem = malloc(size);
    if (!mem)
        return NULL;
    t_gc_node *node = malloc(sizeof(t_gc_node));
    if (!node)
    {
        free(mem);
        return NULL;
    }
    node->ptr = mem;
    node->next = gc->head;
    gc->head = node;
    return mem;
}

void ft_gc_free_all(t_gc *gc)
{
    t_gc_node *node = gc->head;
    while (node)
    {
        t_gc_node *next = node->next;
        free(node->ptr);
        free(node);
        node = next;
    }
    gc->head = NULL;
}
