CREATE TABLE "list_user" (
    list_user_id bigint GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    user_id bigint,
    list_id bigint,
    user_order bigint
);
