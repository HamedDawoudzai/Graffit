#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024

#ifndef MAT_SIZE
#define MAT_SIZE 3 // A small graph
#endif

typedef struct user_struct
{
  char name[MAX_STR_LEN];
  struct friend_node_struct *friends;
  struct brand_node_struct *brands;
  bool visited;
} User;

typedef struct friend_node_struct
{
  User *user;
  struct friend_node_struct *next;
} FriendNode;

typedef struct brand_node_struct
{
  char brand_name[MAX_STR_LEN];
  struct brand_node_struct *next;
} BrandNode;

FriendNode *allUsers = NULL;

int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Given the head to a FriendNode linked list, returns true if a
 * given user's name exists in the list. Returns false otherwise.
 */
bool in_friend_list(FriendNode *head, User *node)
{
  for (FriendNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->user->name, node->name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Given the head to a BrandNode linked list, returns true if a
 * given brand's name exists in the list. Returns false otherwise.
 */
bool in_brand_list(BrandNode *head, char *name)
{
  for (BrandNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->brand_name, name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Given the head to a FriendNode linked list, inserts a given user
 * into the linked list and returns the head of the new linked list.
 * The insertion is done in alphabetical order. If the user already
 * exists, no modifications are made and the list is returned as is.
 */
FriendNode *insert_into_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (in_friend_list(head, node))
  {
    printf("User already in list\n");
    return head;
  }

  FriendNode *fn = calloc(1, sizeof(FriendNode));
  fn->user = node;
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->user->name, node->name) > 0)
  {
    fn->next = head;
    return fn;
  }

  FriendNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

/**
 * Given the head to a BrandNode linked list, inserts a given brand into
 * the linked list and returns the head of the new linked list. The
 * insertion is done in alphabetical order. If the brand already exists,
 * no modifications are made and the list is returned as is.
 */
BrandNode *insert_into_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (in_brand_list(head, node))
  {
    printf("Brand already in list\n");
    return head;
  }

  BrandNode *fn = calloc(1, sizeof(BrandNode));
  strcpy(fn->brand_name, node);
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->brand_name, node) > 0)
  {
    fn->next = head;
    return fn;
  }

  BrandNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

/**
 * Given the head to a FriendNode linked list, removes a given user from
 * the linked list and returns the head of the new linked list. If the user
 * does not exist, no modifications are made and the list is returned as is.
 */
FriendNode *delete_from_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (!in_friend_list(head, node))
  {
    printf("User not in list\n");
    return head;
  }

  if (strcmp(head->user->name, node->name) == 0)
  {
    FriendNode *temp = head->next;
    free(head);
    return temp;
  }

  FriendNode *cur;
  for (cur = head; cur->next->user != node; cur = cur->next)
    ;

  FriendNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

/**
 * Given the head to a BrandNode linked list, removes a given brand from
 * the linked list and returns the head of the new linked list. If the brand
 * does not exist, no modifications are made and the list is returned as is.
 */
BrandNode *delete_from_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (!in_brand_list(head, node))
  {
    printf("Brand not in list\n");
    return head;
  }

  if (strcmp(head->brand_name, node) == 0)
  {
    BrandNode *temp = head->next;
    free(head);
    return temp;
  }

  BrandNode *cur;
  for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
    ;

  BrandNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

/**
 * Given a user, prints their name, friends, and liked brands.
 */
void print_user_data(User *user)
{
  printf("User name: %s\n", user->name);

  printf("Friends:\n");
  for (FriendNode *f = user->friends; f != NULL; f = f->next)
  {
    printf("   %s\n", f->user->name);
  }

  printf("Brands:\n");
  for (BrandNode *b = user->brands; b != NULL; b = b->next)
  {
    printf("   %s\n", b->brand_name);
  }
}

/**
 * Given a brand, returns the index of the brand inside the brand_names array.
 * If it doesn't exist in the array, return -1
 */
int get_brand_index(char *name)
{
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (strcmp(brand_names[i], name) == 0)
    {
      return i;
    }
  }

  printf("Brand '%s' not found\n", name);
  return -1; // Not found
}

/**
 * Given a brand, prints their name, index (inside the brand_names
 * array), and the names of other similar brands.
 */
