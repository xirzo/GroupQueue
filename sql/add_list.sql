CREATE OR REPLACE FUNCTION add_list(name varchar(255)) RETURNS bigint AS $$
    INSERT INTO list (list_name) VALUES (name) RETURNING list_id;
$$ LANGUAGE SQL;
