CREATE OR REPLACE FUNCTION add_list (name varchar(255)) RETURNS bigint AS $$
BEGIN
    INSERT INTO list (list_name) VALUES (name) RETURNING list_id;
EXCEPTION WHEN others THEN
    RETURN NULL;
END;
$$ LANGUAGE plpgsql;
