
# Group Queue 📋⏳

Group Queue is a program made for managing queues for groups in ITMO University.

---

## Configuration Files 🛠️

1. **`users.json`**
   A JSON file containing user data required for the application. Put it in the project directory.

   Example `users.json`:

   ```json
   [
      {
        "telegram_id": 1111111,
        "first_name": "Федор",
        "surname": "Федоров",
        "second_name": "Федорович",
        "admin": false
      },
   ]
   ```

## Build and Run with Docker 🐳

### Build the Docker Image 📦

To build the Docker image for Group Queue, run:

```bash
docker build -t group-queue .
```

### Run the Docker Container ▶️

You can run the container like this:

```bash
docker run -p 5000:5000 \
    -e port=5000 \ 
    group-queue
```

### Access Postgres

```
sudo -u postgres psql
```

Then load contents of sql directory

```psql
\i <name_of_sql_file>.sql
```

---

## Environment Variables 🌐

The application supports the following environment variables:

- **`port`**: Specifies the port on which the application will run (default: `5000`).
