CREATE TABLE "user"(
    user_id bigserial PRIMARY KEY,
    telegram_id bigint UNIQUE,
    first_name varchar(255),
    surname varchar(255),
    last_name varchar(255),
    is_admin boolean
);