void print_brand_data(char *brand_name)
{
  int idx = get_brand_index(brand_name);
  if (idx < 0)
  {
    printf("Brand '%s' not in the list.\n", brand_name);
    return;
  }

  printf("Brand name: %s\n", brand_name);
  printf("Brand idx: %d\n", idx);

  printf("Similar brands:\n");
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0)
    {
      printf("   %s\n", brand_names[i]);
    }
  }
}

/**
 * Read from a given file and populate a the brand list and brand matrix.
 **/
void populate_brand_matrix(char *file_name)
{
  // Read the file
  char buff[MAX_STR_LEN];
  FILE *f = fopen(file_name, "r");
  fscanf(f, "%s", buff);
  char *line = buff;
  // Load up the brand_names matrix
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (i == MAT_SIZE - 1)
    {
      strcpy(brand_names[i], line);
      break;
    }
    int index = strchr(line, ',') - line;
    strncpy(brand_names[i], line, index);
    line = strchr(line, ',') + sizeof(char);
  }
  // Load up the brand_adjacency_matrix
  for (int x = 0; x < MAT_SIZE; x++)
  {
    fscanf(f, "%s", buff);
    for (int y = 0; y < MAT_SIZE; y++)
    {
      int value = (int)buff[y * 2];
      if (value == 48)
      {
        value = 0;
      }
      else
      {
        value = 1;
      }
      brand_adjacency_matrix[x][y] = value;
    }
  }
}

/*
typedef struct user_struct
{
  char name[MAX_STR_LEN];
  struct friend_node_struct *friends;
  struct brand_node_struct *brands;
  bool visited;
} User;

typedef struct friend_node_struct
{
  User *user;
  struct friend_node_struct *next;
} FriendNode;

typedef struct brand_node_struct
{
  char brand_name[MAX_STR_LEN];
  struct brand_node_struct *next;
} BrandNode;*/
/**
 * TODO: Complete this function
 * Given a name, creates a new user on the platform and returns the created user.
 * When the user is created, they must also be inserted into the allUsers linked list.
 * If a user already exists with the same name, do nothing and return NULL instead.
 */

// bool in_friend_list(FriendNode *head, User *node)
User *create_user(char *name) // existing test****************************************************************
{
  for (FriendNode *current_user_all_users_friend_node_linked_list = allUsers; current_user_all_users_friend_node_linked_list != NULL; current_user_all_users_friend_node_linked_list = current_user_all_users_friend_node_linked_list->next)
  {
    if (strcmp(current_user_all_users_friend_node_linked_list->user->name, name) == 0)
    {
      return NULL;
    }
  }
  User *new_user_node_for_test = (User *)calloc(1, sizeof(User));
  if (new_user_node_for_test == NULL)
  {
    return NULL;
  }
  strcpy(new_user_node_for_test->name, name);
  new_user_node_for_test->friends = NULL;
  new_user_node_for_test->brands = NULL;
  new_user_node_for_test->visited = false;
  allUsers = insert_into_friend_list(allUsers, new_user_node_for_test);
  return new_user_node_for_test;
}

/**
 * TODO: Complete this function
 * Removes a given user from the platform. The user must be removed from the allUsers linked list and the friend list of
 * any users that they belong to. Return 0 if the user was successfully removed.
 * If the user does not exist, return -1 instead.
 */
int delete_user(User *user) // NO TEST: MANUALLy HAVE TO TEST
{
  if (!in_friend_list(allUsers, user))
  {
    printf("User not in allUsers.\n");
    return -1;
  }
  FriendNode *current_user_node_in_allUsers = allUsers;
  while (current_user_node_in_allUsers != NULL)
  {
    if (current_user_node_in_allUsers->user == user)
    {
      current_user_node_in_allUsers = current_user_node_in_allUsers->next;
      continue;
    }
    current_user_node_in_allUsers->user->friends = delete_from_friend_list(current_user_node_in_allUsers->user->friends, user);
    current_user_node_in_allUsers = current_user_node_in_allUsers->next;
  }
  BrandNode *currentBrand = user->brands;
  while (currentBrand != NULL)
  {
    BrandNode *nextBrand = currentBrand->next;
    free(currentBrand);
    currentBrand = nextBrand;
  }
  allUsers = delete_from_friend_list(allUsers, user);
  free(user);

  return 0;
}

