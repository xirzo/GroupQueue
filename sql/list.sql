CREATE TABLE "list" (
    list_id bigint GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    list_name varchar(255) UNIQUE
);
