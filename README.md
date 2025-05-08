
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

2. **`data.db3`** *(optional)*  
   If you have an existing SQLite database file (`data.db3`), you may put it in the project directory.

---

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
---

## Environment Variables 🌐
The application supports the following environment variables:
- **`port`**: Specifies the port on which the application will run (default: `5000`).
