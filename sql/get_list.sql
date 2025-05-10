CREATE OR REPLACE FUNCTION "get_list"(id bigint)
RETURNS TABLE (
    list_id bigint, 
    list_name varchar(255) 
) AS $$
BEGIN
    RETURN QUERY
    SELECT l.list_id, l.list_name
    FROM list l 
    WHERE l.list_id = id;

    IF NOT FOUND THEN
        RAISE NOTICE 'List with id % is not found', id;
    END IF;

EXCEPTION
    WHEN OTHERS THEN
        RAISE EXCEPTION 'Error getting list: %', SQLERRM;
END;
$$ LANGUAGE plpgsql;
