# Group Queue

Group Queue is a program made for managing assignments queues, initially I wrote it for my group university group.

> [!NOTE]  
> This app is just a backend, it requires some sort of a frontend, but now there is only a legacy one, which is not compatible with this.

## Prerequisite

Install all of the following libraries

### Dependencies

- [libpq](https://www.postgresql.org/docs/current/libpq.html) - PostgreSQL C client library.
- [logger](https://github.com/xirzo/logger) - my simple C logger library
- [libevent](https://libevent.org/) - Event notification library.
- [cJSON](https://github.com/DaveGamble/cJSON) - Lightweight JSON parser in C.

## Build

```sh
git clone https://github.com/xirzo/groupqueue
cd groupqueue && mkdir build
cmake -B build
cmake --build build
```

## Endpoints

### Health Check

#### GET `/health`

Simple health check to verify the server is running.
Returns "OK" (plain text)

Status Codes:

- 200: Server is healthy

### List Management

#### POST `/list/add`

Creates a new list with the specified name.  
Input JSON:

```json
{
  "list_name": "My List"
}
```

"List was added" (plain text)  
Status Codes:

- 200: List created successfully
- 400: Invalid parameters or list already exists

#### POST `/list/id`

Retrieves the ID of a list by its name.  
Input JSON:

```json
{
  "list_name": "My List"
}
```

Response JSON:

```json
{
  "list_id": 123
}
```

Status Codes:

- 200: List found
- 400: Invalid parameters
- 404: List not found

#### POST `/list/get`

Retrieves details for a specific list by its ID.  
Input JSON:

```json
{
  "list_id": 123
}
```

Response JSON:

```json
{
  "list_id": 123,
  "list_name": "My List"
}
```

Status Codes:

- 200: List found
- 400: Invalid parameters
- 404: List not found

#### POST `/list/all`

Retrieves all available lists.  
None (empty body)  
Response JSON:

```json
{
  "lists": [
    {
      "list_id": 123,
      "list_name": "My List"
    },
    {
      "list_id": 456,
      "list_name": "Another List"
    }
  ]
}
```

Status Codes:

- 200: Operation successful
- 400: Error retrieving lists

#### POST `/list/delete`

Deletes a list by its ID, including all associated user entries.  
Input JSON:

```json
{
  "list_id": 123
}
```

"List was deleted" (plain text)  
Status Codes:

- 200: List deleted successfully
- 400: Invalid parameters
- 404: List not found

### User Management

#### POST `/user/add`

Creates a new user with the specified details.  
Input JSON:

```json
{
  "first_name": "John",
  "surname": "D",
  "last_name": "Doe",
  "telegram_id": 12345678,
  "is_admin": false
}
```

"User was added" (plain text)  
Status Codes:

- 200: User created successfully
- 400: Invalid parameters or user already exists

#### POST `/list/users/swap`

Swaps the positions (order) of two users within a specific list.  
Input JSON:

```json
{
  "list_id": 123,
  "user_a_id": 456,
  "user_b_id": 789
}
```

"Users were swapped" (plain text)  
Status Codes:

- 200: Users swapped successfully
- 400: Invalid parameters or users not found in list

#### POST `/list/users`

Retrieves all users in a specific list with their order and details.  
Input JSON:

```json
{
  "list_id": 123
}
```

Response JSON:

```json
{
  "users": [
    {
      "user_id": 8,
      "telegram_id": 618211245,
      "first_name": "Bebr",
      "surname": "Kek",
      "last_name": "Lolovich",
      "is_admin": false,
      "order": 1
    },
    {
      "user_id": 7,
      "telegram_id": 618214141,
      "first_name": "Aleks",
      "surname": "Lol",
      "last_name": "Kekovich",
      "is_admin": true,
      "order": 2
    }
  ]
}
```

Status Codes:

- 200: Users retrieved successfully
- 400: Invalid parameters
- 404: List not found

## Sources

- [Postgresql docs](https://www.postgresql.org/docs/current)