/**
 * TODO: Complete this function
 * Given a pair of valid users, create a friendship. A user's friends list must remain in alphabetical order.
 * Return 0 if the friendship was successfully created. Return -1 if the pair were already friends.
 */
int add_friend(User *user, User *friend) // EXISTING TEST *********************************************
{
  if (user == NULL || friend == NULL)
  {
    printf("Invalid users.\n");
    return -1;
  }

  if (in_friend_list(user->friends, friend) || in_friend_list(friend->friends, user))
  {
    return -1;
  }
  friend->friends = insert_into_friend_list(friend->friends, user);
  user->friends = insert_into_friend_list(user->friends, friend);
  return 0;
}

/**
 * TODO: Complete this function
 * Given a paid of valid users, remove their friendship. A user's friends list must remain in alphabetical order.
 * Return 0 if the pair are no longer friends. Return -1 if the pair were not friends to begin with.
 */
int remove_friend(User *user, User *friend) // NO EXISTING TEST: MANUALLY CHECK YOURSELF???????????????????????????????
{
  if (user == NULL || friend == NULL)
  {
    printf("Invalid users.\n");
    return -1;
  }

  if (!in_friend_list(user->friends, friend) || !in_friend_list(friend->friends, user))
  {
    return -1;
  }
  friend->friends = delete_from_friend_list(friend->friends, user);
  user->friends = delete_from_friend_list(user->friends, friend);

  return 0;
}

/**
 * TODO: Complete this function
 * Given a valid user and the name of a brand, create a link between the user and the brand. A user's brands
 * list must remain in alphabetical order. Return 0 if the link was successfully created.
 * Return -1 if the link already existed or if the brand name is invalid.
 */
int follow_brand(User *user, char *brand_name) // existing test
{
  if (user == NULL || brand_name == NULL)
  {
    printf("Invalid user or brand name.\n");
    return -1;
  }
  if (in_brand_list(user->brands, brand_name))
  {
    printf("%s is following %s already.\n", user->name, brand_name);
    return -1;
  }
  int brand_index_in_brand_list = get_brand_index(brand_name);
  if (brand_index_in_brand_list == -1)
  {
    printf("brand non-existent.\n");
    return -1;
  }
  user->brands = insert_into_brand_list(user->brands, brand_name);
  return 0;
}

/**
 * TODO: Complete this function
 * Given a valid user and the name of a brand, remove the link between the user and the brand. A user's brands list must remain
 * in alphabetical order. Return 0 if the link was successfully removed.
 * Return -1 if the link did not previously exist or if the brand name is invalid.
 */
int unfollow_brand(User *user, char *brand_name) // NO EXISTING TEST??????????????????????????
{
  if (user == NULL || brand_name == NULL)
  {
    printf("Invalid user or name.\n");
    return -1;
  }
  if (!in_brand_list(user->brands, brand_name))
  {
    printf("not following");
    return -1;
  }
  int brand_index_in_brand_list = get_brand_index(brand_name);
  if (brand_index_in_brand_list == -1)
  {
    printf("this brand '%s' doesn't exist.\n", brand_name);
    return -1;
  }
  user->brands = delete_from_brand_list(user->brands, brand_name);
  return 0;
}

/**
 * TODO: Complete this function
 * Given a pair of valid users, return the number of mutual friends between them.
 * A mutual friend is a user that exists in the friends list of both User a and User b.
 */
int get_mutual_friends(User *a, User *b) // exisiting test
{
  if (a == NULL || b == NULL)
  {
    printf("Invalid users.\n");
    return -1;
  }

  int num_of_mutuals = 0;

  FriendNode *friend_list_of_User_A = a->friends;
  while (friend_list_of_User_A != NULL)
  {
    if (in_friend_list(b->friends, friend_list_of_User_A->user))
    {
      num_of_mutuals = num_of_mutuals + 1;
    }
    friend_list_of_User_A = friend_list_of_User_A->next;
  }

  return num_of_mutuals;
}
/**
 * TODO: Complete this function
 * A degree of connection is the number of steps it takes to get from
 * one user to another. Returns a non-negative integer representing
 * the degrees of connection between two users.Given a pair of valid users, return the degrees of connection between both users.
 * The "degrees of connection" is the shortest number of steps it takes to get from one user to the other.
 * If a connection cannot be formed, return -1.
 */
