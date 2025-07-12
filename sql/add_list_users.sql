CREATE OR REPLACE PROCEDURE add_list_users(list_add_id bigint)
AS $$
BEGIN
    INSERT INTO list_user (
        list_id,
        user_id,
        user_order
    )
    SELECT
        list_add_id,
        user_id,
        ROW_NUMBER() OVER (ORDER BY RANDOM()) as user_order
    FROM "user";
END;
$$
LANGUAGE plpgsql;
