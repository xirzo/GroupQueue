CREATE OR REPLACE PROCEDURE delete_list(IN id bigint)
LANGUAGE plpgsql
AS $$
BEGIN
    DELETE FROM list WHERE list_id = id;
    
    IF NOT FOUND THEN
        RAISE NOTICE 'No list found with id %', id;
    END IF;
    
EXCEPTION WHEN OTHERS THEN
    RAISE EXCEPTION 'Error deleting list: %', SQLERRM;
END;
$$;