int get_degrees_of_connection(User *a, User *b)
{

  if (a == NULL || b == NULL)
  {
    return -1;
  }
  if (a == b)
  {
    return 0;
  }
  for (FriendNode *current_user_in_allUsers = allUsers; current_user_in_allUsers != NULL; current_user_in_allUsers = current_user_in_allUsers->next)
  {
    current_user_in_allUsers->user->visited = false;
  }

  int degrees = 0;
  int current_level_users = 1;
  int next_level_users = 0;

  a->visited = true;

  while (current_level_users > 0)
  {
    for (int i = 0; i < current_level_users; ++i)
    {
      User *current_user_looked_for = a;
      for (int j = 0; j < i; ++j)
      {
        current_user_looked_for = current_user_looked_for->friends->next->user;
      }
      FriendNode *current_Friend_looked_at = current_user_looked_for->friends;
      while (current_Friend_looked_at != NULL)
      {
        User *friendUser = current_Friend_looked_at->user;
        if (friendUser == b)
        {
          return degrees + 1;
        }
        if (!friendUser->visited)
        {
          friendUser->visited = true;
          next_level_users++;
        }
        current_Friend_looked_at = current_Friend_looked_at->next;
      }
    }
    degrees = degrees + 1;
    current_level_users = next_level_users;
    next_level_users = 0;
  }
  return -1;
}

/**
 * TODO: Complete this function
 * Marks two brands as similar.Given two brand names, mark the two brands as similar in the brand_adjacency_matrix variable.
 * If either brand name is invalid, do nothing.
 */
void connect_similar_brands(char *brandNameA, char *brandNameB)
{
  if (brandNameA == NULL || brandNameB == NULL)
  {
    return;
  }
  int brand_index_in_brand_listA = get_brand_index(brandNameA);
  int brand_index_in_brand_listB = get_brand_index(brandNameB);
  if (brand_index_in_brand_listA == -1 || brand_index_in_brand_listB == -1)
  {
    printf("Invalid brand names.\n");
    return;
  }
  brand_adjacency_matrix[brand_index_in_brand_listB][brand_index_in_brand_listA] = 1;
  brand_adjacency_matrix[brand_index_in_brand_listA][brand_index_in_brand_listB] = 1;
}

/**
 * TODO: Complete this function
 * Returns a suggested friend for the given user.Given a user, suggest a new friend for them. To find the best match,
 * the new suggested friend should have the highest number of mutually liked brands amongst all other valid candidates.
 * If a tie needs to be broken, select the user with the name that comes first in reverse-alphanumerical order.
 * The suggested friend must be a valid user, cannot be the user themself, nor someone that they're already friends with.
 * If the user is already friends with everyone on the platform, return NULL.
 */
User *get_suggested_friend(User *user)
{

  if (user == NULL)
    return NULL;

  User *most_favourable_suggested_friend = NULL;
  int max_num_of_mutual_brands = -1;

  for (FriendNode *current_user_in_allUsers = allUsers; current_user_in_allUsers != NULL; current_user_in_allUsers = current_user_in_allUsers->next)
  {

    User *possible_most_favourable_friend = current_user_in_allUsers->user;

    if (possible_most_favourable_friend == user)
    {
      continue;
    }
    if (in_friend_list(user->friends, possible_most_favourable_friend))
    {
      continue;
    }
    int number_of_mutual_brands_between_current_and_brandname = 0;
    for (BrandNode *current_brand = user->brands; current_brand != NULL; current_brand = current_brand->next)
    {
      if (in_brand_list(possible_most_favourable_friend->brands, current_brand->brand_name))
        number_of_mutual_brands_between_current_and_brandname++;
    }
    if ((number_of_mutual_brands_between_current_and_brandname == max_num_of_mutual_brands && strcmp(possible_most_favourable_friend->name, most_favourable_suggested_friend->name) > 0) || number_of_mutual_brands_between_current_and_brandname > max_num_of_mutual_brands)
    {

      most_favourable_suggested_friend = possible_most_favourable_friend;
      max_num_of_mutual_brands = number_of_mutual_brands_between_current_and_brandname;
    }
  }
  return most_favourable_suggested_friend;
}

