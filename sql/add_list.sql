CREATE OR REPLACE FUNCTION "add_list"(list_name varchar(255))
RETURNS bigint AS $$
DECLARE
    new_list_id bigint;
BEGIN
    INSERT INTO list(list_name) VALUES (list_name) 
    RETURNING list_id INTO new_list_id;

    RETURN new_list_id;

    EXCEPTION 
        WHEN unique_violation THEN
            RETURN NULL;
        WHEN OTHERS THEN
            RAISE EXCEPTION 'Error adding list: %', SQLERRM;
END;
$$ LANGUAGE plpgsql;
