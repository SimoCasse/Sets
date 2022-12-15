#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t djb33x_hash(const char *key, const size_t keylen)
{
    size_t hash = 5381;

    for (size_t i = 0; i < keylen; i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }

    return hash;
}

struct set_node
{
    const char *key;
    size_t key_len ;
    struct set_node *next;
};

struct set_table
{
    struct set_node **nodes;
    size_t hashmap_size ;
};

struct set_table *set_table_new (const size_t hashmap_size )
{
    struct set_table *table = malloc (sizeof (struct set_table ));
    if (!table)
    {
        return NULL;
    }
    table->hashmap_size = hashmap_size ;
    table->nodes = calloc (table->hashmap_size , sizeof (struct set_node *));
    if (!table->nodes)
    {
        free(table);
        return NULL;
    }
    return table;
}

struct set_node *set_insert(struct set_table *table, const char *key, const size_t key_len)
{
    size_t hash = djb33x_hash(key, key_len);
    size_t index = hash % table->hashmap_size;
    struct set_node *head = table->nodes[index];
    if (!head)
    {
        table->nodes[index] = malloc(sizeof(struct set_node));
        if (!table->nodes[index])
        {
            return NULL;
        }
        table->nodes[index]->key = key;
        table->nodes[index]->key_len = key_len;
        table->nodes[index]->next = NULL;
        return table->nodes[index];
    }
    struct set_node *new_item = malloc(sizeof(struct set_node));
    if (!new_item)
    {
        return NULL;
    }
    new_item->key = key;
    new_item->key_len = key_len;
    new_item->next = NULL;
    struct set_node *tail = head;
    while (head)
    {
        tail = head;
        head = head->next;
    }
    tail->next = new_item;
}

struct set_node *Search(struct set_table* table, const char* key)
{
  size_t hash = djb33x_hash(key, strlen(key));
  size_t index = hash % table->hashmap_size;

  if(!table->nodes[index])
  {
    return NULL;
  }

  printf("%s", table->nodes[index]->key);

  return table->nodes[index];

}

struct set_node *Pop(struct set_node* node_to_pop)
{
  if(!node_to_pop)
  {
    return NULL;
  }

  free(node_to_pop);

  node_to_pop->key = NULL;
  node_to_pop->key_len = 0;

  return NULL;
}

int main (int argc, char **argv)
{
  struct set_table *table = set_table_new(20);

  set_insert(table, "hello", 5);

  set_insert(table, "mela", strlen("mela"));

  Pop(Search(table, "hello"));

  Search(table, "hello");
}