/**
 * TODO: Complete this function
 * Adds n suggested friends for the given user.
 * Returns how many friends were successfully followed.
 *
 */
int add_suggested_friends(User *user, int n)
{

  if (user == NULL)
  {
    printf("Invalid user\n");
    return 0;
  }
  if (n <= 0)
  {
    return 0;
  }
  int friends_successfully_added_to_users_friendlist = 0;

  while (friends_successfully_added_to_users_friendlist < n)
  {
    User *best_suggested_friend_current = get_suggested_friend(user);

    if (best_suggested_friend_current == NULL)
    {
      break;
    }
    if (best_suggested_friend_current != user && !in_friend_list(user->friends, best_suggested_friend_current))
    {
      user->friends = insert_into_friend_list(user->friends, best_suggested_friend_current);
      friends_successfully_added_to_users_friendlist++;
    }
  }
  return friends_successfully_added_to_users_friendlist;
}

/**
 * TODO: Complete this function
 * Follows n suggested brands for the given user.
 * Returns how many brands were successfully followed.
 * Given a user and a positive interger n, suggest n new brands for them.
 * To find the best matches, suggested brands with the most similarities with the brands that the user already follows.
 * If a tie needs to be broken, select the brand with the name that comes first in reverse-alphanumerical order.
 * The suggested brand must be a valid brand and cannot be a brand that the user already follows.
 * There might not be enough brands on the platform to sastify n, so return the amount of brands successfully followed.
 * Like add_suggested_friends, sometimes, adding a brand with a similarity rating of 0 is the best option.
 */
int follow_suggested_brands(User *user, int n)
{
  if (user == NULL || n <= 0)
  {
    printf("Invalid user or invalid number of suggested brands.\n");
    return 0;
  }

  int num_of_brands_followed = 0;
  int *brands_already_followed_currently = (int *)calloc(MAT_SIZE, sizeof(int));
  for (BrandNode *current_brand_node_in_user_brandlist = user->brands; current_brand_node_in_user_brandlist != NULL; current_brand_node_in_user_brandlist = current_brand_node_in_user_brandlist->next)
  {
    int brand_index_in_brand_list = get_brand_index(current_brand_node_in_user_brandlist->brand_name);
    if (brand_index_in_brand_list != -1)
    {
      brands_already_followed_currently[brand_index_in_brand_list] = 1;
    }
  }
  for (int i = 0; i < n; i++)
  {
    int most_similar_for_comparison = -1;
    int best_brand_in_brand_list = -1;

    for (int brand_index_in_brand_list = 0; brand_index_in_brand_list < MAT_SIZE; brand_index_in_brand_list++)
    {
      if (brands_already_followed_currently[brand_index_in_brand_list] == 0)
      {
        int similarity_of_brands_in_comapre = 0;

        for (BrandNode *current_brand_node_in_user_brandlist = user->brands; current_brand_node_in_user_brandlist != NULL; current_brand_node_in_user_brandlist = current_brand_node_in_user_brandlist->next)
        {
          int cur_brand_index_in_brand_list = get_brand_index(current_brand_node_in_user_brandlist->brand_name);
          if (brand_adjacency_matrix[brand_index_in_brand_list][cur_brand_index_in_brand_list] == 1)
          {
            similarity_of_brands_in_comapre++;
          }
        }

        if (similarity_of_brands_in_comapre > most_similar_for_comparison)
        {
          most_similar_for_comparison = similarity_of_brands_in_comapre;
          best_brand_in_brand_list = brand_index_in_brand_list;
        }
        else if (similarity_of_brands_in_comapre == most_similar_for_comparison)
        {
          if (strcmp(brand_names[brand_index_in_brand_list], brand_names[best_brand_in_brand_list]) > 0)
          {
            best_brand_in_brand_list = brand_index_in_brand_list;
          }
        }
      }
    }

    if (best_brand_in_brand_list != -1)
    {
      user->brands = insert_into_brand_list(user->brands, brand_names[best_brand_in_brand_list]);
      brands_already_followed_currently[best_brand_in_brand_list] = 1;
      num_of_brands_followed++;
    }
  }
  free(brands_already_followed_currently);
  return num_of_brands_followed;
}
