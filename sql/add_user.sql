CREATE OR REPLACE FUNCTION add_user(
    user_telegram_id bigint,
    user_first_name varchar(255), 
    user_surname varchar(255),
    user_last_name varchar(255),
    user_is_admin boolean
) RETURNS bigint AS $$
    INSERT INTO "user" (
        telegram_id, 
        first_name, 
        surname, 
        last_name, 
        is_admin
    ) 
    VALUES (
        user_telegram_id, 
        user_first_name, 
        user_surname, 
        user_last_name, 
        user_is_admin
    ) 
RETURNING user_id;
$$ LANGUAGE SQL;
